# ESP8266-soil-moisture
Small arduino script that reads 1 to 5 soil mositure sensors at a given interval and stores them in a remote database
___

>
> Note this README and repository are still work in progress
>

## Requirements

### Hardware
* 1x ESP8266 or ESP32 ([I used this one](https://www.seeedstudio.com/NodeMCU-v2-Lua-based-ESP8266-development-kit.html))
* 1-5x Capacitive soil moisture sensors ([these](https://www.seeedstudio.com/Grove-Capacitive-Moisture-Sensor-Corrosion-Resistant.html))
* 1x [ADS1115](http://www.ebay.com/sch/ADS1115)
* Some wires

### Software dependencies
* [Arduino MySQL connector](https://github.com/ChuckBell/MySQL_Connector_Arduino)
* [Adafruit ADS1X15](https://github.com/adafruit/Adafruit_ADS1X15) (Only when using more then 1 moisture sensor)

### Database
* A MySQL database running in your network
* With a database called 'plants'
  * Create it with ''
* A table called 'SoilMoisture' with the following columns with the given datatypes
    * Create it with ''

id  | plant_id | value  | date
--- | -------- | ------ | --------
int | int      | double | DateTime

* A table called 'Plants' with the following columns with the given datatypes
    * Create it with ''

id  | name
--- | --------
int | tinytext

### Case
