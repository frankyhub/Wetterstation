/*
 * Wetterstation Temperatur, Luftdruck, Luftfeuchte, Meereshöhe
 * Board: NANO
 * Board Einstellung
*  Old Bootloader!
*  KHF 22.01.2021
*  1x Nano V3
*  1x BME280 Sensor
*  1x 1,3 Zoll OLED Display
*  Schriftarten: https://arduino-projekte.info/schriftarten-fuer-oled-display/#torussansbold8_8r
*   Das Display und der Sensor werden am A4(SDA) und am A5(SCL) angeschlossen
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h> //V2.1.2

#define SEALEVELPRESSURE_HPA (1013.25)

#include <U8g2lib.h> //V2.28.10

Adafruit_BME280 bme; // I2C
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

unsigned long delayTime;

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
//    u8g2.setFont(u8g2_font_courR14_tf);
//    u8g2.setFont(u8g2_font_courR08_tf);
    u8g2.setFont(u8g2_font_courR10_tf);
    u8g2.firstPage();
    do {

      
      u8g2.setCursor(8, 14);
      u8g2.print("KHF Barometer");
      u8g2.setCursor(62, 14);
     
      
      
      u8g2.setCursor(2, 30);
      u8g2.print("Temp: ");
      u8g2.setCursor(48, 30);
      u8g2.print(bme.readTemperature());
      u8g2.print(" \xB0""C");
  


      u8g2.setCursor(2, 46);
      u8g2.print("LuFe:");
      u8g2.setCursor(48, 46);
      u8g2.print(bme.readHumidity());
      u8g2.print(" %");

      u8g2.setCursor(2, 62);
      u8g2.print("Ludr:");
      u8g2.setCursor(48, 62);
      u8g2.print(bme.readPressure()/100);
      u8g2.print("hPa");

/*
      u8g2.setCursor(2, 62);
      u8g2.print("Meho:");
      u8g2.setCursor(62, 62);
      u8g2.print(bme.readAltitude(SEALEVELPRESSURE_HPA)/1.21);
*/ 
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
