int frequency1 = 10000;  // Frequency in Hertz (e.g., 440 Hz for A4)
int frequency2 = frequency1 / 3;  // 1/3 of the first frequency
int duration = 5000;  // Duration in milliseconds

void setup() {
  pinMode(9, OUTPUT);  // Set D9 as an output pin
  pinMode(12, OUTPUT); // Set D12 as an output pin
}

void loop() {
  // Generate a square wave on D9 using the tone() function
  tone(9, frequency1, duration); // Generate the tone for the specified duration
  
  // Generate a square wave on D12 using manual toggling
  for (int i = 0; i < duration * frequency2 / 1000; i++) {
    digitalWrite(12, HIGH); // Set D12 to HIGH
    delayMicroseconds(500000 / frequency2); // Half of the period
    digitalWrite(12, LOW); // Set D12 to LOW
    delayMicroseconds(500000 / frequency2); // Half of the period
  }
  
  delay(500); // Pause for a moment between the two square waves
}
