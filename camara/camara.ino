#include <DHT.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

/* ************************************************************************** */
/* *                              CONSTANTS                                 * */
/* ************************************************************************** */


#define RS                          (7)
#define EN                          (6)
#define D4                          (5)
#define D5                          (4)
#define D6                          (3)
#define D7                          (2)

#define BUT_UPI                     (10)
#define BUT_ENT                     (9)
#define BUT_DOW                     (8)

#define LED_1                       (A0)
#define LED_2                       (A1)
#define LED_3                       (A2)

#define DHTPIN                      (11)
#define DHTTYPE                     DHT11

#define CONT_EXIT_EXT               (2)
#define CONT_EXIT_INT               (5)

#define LIM_FINAL                   (90)
#define LIM_INICIAL                 (0)

#define LIM_FINAL_HIS               (10)
#define LIM_INICIAL_HIS             (0)

#define ADRESS_ENABLE               (8)

#define SENSOR_RESPONSE_MAX_LEN     (64)
#define SENSOR_SERIAL_PORT_RX_PIN   (13)
#define SENSOR_SERIAL_PORT_TX_PIN   (12)
#define SENSOR_DEFAULT_BAUD_RATE    (9600L)

#define SENSOR_MODE_COMMAND         (0)
#define SENSOR_MODE_STREAMING       (1)
#define SENSOR_MODE_POLLING         (2)

#define SENSOR_MULTIPLIER           (100)
#define INTERVAL                    (60000L)

/* ************************************************************************** */
/* *                        MANIPULACION EEPROM                             * */
/* ************************************************************************** */

#define EDDRES                     (0)

/* ************************************************************************** */
/* *                              GLOBALS                                   * */
/* ************************************************************************** */

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
static SoftwareSerial * g_sensor = NULL;

static int lmt_gas      = 0;
static int hist_gas     = 0;

static int lmt_umidade  = 0;
static int hist_umidade = 0;

unsigned int cnt = 0;
unsigned int cnt_in = 0;

static unsigned long previousMillis = 0;

static boolean enable_gas = false;

/* ************************************************************************** */
/* *                              IMPLEMENTATION                            * */
/* ************************************************************************** */

static boolean saida_menu( )
{
 
  boolean retur_exit = false;

  if( cnt <= 0 )
  {
    if( cnt_in <= 0 )
    {
      retur_exit = true;
      return retur_exit;
    }

    cnt_in --;
  }

  cnt --;
  return retur_exit;
}


static const char * sensor_send_command( const char * cmd  )
{
  static char buf[ SENSOR_RESPONSE_MAX_LEN + 1 ] = {0};
  int i = 0;

  g_sensor->print(cmd);
  g_sensor->print("\r\n");
  delay(500);

  while(g_sensor->available())
    buf[i++] = g_sensor->read();

  buf[i-2] = '\0';

  return buf;
}


static uint32_t sensor_get_co2_sample( int filtered )
{
  const char * resp = sensor_send_command((filtered)?"Z":"z");
  uint32_t z = atoi( resp + 3 );
  return z * SENSOR_MULTIPLIER;
}


static const char * sensor_get_config( void )
{
  const char * resp = sensor_send_command("*");
  return resp + 3;
}


static uint32_t sensor_get_multiplier( void )
{
  const char * resp = sensor_send_command(".");
  uint32_t m = atoi( resp + 3 );
  return m;
}


static uint32_t sensor_get_scale_concentration( void )
{
  uint32_t lo = 0;
  uint32_t hi = 0;
  uint32_t sc = 0;
  const char * resp = NULL;

  resp = sensor_send_command("p 0");
  hi = atoi( resp + 9 );

  resp = sensor_send_command("p 1");
  lo = atoi( resp + 9 );

  sc = (hi * 256) + lo;

  return sc * SENSOR_MULTIPLIER;
}


static void sensor_set_scale_concentration( uint32_t sc )
{
  char cmd[16] = {0};

  uint32_t lo = (sc / SENSOR_MULTIPLIER) % 256;
  uint32_t hi = (sc / SENSOR_MULTIPLIER) / 256;

  sprintf( cmd, "P 0 %d", hi );
  sensor_send_command(cmd);

  sprintf( cmd, "P 1 %d", lo );
  sensor_send_command(cmd);
}


