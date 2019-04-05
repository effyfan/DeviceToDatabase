int motionPin = 2;
float motionReading;
float lastMotionReading;

void setup() {
  Serial.begin(9600);
}

void loop() {
  motionReading = digitalRead(motionPin);
  if (motionReading != lastMotionReading) {
    Serial.print(motionReading);
    Serial.println(" MOTION");
    delay(30);
    lastMotionReading = motionReading;
  }
}
