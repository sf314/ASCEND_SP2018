

/*
Need:
Serial connection to SAMD
Select pin for SAMD
TX pin to receive from SAMD
*/

/*
Mission operations:

Every 10 seconds
    Select the SAMD
    Drive the speaker using tone 800Hz (50ms)
    Read data from Serial (just into a string);

*/

// Declarations:
int selectPin = 6;
int buzzerPin = A5;
int toneFreq = 100;

void setup() {
    Serial.begin(9600);
    Serial3.begin(9600);
    
    pinMode(selectPin, OUTPUT);
    // pinMode(buzzerPin, OUTPUT);
    
    digitalWrite(selectPin, LOW);
    
    Serial.println("Mega booting up...");
    tone(buzzerPin, 220);
    delay(200);
    noTone(buzzerPin);
    delay(1800);
    
    tone(buzzerPin, 220);
    delay(500);
    tone(buzzerPin, 440);
    delay(500);
    tone(buzzerPin, 880);
    delay(1000);
    noTone(buzzerPin);
    
    delay(1000);
}


void loop() {
    Serial.println("Pinging...");
    
    // Select the SAMD
    Serial.println("\tSelecting SAMD...");
    digitalWrite(selectPin, HIGH);
    delay(10); // For accuracy
    
    // Play the tone for 50ms
    Serial.println("\tPlaying tone...");
    tone(buzzerPin, toneFreq);
    delay(50);
    noTone(buzzerPin);    
    
    // Deselect the SAMD
    Serial.println("\tDeselecting the SAMD");
    digitalWrite(selectPin, LOW);
    
    
    // Read data
    Serial.println("\tReading data...");
    String dataString = "";
    if (!Serial3.available()) {
        Serial3.begin(9600);
        Serial.println("\tSerial3 was not available!");
        int ct = 0;
        while (!Serial3.available() && ct < 20) {
            delay(50);
            ct++;
        }
        if (ct == 20) {
            Serial.println("\tCount hit 20. Data was not available");
        } else {
            Serial.println("\tData received!");
        }
        
        while (Serial3.available()) {
            char c = Serial3.read();
            dataString += String(c);
        }
    } else {
        Serial.print("\tSerial3 was available. Saving bits: ");
        while (Serial3.available()) {
            char c = Serial3.read();
            Serial.print(c);
            dataString += String(c);
        }
        Serial.println();
    }
    
    Serial.println("DataString: " + dataString);
    Serial.println("\n\n");
    
    delay(5000); // 5 seconds
}