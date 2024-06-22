/*************************************************************************************************
                                      PROGRAMMINFO
**************************************************************************************************
  Funktion: ESP32 Wetterstation mit WLAN-Manager - Wetterdaten Tegernsee und Raumklima DHT11 OLED1,3"
            Gas-Sensor MQ135, Buzzer und Gas LED-Ampel
**************************************************************************************************
  Version: 22.06.2024
**************************************************************************************************
  Board: ESP32vn IoT UNO V1.0.4 /1.0.6

**************************************************************************************************
  C++ Arduino IDE V1.8.19
**************************************************************************************************
  Einstellungen:
  https://dl.espressif.com/dl/package_esp32_index.json
  http://dan.drown.org/stm32duino/package_STM32duino_index.json
  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json
**************************************************************************************************

  Server-Abfrage:
  http://api.openweathermap.org/data/2.5/weather?q=Tegernsee,DE&APPID=c2c58a80ce6377889800c284c540c505

  JSON-Meldung:
  {"coord":{"lon":11.7527,"lat":47.7097},
  "weather":
  [{"id":804,"main":"Clouds","description":"overcast clouds","icon":"04d"}],

  "base":"stations",
  "main":
  {"temp":288.35,
  "feels_like":288.54,
  "temp_min":285.19,
  "temp_max":291.7,
  "pressure":1016,
  "humidity":100,
  "sea_level":1016,
  "grnd_level":932},
  "visibility":273,
  "wind":{"speed":1.75,"deg":30,"gust":3.01},

  "clouds":{"all":100},
  "dt":1625628595,
  "sys":{"type":2,"id":2001431,"country":"DE","sunrise":1625628176,"sunset":1625685150},
  "timezone":7200,"id":2823679,"name":"Tegernsee","cod":200

  Beispiele für Schriftarten:
  Schrift­hö­he in Pixeln (px)
  6px: u8g2_font_5x7_tr
  7px: u8g2_font_torussansbold8_8r
  8px: u8g2_font_ncenB08_tr
  10px: u8g2_font_t0_15b_me
  12px: u8g2_font_helvB12_tf
  13px: u8g2_font_t0_22_te
  14px: u8g2_font_helvB14_tf
  17px: u8g2_font_timB18_tf
  18px: u8g2_font_lubB18_tr
  20px: u8g2_font_courB24_tf
  23px: u8g2_font_timB24_tf
  25px: u8g2_font_helvR24_tf
  32px: u8g2_font_logisoso32_tf
  42px: u8g2_font_fub42_tf
  58px: u8g2_font_logisoso58_tf
  62px: u8g2_font_logisoso62_tn

 **************************************************************************************************/

#include <SimpleDHT.h> //by Winlin 1.0.14       
int pinDHT11 = 27;
SimpleDHT11 dht11(pinDHT11);
#include <Adafruit_Sensor.h> //by Adafruit 0.8.0
#include <WiFi.h> //by Arduino 1.2.7
#include <WiFiUdp.h> 
#include <NTPClient.h> //Easy NTPC ba Harshen Alva 1.1.0
#include <HTTPClient.h> //Arduino HTTPClient by Arduino 0.4.0
#include <Arduino_JSON.h> //by Arduino 0.1.0

//------- Gas Sensor MQ135 ------------
int Buzzer = 26;        
int Gas_analog = 39;  
int Gas_digital = 2;   

#define led_rot 25
#define led_gelb 17
#define led_gruen 16
//----------------------------------------


String openWeatherMapApiKey = "c2c58a80ce6377889800c284c540c505";
String city = "Tegernsee";
String countryCode = "DE";

unsigned long lastTime = 0;
unsigned long timerDelay = 10000; //10 Sekunden
String jsonBuffer;

//-----------------------------------------------
#include <Arduino.h>
#include <U8g2lib.h>


#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
unsigned long delayTime;
//-----------------------------------------------
int ktemp;
int mintemp;
int maxtemp;
int sicht;

//----------------clock----------------------------
// Define NTP Client to get time

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "de.pool.ntp.org");


 // Internetzeit
