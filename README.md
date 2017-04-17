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

<p>Para mensuração de Humidade e temperatura, fora usado o sensor DHT11 mostrado na figura 2.</p>

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


Figura 06: TRIAC BTA26 e o optoacoplador MOC3021.
