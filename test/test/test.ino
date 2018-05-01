// SAMD pretends to be a Mega for a day

int selectPin = 2; //D2
int buzzerPin = 7; // For sending buzz directly to Due
int toneFreq = 1000;
int ledPin = 13;


void setup() {
    SerialUSB.begin(9600); // For printing to console
    Serial1.begin(9600); // For receiving data from Due
    
    pinMode(selectPin, OUTPUT);
    pinMode(ledPin, OUTPUT);
    digitalWrite(selectPin, LOW);
    
    delay(100);
    SerialUSB.println("SAMD startup");
    delay(1000);
}

void loop() {
    // Every 3 seconds, signal
    digitalWrite(ledPin, HIGH);
    
    // Signal up. Don't wait 10ms! too slow!
    SerialUSB.println("Signal Due");
    digitalWrite(selectPin, HIGH);
    
    // Play 'tone' on 'buzzer' for 50ms
    SerialUSB.println("Playing tone");
    tone(buzzerPin, toneFreq);
    delay(50);
    noTone(buzzerPin); 
    
    // Deselect Due
    digitalWrite(selectPin, LOW);
    SerialUSB.println("Deselecting Due");
    
    // Read number from serial
    SerialUSB.println("Reading data...");
    String dataString = "";
    if (Serial1.available()) {
        // If available, read
        SerialUSB.print("Serial1 available. Saving: ");
        while (Serial1.available()) {
            char c = Serial1.read();
            SerialUSB.println(c);
            dataString += String(c);
        }
        SerialUSB.println();
    } else {
        // If unavailable, try to reconnect, then read
        SerialUSB.println("Serial1 not available. Re-begin.");
        Serial1.begin(9600);
        int ct = 0;
        while (!Serial1.available() && ct < 20) {
            delay(20);
            ct++;
        }
        if (ct == 20) {SerialUSB.println("Ct = 20: Data not available.");};
        
        while (Serial1.available()) {
            char c = Serial1.read();
            dataString += String(c);
        }
    }
    
    SerialUSB.println("DataString: " + dataString);
    SerialUSB.println("\n\n");

    digitalWrite(ledPin, LOW);
    delay(5000);
}