static void sensor_set_zero_fresh_air( void )
{
  sensor_send_command("G");
}


static uint32_t sensor_set_known_concentration( uint32_t ppm )
{
  char cmd[16] = {0};
  sprintf( cmd, "X %d", ppm / SENSOR_MULTIPLIER );
  sensor_send_command( cmd );
}


static void sensor_set_mode( int mode )
{
  char cmd[16] = {0};
  sprintf( cmd, "K %d", mode );
  sensor_send_command( cmd );
}

static void bounce( int *botao ) {

  boolean solta = true;

  if ( !digitalRead(BUT_UPI) || !digitalRead(BUT_ENT) || !digitalRead(BUT_DOW) ) {

    cnt = CONT_EXIT_EXT;
    cnt_in = CONT_EXIT_INT;

    while (solta) {

      if ( !digitalRead(BUT_UPI) )
        *botao = 1;
      else if ( !digitalRead(BUT_ENT) )
        *botao = 2;
      else if ( !digitalRead(BUT_DOW) )
        *botao = 3;
      if ( digitalRead(BUT_UPI) && digitalRead(BUT_ENT) && digitalRead(BUT_DOW) )
        solta = false;

      delay(100);
    }
  }
}

static void apresentacao_controller( int *lmt_, int *hist_, boolean gas_, boolean habili ) {
  
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("LIMITE = ");
  
  if( *lmt_ < 10 )
    lcd.print("0");
    
  lcd.print(*lmt_);
  lcd.print("%");
  
  lcd.setCursor(0, 1);
  lcd.print("HISTERESE = ");
  
  if( *hist_ < 10 )
    lcd.print("0");
  
  lcd.print(*hist_);
  lcd.print("%"); 

  if( gas_ ) {
    lcd.setCursor(0, 2);
    lcd.print("GAS HABIL. = ");
    if( habili ) 
      lcd.print("SIM");
    else
      lcd.print("NAO");
  }
}

static void controller( int *limite, int limit_max, int limit_min, int posicao_x, int posicao_y, int endderes ) {
  
  int btn = 0;
  int contador = *limite;
  boolean sub_laco = true;

  lcd.setCursor( posicao_y, posicao_x );  
  if( contador < 10 )
    lcd.print("0");
  lcd.print( contador );
  lcd.print("%<");
  
  while( sub_laco ) {

    sub_laco = !saida_menu( );

    bounce( &btn );

    switch( btn ) {

      case 1: contador++;
        if( contador > limit_max )
          contador = limit_max;
          
        lcd.setCursor( posicao_y, posicao_x );
        
        if( contador < 10 )
          lcd.print("0");
          
        lcd.print( contador );
        lcd.print("%<");
        
        break;
      case 2: sub_laco = false; break;
      case 3: contador--;
        if( contador < limit_min )
          contador = limit_min;
        lcd.setCursor( posicao_y, posicao_x );
        
        if( contador < 10 )
          lcd.print("0");
          
        lcd.print( contador );
        lcd.print("%<");
                  
        break;      
    }
    
    btn = 0;
  }

  EEPROM.write( EDDRES + endderes, contador );
  
  *limite = contador;
  lcd.setCursor( posicao_y, posicao_x );  
  if( contador < 10 )
    lcd.print("0");
  lcd.print( contador );
  lcd.print("% ");  
  
}


static void testando( int *count, boolean *led1, boolean *led2, boolean *led3) {

  switch( *count ) {
    
    case 1:
      *led1 = !*led1; 
      lcd.setCursor(0, 0);
      if( *led1 ) {
        lcd.print("SOLEN.=  LIGADO<");
        digitalWrite( LED_1, HIGH);
      } else {
        lcd.print("SOLEN.= DESLIGA<");
        digitalWrite( LED_1, LOW);
      }
      break;
    case 2:
      *led2 = !*led2; 
      lcd.setCursor(0, 1);
      if( *led2 ) {
        lcd.print(" OFF  =  LIGADO<");
        digitalWrite( LED_2, HIGH);
      } else {
        lcd.print(" OFF  = DESLIGA<");
        digitalWrite( LED_2, LOW);
      }
      break;
    case 3:
      *led3 = !*led3;
      lcd.setCursor(0, 2);
      if( *led3 ) {
        lcd.print("BOMBA =  LIGADO<");
        digitalWrite( LED_3, HIGH);
      } else {
        lcd.print("BOMBA = DESLIGA<");
        digitalWrite( LED_3, LOW);
      }
      break;
  }
}

