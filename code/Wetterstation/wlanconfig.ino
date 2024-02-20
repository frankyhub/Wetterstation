/* ***********************************************************************
 *  Dies dient zur Konfiguration und zum Aufbau einer WLAN-Verbindung
 *  
 *  - Laden der Zugangsdaten aus dem Flash
 *       - ssid
 *       - password
 *       - ntpserver
 *  - Versuch eine Verbindung mit diesen Anmeldeinformationen herzustellen
 *  - Wenn eine Verbindung möglich ist, wird folgendes passieren:
 *      - eine Änderung im AP-Modus (NICHT neu gestartet!!!)
 *      - Aufruf des WLAN-Managers http://192.168.4.1
 *      - zeigt HTML-Code aus wlanconf.h mit ausgefüllten Anmeldeinformationen an
 *      - Benutzer speichert die Zugangsdaten
 *      - Beim Absenden des Formulars per Absenden werden die Anmeldeinformationen in Flash aktualisiert (ONLÝ THEN*)
 *      - Startet im Stationsmodus neu und beginnt den nächsten Versuch, sich mit den neuen Zugangdaten zu verbinden
 *  => Dies geschieht unendlich, bis eine Verbindung hergestellt werden kann.
 *      
 *  Es wird beim Aufruf von "makeWLAN()" (usaly in 'setup()') verwendet.
 *  Es wirdt "true" zurückgegeben, wenn die Verbindung erfolgreich war
 *  
 *  Libraries:
 *    - Wifi.h
 *    - Preferences.h
 *    - ESPAsyncWebServer.h
 *  
 *  Die WLAN-Instanz heißt "WiFi"
 *  Die Webserver-Instanz (nur im AP-Modus) heißt "Server"
 *  
 *  Wenn TESTMODUS wahr ist, wird nach einer erfolgreichen Verbindung ein falsches Passwort in Flash gespeichert. Also
 *  es stellt nach dem nächsten Neustart keine Verbindung her und startet den Vorgang von Anfang an.
 *  
 *  *: Wenn das WLAN offline ist und der ESP32 deshalb nach neuen Daten fragt, kannst du es ignorieren.
 *     Starte den ESP32 neu, wenn dein WLAN wieder verfügbar ist. Die Anmeldeinformationen ändern sich erst, wenn
 *     sie neu abgespeichert wurden.
 */

#include <WiFi.h>
#include <Preferences.h>
#include "ESPAsyncWebServer.h" //PathVariableHandlers by Chris Mullins 2.0.0

#include "wlanconf.h"  //HTML-code fuer WLAN-setup

#define TESTMODUS false

// Webserver on port 80
AsyncWebServer server(80);

// WLAN credentials
Preferences preferences;
String ssid;
String password;
String ntpserver;
int    bootAsAP;
#define MAXWLANTRY 10  // AP-Modus nach neuen Anmeldeinformationen fragen

// ### Im 'setup() aufrufen'
int makeWLAN()
{
  preferences.begin("credentials", false);

  ssid = preferences.getString("ssid", ""); 
  password = preferences.getString("password", "");
  ntpserver = preferences.getString("ntpserver", "de.pool.ntp.org");
    
  delay(250);
  Serial.println(F("Wetterstation: "));
  Serial.println(ssid);

  int tryCount = 0;
  if (ssid == "" || password == ""){
    Serial.println("Keine WLAN-Logindaten gespeichert!");
    ssid = "blub"; password = "blah";
    tryCount = MAXWLANTRY;
  }

  if (tryCount < MAXWLANTRY) {
    // Mit Wi-Fi verbinden
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Verbinde mit WiFi ..");
    while ((WiFi.status() != WL_CONNECTED) && (tryCount<MAXWLANTRY)) {
      Serial.print('.');
      delay(1000);
      tryCount++;
    }
    IPAddress lip(0,0,0,0);
    if (WiFi.localIP() == lip) {
      tryCount = MAXWLANTRY;
    }
  }

  if (tryCount < MAXWLANTRY) {
    Serial.print(F("IP-Adresse per DHCP ist "));
    Serial.println(WiFi.localIP());  
    // Für den Automatik-Test
    if (TESTMODUS) {
      preferences.putString("password", "XXX"); 
    }
    return(true);   // Wenn die Verbindung in Ordnung ist, stoppe hier und kehre zurück
  }

  // Wenn keine Verbindung möglich ist, starte die Konfigurationswebsite im AP-Moduse 
  makeConfigAP();

  return(false);    
                   
}


// ### Baut eine eigene "WLAN-Wetterstation" auf und startet unter http://192.168.4.1 ein Webinterface
// Diese Funktion läuft in der Schleife, bis neue Anmeldeinformationen gegeben sind - in diesem Fall startet sie neu
void makeConfigAP() {

  Serial.print("Erstelle AP (Access Point)…");
  WiFi.mode(WIFI_AP);
  delay(500);  
  WiFi.softAP("Wetterstation");

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.on("/", HTTP_ANY, [](AsyncWebServerRequest *request){

    int paramsNr = request->params();
    Serial.println(paramsNr);
    String ssid = "";
    String password = "";
    int paramCountOk = 0;
    int gotAnswer = false;
  
    for(int i=0;i<paramsNr;i++){
  
      AsyncWebParameter* p = request->getParam(i);
      Serial.print("Param name: ");
      Serial.println(p->name());
      Serial.print("Param value: ");
      Serial.println(p->value());
      Serial.println("------");
      if (p->name() == "ssid") {
        ssid = p->value();
        paramCountOk++;
      }
      if (p->name() == "password") {
        password = p->value();
        paramCountOk++;
      }
      // is used for my clock-projects :) and
      // here commented out in HTML-code.
      if (p->name() == "ntpserver") {   
        ntpserver = p->value();
        paramCountOk++;
      }

      if (paramCountOk >= paramsNr) {
        preferences.putString("ssid", ssid); 
        preferences.putString("password", password); 
        preferences.putString("ntpserver", ntpserver); 
        preferences.putInt("bootAP", 0); 
        preferences.end();
        gotAnswer = true;
      }
    }
  
    String s = MAIN_page; //HTML-Inhalte lesen
    if (gotAnswer) {
      s.replace("*mark1begin*", "<!--");
      s.replace("*mark1end*", "-->");
      s.replace("*feedback*", "<div style='color:#CC0000'><h1>Verbindung wird versucht...</h1><h2>Sollte die Wetterstation nicht starten, bitte in 20 Sek. wieder mit dem WLAN Wetterstation verbinden und diese Seite neu aufrufen.</h2></div>");
    } else {
      s.replace("*mark1begin*", "");
      s.replace("*mark1end*", "");
      s.replace("*feedback*", "");
      s.replace("*ssid*", preferences.getString("ssid", ""));
      s.replace("*password*", preferences.getString("password", ""));
      s.replace("*ntpserver*", preferences.getString("ntpserver", ""));
    }
    request->send(200, "text/html", s);
    if (gotAnswer) {
      delay(3000);
      ESP.restart();
    }

  }); 
  server.begin();        // Server start
  Serial.println("HTTP-Server gestartet");

  serverLoop();
}

// ### Eine einfache Schleife zur Anzeige des Status auf dem seriellen Monitor, dass der WLAN-Manager läuft
void serverLoop() {
  while (true) {
    delay(1000);
    Serial.println("WLAN-Netzwerk Wetterstation waehlen und 192.168.4.1 im Browser starten ...");
  }
}
