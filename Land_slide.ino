#include <SoftwareSerial.h>

// GSM Pins (RX, TX)
SoftwareSerial mySerial(9, 8);

// Pin Definitions
const int soilMoisturePin = A0; 
const int waterLevelPin = A1;   
const int buzzerPin = 10;        
const int redLedPin = 6;        
const int greenLedPin = 5;     

// Thresholds
const int soilThreshold = 81;   
const int waterThreshold = 40;  
const int combinedThreshold = 50; 

// Flags to prevent repeated SMS
bool combinedSent = false;
bool waterSent = false;
bool soilSent = false;

// Function to send SMS
void sendSMS(String message) {
  mySerial.println("AT+CMGF=1");    
  delay(1000);
  
  mySerial.println("AT+CMGS=\"+91********\""); // Your number
  delay(1000);
  
  mySerial.println(message);
  delay(100);
  
  mySerial.write(26); // CTRL+Z
  delay(3000);

  Serial.println("SMS Sent: " + message);
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(38400);

  pinMode(buzzerPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  
  digitalWrite(buzzerPin, LOW);
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, HIGH);
}

void loop() {
  int soilValue = analogRead(soilMoisturePin);
  int waterValue = analogRead(waterLevelPin);
  
  int soilMoisturePercent = map(soilValue, 1023, 0, 0, 100); 
  int waterLevelPercent = map(waterValue, 0, 1023, 0, 100); 
  
  int combinedPercent = (soilMoisturePercent + waterLevelPercent) / 2;

  Serial.print("Soil: "); Serial.print(soilMoisturePercent);
  Serial.print("% | Water: "); Serial.print(waterLevelPercent);
  Serial.print("% | Combined: "); Serial.print(combinedPercent);
  Serial.println("%");

  // --- CASE 3: COMBINED ---
  if (combinedPercent >= combinedThreshold) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
    digitalWrite(buzzerPin, HIGH);

    Serial.println("ALERT: Combined threshold reached!");

    if (!combinedSent) {
      sendSMS("ALERT! Landslide Detected ");
      combinedSent = true;
    }

    // Reset others
    waterSent = false;
    soilSent = false;
  }

  // --- CASE 1: WATER ---
  else if (waterLevelPercent >= waterThreshold) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
    digitalWrite(buzzerPin, HIGH); 

    Serial.println("ALERT: Water level high!");

    if (!waterSent) {
      sendSMS("ALERT! Water level is high");
      waterSent = true;
    }

    combinedSent = false;
    soilSent = false;
  }

  // --- CASE 2: SOIL ---
  else if (soilMoisturePercent >= soilThreshold) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
    digitalWrite(buzzerPin, LOW);

    Serial.println("ALERT: Soil moisture high!");

    if (!soilSent) {
      sendSMS("ALERT! Soil moisture is high");
      soilSent = true;
    }

    combinedSent = false;
    waterSent = false;
  }

  // --- NORMAL ---
  else {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(buzzerPin, LOW);

    Serial.println("Status: Normal");

    // Reset all flags when normal
    combinedSent = false;
    waterSent = false;
    soilSent = false;
  }

  delay(1000);
}