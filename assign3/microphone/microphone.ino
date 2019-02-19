int soundPin = 3;
float soundReading;
float lastSoundReading;

void setup() {
  Serial.begin(9600);
}

void loop() {
  soundReading = 1023 - digitalRead(soundPin);
  Serial.println(soundReading);
  delay(10);
}
