#include <SPIFFS.h>
#include <SPI.h>
#include <SD.h>

#include <ArduinoJson.h>

#include <DHT.h>

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#else
#error Platform not supported
#endif
#include <ESPAsyncWebServer.h>

#include <NTPClient.h>
#include <WiFiUdp.h>

#include <ThingSpeak.h>

// parameters
uint32_t logTime = 30 * 1000; // in seconds * 1000
uint32_t pumpCheckTime = 1 * 1000; // in seconds * 1000
byte maxHumidity = 80;
byte minHumidity = 20;
byte maxWaterValue = 70;
byte minWaterValue = 5;

// pins
const byte pinSD = 5;
const byte soilMoistureSensorCount = 3;
const byte pinSoilMoisture[soilMoistureSensorCount] = {32, 32, 32}; // analog
const byte pinWaterSensor = 34; // analog
const byte pinDHT = 27;
const byte pinPump1 = 17;
const byte pinPump2 = 21;
const byte pinInfo = 15;

// internal variables
uint32_t logLastTime = 0; // for logging
uint32_t pumpChecklastTime = 0; // for pump control

// reading vars
float soilMoisture[soilMoistureSensorCount] = {0, 0, 0};
float soilMoistureAvg = 0;
float humidity = 0;
float temperature = 0;
float waterLevel = 0;

// pump variables
bool pumpState1 = false;
bool pumpState2 = false;

DHT dht(pinDHT, DHT22);

// json doc
const char* jsonTitles[] = {
  "time",
  "soilMoisture",
  "soilMoistureAvg",
  "hum",
  "temp",
  "waterLevel",
  "pump1",
  "pump2"
};
StaticJsonDocument<400> jsonDoc;
JsonArray soilMoistureJson;

// logging
const char* fileName = "/log.txt";
const char* configName = "/config.txt";
const byte maxBackups = 10;
byte currBackup = 0;

// wifi
const char* ssid = "WIFINAME";
const char* password = "WIFIPASSWORD";

// Define NTP Client to get time
String prettyDate = "";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// thingspeak
const uint16_t channelID = 1234567;
const char* channelApiKey = "AAAAAAAAAAAAAAAA";
WiFiClient client;

// web server and socket
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// info variables
bool sdStatus = true;
bool spiffsStatus = true;

void setup()
{
  // init serial
  Serial.begin(115200);

  // init pins
  pinMode(pinPump1, OUTPUT);
  pinMode(pinPump2, OUTPUT);
  pinMode(pinInfo, OUTPUT);

  InfoPin(true);

  // init dht
  dht.begin();

  // init SD
  Serial.print("Mounting SD card... ");
  if (!SD.begin(pinSD))
  {
    Serial.println("FAILED!");
    sdStatus = false;
  }
  Serial.println("done");
  CreateFile(SD, fileName);

  // init SPIFFS
  Serial.print("Mounting SPIFFS... ");
  if (!SPIFFS.begin())
  {
    Serial.println("FAILED!");
    spiffsStatus = false;
  }
  Serial.println("done");

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  Serial.print("connecting to ");
  if (!UseConfigCredentials(SD, configName))
  {
    Serial.print(ssid);  
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(250);
      Serial.print(".");
    }
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // init NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(-3 * 3600);

  // init thingspeak
  ThingSpeak.begin(client);

  // web client handler
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/styles.css", "text/css");
  });
  server.on("/graphs.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/graphs.html", "text/html");
  });
  server.on("/graphs.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/graphs.js", "text/javascript");
  });
  server.on("/config.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/config.html", "text/html");
  });  
  server.on("/config.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/config.js", "text/javascript");
  });
  server.on("/reading.html", HTTP_GET, [](AsyncWebServerRequest *request){
    UpdateSensors();
    request->send(SPIFFS, "/reading.html", "text/html");
  });
  server.on("/reading.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/reading.js", "text/javascript");
  });
  server.on("/download", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, fileName, String(), true);
  });
  server.on("/log.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/log.html", "text/html");
  });
  server.on("/info.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/info.html", "text/html", false, processor_info);
  });
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/favicon.ico", "image/ico");
  });  
  server.on("/preview", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SD, fileName, "text/plain");
  });
  server.on("/delete", HTTP_GET, [](AsyncWebServerRequest *request){
    DeleteFile(SD, fileName);
    CreateFile(SD, fileName);
    request->send(SPIFFS, "/log.html", "text/html");
  });
  server.on("/test_ws.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/test_ws.html", "text/html");
  });
  /////////////////////

  // init json document
  UpdateTime();
  jsonDoc[jsonTitles[0]] = prettyDate;
  soilMoistureJson = jsonDoc.createNestedArray(jsonTitles[1]);
  for (byte i = 0; i < soilMoistureSensorCount; i++)
    soilMoistureJson.add(soilMoisture[i]);
  jsonDoc[jsonTitles[2]] = soilMoistureAvg;
  jsonDoc[jsonTitles[3]] = humidity;
  jsonDoc[jsonTitles[4]] = temperature;
  jsonDoc[jsonTitles[5]] = waterLevel;
  jsonDoc[jsonTitles[6]] = pumpState1;
  jsonDoc[jsonTitles[7]] = pumpState2;

  // init server and websocket
  server.begin();
  ws.onEvent(OnWsEvent);
  server.addHandler(&ws);

  if (sdStatus && spiffsStatus)
    InfoPin(false);  
}

void loop()
{
  if (DoTick(millis(), pumpChecklastTime, pumpCheckTime)) // tick function deals with millis overflow
  {
    pumpChecklastTime = millis();
    UpdateAnalogSensors();
    if (DoTick(millis(), logLastTime, logTime))
    {
      InfoPin(true);
      logLastTime = millis();
      RedDHT();
      LogReadings();
      CreateBackup(SD, fileName);
      UploadThingSpeak();
      ws.cleanupClients();
      if (sdStatus && spiffsStatus)
        InfoPin(false);
    }
  }
  
  PumpControl();
}
