<a name="oben"></a>

# Wetterstation
Wetterdaten, Raumklima, Datum und Uhrzeit

## Story:
Diese Anleitung beschreibt den Aufbau einer Wetterstation in einem Holzgehäuse mit einem ESP32. Beim ESP32 ist ein DHT11-Sensor und am I²C-Bus ein OLED 1,3"-Display angeschlossen. Die Wetterstation beinhaltet einen WiFi-Manager für die Einstellung der WLAN-Zugangsdaten (SSID und Passwort). Nach den ersten Start der Wetterstation, muss in den WLAN-Einstellungen des Handy oder Tablet das Netzwerk "Wetterstation" ausgewählt und über einen Browser die IP-Adresse 192.168.4.1 aufgerufen werden. Danach startet die "Wetterstation WLAN-Einstellung" und die Zugangsdaten für das WLAN können eingetragen werden. Sind die Zugangsdaten korrekt, startet die Wetterstation.
Die Wetterstation zeigt beim Start für 5 Sekunden einen Smiley. In dieser Zeit werden die aktuelle Uhrzeit, das Datum und die aktuellen Wetterdaten aus der Region Tegernsee von de.pool.ntp.org und von openweathermap.org geladen.
Sind die Daten geladen, verschwindet der Smiley und es werden in regelmäßigen Abständen drei Seiten mit Datum und Uhrzeit, danach das Raumklima mit Temperatur und Luftfeuchte, zuletzt die Wetterdaten Außen-Temperatur, Windgeschwindigkeit und Luftdruck angezeigt.

![Bild](/png/Wetterstation.png)

## Hardware:
Das benötigen wir für die Wetterstation:


| Anzahl | Bezeichnung | 
| -------- | -------- | 
| 1        | ESP32       |
| 1       | ESP32-Shield        | 
| 20      | Stift/Buchsenleiste       | 
| 1       | DHT11-Sensor       | 
| 1       | 10k Widerstand      | 
| 1       | OLED Display 1,3" I²C        | 
| 1       | Gas-Sensor (optional)        |
| 1       | LED rot, gelb grün (optional)        |
| 1      | Batterieclip       | 
| 1      | 9V Batterie oder ein 8V Netzteil       | 
| 1      |Sperrholzplatte 4mm x 600x300        | 
| 1      |3D-Druck Teile für den ESP32, das OLED-Display und den DHT11        | 
| 1      | Schaltdraht      | 
| 1      | Sekundenkleber      | 


## Der Aufbau erfolgt in 5 Schritten:

1. Schritt 3D-Druckteile zeichnen und drucken

+ 3D-Druckteile zeichnen und drucken
+ 4 x ESP32-Pins d=5mm h=7mm
+ 4 x OLED-Display Pins 10x4x1,6mm
+ 1 x DHT11-Halterung 24x8x21 mit DHT11 Aussparung

![Bild](/png/3D.png)

---

2. Schritt Gehäuse lasern

