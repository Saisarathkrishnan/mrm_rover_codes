const int phPin = 34;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int raw = analogRead(phPin);

  float voltage = raw * (5.0 / 1023.0);

  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.print("  Ph READING BSDK: ");
  Serial.println(voltage, 3);

  delay(1000);
}