#define MY_NTP_SERVER "de.pool.ntp.org"
#define MY_TZ "CET-1CEST,M3.5.0/02,M10.5.0/03" // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
time_t now;
tm tm;
void showTime() {
  time(&now);
  localtime_r(&now, &tm); // update the structure tm with the current time
   Serial.print("Jahr:");
    Serial.print(tm.tm_year + 1900); // years since 1900
    Serial.print("\tmonth:");
    Serial.print(tm.tm_mon + 1); // January = 0 (!)
    Serial.print("\tday:");
    Serial.print(tm.tm_mday); // day of month
    Serial.print("\thour:");
    Serial.print(tm.tm_hour); // hours since midnight 0-23
    Serial.print("\tmin:");
    Serial.print(tm.tm_min); // minutes after the hour 0-59
    Serial.print("\tsec:");
    Serial.print(tm.tm_sec); // seconds after the minute 0-61*
    Serial.print("\twday");
    Serial.print(tm.tm_wday); // days since Sunday 0-6
  
  Serial.println();
}


//Week Days
String weekDays[7] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};

//Month names
  String months[12] = {"Jan", "Feb", "M""\xE4""rz", "Apr", "Mai", "Juni", "Juli", "Aug", "Sept", "Okt", "Nov", "Dez"};
//-----------------------------------------------

