#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

DHT dht;

#define BUTTON1_PIN 15 // Change to D15
#define BUTTON2_PIN 2  // Change to D2
#define BUTTON3_PIN 4  // Change to D4

#define FAN_OFF 0
#define FAN_SPEED1 1
#define FAN_SPEED2 2

char auth[] = "3bqpv_UimLsfJhIg3YA_1pY_cVWu0SUg";  // Blynk authentication token
char ssid[] = "NextTech";  // WiFi SSID
char pass[] = "12345678";  // WiFi password

LiquidCrystal_I2C lcd(0x27,16,2);

int fan_level = FAN_OFF;
bool system_on = false;
int Temperature;
int Humidity;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  lcd.setCursor(3,0); 
  lcd.print("system off");
  dht.setup(13);
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
  pinMode(BUTTON3_PIN, INPUT);
}

void loop() {
  Blynk.run();
  Temperature = dht.getTemperature();
  Humidity = dht.getHumidity();
  
  Serial.print("Humidity: " + String(Humidity) + " %");
  Serial.print("\t");
  Serial.println("Temperature: " + String(Temperature) + " C");

  // Update Blynk data
  Blynk.virtualWrite(V4, Temperature);
  Blynk.virtualWrite(V5, Humidity);
  if (digitalRead(BUTTON1_PIN) == HIGH) {
    lcd.clear();
    if (!system_on) {
      lcd.setCursor(1, 0);
      lcd.print("System Online!");
      lcd.setCursor(4, 1);
      lcd.print("Loading!");
      delay(1000);
      lcd.clear();
      lcd.setCursor(1,2);
      Serial.println("Button 1 pressed: System Online");
      system_on = true;
    } else {
      lcd.setCursor(1, 0);
      lcd.print("System Offline");
      lcd.setCursor(1,1);
      lcd.print("Goodbye!");
      delay(2500);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("System Offline");
      Serial.println("Button 1 pressed: System Offline | Goodbye");
      system_on = false;
    }
    delay(1000);
    if (system_on) {
      lcd.clear();
      lcd.setCursor(3, 0);
      displayFanSpeed();
    }
  }

  if (system_on && digitalRead(BUTTON2_PIN) == HIGH) {
    lcd.clear();
    fan_level = FAN_SPEED1;
    lcd.setCursor(3, 0);
    displayFanSpeed();
    lcd.setCursor(1, 1);
    lcd.print("Speed Set to 1");
    Serial.println("Button 2 pressed: Fan Speed Set to 1");
  }

  if (system_on && digitalRead(BUTTON3_PIN) == HIGH) {
    lcd.clear();
    fan_level = FAN_SPEED2;
    lcd.setCursor(3, 0);
    displayFanSpeed();
    lcd.setCursor(1, 1);
    lcd.print("Speed Set to 2");
    Serial.println("Button 3 pressed: Fan Speed Set to 2");
  }
  delay(100);
}

void displayFanSpeed() {
  lcd.setCursor(0,0);
  lcd.print("PjX32|Fans:");
  lcd.print(fan_level);
}

BLYNK_WRITE(V0) {
  int pinValue = param.asInt(); // pinValue will be 1 if the button is pushed, 0 if it's not
  
  if (pinValue == 1) { // Equivalent to if(digitalRead(BUTTON1_PIN) == HIGH)
    lcd.clear();
    if (!system_on) {
      lcd.setCursor(1, 0);
      lcd.print("System Online!");
      lcd.setCursor(4, 1);
      lcd.print("Loading!");
      delay(1000);
      lcd.clear();
      lcd.setCursor(1,2);
      Serial.println("Button 1 pressed: System Online");
      system_on = true;
    } else {
      lcd.setCursor(1, 0);
      lcd.print("System Offline");
      lcd.setCursor(1,1);
      lcd.print("Goodbye!");
      delay(2500);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print(" System Offline");
      Serial.println(" Button 1 pressed: System Offline | Goodbye");
      system_on = false;
    }
    delay(1000);
    if (system_on) {
      lcd.clear();
      lcd.setCursor(3, 0);
      displayFanSpeed();
    }
  }
}

BLYNK_WRITE(V1) {
  int pinValue = param.asInt();
  
  if (pinValue == 1) {
    lcd.clear();
    fan_level = FAN_SPEED1;
    lcd.setCursor(3, 0);
    displayFanSpeed();
    lcd.setCursor(1, 1);
    lcd.print("Speed Set to 1");
    Serial.println("Button 2 pressed: Fan Speed Set to 1");
  }
}

BLYNK_WRITE(V2) {
  int pinValue = param.asInt();
  
  if (pinValue == 1) {
    lcd.clear();
    fan_level = FAN_SPEED2;
    lcd.setCursor(3, 0);
    displayFanSpeed();
    lcd.setCursor(1, 1);
    lcd.print("Speed Set to 2");
    Serial.println("Button 3 pressed: Fan Speed Set to 2");
  }
}
