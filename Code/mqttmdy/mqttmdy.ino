#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

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
mqttClient.setUsernamePassword(mqttuser,mqttpass);
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
    mqttClient.print(123);
    mqttClient.endMessage();

    mqttClient.beginMessage(topic2);
    mqttClient.print(123);
    mqttClient.endMessage();

    mqttClient.beginMessage(topic3);
    mqttClient.print(123);
    mqttClient.endMessage();

  

    Serial.println();
  }
}
