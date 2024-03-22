#include <Wire.h>
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <WebSocketsServer.h>

#define ZIGBEE_RX_PIN 2
#define ZIGBEE_TX_PIN 3

// WiFi credentials
const char* ssid = "NMTA_GF";
const char* password = "1234567890";

// Global variables
SoftwareSerial zigbeeSerial(ZIGBEE_RX_PIN, ZIGBEE_TX_PIN); // RX, TX
WiFiClient wifiClient;
WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
 {
  // Handle WebSocket events
  switch(type)
   {
    case WStype_CONNECTED:
      Serial.printf("[%u] WebSocket client connected.\n", num);
      break;
    case WStype_DISCONNECTED:
      Serial.printf("[%u] WebSocket client disconnected.\n", num);
      break;
    case WStype_TEXT:
      Serial.printf("[%u] Received data: %s\n", num, payload);
      // Process received data
      processData(payload, length);
      break;
    default:
      break;
  }
}

void setup()
 {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Initialize Zigbee communication
  zigbeeSerial.begin(9600);

  // Connect to WiFi network
  connectToWiFi();

  // Start WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() 
{
  webSocket.loop();
  
  if (zigbeeSerial.available()) 
  {
    // Read data from Zigbee module
    char data = zigbeeSerial.read();

    // Send data to WebSocket clients
    webSocket.broadcastTXT(data);
  }

  // Add other functionalities as needed
  delay(1000); // Adjust delay as needed
}

void processData(uint8_t * payload, size_t length)
 {
  // Process received data based on data format
  // For example, if data format is JSON, parse JSON data and extract relevant information
  // Display information on Serial Monitor or LCD display

  // Example: Display received data on Serial Monitor
  Serial.print("Received Data: ");
  for (size_t i = 0; i < length; i++) 
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to WiFi");
}