static void controlle_teste() {

  boolean laco = true;
  boolean habilitar = false;
  int botao = 0;
  int count = 1;

  boolean led1 = false;
  boolean led2 = false;
  boolean led3 = false;

  while(laco) {

    bounce( &botao );

    switch( botao ) {
      
      case 1: count --;
          habilitar = true;
          botao = 0;
          break;
      case 2: testando( &count, &led1, &led2, &led3 );
          botao = 0;
          break;
      case 3: count ++;
          habilitar = true;
          botao = 0;
          break;
    }

    if( habilitar ) {

      switch( count ) {
        case 1:
          lcd.setCursor(15, 0);
          lcd.print("<");
          lcd.setCursor(15, 1);
          lcd.print(" ");
          lcd.setCursor(15, 2);
          lcd.print(" ");
          break;
        case 2:
          lcd.setCursor(15, 0);
          lcd.print(" ");
          lcd.setCursor(15, 1);
          lcd.print("<");
          lcd.setCursor(15, 2);
          lcd.print(" ");
          break;
        case 3:
          lcd.setCursor(15, 0);
          lcd.print(" ");
          lcd.setCursor(15, 1);
          lcd.print(" ");
          lcd.setCursor(15, 2);
          lcd.print("<");
          break;
      }
    }
    
    habilitar = false;
    if( count >= 4 || count <= 0 || saida_menu( ) )
      laco = false;

  }
}

static void apresentacao_teste( ) {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("SOLEN.= DESLIGA<");
  lcd.setCursor(0, 1);
  lcd.print(" OFF  = DESLIGA ");
  lcd.setCursor(0, 2);
  lcd.print("BOMBA = DESLIGA ");
}

static void sub_menu_gas( ) {

  boolean loop_enable = true;
  int bot_ = 0;
  int limite = 0;
  
  apresentacao_controller( &lmt_gas, &hist_gas, true, enable_gas );
  /* 
    Os parametros sao o valor do limete de gas, limite final, limite inicial, linha do LCD e sua coluna e por último,
    o valor do endereco da memoria da eeprom
   */
  controller( &lmt_gas,  LIM_FINAL, LIM_INICIAL,     0, 9, 0  );

  hist_gas = LIM_INICIAL;

  if( lmt_gas < LIM_FINAL_HIS )
    limite = lmt_gas - 1;
  else
    limite = LIM_FINAL_HIS;

  controller( &hist_gas, limite,    LIM_INICIAL_HIS, 1, 12, 2 );

  while( loop_enable ) {

    bounce( &bot_ );

    switch( bot_ ) {
      
      case 1: enable_gas = !enable_gas;
              lcd.setCursor(13, 2);
              if(enable_gas)
                lcd.print("SIM");
              else
                lcd.print("NAO");
              bot_ = 0;  
              break;
              
      case 2: loop_enable = false; break;
      
      case 3: enable_gas = !enable_gas;
              lcd.setCursor(13, 2);
              if(enable_gas)
                lcd.print("SIM");
              else
                lcd.print("NAO");
              bot_ = 0;  
              break;
      default: break;

    }
  }
  EEPROM.write( EDDRES + ADRESS_ENABLE, enable_gas );
}

static void sub_menu_umidade( ) {

  int limite = 0;

  apresentacao_controller( &lmt_umidade, &hist_umidade, false, false );
  controller( &lmt_umidade,  LIM_FINAL, LIM_INICIAL,     0, 9, 4  );

  hist_umidade = LIM_INICIAL;

  if( lmt_umidade < LIM_FINAL_HIS )
    limite = lmt_umidade - 1;
  else
    limite = LIM_FINAL_HIS;

  controller( &hist_umidade, limite,    LIM_INICIAL_HIS, 1, 12, 6 );

}