+ Wir erstellen mit [BOXES.PY](https://festi.info/boxes.py/) eine Lasercutter Vorlage
+ Wähle die ClosedBox - Fully closed box mit den Maßen x=100, y=80, h=80 und thickness=4
+ Zusätzlich wird eine Aussparung für den DHT11-Sensor in der Rückwand und
+ eine Aussparung für den Spannungsanschluss des ESP32 in der Seitenwand eingefügt
+ Wir lasern mit den Einstellungen Geschw./Leistung = 12.0/100 in einem Durchgang


![Bild](/png/lasercutter.jpg)


---

3. Schritt Verdrahten

+ Wir löten auf das ESP32-Shield die Buchsenleisten
+ Wir schließen mit Schaltdraht den DHT11-Sensor am ESP32-Shield GPIO27 an
+ Am ESP32-Shield GPIO27 wird noch ein 10kΩ Pullupwiderstand gegen +5V angelötet
+ Wir verdrahten den DHT11 mit +5V und GND
+ Wir verbinden mit Schaltdraht den I²C-Bus mit dem Display
+ Dazu werden die Lötpunkte SLA, SLC, 3.3V und GND mit dem Display verbunden
+ Ist der ESP32 auf dem Shield, der Sensor und das Display angelötet, erfolgt der erste Test
+ Wir laden einen I²C-Scanner in den ESP32 und kontrollieren ob der ESP32 die Busteilnehmer erkennt
+ Wird das Display erkannt, laden wir das Haupt-Programm in den ESP32
  

![Bild](/png/dht11.png)


![Bild](/png/espshield.png)

![Bild](/png/wetter5.JPG)

---

4. Schritt Montage

+ Mit Sekundenkleber die 3D-Druck Bauteilfüße auf den ESP32 kleben
+ Die 3D-Druck Leisten auf das OLED-Display kleben
+ Den ESP32 auf die Grundplatte und die Sensorhalterung an die Gehäusefront kleben
+ VORSICHTIG das Display an die Frontseite (Durchbruch) kleben
+ Vor dem finalen verkleben des Gehäuses erfolgt ein Funktionstest
+ War der Funktionstest erfolgreich, kann das Gehäuse vollständig verklebt werden


![Bild](/png/wetter7.JPG)  

![Bild](/png/wetter8.JPG)  

![Bild](/png/wetter9.JPG)  


Erster Funktionstest am Seriellen Monitor:

![Bild](/png/wetter10.png)  

---

5. Schritt Inbetriebnahme

+ Im Handy oder Tablet die WLAN-Einstellungen öffnen
+ Das Netzwerk "Wetterstation" aufrufen
+ Im Browser die IP-Adresse 192.168.4.1 aufrufen
+ Die Seite "Wetterstation WLAN Einstellung" öffnet sich
+ Die WLAN-Zugangsdaten SSID und Passwort eingeben
+ Der Zeitserver ist voreingestellt und muss nicht verändert werden
+ Ist die WLAN-Verbindung korrekt, startet die Wetterstation mit einem Smiley am Display
+ Danach werden abwechselnd das aktuelle Datum mit Uhrzeit, danach das Raumklima mit Temperatur und Luftfeuchte und die aktuellen Wetterdaten aus der Region Tegernsee angezeigt
+ Die angezeigten Daten wechseln in regelmäßigen Abständen

Aufruf "Wetterstation" in den WLAN-Einstellungen

![Bild](/png/ws_ap_einstellung.png)  


Im Browser die IP-Adresse 192.168.4.1 öffnen, die WLAN Zugangsdaten eingeben und speichern

![Bild](/png/ws_wlaneinstellung2.png)  

Sind die WLAN-Zugangsdaten korrekt, startet die Wetterstation

![Bild](/png/ws_wlaneinstellung3.png)  

Startet die Wetterstation nicht, muss der Vorgang wiederholt werden.
Kontrolle am Seriellen Monitor:

![Bild](/png/wetter11.png)  

![Bild](/png/wetter12.png)  

Fertig, Glückwunsch!

---

## Troubleshooting, mögliche Fehlerursachen

+ Sichtkontrolle - sind alle Lötstellen ok?
+ Sind Lötbrücken zusehen oder sind Bauteilfüße fälschlicherweise mit einander verbunden?
+ Sitzt der ESP32 korrekt auf dem Shield? Auf die Polung achten und und Bauteilfüße kontrollieren.
+ Sind die Anschlussdrähte zum Sensor und Display ok?
+ Ist die Spannungsversorgung für den ESP32 ok?
+ Wurde das Programm korrekt geladen? Meldung: Hochladen abgeschlossen.
+ Wird das Display vom ESP32 erkannt? Mit dem I²C-Scanner prüfen.
+ Werden die Sensordaten und die Uhrzeit/Datum ausgelesen? Mit dem Seriellen Montitor der Arduino IDE prüfen.
+ Wurden die WLAN-Zugangsdaten korrekt eingegeben?

---

## Abkürzungen

+ OLED - Organic Light Emitting Diode
+ u8g2 Library - Universal 8Bit Graphics Library
+ I²C - Inter-Integrated Circuit (Serieller Datenbus)
+ SCL - Serial Clock
+ SDA - Serial Data

---

## C++ Programm I2C Scanner

```C++
// ESP32 I2C Scanner 
// ESP32 DevKit / ESP32vn IoT UNO - Arduino IDE 1.8.
#include <Wire.h>
void setup()
{
  Serial.begin (115200);  
  Wire.begin (21, 22);   // sda= GPIO_21 /scl= GPIO_22
}
void Scanner ()
{
  Serial.println ();
  Serial.println ("I2C scanner. Scanning ...");
  byte count = 0;
  Wire.begin();
  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission (i);          // Begin I2C transmission Address (i)
    if (Wire.endTransmission () == 0)  // Receive 0 = success (ACK response) 
    {
      Serial.print ("Found address: ");
      Serial.print (i, DEC);
      Serial.print (" (0x");
      Serial.print (i, HEX);     // PCF8574 7 bit address
      Serial.println (")");
      count++;
    }
  }
  Serial.print ("Found ");      
  Serial.print (count, DEC);        // numbers of devices
  Serial.println (" device(s).");
}
void loop()
{
  Scanner ();
  delay (100);
}
```
  


---
<div style="position:absolute; left:2cm; ">   
<ol class="breadcrumb" style="border-top: 2px solid black;border-bottom:2px solid black; height: 45px; width: 900px;"> <p align="center"><a href="#oben">nach oben</a></p></ol>
</div>

---

  




   
