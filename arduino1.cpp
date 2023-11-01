#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h> // Include the WiFi library

#define TRIG_PIN 2
#define ECHO_PIN 4
#define RAIN_SENSOR_PIN 34 // ADC1_0 on ESP32
#define BUZZER_PIN 18
#define BUTTON_PIN 19

const char* ssid = "Wokwi-GUEST"; // Replace with your Wi-Fi network name
const char* password = ""; // Replace with your Wi-Fi password

// Beeceptor Endpoint URL
const char* beeceptorURL = "https://floodmonitoring.free.beeceptor.com"; // Replace with your Beeceptor URL

// Simulated GPS data
float latitude = 42.3601;
float longitude = -71.0589;

LiquidCrystal_I2C lcd(0x27, 20, 4);

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

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
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
    
    // Send the data alert to Beeceptor
    sendAlertToBeeceptor(waterLevel, rainValue);
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

  delay(6000);  // Simulate data update rate
}

void sendAlertToBeeceptor(int waterLevel, int rainValue) {
  // Create a JSON payload with the alert data
  String alertData = "{\"waterLevel\":" + String(waterLevel) + ",\"rainValue\":" + String(rainValue) + "}";

  // Create an HTTP client
  WiFiClient client;

  // Make an HTTP POST request to Beeceptor
  if (client.connect(beeceptorURL, 80)) { // Use port 80 for HTTP
    client.print("POST ");
    client.print(beeceptorURL);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(beeceptorURL);
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(alertData.length());
    client.println();
    client.println(alertData);
  }

  // Handle the response (you can add code to check the response if needed)
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // Disconnect
  client.stop();
}
