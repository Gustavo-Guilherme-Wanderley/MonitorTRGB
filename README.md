# MonitorTRGB (Monitoramento em Tempo Real de Gás do Botijão)

<img src="https://user-images.githubusercontent.com/77367556/235567022-f3dd2792-9bad-4b84-8a0c-21cdb4caf1b5.jpeg" width="480">

> Status: Em desenvolvimento ⚠️

### Este é um projeto de desenvolvimento embarcado baseado na plataforma ESP32.

## Componentes Utilizados

- ESP32
- Módulo HX711
- Células de carga
- Display LCD

## Pinagem

| Componente      | Pino        | Conexão ESP32 |
|-----------------|-------------|---------------|
| Célula de carga | Vermelho (E+)| HX711 E+      |
| Célula de carga | Preto (E-)   | HX711 E-      |
| Célula de carga | Branco (A+)  | HX711 A+      |
| Célula de carga | Verde (A-)   | HX711 A-      |
| HX711           | DT           | GPIO 33       |
| HX711           | SCK          | GPIO 27       |
| Display LCD     | SDA          | GPIO 21       |
| Display LCD     | SCL          | GPIO 22       |

## Instruções

1. **Preparação do Hardware:**
   - Conecte a célula de carga ao amplificador HX711 conforme descrito na tabela de pinagem.
   - Conecte o amplificador HX711 ao ESP32 usando os pinos DT e SCK conforme descrito.
   - Conecte o display LCD ao ESP32 usando os pinos SDA e SCL conforme descrito.

2. **Configuração do Ambiente de Desenvolvimento:**
   - Instale a IDE Arduino.
   - Instale as bibliotecas necessárias: `HX711_ADC`, `WiFi`, `Blynk`, e `LiquidCrystal_I2C` através do gerenciador de bibliotecas da IDE Arduino.

3. **Programação do ESP32:**
   - Configure o ESP32 para se conectar ao WiFi utilizando a biblioteca `WiFi.h`.
   - Utilize a biblioteca `HX711.h` para ler os valores da célula de carga.
   - Calibre a balança utilizando um objeto de peso conhecido.
   - Envie os valores para o Blink Cloud utilizando a biblioteca `Blink.h`.

## Protótipo Físico

<img src="https://user-images.githubusercontent.com/77367556/236104437-072ee360-49c5-4a3a-b271-420aaec134cf.jpg" width="480">

> Em breve mais informações ⚠️

## Código

### Configuração Inicial

```cpp
#define BLYNK_TEMPLATE_ID "TMPL-zfaCATI"
#define BLYNK_DEVICE_NAME "Monitor de Peso"
#define BLYNK_AUTH_TOKEN "" // Seu Token de autenticação

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Suas credenciais de WIFI
// Login e Senha
char ssid[] = "";
char pass[] = "";
