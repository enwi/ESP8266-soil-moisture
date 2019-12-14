# ESP8266-soil-moisture
Small arduino script that reads 1 to 5 soil moisture sensors at a given interval and stores them on a remote database
___

>
> Note this README and repository are still a work in progress
>

## Requirements

### Hardware
* 1x ESP8266 or ESP32 ([I used this one](https://www.seeedstudio.com/NodeMCU-v2-Lua-based-ESP8266-development-kit.html))
* 1-5x Capacitive soil moisture sensors ([these](https://www.seeedstudio.com/Grove-Capacitive-Moisture-Sensor-Corrosion-Resistant.html))
* 1x [ADS1115](http://www.ebay.com/sch/ADS1115) (Only when using more than 1 moisture sensor)
* Some wires

### Software dependencies
* [Arduino MySQL connector](https://github.com/ChuckBell/MySQL_Connector_Arduino)
* [Adafruit ADS1X15](https://github.com/adafruit/Adafruit_ADS1X15) (Only when using more than 1 moisture sensor)

### Database
* A MySQL database running in your network
  * Install with `sudo apt-get install mysql-server`
  * Or if that does not work try `sudo apt-get install mariadb-server`
* Login to database (If you can't login follow steps down below)
  * `sudo mysql -u root -p`
* Create a database called 'plants'
  * `CREATE DATABASE plants;`
* Create a table called 'Plants' with the following columns with the given datatypes
```
CREATE TABLE `Plants` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `name` text NOT NULL,
  `min_soil_moisture` int(11) NOT NULL DEFAULT '0',
  `max_soil_moisture` int(11) NOT NULL DEFAULT '1024',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
```
id      | name | min_soil_moisture | max_soil_moisture
------- | ---- | ----------------- | -----------------
int(11) | text | int(11)           | int(11)
* Create a table called 'SoilMoisture' with the following columns with the given datatypes
```
CREATE TABLE `SoilMoisture` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `plant_id` int(11) unsigned NOT NULL,
  `value` double NOT NULL,
  `date` datetime NOT NULL COMMENT 'The time the moisture data was measured',
  PRIMARY KEY (`id`),
  KEY `plant_id` (`plant_id`),
  CONSTRAINT `SoilMoisture_ibfk_1` FOREIGN KEY (`plant_id`) REFERENCES `Plants` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
```
id      | plant_id | value  | date
------- | -------- | ------ | --------
int(11) | int(11)  | double | datetime

#### Network access
* Make the database accessible to your network
  * Open one of the following files (the one that exists on your system containing the line `bind-address = 127.0.0.1`)
    * `sudo nano /etc/mysql/mysql.conf.d/mysqld.cnf`
    * `sudo nano /etc/mysql/mariadb.conf.d/50-server.cnf`
  * Change the line `bind-address            = 127.0.0.1` to `#bind-address            = 127.0.0.1`
  * Restart the mysql service with `sudo service mysql restart`
  * Now you can access the database from your network

#### Client access
* Give access to the database to your clients
  * Find out and write down the ip address of your client (aka the ESP8266 or whatever)
  * Login to database with `sudo mysql -u root -p`
  * Run command to give access to just the `plants` database
    * `GRANT ALL PRIVILEGES ON <database name>.* TO <username> @'<ip address>' IDENTIFIED BY '<password>';`
    * Where `<database name>` is the name of the database, in this case `plants`
    * Where `<username>` is the username used by the client to login, in this case `esp`
    * Where `<ip address>` is the ip address of the client, for example `192.168.2.6`
    * Where `<password>` is the password used by the client to login, in this case `esp`
    * Full command `GRANT ALL PRIVILEGES ON plants.* TO esp @'192.168.2.6' IDENTIFIED BY 'esp';`
  * If you have more than one client repeat the steps above for each of them
  * Now you have access to the database from your clients

#### What to do if you can't login with default user 'root' which has no password?
1. Try setting one with: `sudo mysqladmin -u root password newpass`

If that does not work try the following
1. Stop the MySQL service with `sudo /etc/init.d/mysql stop` or `sudo service mysql stop`
2. Start the MySQL server w/o password with `sudo mysqld_safe --skip-grant-tables &`
3. Connect to the MySQL server with `sudo mysql -u root`
4. Set a new password with 
    1. `use mysql;`
    2.
        * MySQL 5.7.5 and earlier: `update user set password=PASSWORD("NEW-ROOT-PASSWORD") where User='root';`
        * MySQL 5.7.6 and newer: `SET PASSWORD FOR 'root'@'localhost' = PASSWORD("newpass");`
    3. `flush privileges;`
    4. `quit`
5. Stop MySQL server with `sudo killall mysqld`
6. Restart to start clean with `sudo reboot`

### Case
You can find the case for the soil moisture sensors [here](https://www.thingiverse.com/thing:3980520)
