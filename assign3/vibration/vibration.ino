int vibrationPin = 4;
float vibrationReading;
float lastVibrationReading;

void setup() {
    Serial.begin(9600);
}

void loop() {
    vibrationReading = digitalRead(vibrationPin);
    if (vibrationReading != lastVibrationReading) {
      Serial.print(vibrationReading);
      Serial.println(" VIBR");  
      lastVibrationReading = vibrationReading; 
    }
}
