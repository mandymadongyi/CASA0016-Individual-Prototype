#define SensorPin A0    
#include <OneWire.h>    // water tem library  part 1
#include <DallasTemperature.h>    // water library part2


//below part sets up DS18B20 sensor for water temperature
const int SENSOR_PIN = 11; // Arduino pin connected to DS18B20 sensor's DQ pin
OneWire oneWire(SENSOR_PIN);         // setup a oneWire instance
DallasTemperature tempSensor(&oneWire); // pass oneWire to DallasTemperature library
float tempCelsius;    // temperature in Celsius



void setup(void)
{
 
  Serial.begin(9600);  
  Serial.println("pH meter experiment!");    //Test the serial monitor

  //DS18B20 initialization
  tempSensor.begin();    // initialize the temp sensor
}

void loop(void)
{

  float v = analogRead(A0);
  float Ph = map(v,0,800,14.00,0.00);
  float PH = -0.0175*v + 14;
  Serial.println(v);
  
  Serial.println(PH);
  delay(1000);

  //below part measures temp

  tempSensor.requestTemperatures();             // send the command to get temperatures
  delay(1000);
  tempCelsius = tempSensor.getTempCByIndex(0);  // read temperature in Celsius

  Serial.println(tempCelsius);

}
