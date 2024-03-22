#include <Wire.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// Define pin connections for components
#define LM35_PIN A0
#define LCD_RS_PIN 12
#define LCD_EN_PIN 11
#define LCD_D4_PIN 5
#define LCD_D5_PIN 4
#define LCD_D6_PIN 3
#define LCD_D7_PIN 2
#define GSM_TX_PIN 8
#define GSM_RX_PIN 7
#define BUZZER_PIN 6
#define MQ_SENSOR_PIN A1
// Define other pin connections as needed

// Global variables
float temperature; // Variable to store temperature readings
float peakTemperature = -100.0; // Initial peak temperature value (lower than any expected value)
char message[100]; // Buffer for message to be sent via GSM module

// Initialize components
LiquidCrystal lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);
SoftwareSerial gsm(GSM_RX_PIN, GSM_TX_PIN); // RX, TX
// Initialize other components as needed

void setup() 
{
  // Initialize LCD
  lcd.begin(16, 2);

  // Initialize GSM module
  gsm.begin(9600);
  delay(1000);
  gsm.println("AT");
  delay(1000);
  gsm.println("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);

  // Initialize other components
  pinMode(BUZZER_PIN, OUTPUT);
  // Initialize Zigbee module, gas sensor, water level sensor, etc.
}

void loop()
 {
  readTemperature();
  updatePeakTemperature();
  displayTemperatureOnLCD();
  sendTemperatureMessage();
  checkGasSensor();
  checkWaterLevel();
  activateBuzzer();
  // Other functionalities as needed
  delay(1000); // Adjust delay as needed
}

void readTemperature() 
{
  // Read temperature from LM35 sensor and store in 'temperature' variable
  temperature = analogRead(LM35_PIN) * 0.48876; // Convert ADC value to temperature in Celsius
}

void updatePeakTemperature()
{
  // Update peak temperature if the current temperature is higher
  if (temperature > peakTemperature) {
    peakTemperature = temperature;
    // Check if the current temperature exceeds the emergency threshold
    if (temperature > 40.0) 
    { // Adjust threshold as needed
      triggerEmergency();
    }
  }
}

void displayTemperatureOnLCD()
 {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.setCursor(0, 1);
  lcd.print(temperature);
  lcd.print(" C");
}

void sendTemperatureMessage() 
{
  // Compose message with temperature data
  sprintf(message, "Temperature: %.2f C (Peak: %.2f C)", temperature, peakTemperature);

  // Send message via GSM module
  gsm.println("AT+CMGS=\"+1234567890\""); // Replace with recipient's phone number
  delay(1000);
  gsm.println(message);
  delay(1000);
  gsm.write(26); // Send Ctrl+Z to end SMS
  delay(1000);
}

void checkGasSensor()
 {
  // Read data from MQ gas sensor and take appropriate action if gas level is above threshold
}

void checkWaterLevel() 
{
  // Read data from water level sensor and take appropriate action if water level is above/below threshold
}

void activateBuzzer()
 {
  // Activate piezoelectric buzzer for alarm/notification
  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);
}

void triggerEmergency()
 {
  // Perform actions for triggering emergency
  // For example, send emergency alert, activate alarms, stop operations, etc.
  // Add your emergency handling code here
  sendEmergencyAlert();
  activateBuzzer(); // Activate buzzer as part of emergency alert
}

void sendEmergencyAlert() 
{
  // Send emergency alert message via GSM module
  gsm.println("AT+CMGS=\"+1234567890\""); // Replace with emergency contact number
  delay(1000);
  gsm.println("Emergency: High Temperature Detected! Evacuate immediately.");
  delay(1000);
  gsm.write(26); // Send Ctrl+Z to end SMS
  delay(1000);
}
