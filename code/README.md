<a name="oben"></a>

# Wetterstation mit Gassensor MQ135


![Bild](/pic/Wetterstaio_G.png)

## Story:
Ziel der Erweiterung um einen Gassensor ist es, die CO2-Konzentration in Teilen pro Million zu messen, sie auf 
einem Bildschirm anzuzeigen und uns eine akustische Warnung zu geben, wenn sie eine gefährliche 
Konzentration erreicht. Die Anzeige erfolgt auch über drei verschiedenfarbige 
LEDs, die schnell eine visuelle Information über die Höhe der CO2-Konzentration geben. 

Der MQ-135-Sensor misst die CO2-Konzentration in Teilen pro Million. Die Daten 
werden auch im zweifarbigen OLED-Bildschirm mit 1,3 Zoll gesendet, um jeden Wert in 
Intervallen anzuzeigen. Die grüne LED leuchtet, wenn die Konzentration 
gleich oder kleiner als 550 ist. Die gelbe LED leuchtet, wenn die Konzentration zwischen 551 und 799 
Teilen pro Million CO2 liegt und die rote LED leuchtet zusammen mit einem Summerton, wenn die Konzentration 
799 Teile pro Million übersteigt. Diese drei Werte können auch konfiguriert werden.
Es ist sehr wichtig, den Sensor unabhängig mit 5 VDC zu versorgen, da er 850 mW verbraucht.

## Kalibrierung:
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
mit dem Wert einzustellen, den wir als Referenz für die CO2-Konzentration in der Luft für die Warngrenzen im 
Sketch unseres Projekts lassen wollen.

<div style="position:absolute; left:2cm; ">   
<ol class="breadcrumb" style="border-top: 2px solid black;border-bottom:2px solid black; height: 45px; width: 900px;"> <p align="center"><a href="#oben">nach oben</a></p></ol>
</div>  
