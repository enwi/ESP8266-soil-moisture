
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <MySQL.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>

//
// Debug
//
const boolean DEBUG = true;

//
// Select the ADC
//
Adafruit_ADS1115 ads;  //Use this for the 16-bit version
//Adafruit_ADS1015 ads;  // Use this for the 12-bit version

//
// WiFi config
//
const char *ssid = "Your WiFi SSID/Name";
const char *password = "Your WiFi password";

//
// DB config
//
const char *dbAddress = "192.168.2.11"; // ip address of the host/server of your database
const int dbPort = 3306; // the port at which to reach the database
static char dbUser[] = "esp"; // SQL user
static char dbPassword[] = "esp"; // SQL password
Connector dbConnection;

//
// Update interval
//
const long checkInterval = 5000; // Interval to check for changes in milliseconds
const long sendInterval = 300000; // Interval to always send data
long lastCheckTime = 0;
long lastSendTime = -sendInterval; // This makes all values update once

//
// Plant config
// Active = true
// plant*Id = identifier of plant in database
//
const boolean plant1 = true;
const int plant1Id = 0;
const boolean plant2 = true;
const int plant2Id = 1;
const boolean plant3 = true;
const int plant3Id = 2;
const boolean plant4 = true;
const int plant4Id = 3;
const boolean plant5 = true;
const int plant5Id = 4;

int16_t plant1Value = -1;
int16_t plant2Value = -1;
int16_t plant3Value = -1;
int16_t plant4Value = -1;
int16_t plant5Value = -1;

void acquireData()
{
  if(plant1)
  {
    const int16_t tmp = map(analogRead(A0), 0, 1024, 1024, 0);
    if(abs(plant1Value-tmp) > 10)
    {
      plant1Value = tmp;
      updatePlant(plant1Id,plant1Value);
    }
  }
  if(plant2)
  {
    const int16_t tmp = map(ads.readADC_SingleEnded(0), 0, 0x7FF0, 1024, 0);
    if(abs(plant2Value-tmp) > 10)
    {
      plant2Value = tmp;
      updatePlant(plant2Id,plant2Value);
    }
  }
  if(plant3)
  {
    const int16_t tmp = map(ads.readADC_SingleEnded(1), 0, 0x7FF0, 1024, 0);
    if(abs(plant3Value-tmp) > 10)
    {
      plant3Value = tmp;
      updatePlant(plant3Id,plant3Value);
    }
  }
  if(plant4)
  {
    const int16_t tmp = map(ads.readADC_SingleEnded(2), 0, 0x7FF0, 1024, 0);
    if(abs(plant4Value-tmp) > 10)
    {
      plant4Value = tmp;
      updatePlant(plant4Id,plant4Value);
    }
  }
  if(plant5)
  {
    const int16_t tmp = map(ads.readADC_SingleEnded(3), 0, 0x7FF0, 1024, 0);
    if(abs(plant5Value-tmp) > 10)
    {
      plant5Value = tmp;
      updatePlant(plant5Id,plant5Value);
    }
  }
}

void sendData()
{
  if(plant1)
  {
    plant1Value = map(analogRead(A0), 0, 1024, 1024, 0);
    updatePlant(plant1Id,plant1Value);
  }
  if(plant2)
  {
    plant2Value = map(ads.readADC_SingleEnded(0), 0, 0x7FF0, 1024, 0);
    updatePlant(plant2Id,plant2Value);
  }
  if(plant3)
  {
    plant3Value = map(ads.readADC_SingleEnded(1), 0, 0x7FF0, 1024, 0);
    updatePlant(plant3Id,plant3Value);
  }
  if(plant4)
  {
    plant4Value = map(ads.readADC_SingleEnded(2), 0, 0x7FF0, 1024, 0);
    updatePlant(plant4Id,plant4Value);
  }
  if(plant5)
  {
    plant5Value = map(ads.readADC_SingleEnded(3), 0, 0x7FF0, 1024, 0);
    updatePlant(plant5Id,plant5Value);
  }
}

void updatePlant(int plantID, int value)
{
  println("Updating plant " + String(plantID));
  char query[128];
  sprintf(query, "INSERT INTO plants.SoilMoisture (plant_id,value,date) VALUES (%i,%i.0,now())",
          plantID,
          value);
  print("Try insert: ");
  println(query);
  if (dbConnection.cmd_query(query))
  {
    println("Success");
  } else
  {
    println("Failed");
  }
}

template<typename T>
void print(T output)
{
  if(DEBUG)
  {
    Serial.print(output);
  }
}

template<typename T>
void println(T output)
{
  if(DEBUG)
  {
    Serial.println(output);
  }
}

void println()
{
  if(DEBUG)
  {
    Serial.println();
  }
}

void setup()
{
  if(DEBUG)
  {
    Serial.begin(115200);
  }
  println("Connecting to Wifi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    print(".");
  }
  println();
  print("IP: ");
  println(WiFi.localIP());
  ads.setGain(GAIN_ONE);
  ads.begin();
}

void loop()
{
  long now = millis();
  if (now - lastCheckTime > checkInterval)
  {
    lastCheckTime = now;
    println("Checking DB connection");
    bool connected = dbConnection.is_connected();
    if (!connected)
    {
      println("Reconnecting DB");
      dbConnection.mysql_connect(dbAddress, dbPort, dbUser, dbPassword);
    }
    acquireData();
  }

  if(now - lastSendTime > sendInterval)
  {
    lastSendTime = now;
    println("Checking DB connection");
    bool connected = dbConnection.is_connected();
    if (!connected)
    {
      println("Reconnecting DB");
      dbConnection.mysql_connect(dbAddress, dbPort, dbUser, dbPassword);
    }
    sendData();
  }
}