static void sub_menu_teste( ) {
  apresentacao_teste( );
  controlle_teste( );
  digitalWrite( LED_1, LOW);
  digitalWrite( LED_2, LOW);
  digitalWrite( LED_3, LOW);
}

static void ctr_gas( int *bt_press, boolean *ext_menu ) {

  int botao = 0;
  boolean laco = true;
  *bt_press = 0;
   
  lcd.setCursor(0, 0);
  lcd.print("    CONTROLE    ");
  lcd.setCursor(0, 1);
  lcd.print("       DE       ");
  lcd.setCursor(0, 2);
  lcd.print("      GAS       ");

  while(laco) {
    
    bounce( &botao );
    
    switch( botao ) {
      case 1: *ext_menu = false;
          laco = false;
          break;
      case 2: sub_menu_gas( );
          *bt_press = 1;
          laco = false;
          break;
      case 3: *bt_press = botao;
          laco = false;
          break;
      default: break;
    } 

    if(saida_menu( )) {
      laco = false;
      *ext_menu = false;
    } 
  }
}

static void ctr_umidade( int *bt_press, boolean *ext_menu ) {

  int botao = 0;
  boolean laco = true;
  *bt_press = 0;

  lcd.setCursor(0, 0);
  lcd.print("    CONTROLE    ");
  lcd.setCursor(0, 1);
  lcd.print("       DE       ");
  lcd.setCursor(0, 2);
  lcd.print("     UMIDADE    ");

  while(laco) {
    
    bounce( &botao );
    
    switch( botao ) {
      case 1: *bt_press = botao;
          laco = false;
          break;
      case 2: sub_menu_umidade( );
          *bt_press = 3;
          laco = false;
           if(saida_menu( ))
            *ext_menu = false;
          break;
      case 3: *bt_press = botao;
          *bt_press = 4;
          laco = false;
          break;
      default: break;
    } 

     if(saida_menu( )) {
      laco = false;
      *ext_menu = false;
    } 
  }
}

static void teste_selenoid( int *bt_press, boolean *ext_menu ) {

  int botao = 0;
  boolean laco = true;
  *bt_press = 0;

  lcd.setCursor(0, 0);
  lcd.print("     TESTES     ");
  lcd.setCursor(0, 1);
  lcd.print("      PARA      ");
  lcd.setCursor(0, 2);
  lcd.print("   SOLENOIDES   ");

  while(laco) {
    
    bounce( &botao );
    
    switch( botao ) {
      case 1: *bt_press = 3;
          laco = false;
          break;
      case 2: sub_menu_teste( );
          *bt_press = 4;
          laco = false;
          break;
      case 3: *ext_menu = false; 
          laco = false;
          break;
    }

    if(saida_menu( )) {
      laco = false;
      *ext_menu = false;
    } 
  }
  
}

static void menu( ) {

  int bt_press = 1;
  boolean ext_menu = true;

  digitalWrite( LED_1, LOW);
  digitalWrite( LED_2, LOW);
  digitalWrite( LED_3, LOW);

  while(ext_menu) {

    bounce( &bt_press );

    switch( bt_press ) {
      case 1: lcd.clear(); ctr_gas( &bt_press, &ext_menu );        lcd.clear(); break;
      case 3: lcd.clear(); ctr_umidade( &bt_press, &ext_menu );    lcd.clear(); break;
      case 4: lcd.clear(); teste_selenoid( &bt_press, &ext_menu ); lcd.clear(); break;
      default:                   ext_menu = false;                              break;
    }
  }
}

/* ************************************************************************** */
/* *                                SETUP                                   * */
/* ************************************************************************** */


