# MonitorTRGB (Monitoramento em Tempo Real de Gás do Botijão)

<img src="https://user-images.githubusercontent.com/77367556/235567022-f3dd2792-9bad-4b84-8a0c-21cdb4caf1b5.jpeg" width="480">

> Status: Em desenvolvimento ⚠️

### Este é um projeto de desenvolvimento embarcado baseado na plataforma ESP32.

## Componentes Utilizados

- ESP32
- Módulo HX711
- Células de carga
- Display LCD(Com I2C)

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
```
### Bibliotecas e Pinagem
```
#include <HX711_ADC.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

const int HX711_dout = 33; // mcu > HX711 dout pin, must be external interrupt capable!
const int HX711_sck = 27; // mcu > HX711 sck pin

// HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);
```
### Variáveis Globais e Configuração da Célula de Carga
```
const int calVal_eepromAdress = 0;
unsigned long t = 0;
volatile boolean newDataReady;

float i = 0;
float p = 0;
```

### Configuração do Setup
```
void setup() {
  Blynk.begin(auth, ssid, pass);
  
  lcd.init();  
  lcd.backlight();
  Serial.begin(56700); 
  delay(10);
  Serial.println();
  Serial.println("Starting...");

  float calibrationValue; // calibration value
  calibrationValue = 696.0; // uncomment this if you want to set this value in the sketch
#if defined(ESP8266) || defined(ESP32)
  EEPROM.begin(512); // uncomment this if you use ESP8266 and want to fetch the value from eeprom
#endif
  EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the value from eeprom

  LoadCell.begin();
  // LoadCell.setReverseOutput();
  unsigned long stabilizingtime = 2000; // tare precision can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; // set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  } else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }

  attachInterrupt(digitalPinToInterrupt(HX711_dout), dataReadyISR, FALLING);
}
```
### Loop Principal
```
void loop() {
  Blynk.run();
  
  const int serialPrintInterval = 500; // increase value to slow down serial print activity

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      i = LoadCell.getData();
      newDataReady = 0;
      Serial.print("Load_cell output val: ");
      p = i / 1000;
      Serial.println(p);  
      lcd.clear(); 
      lcd.print(p);
      lcd.print(" Kg");
      Blynk.virtualWrite(V0, p);      
      // Serial.print("  ");
      // Serial.println(millis() - t);
      t = millis();
    }
  }

  // receive command from serial terminal, send 't' to initiate tare operation:
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }

  // check if last tare operation is complete
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }
}
```
> Em breve mais melhorias na documentação!
