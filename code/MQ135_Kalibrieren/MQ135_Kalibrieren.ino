/*************************************************************************************************
                                      PROGRAMMINFO
**************************************************************************************************
Funktion: Gassensor MQ135 kalibirieren

**************************************************************************************************
Version: 21.06.2024
**************************************************************************************************
Board: ESP32vn IoT UNO
**************************************************************************************************
Libraries:
https://github.com/espressif/arduino-esp32/tree/master/libraries
C:\Users\User\Documents\Arduino
D:\gittemp\Arduino II\A156_Wetterdaten_V3
**************************************************************************************************
C++ Arduino IDE V1.8.19

**************************************************************************************************
Einstellungen:
https://dl.espressif.com/dl/package_esp32_index.json
http://dan.drown.org/stm32duino/package_STM32duino_index.json
http://arduino.esp8266.com/stable/package_esp8266com_index.json

Story:
Ziel dieses Projekts ist es, die CO2-Konzentration in Teilen pro Million zu messen, sie auf 
einem Bildschirm anzuzeigen und uns eine akustische Warnung zu geben, wenn sie eine gefährliche 
Konzentration erreicht, sowie uns über die Temperatur und Luftfeuchtigkeit zu informieren. Um die 
Lebensdauer des Bildschirms zu verlängern, hielt ich es für eine gute Option, drei verschiedenfarbige 
LEDs anzubringen, die uns eine schnelle visuelle Information über die Höhe der CO2-Konzentration geben, 
ohne dass wir die Daten sehen müssen. 

Datum, Uhrzeit und die Wetterdaten der Region Tegernse werden aus dem Internet aktulisiert.Der MQ-135-Sensor misst die CO2-Konzentration in 
Teilen pro Million, der DHT-11-Sensor misst die Temperatur und die Luftfeuchtigkeit. Diese drei Daten 
werden an einen zweifarbigen OLED-Bildschirm mit 1,3 Zoll gesendet, um jeden Wert für 3 Sekunden in 
Intervallen von 1 Minute anzuzeigen. Der Bildschirm bleibt während dieser Zeit ausgeschaltet. Diese 
Zeitwerte können konfiguriert werden. Während der Bildschirm ausgeschaltet bleibt, informieren uns 
die drei LEDs visuell über die vorhandenen CO2-Werte. Die grüne LED leuchtet, wenn die Konzentration 
gleich oder kleiner als 550 ist. Die gelbe LED leuchtet, wenn die Konzentration zwischen 551 und 799 
Teilen pro Million CO2 liegt und die rote LED leuchtet zusammen mit einem Summerton, wenn die Konzentration 
799 Teile pro Million übersteigt. Diese drei Werte können auch konfiguriert werden.
Es ist sehr wichtig, den Sensor unabhängig mit 5 VDC zu versorgen, da er 850 mW verbraucht.

Kalibrierung:
Als erstes muss der MQ-135-Sensor vorbereitet werden, damit er die richtigen Werte misst, die wir im 
nächsten Schritt verwenden werden. Dazu muss der Sensor im Neuzustand etwa 2 Stunden lang mit 5 Volt 
betrieben werden, damit die bei der Herstellung entstandenen Verunreinigungen beseitigt werden. 
Danach ist es am besten, den Referenzabgleich des RZERO-Widerstands in der endgültigen Schaltung 
vorzunehmen, da dies mit den normalerweise verwendeten Versorgungs- und Betriebsspannungen geschieht 
und die Messung optimal ist.
Ziel dieser Kalibrierung ist es, den Widerstand des atmosphärischen CO2-Niveaus so einzustellen, 
dass der MQ-135-Sensor eine Konzentration zwischen 300 und 450 ppm misst, was der normalen Konzentration 
dieses Gases in der Umwelt entspricht. 

Wenn alles angeschlossen ist, starten wir die Arduino-IDE, laden und starten den Sketch "MQ135_Kalibrieren.ino". 
Zuvor muss die MQ135-Bibliothek installiert werden. Wir öffnen den seriellen Monitor und sehen den ppm-Messwert; 
wir müssen den RZERO-Wert in der Datei MQ135.h im Bibliotheksordner der Arduino IDE ändern und speichern, bis der 
ppm-Messwert derjenige ist, den wir als Arbeitsreferenz der Schaltung belassen wollen. Der zu ändernde Wert ist 
der grün gefärbte Wert.

Um die Einstellung vorzunehmen, müssen wir den Wert von RZERO ändern, die Änderungen der Datei MQ135.h 
der Bibliothek in ihrem Ordner der Arduino IDE speichern und den Kalibrierungs-Sketch neu laden, den 
neuen Wert der CO2-Konzentration im seriellen Monitor überprüfen und den Vorgang wiederholen, um RZERO 
mit dem Wert einzustellen, den wir als Referenz für die CO2-Konzentration in der Luft für die Warngrenzen i
m Sketch unseres Projekts lassen wollen.


**************************************************************************************************/
#include "MQ135.h"
#define RZERO 1

MQ135 gasSensor = MQ135(39);
int val; 
int sensorPin = 39; 
int sensorValue = 0;

void setup() { 
  Serial.begin(115200);
  pinMode(sensorPin, INPUT); 
} 
 
void loop() { 
  val = analogRead(A0); 
  Serial.print ("raw = "); 
  Serial.println (val); 
  float zero = gasSensor.getRZero(); 
  Serial.print ("RZERO: "); 
  Serial.println (zero); 
  float ppm = gasSensor.getPPM(); 
  Serial.print ("ppm: "); 
  Serial.println (ppm); 
  delay(2000); 
}
