<h1>Câmara de carbonização para CO²</h1>

<p>Para o funcionamento da câmara de carbonatação proposta, foi desenvolvido um algoritmo de programação, capaz de interagir e criar uma interface com os programas dos dispositivos de micro controle que foram utilizados no funcionamento da câmara de carbonatação. Este algoritmo permite atuar e comandar os programas dos dispositivos de micro controle em suas variáveis mensuráveis como concentração de CO2, leitura de temperatura, leitura de umidade relativa.</p>
<p>O algoritmo foi programado em uma linguagem de programação denominada “C + +” com a plataforma de desenvolvimento Arduino.</p>
<p>Fora definido como micro controle de funcionamento a avaliação, análise e atuação da concentração de CO2 no interior da câmara de carbonatação, bem como os recursos necessários para manter uma concentração definida, como aberturas de válvulas para injeção de CO2.</p>
<p>Para tal, o algoritmo permitiu interagir com o programa do sensor principal de avaliação de CO2 em diversos aspectos aos quais descreve-se a seguir:</p>
<ul>
<li>A interface apresenta em um display LCD 16x4 que é mostrado diversos dados que podem ser visualizados e alterados de acordo com o experimento.</li>
<li>Com um MENU de configurção que poderá definir o controle automático de carbonatação atribuindo ao controle automatico do protótipo.</li>
</ul>
<p>As variáveis mensuráveis inerentes ao micro controle para a câmara de carbonatação acelerada são concentração de CO2, temperatura, umidade relativa do ar, porém, as únicas variáveis microcontroladas são concentração de CO2 e ar.</p>

<h3>Sensor principal para a mensuração do CO²</h3>

<p>O dispositivo avaliador de CO2 fora adquirido nos Estados Unidos da América, através da empresa CO2 METER Inc. A Figura 1 demonstra o sensor para avaliação de concentração de dióxido de carbono tipo COZIR Wide Range 5-100% CO2 Sensor, com faixa de avaliação entre 0% e 100%, e funcionamento em 20Hz.</p>

<img src="https://github.com/456789123/Carbonization_Campaign/blob/master/Fotos/unnamed.jpg" alt="COZIR Wide Range 5-100% CO2 Sensor">

Figura 01: Sensor de avaliação de concentração de CO2.

<p>Para mensuração de umidade e temperatura, fora usado o sensor DHT11 mostrado na figura 2.</p>

<img src="https://github.com/456789123/Carbonization_Campaign/blob/master/Fotos/DHT11.jpg" alt="DHT11">

Figura 02: Sensor DHT11.

<p>O DHT11 é um sensor de temperatura e umidade que permite fazer leituras de temperaturas entre 0 a 50 Celsius e umidade entre 20 a 90%, muito usado para projetos com Arduino.</p>
<p>O elemento sensor de temperatura é um termistor do tipo NTC e o sensor de Umidade é do tipo HR202, o circuito interno faz a leitura dos sensores e se comunica a um microcontrolador através de um sinal serial de uma via.</p>

<p>Para o controle de todo o protótipo fora usado a plataforma Arduino Uno conforme a figura 03:</p>
<img src="https://github.com/456789123/Carbonization_Campaign/blob/master/Fotos/Arduino_Uno.jpg" alt="Arduino Uno">

Figura 03: Plataforma Arduino Uno.

<p>Para visualização de dados e configurações de sistema fora usado um display LCD 16x2 como mostrado na figura 04.</p>
<img src="https://github.com/456789123/Carbonization_Campaign/blob/master/Fotos/display.jpg" alt="Display 16x4">

Figura 04: Display 16x4.

<p>Para controle da solenoide está sendo usado um fotoacoplador optoacoplador MOC3021 associado com o TRIAC BTA26 600Volts por 26 Amperes.</p>
<p>O circuito de acoplamento está sendo mostrado na figura 05.</p>
<img src="https://github.com/456789123/Carbonization_Campaign/blob/master/Fotos/foto_triac.gif" alt="Circuito optoacoplador">

Figura 05: Circuito optoacoplador.

<p>Abaixo é mostrado o TRIAC BTA26 junto com o MOC3021</p>

<img src="https://github.com/456789123/Carbonization_Campaign/blob/master/Fotos/triac.jpg" alt="Circuito optoacoplador">
<img src="https://github.com/456789123/Carbonization_Campaign/blob/master/Fotos/moc3021.jpg" alt="Circuito optoacoplador">

Figuras 06 e 07: TRIAC BTA26 e o optoacoplador MOC3021.

<p>Na figura 08 pode visualizar o esquema do circuito do controle da câmara.</p>

<img src="https://github.com/456789123/Carbonization_Campaign/blob/master/Fotos/Esquema_Controle.png" alt="Esquema do circuito de controle">

Figuras 08. Esquemário do circuito de controle.

<p>Para o sensor COZIR GC-0016 (0-100%) será acoplado nos pino 19 para RX e 18 para RX com BAUDRATE de 9600 Bauds para leitura de Gás CO² padrão do fabricante.</p>


<h3>Configuração de Menu</h3>

<p>Aqui é mostrada a tela principal que mostra a concentração de Gás, temperatura e umidade no interior da câmara. Figura 09</p>

<img src="https://github.com/456789123/Carbonization_Campaign/blob/master/Fotos/Tela_principal.png" alt="Tela principal">
Figura 09. Tela principal.

<p>Ao aperta o botão de Menu entrará nas configurações de parâmetros.</p>
<p>Os botões UP e DOW são para navegação do MENU. As três telas estão disponíveis assim que perscionado o botão MENU conforme as figuras de 10 a 13.</p>

<img src="https://github.com/456789123/Carbonization_Campaign/blob/master/Fotos/Primeira_tela.png">
<img src="https://github.com/456789123/Carbonization_Campaign/blob/master/Fotos/Segunda_Tela.png">
<img src="https://github.com/456789123/Carbonization_Campaign/blob/master/Fotos/Terceira_Tela.png">
Figuras de 10 a 13. Apresentações de tela de Menu.

<p>Na primeira tela entrará para a configuração de controle de gás como mostra na figura 14.</p>

<img src="https://github.com/456789123/Carbonization_Campaign/blob/master/Fotos/Parametros_gas.png">
Figura 14. Tela de controle de gás.

<p>Nesta Tela é apresentado o limite da concetração de gás no interior da câmara. Logo abaixo é mostrado a configuraração para o controle de histerese para a concentração do gás como explicado no link: <a>https://en.wikipedia.org/wiki/Hysteresis</a>
Logo abaixo é mostrada a opção de Habilitar a solenóide para se torna ativa(SIM). Para desativar a solenoide deixará em (NAO).</p>

<p>Na segunda tela Controle de Umidade, estará disponivel o controle automatico de umidade que é parecido com o controle de gás. Figura 15</p>
<h5>Por enquanto este controle não está atuando com propositos de funcionar no futuro.</h5>

<img src="https://github.com/456789123/Carbonization_Campaign/blob/master/Fotos/Parametros_umidade.png">
Figura 15. Tela de controle de umidade.

<p>Na terceira tela irá mostrar as opções de testes para os controles para verificação dos circuitos de alta tensão. Poderá testar a selenoide de gás, controle de umidade e controle da bomba de vácuo para verificar se as mesmas estão funcionando perfeitamente. Figura 16</p>

<img src="https://github.com/456789123/Carbonization_Campaign/blob/master/Fotos/Testes.png">
Figura 16. Tela de controle de testes.