void setup() {
  
  lcd.begin(16, 4);
  dht.begin();
 
  pinMode(BUT_UPI, INPUT);
  pinMode(BUT_ENT, INPUT);
  pinMode(BUT_DOW, INPUT);

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);

  g_sensor = new SoftwareSerial( SENSOR_SERIAL_PORT_RX_PIN, SENSOR_SERIAL_PORT_TX_PIN );
  g_sensor->begin( SENSOR_DEFAULT_BAUD_RATE );

  sensor_send_command("\r\n");                /* first clear the buffer */

  sensor_set_mode( SENSOR_MODE_POLLING );
  sensor_set_scale_concentration( 1000000L ); /* second, sends the scale to read for 1 million ppm */

  lmt_gas      = EEPROM.read(EDDRES);
  hist_gas     = EEPROM.read(EDDRES + 2);
  lmt_umidade  = EEPROM.read(EDDRES + 4);
  hist_umidade = EEPROM.read(EDDRES + 6);
  enable_gas   = EEPROM.read(EDDRES + 8);

  if( ( lmt_gas == 0 || lmt_gas == 255 ) && ( hist_gas == lmt_umidade || hist_gas == 255 ) && 
    ( lmt_umidade == 0 || lmt_umidade == 255 ) && ( hist_umidade == 0 || hist_umidade == 255 ) ) {

    lmt_gas      = LIM_INICIAL;
    hist_gas     = LIM_INICIAL_HIS;
    lmt_umidade  = LIM_INICIAL;
    hist_umidade = LIM_INICIAL_HIS;
    
  }

  lcd.setCursor(0, 0);
  lcd.print("  UNIVERSIDADE  ");
  lcd.setCursor(0, 1);
  lcd.print("       DE       ");
  lcd.setCursor(0, 2);
  lcd.print("    BRASILIA    ");
  lcd.setCursor(0, 3);
  lcd.print("      UnB       ");

  delay(5000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("   CAMARA DE    ");
  lcd.setCursor(0, 1);
  lcd.print("      GAS       ");
  lcd.setCursor(0, 2);
  lcd.print("    CARBONO     ");
  lcd.setCursor(0, 3);
  lcd.print(" Pro. VALDIRENE ");

  delay(5000);
  lcd.clear();
  
}

/* ************************************************************************** */
/* *                                LOOP                                    * */
/* ************************************************************************** */

void loop() {

  uint32_t co_2 = sensor_get_co2_sample( 1 );
  float co2_percente = co_2 / 10000.0;
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  int botao_press = 0;

  uint32_t lmt_co2 = lmt_gas - hist_gas;
  int lmt_umid = lmt_umidade - hist_umidade;

  unsigned long currentMillis = millis();

  static boolean leitura_sensor = true;

  if(isnan(t) || isnan(h)) {
    
    lcd.setCursor(0, 1);
    lcd.print("Sensor Humidade ");
    lcd.setCursor(0, 2);
    lcd.print("    ERRO!!!     ");

    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_3, LOW);
    
  } else {

    lcd.setCursor(0, 0);
    lcd.print("CO2 = ");
    lcd.print("           ");
    lcd.setCursor(6, 0);
    lcd.print(co_2);
    lcd.print(" ppm");

    lcd.setCursor(0, 1);
    lcd.print("CO2(%) = ");
    lcd.print("        ");
    lcd.setCursor(9, 1);
    lcd.print(co2_percente);
    lcd.print("%");

    lcd.setCursor(0, 2);
    lcd.print("Temp.= ");
    lcd.print("         ");
    lcd.setCursor(7, 2);
    lcd.print(t);
    lcd.print(" *C");

    lcd.setCursor(0, 3);
    lcd.print("Umidade = ");
    lcd.print("      ");
    lcd.setCursor(10, 3);
    lcd.print(h);
    lcd.print("% ");

    bounce( &botao_press );

    if( botao_press == 2 ) {
      menu( );
      botao_press = 0;
    }

    if(enable_gas) {
      if( co2_percente < lmt_co2 ) {
           digitalWrite( LED_1, HIGH );
           digitalWrite( LED_3, HIGH );
      } else {
          digitalWrite( LED_1, LOW );
          if( (unsigned long)(currentMillis - previousMillis) >= INTERVAL ) {
            leitura_sensor = !leitura_sensor;
            previousMillis = millis( );
          }

          digitalWrite( LED_3, leitura_sensor);
      }
      
    } else {
      digitalWrite( LED_1, LOW);
      leitura_sensor = false;
    }

    if( h < lmt_umid )
           digitalWrite( LED_2, HIGH);
      else digitalWrite( LED_2, LOW );

  }
}