void setup() {
  Serial.begin(115200);
   //init WiFi
  Serial.println("Connecting to WiFi");
  while (!makeWLAN()) {
    Serial.println("Cannot connect :(");
    delay(1000);
  }

//------- Gas Sensor MQ135 ------------
    pinMode(Buzzer, OUTPUT);      
    pinMode(Gas_digital, INPUT);
    pinMode(led_rot, OUTPUT);
    pinMode(led_gruen, OUTPUT);
    pinMode(led_gelb, OUTPUT);
//----------------------------------------  

  Serial.println("Aktualisierung ist auf 10 Sekunden eingestellt.");

  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600 Winterzeit
  // GMT +2 = 7200 Sommerzeit
  timeClient.setTimeOffset(7200);

  u8g2.begin();

  int BildschirmBreite = u8g2.getDisplayWidth();
  int BildschirmHoehe = u8g2.getDisplayHeight();



//Bitmap hier einfügen:
//Smiley XBM erstellt mit GIMP
//Smiley 50x50
#define smiley_width 50
#define smiley_height 50
static unsigned char smiley[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xfc, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff,
   0xff, 0x03, 0x00, 0x00, 0x00, 0xc0, 0x07, 0xc0, 0x0f, 0x00, 0x00, 0x00,
   0xf0, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x78, 0x00,
   0x00, 0x00, 0x1c, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00,
   0xc0, 0x01, 0x00, 0x80, 0x07, 0x00, 0x00, 0x80, 0x03, 0x00, 0x80, 0x01,
   0x08, 0xc0, 0x00, 0x07, 0x00, 0xc0, 0x01, 0x1c, 0xc0, 0x00, 0x0e, 0x00,
   0xe0, 0x00, 0x1c, 0xe0, 0x01, 0x0c, 0x00, 0x60, 0x00, 0x1e, 0xe0, 0x01,
   0x18, 0x00, 0x30, 0x00, 0x1e, 0xe0, 0x01, 0x38, 0x00, 0x30, 0x00, 0x3e,
   0xe0, 0x01, 0x30, 0x00, 0x18, 0x00, 0x1e, 0xe0, 0x01, 0x70, 0x00, 0x18,
   0x00, 0x1e, 0xe0, 0x01, 0x60, 0x00, 0x1c, 0x00, 0x1e, 0xe0, 0x01, 0x60,
   0x00, 0x0c, 0x00, 0x1c, 0xe0, 0x01, 0x60, 0x00, 0x0c, 0x00, 0x1c, 0xc0,
   0x01, 0xc0, 0x00, 0x0c, 0x00, 0x1c, 0xc0, 0x00, 0xc0, 0x00, 0x0c, 0x00,
   0x00, 0x80, 0x00, 0xc0, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00,
   0x06, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x0e, 0x04, 0x00, 0x00, 0x80,
   0xc1, 0x00, 0x0c, 0x03, 0x00, 0x00, 0x00, 0xc1, 0x00, 0x0c, 0x04, 0x00,
   0x00, 0x80, 0xc0, 0x00, 0x0c, 0x04, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x0c,
   0x04, 0x00, 0x00, 0x80, 0xc0, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x80, 0xe0,
   0x00, 0x1c, 0x08, 0x00, 0x00, 0x40, 0x60, 0x00, 0x18, 0x08, 0x00, 0x00,
   0x40, 0x60, 0x00, 0x18, 0x10, 0x00, 0x00, 0x20, 0x70, 0x00, 0x38, 0x20,
   0x00, 0x00, 0x10, 0x30, 0x00, 0x30, 0x40, 0x00, 0x00, 0x10, 0x38, 0x00,
   0x70, 0x80, 0x00, 0x00, 0x04, 0x18, 0x00, 0x60, 0x00, 0x01, 0x00, 0x02,
   0x1c, 0x00, 0xe0, 0x00, 0x06, 0x80, 0x01, 0x0c, 0x00, 0xc0, 0x01, 0x70,
   0x30, 0x00, 0x07, 0x00, 0x80, 0x03, 0x80, 0x05, 0x00, 0x07, 0x00, 0x00,
   0x07, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0x0e, 0x00, 0x00, 0xe0, 0x01,
   0x00, 0x00, 0x3c, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00,
   0x3c, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x80,
   0xbf, 0xf4, 0x07, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x01, 0x00, 0x00,
   0x00, 0x00, 0xd0, 0x2f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00 };
   
  u8g2.setFont(u8g2_font_courR10_tf);
  u8g2.firstPage();

  do {
    u8g2.clearBuffer();
    u8g2.drawXBM(40, 0, smiley_width, smiley_height, smiley);
        u8g2.setCursor(12, 62);
        u8g2.setFont(u8g2_font_courB08_tf);
        u8g2.print("Ich lade die Daten");
   } while ( u8g2.nextPage() );
  delay(3000);
  
}//Ende Setup

  
void loop() {


  //----------------Clock----------------------
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  showTime();
  //  Serial.print("Epoch Time: ");
  //  Serial.println(epochTime);

  String formattedTime = timeClient.getFormattedTime();
  // Serial.print("Formatted Time: ");
  // Serial.println(formattedTime);

  int currentHour = timeClient.getHours();
  char stunden = timeClient.getHours();


  //  Serial.print("Hour: ");
  //  Serial.println(currentHour);

  int currentMinute = timeClient.getMinutes();
  //  Serial.print("Minutes: ");
  //  Serial.println(currentMinute);

  int currentSecond = timeClient.getSeconds();
  //  Serial.print("Seconds: ");
  //  Serial.println(currentSecond);

  String weekDay = weekDays[timeClient.getDay()];
  char tag = timeClient.getDay();
   Serial.println("**************** Datum und Uhrzeit ********************");
   Serial.println(tag);

    Serial.print("Wochentag: ");
    Serial.println(weekDay);

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime);

  int monthDay = ptm->tm_mday;
    Serial.print("Tag: ");
    Serial.println(monthDay);

  int currentMonth = ptm->tm_mon + 1;
   Serial.print("Monat: ");
   Serial.println(currentMonth);

  String currentMonthName = months[currentMonth - 1];
    Serial.print("Monat: ");
    Serial.println(currentMonthName);

  int currentYear = ptm->tm_year + 1900;
    Serial.print("Jahr: ");
    Serial.println(currentYear);

  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
    Serial.print("Datum: ");
    Serial.println(currentDate);

    Serial.println("");

  //---------------------------------------------------------

  Serial.println("=================================");
  Serial.println("Abfrage DHT11 Raumklima:");
  float temperature = 0;
  float humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
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
  delay(5000); // DHT11 Abfragezeit

  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi Status
    if (WiFi.status() == WL_CONNECTED) {
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;

      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);
      JSONVar myObject = JSON.parse(jsonBuffer);

      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Eingabe fehlgeschlagen!");
        return;
      }
      ktemp = (myObject["main"]["temp"]);
      mintemp = (myObject["main"]["temp_min"]);
      maxtemp = (myObject["main"]["temp_max"]);

      //      Serial.print("JSON object = ");
      Serial.println("Meine Wetterdaten:");
      Serial.print("Ort: ");
      Serial.println(myObject["name"]);
      Serial.print("Aktuelle Temperatur: ");
      Serial.print(myObject["main"]["temp"]);
      Serial.println("K ");
      Serial.print("Aktuelle Temperatur: ");
      Serial.print(ktemp - 273.15);
      Serial.println("°C ");
      Serial.print("Tages-Min. Temperatur: ");
      Serial.print(mintemp);
      Serial.println("K ");

      Serial.print("Tages-Min. Temperatur: ");
      Serial.print(mintemp - 273.15);
      Serial.println("°C ");

      Serial.print("Tages-Max. Temperatur: ");
      Serial.print(maxtemp);
      Serial.println("K ");

      Serial.print("Tages-Max. Temperatur: ");
      Serial.print(maxtemp - 273.15);
      Serial.println("°C ");

      Serial.print("Wolken: ");
      Serial.print(myObject["clouds"]["all"]);
      Serial.println("% ");

      Serial.print("Luftdruck: ");
      Serial.print(myObject["main"]["pressure"]);
      Serial.println("hPa");
      Serial.print("Luftfeuchte: ");
      Serial.print(myObject["main"]["humidity"]);
      Serial.println("% ");
      Serial.print("Wind Geschwindigkeit: ");
      Serial.print(myObject["wind"]["speed"]);
      Serial.println("m/s ");

      sicht = (myObject["visibility"]);
      sicht = sicht / 100;
      Serial.print("Sicht: ");
      Serial.print(sicht);
      Serial.println("km ");

      //---------------------------------------------------------


