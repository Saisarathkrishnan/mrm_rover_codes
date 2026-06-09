#include <HX711.h>

#define DT_PIN 23
#define SCK_PIN 22

HX711 scale;

// Change this after calibration
float calibration_factor = -7050.0;

void setup() {
  Serial.begin(115200);

  scale.begin(DT_PIN, SCK_PIN);

  Serial.println("Remove all weight...");
  delay(3000);

  scale.tare();

  Serial.println("Tare complete.");
}

void loop() {
  Serial.print("Weight: ");
  Serial.print(scale.get_units(10));
  Serial.println(" g");

  delay(500);
}