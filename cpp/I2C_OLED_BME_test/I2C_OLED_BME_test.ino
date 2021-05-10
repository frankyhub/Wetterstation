/*
 * I2C-NANO BEISPIEL
 * Board: NANO
 * Board Einstellung
 * https://arduino.esp8266.com/stable/package_esp8266com_index.json
*  https://dl.espressif.com/dl/package_esp32_index.json
*  Old Bootloader!
*  KHF 18.01.2021
*  1x Nano V3 / STM32 Mikrocontroller 
*  1x BME280 Sensor
*  1x 1,3 Zoll OLED Display
 */




#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

#include <U8g2lib.h>

Adafruit_BME280 bme; // I2C

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

unsigned long delayTime;

char var1[] = "12345678910"; //Zeile 1
char var2[] = "ABCDEFGHIJK"; //Zeile 2
char var3[] = "12345678910"; //Zeile 3
char var4[] = "ABCDEFGHIJK"; //Zeile 4

void setup() {
    Serial.begin(9600);
    u8g2.begin();
    Serial.println(F("BME280 test"));
    bool status;
    status = bme.begin(0x76);  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    
    Serial.println("-- Default Test --");
    delayTime = 1000;
    Serial.println();
}

void loop() { 
    printValues();
    u8g2.setFont(u8g2_font_courR14_tf);
    u8g2.firstPage();
    do {
      u8g2.setCursor(2, 14);
      u8g2.print("Temp: ");
      u8g2.setCursor(62, 14);
      u8g2.print(bme.readTemperature());
      u8g2.print("°");
      u8g2.setCursor(100, 14);


      u8g2.setCursor(2, 46);
      //u8g2.print("12345678910");
      u8g2.print (var3);
      u8g2.setCursor(2, 62);
      //u8g2.print("ABCDEFGHIJK");
      u8g2.print (var4);      



      
      } while ( u8g2.nextPage() );
    delay(delayTime);
}

void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");
    Serial.print("Pressure = ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");
    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");
    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");
    Serial.println();
}
