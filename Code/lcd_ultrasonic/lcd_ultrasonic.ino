//lcd and ultrasonic and fake ph

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27 16 column and 2 rows

int trigPin = 9;    // TRIG pin
int echoPin = 8;    // ECHO pin
#define SensorPin A0   

float duration_us, distance_cm;

void setup() {
  lcd.init();               // initialize the lcd
  lcd.backlight();          // open the backlight 
  pinMode(trigPin, OUTPUT); // config trigger pin to output mode
  pinMode(echoPin, INPUT);  // config echo pin to input mode
  Serial.begin(9600);  
  Serial.println("pH meter experiment!"); 
}


void loop() {
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(echoPin, HIGH);

  // calculate the distance
  distance_cm = 17 - 0.017 * duration_us;
  // ph value
  int v = analogRead(A0);
  int Ph = map(v,0,100,7,0);
  Serial.println(v);
  Serial.println(Ph);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0); // start to print at the first row
  lcd.print("Deepth ");
  lcd.print(distance_cm);
  lcd.print("cm");
  lcd.print("   ");
  lcd.setCursor(0, 1);
  lcd.print("PH value  ");
  lcd.print(Ph);
  Serial.println(distance_cm);
  delay(500);
}
