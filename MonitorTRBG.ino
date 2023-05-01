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

#include <HX711_ADC.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

const int HX711_dout = 33; //mcu > HX711 dout pin, must be external interrupt capable!
const int HX711_sck = 27; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;
volatile boolean newDataReady;

float i = 0;
float p = 0;

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
  //LoadCell.setReverseOutput();
  unsigned long stabilizingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }

  attachInterrupt(digitalPinToInterrupt(HX711_dout), dataReadyISR, FALLING);
}

//interrupt routine:
void dataReadyISR() {
  if (LoadCell.update()) {
    newDataReady = 1; 

  }
}

void loop() {
  Blynk.run();
  
  const int serialPrintInterval = 500; //increase value to slow down serial print activity

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      i = LoadCell.getData();
      newDataReady = 0;
      Serial.print("Load_cell output val: ");
      p=i/1000;
      Serial.println(p);  
      lcd.clear(); 
      lcd.print(p);
      lcd.print(" Kg");
      Blynk.virtualWrite(V0, p);      
      //Serial.print("  ");
      //Serial.println(millis() - t);
      t = millis();
    }
  }

  // receive command from serial terminal, send 't' to initiate tare operation:
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay();
  }

  //check if last tare operation is complete
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }
}
