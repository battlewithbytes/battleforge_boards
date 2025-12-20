/**
 * Arduino Blink Example for STM32F103C8 (Blue Pill)
 *
 * This sketch blinks the onboard LED connected to PC13.
 * Uses the familiar Arduino API with digitalWrite().
 *
 * The Arduino framework for STM32 provides:
 * - pinMode(), digitalWrite(), digitalRead()
 * - analogRead(), analogWrite()
 * - Serial.begin(), Serial.print()
 * - delay(), millis()
 * - And much more!
 */

// Blue Pill LED is on PC13 (active LOW)
#define LED_PIN PC13

void setup() {
  // Initialize the LED pin as output
  pinMode(LED_PIN, OUTPUT);

  // Optional: Initialize Serial for debugging
  // Serial.begin(115200);
  // Serial.println("Arduino Blink Started!");
}

void loop() {
  // Turn LED ON (PC13 is active LOW, so LOW = ON)
  digitalWrite(LED_PIN, LOW);
  delay(500);  // Wait 500ms

  // Turn LED OFF
  digitalWrite(LED_PIN, HIGH);
  delay(500);  // Wait 500ms
}
