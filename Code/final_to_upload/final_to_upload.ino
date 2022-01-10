// water quality monitoring - water level, temperature, ph
// Dongyi Ma  Dec 2021
// start by showing info on lcd, then add ultrasonic sensor for water level meseasuring, then add DS18B20 sensor to meseaure water temperature, and then add ph sensor to detect ph vluae of the liquid
// show the three sensed data locally on lcd and push to mqtt for remote sensing




#include <LiquidCrystal_I2C.h>   // lcd library
#include <OneWire.h>    // water tem library  part 1
#include <DallasTemperature.h>    // water library part2
#include <ArduinoMqttClient.h>  // mqtt library part 1
#include <WiFiNINA.h>       // mqtt library part 2  
#include "GuessWhat.h"    //password file needs to be removed before uploading to github


//below part sets up lcd
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27 16 column and 2 rows


// below part sets up ultrasonic sensor for water level
int trigPin = 9;    // TRIG pin
int echoPin = 8;    // ECHO pin
int h = 17; // the height above sea level of the ultrasonic sensor for measuring the water level
float duration_us, distance_cm, level;

//below part sets up DS18B20 sensor for water temperature
const int SENSOR_PIN = 5; // Arduino pin connected to DS18B20 sensor's DQ pin
OneWire oneWire(SENSOR_PIN);         // setup a oneWire instance
DallasTemperature tempSensor(&oneWire); // pass oneWire to DallasTemperature library
float tempCelsius;    // temperature in Celsius


//below part sets up analoge input votage for liquid ph
#define SensorPin A0   // ph input


//below part sets up mqtt sender configuartion

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

char mqttuser[] = SECRET_MQTTUSER;
char mqttpass[] = SECRET_MQTTPASS;
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "mqtt.cetools.org";
int        port     = 1884;
const char topic1[]  = "student/CASA0016/ucfndma/water_quality_monitoring_system/Water Level";

const char topic2[]  = "student/CASA0016/ucfndma/water_quality_monitoring_system/Water Temperature";

const char topic3[]  = "student/CASA0016/ucfndma/water_quality_monitoring_system/PH Value";


//set interval for sending messages (milliseconds)
const long interval = 8000;
unsigned long previousMillis = 0;

int count = 0;

void setup() {
  //lcd initialization
  lcd.init();               // initialize the lcd
  lcd.backlight();          // open the backlight

  //ultrasonic initialization
  pinMode(trigPin, OUTPUT); // config trigger pin to output mode
  pinMode(echoPin, INPUT);  // config echo pin to input mode
  Serial.begin(9600);

  //DS18B20 initialization
  tempSensor.begin();    // initialize the temp sensor

  //mqtt initialization

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  mqttClient.setUsernamePassword(mqttuser, mqttpass);
  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
}


void loop() {
  //below part measures water level
  // generate 10-microsecond pulse to TRIG pin

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(echoPin, HIGH);

  distance_cm = 0.017 * duration_us; //equation used to measure distance since length equals to speed times time
  level = h - distance_cm; // calculate the water level by taking the distance out of the height above sea level of ultrasonic sensor


  //below part measures temp

  tempSensor.requestTemperatures();             // send the command to get temperatures
  delay(1000);
  tempCelsius = tempSensor.getTempCByIndex(0);  // read temperature in Celsius


  //below part measures ph value using mapping and equaltion. mapping only shows integers.
  //the equaltion is calculated by hand while there is a 400mV level up and 0mV corresponds to PH 14 and 800 mV corresponds to PH 0.

  float v = analogRead(A0);
  float PH = -0.03461518 * v + 23.6153846;
  Serial.println(v);

  Serial.println(PH);


  //below part displays data locally on lcd order is differnet because lcd space is limited

  lcd.clear();
  lcd.setCursor(0, 0); //move the cursor to the first bit of the first line
  lcd.print("T=");  //use "" for texts, blank space also matters
  lcd.print(tempCelsius);
  lcd.print("C "); // don't forget the unit
  lcd.print("PH=");
  lcd.print(PH);
  lcd.setCursor(0, 1);//the first bit of the second line
  
 

 // lcd.setCursor(5, 1); // the 6 bit of the second line since ph and lvl will share one line space
  lcd.print("Level=");
  lcd.print(level);
  lcd.print("cm");
  delay(500);

  //below part visualize data remotely on mqtt
  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  mqttClient.poll();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    //record random value from A0, A1 and A2
    int Rvalue = analogRead(A0);


    Serial.print("Sending message to topic: ");
    Serial.println(topic1);
    Serial.println(Rvalue);

    // send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(topic1);
    mqttClient.print(level);
    mqttClient.endMessage();

    mqttClient.beginMessage(topic2);
    mqttClient.print(tempCelsius);
    mqttClient.endMessage();

    mqttClient.beginMessage(topic3);
    mqttClient.print(PH);
    mqttClient.endMessage();

  }


}
