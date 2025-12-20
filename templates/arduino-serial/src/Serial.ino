/**
 * Arduino Serial Example for STM32F103C8 (Blue Pill)
 *
 * Demonstrates serial communication using the Arduino API.
 * Connect to PA9 (TX) and PA10 (RX) at 115200 baud.
 */

#define LED_PIN PC13

void setup() {
  // Initialize LED
  pinMode(LED_PIN, OUTPUT);

  // Initialize Serial at 115200 baud
  Serial.begin(115200);

  // Wait for serial connection
  while (!Serial) {
    ; // Wait for serial port to connect
  }

  Serial.println("=================================");
  Serial.println("STM32 Arduino Serial Demo");
  Serial.println("=================================");
  Serial.println("Type a message and press Enter...");
  Serial.println();
}

void loop() {
  // Check if data is available
  if (Serial.available() > 0) {
    // Read the incoming string
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.length() > 0) {
      // Echo back with info
      Serial.print("Received (");
      Serial.print(input.length());
      Serial.print(" chars): ");
      Serial.println(input);

      // Toggle LED on each message
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
  }

  // Optional: periodic status message
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 5000) {
    Serial.print("Uptime: ");
    Serial.print(millis() / 1000);
    Serial.println(" seconds");
    lastPrint = millis();
  }
}
