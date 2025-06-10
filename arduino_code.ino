#include <SPI.h>
#include <MFRC522.h> // Include MFRC522 RFID library

// Define RFID module connections
#define SS_PIN 10  // Slave Select (SS) pin of RC522 to Arduino D10
#define RST_PIN 9  // Reset (RST) pin of RC522 to Arduino D9

// Define relay pin for the solenoid lock
#define RELAY_PIN 7 // Digital pin controlling the relay

// Define optional feedback LEDs/Buzzer pins
#define GREEN_LED_PIN 8 // Green LED for access granted
#define RED_LED_PIN 6   // Red LED for access denied
#define BUZZER_PIN 5    // Buzzer for audio feedback

// Create an instance of the MFRC522 class
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Store a few authorized UIDs (Replace with your own RFID tag UIDs!)
// You can get UIDs by running the MFRC522 'DumpInfo' example sketch.
byte authorizedUIDs[][4] = {
  {0xDE, 0xAD, 0xBE, 0xEF}, // Example Tag 1 (replace with your actual tag's UID)
  {0x12, 0x34, 0x56, 0x78}, // Example Tag 2
  // Add more authorized UIDs here as needed
};

const int NUM_AUTHORIZED_UIDS = sizeof(authorizedUIDs) / sizeof(authorizedUIDs[0]);

// Variable to track the lock state (optional, if lock is toggled)
bool lockState = true; // true = locked, false = unlocked

void setup() {
  Serial.begin(9600);   // Initialize serial communication for debugging
  SPI.begin();          // Initialize SPI bus
  mfrc522.PCD_Init();   // Initialize MFRC522 module

  // Configure pin modes
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initial state: Lock the door (HIGH for active-low relay, LOW for active-high relay)
  // Assuming active-high relay for simplicity. Adjust if your relay is active-low.
  digitalWrite(RELAY_PIN, HIGH); // Lock the door
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH); // Show red LED initially (locked)
  Serial.println("SmartDoorLock initialized. Door is Locked.");
  Serial.println("Scan an RFID tag...");
}

void loop() {
  // Look for new RFID cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Print UID to Serial Monitor for debugging
  Serial.print("Scanned UID:");
  String scannedUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    scannedUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") + String(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  scannedUID.toUpperCase(); // Convert to uppercase for consistent comparison

  // Check if the scanned UID is authorized
  if (checkUID(mfrc522.uid.uidByte)) {
    Serial.println("Access Granted!");
    accessGranted();
    // Toggle lock state or simply unlock/lock based on your desired logic
    lockState = !lockState; // Toggle
    if (lockState) {
      digitalWrite(RELAY_PIN, HIGH); // Lock
      Serial.println("Door Locked.");
    } else {
      digitalWrite(RELAY_PIN, LOW); // Unlock
      Serial.println("Door Unlocked.");
    }
  } else {
    Serial.println("Access Denied!");
    accessDenied();
  }

  mfrc522.PICC_HaltA();       // Halt PICC (Picc is the card)
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD (PCD is the reader)
  delay(2000); // Small delay before next scan
}

// Function to compare scanned UID with authorized UIDs
bool checkUID(byte *testUID) {
  for (int i = 0; i < NUM_AUTHORIZED_UIDS; i++) {
    bool match = true;
    for (byte j = 0; j < 4; j++) { // Assuming 4-byte UIDs
      if (testUID[j] != authorizedUIDs[i][j]) {
        match = false;
        break;
      }
    }
    if (match) {
      return true;
    }
  }
  return false;
}

// Feedback functions
void accessGranted() {
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, LOW);
  tone(BUZZER_PIN, 1000); // Play a tone
  delay(500);
  noTone(BUZZER_PIN);
  digitalWrite(GREEN_LED_PIN, LOW);
}

void accessDenied() {
  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, LOW);
  tone(BUZZER_PIN, 500); // Play a different tone
  delay(1000);
  noTone(BUZZER_PIN);
  digitalWrite(RED_LED_PIN, LOW);
}