//------- Gas Sensor MQ135 ------------
    int gassensorAnalog = analogRead(Gas_analog);
    int gassensorDigital = digitalRead(Gas_digital);
  delay(100);

  
  Serial.print("Gas Sensor: ");
  Serial.print(gassensorAnalog);
  Serial.print("\t");
  Serial.print(gassensorDigital);
  Serial.print("\t");
  Serial.print("\t");

  if (gassensorAnalog <= 300)  {
    Serial.println("kein Gas"); 
    digitalWrite(led_gruen, HIGH);        // OK   
  } else { 
    digitalWrite(led_gruen, LOW); }      

 
  if (gassensorAnalog >= 551 && gassensorAnalog <= 799) {
    digitalWrite(led_gelb, HIGH); 
        Serial.println("Gas erkannt");    
  } else {
    digitalWrite(led_gelb, LOW); }    
   

  
  if (gassensorAnalog > 800) { 
    Serial.println("Gas-Alarm");
    digitalWrite(led_rot, HIGH);         // Gas-Alarm
    digitalWrite (Buzzer, HIGH) ; //send tone

    } else {
    digitalWrite(led_rot, LOW);
    digitalWrite (Buzzer, LOW) ;  //no tone
    }
  delay(100);
//-----------------------------------------------------



      Serial.println("=================================");

      u8g2.setFont(u8g2_font_courR10_tf);
      u8g2.firstPage();


      do {
        u8g2.clearBuffer();
        u8g2.setCursor(6, 10);
        u8g2.print("Datum-Uhrzeit");
        u8g2.setFont(u8g2_font_courR10_tf);
        u8g2.setCursor(30, 27);
        u8g2.print(weekDay);
        u8g2.setCursor(3, 43);
        u8g2.print(monthDay);
        u8g2.print(". ");
        u8g2.print(currentMonthName);
        u8g2.print(" ");
        u8g2.print(currentYear);
        u8g2.setFont(u8g2_font_courB14_tf);
        u8g2.setCursor(40, 63);

        u8g2.print(currentHour);
        u8g2.print(":");
         if (currentMinute < 10) {
           u8g2.print("0");
           u8g2.print(currentMinute);
         }else{
            u8g2.print(currentMinute);
            }
  /*      
       u8g2.print(currentMinute);
        u8g2.print(":");


         if (currentSecond) < 10) {
           u8g2.print("0");
           u8g2.print(currentSecond);
         }else{
            u8g2.print(currentSecond);
          }
*/
        
//        u8g2.print(currentSecond);
        u8g2.setFont(u8g2_font_courR10_tf);
      } while ( u8g2.nextPage() );
      delay(7000);


      do {
        u8g2.clearBuffer();
        u8g2.setCursor(12, 10);
        u8g2.print("Raumklima");

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

        u8g2.setCursor(2, 63);
        u8g2.print("CO2:");
        u8g2.setCursor(51, 63);
        u8g2.print(gassensorAnalog);
        u8g2.print("ppm");

       } while ( u8g2.nextPage() );
      delay(6000);


      do {
        u8g2.clearBuffer();
        u8g2.setCursor(12, 10);
        u8g2.print("Wetterdaten");
        u8g2.setCursor(18, 22);
        u8g2.print("Tegernsee");

        u8g2.setCursor(2, 35);
        u8g2.print("Temp: ");
        u8g2.setCursor(51, 35);
        u8g2.print(maxtemp - 273.15);
        u8g2.print("\xB0""C");

        u8g2.setCursor(2, 49);
        u8g2.print("Wind:");
        u8g2.setCursor(51, 49);
        u8g2.print(myObject["wind"]["speed"]);
        u8g2.print("m/s");
        /*
              u8g2.setCursor(2, 48);
              u8g2.print("LuFe:");
              u8g2.setCursor(51, 48);
              u8g2.print(myObject["main"]["humidity"]);
              u8g2.print("%");
        */
        u8g2.setCursor(2, 63);
        u8g2.print("LuDr:");
        u8g2.setCursor(51, 63);
        u8g2.print(myObject["main"]["pressure"]);
        u8g2.print("hPa");

      } while ( u8g2.nextPage() );
      delay(1000);



      /* Cloudya:
       *   //--------- OLED Seite 1 -----------------------
      u8g2.firstPage();
      do {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_courB10_tf);
        u8g2.setCursor(25, 10);
        u8g2.print(weekDay);
        u8g2.setCursor(10, 27);
        u8g2.print(monthDay);
        u8g2.print(". ");
        u8g2.print(currentMonthName);
        u8g2.print(" ");
        u8g2.print(currentYear);
        u8g2.setFont(u8g2_font_courR10_tf);

        u8g2.setCursor(8, 46);
        u8g2.print("Wolken: ");
        u8g2.print(myObject["clouds"]["all"]);
        u8g2.print(" %");

        //------- IP-Adresse --------------
        u8g2.setFont(u8g2_font_courB08_tf);
        u8g2.setCursor(20, 64);
        u8g2.print(WiFi.localIP());


      } while ( u8g2.nextPage() );
      delay(8000);;

      //--------- OLED Seite 2 Wetterdaten -----------------------------
      do {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_courB10_tf);
        u8g2.setCursor(18, 10);
        u8g2.print("Wetterdaten");
        u8g2.setCursor(28, 22);
        u8g2.print("Tegernsee");
        u8g2.setFont(u8g2_font_courR10_tf);


        u8g2.setCursor(8, 35);
        u8g2.print("Temp: ");
        u8g2.print(maxtemp - 273.15, 1);
        u8g2.print("\xB0""C");

        u8g2.setCursor(8, 49);
        u8g2.print("Wind: ");
        //      u8g2.setCursor(51, 49);
        u8g2.print(myObject["wind"]["speed"]);
        u8g2.print("m/s");

        u8g2.setCursor(8, 63);
        u8g2.print("LuDr: ");
        //      u8g2.setCursor(51, 63);
        u8g2.print(myObject["main"]["pressure"]);
        u8g2.print("hPa");

      } while ( u8g2.nextPage() );
      delay(8000);

      //--------- OLED Seite 3 Wetterdaten -----------------------------
      do {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_courB10_tf);
        u8g2.setCursor(18, 10);
        u8g2.print("Wetterdaten");
        u8g2.setCursor(28, 22);
        u8g2.print("Tegernsee");
        u8g2.setFont(u8g2_font_courR10_tf);


        u8g2.setCursor(4, 36);
        u8g2.print("Feuchte: ");
        u8g2.print(myObject["main"]["humidity"]);
        u8g2.print("%");

        u8g2.setCursor(4, 50);
        u8g2.print("Wolken:  ");
        //      u8g2.setCursor(51, 49);
        u8g2.print(myObject["clouds"]["all"]);
        u8g2.print("%");

        u8g2.setCursor(4, 64);
        u8g2.print("minTemp:");
        u8g2.print(mintemp - 273.15, 1);
        u8g2.print("\xB0""C");

      } while ( u8g2.nextPage() );
      delay(8000);


      //--------- OLED Seite 4 Raumklima---------------------
      do {
        u8g2.setFont(u8g2_font_courB10_tf);
        u8g2.clearBuffer();
        u8g2.setCursor(24, 10);
        u8g2.print("Raumklima");
        u8g2.setFont(u8g2_font_courR10_tf);

        u8g2.setCursor(8, 36);
        u8g2.print("Temp: ");
        //        u8g2.setCursor(51, 30);
        u8g2.print(temperature - 1); //mit Korrektur -1°C
        u8g2.print("\xB0""C");

        u8g2.setCursor(8, 60);
        u8g2.print("LuFe: ");
        //        u8g2.setCursor(51, 44);
        u8g2.print(humidity);
        u8g2.print("%");

        u8g2.setCursor(8, 60);
        //       u8g2.print("Gas:  ");                                 // ************************** GAS SENSOR *****************************
        //        u8g2.setCursor(51, 60);
        //       u8g2.print(gassensorAnalog);
        //       u8g2.print("ppm");

      } while ( u8g2.nextPage() );
      //      delay(1000);
      //---------------------------------------------------------
    }
       */

      //---------------------------------------------------------
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  // Domain-Name
  http.begin(client, serverName);

  // HTTP POST request 200=OK
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP-Antwort Code (200=OK): ");
    Serial.println(httpResponseCode);
    Serial.println("Server-Meldung: ");
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();

  return payload;
}
