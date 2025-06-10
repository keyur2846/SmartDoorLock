#define BLYNK_PRINT Serial // Enables serial debugging for Blynk

#include <ESP8266WiFi.h>      // Required for WiFi connectivity on ESP8266
#include <BlynkSimpleEsp8266.h> // Blynk library for ESP8266

// Your Blynk Auth Token (get this from the Blynk app after creating a project)
char auth[] = "YOUR_BLYNK_AUTH_TOKEN"; // Replace with your actual Auth Token!

// Your WiFi credentials
char ssid[] = "YOUR_WIFI_SSID";     // Replace with your WiFi network name
char pass[] = "YOUR_WIFI_PASSWORD"; // Replace with your WiFi password

// Define a virtual pin for controlling the lock (e.g., a button in Blynk)
#define BLYNK_VIRTUAL_PIN_LOCK V1 // Virtual Pin V1 for lock control

// Define a virtual pin for displaying the lock status (optional feedback)
#define BLYNK_VIRTUAL_PIN_STATUS V2 // Virtual Pin V2 for status display

// Pin connected to the Relay (assuming NodeMCU controls the relay directly or via Arduino)
// If NodeMCU sends commands to Arduino via Serial, this pin might not be directly used for relay
// For simplicity, let's assume NodeMCU directly controls a relay (e.g., D1/GPIO5)
#define NODE_MCU_RELAY_PIN D1 // Connect relay IN to NodeMCU D1 (GPIO5)

// Variable to keep track of the lock state from Blynk's perspective
bool remoteLockState = true; // true = locked (default), false = unlocked

// This function will be called every time the button in Blynk app is pressed
BLYNK_WRITE(BLYNK_VIRTUAL_PIN_LOCK) {
  int pinValue = param.asInt(); // Get the value from the Blynk button (0 or 1)

  if (pinValue == 1) {
    // Button is ON, assume we want to unlock
    digitalWrite(NODE_MCU_RELAY_PIN, LOW); // Unlock (assuming active-low relay for unlock)
    remoteLockState = false;
    Serial.println("NodeMCU: Received command to UNLOCK door from Blynk.");
  } else {
    // Button is OFF, assume we want to lock
    digitalWrite(NODE_MCU_RELAY_PIN, HIGH); // Lock (assuming active-low relay for unlock)
    remoteLockState = true;
    Serial.println("NodeMCU: Received command to LOCK door from Blynk.");
  }

  // Update status in Blynk app
  Blynk.virtualWrite(BLYNK_VIRTUAL_PIN_STATUS, remoteLockState ? "LOCKED" : "UNLOCKED");
}

void setup() {
  // Debugging console
  Serial.begin(115200); // Use a faster baud rate for ESP8266 serial monitor

  pinMode(NODE_MCU_RELAY_PIN, OUTPUT);
  digitalWrite(NODE_MCU_RELAY_PIN, HIGH); // Ensure locked state initially (assuming active-low relay)

  // Connect to Blynk
  Blynk.begin(auth, ssid, pass);

  // You can also write initial state to Blynk here
  Blynk.virtualWrite(BLYNK_VIRTUAL_PIN_STATUS, "LOCKED");
  Serial.println("NodeMCU: Connecting to WiFi and Blynk...");
}

void loop() {
  Blynk.run(); // This runs the Blynk communication loop
  // You can add other NodeMCU specific tasks here if needed
}
