#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TRIG_PIN 2
#define ECHO_PIN 4
#define RAIN_SENSOR_PIN 34 // ADC1_0 on ESP32
#define BUZZER_PIN 18
#define BUTTON_PIN 19

// Simulated GPS data
float latitude = 42.3601;
float longitude = -71.0589;

LiquidCrystal_I2C lcd(0x27, 20, 4); // Set the address to your I2C backpack's address

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Flood Monitoring");
  lcd.setCursor(0, 1);
  lcd.print("and Early Warning");
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  // Simulated rain data (using potentiometer)
  int rainValue = analogRead(RAIN_SENSOR_PIN);

  // Read and process ultrasonic sensor data
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) / 29.1;  // Calculate distance in centimeters

  // Check if the button is pressed
  int buttonState = digitalRead(BUTTON_PIN);

  // Check the water level based on the distance (you need to adjust these thresholds)
  int waterLevel = 0;
  if (distance < 10) {
    waterLevel = 3; // High water level
  } else if (distance < 20) {
    waterLevel = 2; // Medium water level
  } else if (distance < 30) {
    waterLevel = 1; // Low water level
  }

  // Update the LCD
  lcd.setCursor(0, 2);
  lcd.print("Water Level: ");
  lcd.print(waterLevel);

  lcd.setCursor(0, 3);
  lcd.print("Alert: ");
  if (waterLevel >= 2 || rainValue > 500) {
    lcd.print("HIGH");
    // Activate the buzzer
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    lcd.print("LOW");
    // Deactivate the buzzer
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Simulated GPS data
  Serial.print("Latitude: ");
  Serial.print(latitude, 6);
  Serial.print(" Longitude: ");
  Serial.print(longitude, 6);
  Serial.print(" Rain Value: ");
  Serial.print(rainValue);
  Serial.print(" Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(1000);  // Simulate data update rate
}