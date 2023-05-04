<h1>MonitorTRGB (Monitoramento em tempo real de gás do botijão)</h1>


<img src="https://user-images.githubusercontent.com/77367556/235567022-f3dd2792-9bad-4b84-8a0c-21cdb4caf1b5.jpeg" width="480">
> Status: Developing ⚠️

### Isto é um projeto de desenvolvimento embarcado baseado na plataforma ESP32

## Componentes utilizados:

+ Esp32
+ Módulo Hx711
+ Células de carga
+ Display

## Você precisará seguir algumas etapas.

+ Primeiro, você precisará de um ESP32, uma placa microcontroladora com WiFi integrado que pode ser programada usando a linguagem de programação Arduino. Você também precisará de uma célula de carga e um amplificador HX711 para converter o sinal analógico da célula de carga em um sinal digital que possa ser lido pelo ESP32.

+ Conecte a célula de carga ao amplificador HX711. A célula de carga terá quatro fios: vermelho, preto, branco e verde. Conecte o fio vermelho ao pino E+ do amplificador HX711, o fio preto ao pino E- do amplificador HX711, o fio branco ao pino A+ do amplificador HX711 e o fio verde ao pino A- do amplificador HX711.
<img src="https://user-images.githubusercontent.com/77367556/236104105-93fa12cd-73af-4bf2-aca2-544296846c40.png" width="480">

+ Conecte o amplificador HX711 ao ESP32. Conecte o pino DT do amplificador HX711 ao pino D5 do ESP32 e o pino SCK do amplificador HX711 ao pino D6 do ESP32.

+ Agora você pode começar a programar o ESP32 para ler o valor da célula de carga. Você precisará usar uma biblioteca de código aberto chamada HX711.h para se comunicar com o amplificador HX711. Esta biblioteca pode ser baixada e instalada usando o gerenciador de bibliotecas da IDE do Arduino.

+ Em seguida, você precisará configurar o ESP32 para se conectar ao WiFi usando a biblioteca WiFi.h. Certifique-se de que seu ESP32 esteja conectado a uma rede WiFi.

+ Agora você pode começar a ler o valor da célula de carga usando a função HX711.read(). Você precisará calibrar a balança para garantir que ela esteja lendo o valor correto da célula de carga. Isso pode ser feito pesando um objeto conhecido e ajustando o valor da balança até que corresponda ao peso conhecido.

+ Finalmente, você pode enviar o valor da balança para o Blink Cloud usando a biblioteca Blink.h. Certifique-se de que sua conta Blink esteja configurada corretamente e obtenha suas credenciais de API.

+ Envie o valor da balança para o Blink Cloud usando a função Blink.publish(). Certifique-se de que o valor seja enviado em um formato que possa ser lido pelo Blink Cloud.
