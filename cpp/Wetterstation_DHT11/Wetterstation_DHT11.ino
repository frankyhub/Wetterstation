/*
 * Barometric Temperatur und Luftfeuchte
 * Board: NANO
 * Board Einstellung
*  Old Bootloader!
*  KHF 25.05.2021
*  1x Nano V3
*  1x DHT11 Sensor
*  1x 1,3 Zoll OLED Display
*  Schriftarten: https://arduino-projekte.info/schriftarten-fuer-oled-display/#torussansbold8_8r
 */

#include <SimpleDHT.h>
int pinDHT11 = 2;
SimpleDHT11 dht11(pinDHT11);

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <U8g2lib.h> //V2.28.10

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

unsigned long delayTime;

void setup() {
    Serial.begin(9600);
    u8g2.begin();
    }
    

void loop() { 

Serial.println("=================================");
Serial.println("Sample DHT11...");
float temperature = 0;
float humidity = 0;
int err = SimpleDHTErrSuccess;
if((err=dht11.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess){
Serial.print("Read DHT11 failed, err=");
Serial.println(err);
delay(2000);
return;
}


Serial.print("Sample OK: ");
Serial.print((float)temperature);
Serial.print(" *C, ");
Serial.print((float)humidity);
Serial.println(" RH%");
delay(1500); // DHT11 sampling rate is 1HZ.

    u8g2.setFont(u8g2_font_courR10_tf);
    u8g2.firstPage();

    do {
      
      u8g2.setCursor(8, 14);
      u8g2.print("KHF Barometer");
      
      u8g2.setCursor(2, 35); 
      u8g2.print("Temp: ");
      u8g2.setCursor(51, 35);
      u8g2.print(temperature);
      u8g2.print("\xB0""C");
  
      u8g2.setCursor(2, 51); 
      u8g2.print("LuFe:");
      u8g2.setCursor(51, 51);
      u8g2.print(humidity);
      u8g2.print("%");

      } while ( u8g2.nextPage() );
     delay(delayTime);
      }
