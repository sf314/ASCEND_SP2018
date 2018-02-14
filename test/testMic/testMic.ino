// Test analog microphone



// Arbitrary pins
int micPin = 0;
int buzzPin = 5;

void setup() {
    Serial.begin(2000000); // Not 9600, 2million
    pinMode(buzzPin, OUTPUT);
    digitalWrite(buzzPin, LOW);
    
    delay(100);
    Serial.println("Starting...");
}


long currentTime = 0;
long previousTime = 0;
long cbt = 0;
long pbt = 0;


bool active = false;
int data = 0;
bool play = false;
bool buzz = false;
int freq = 220;
void loop() {
    // Sample infinitely. Check analog reading
    if (Serial.available()) {
        char c = Serial.read();
        switch (c) {
            case 't':
                active = !active;
                break;
            case 'b':
                buzz = !buzz;
                Serial.println("Buzz!");
                if (!buzz) {
                    freq = 220;
                }
                break;
            default:
                Serial.println("Invalid command");
        }
    }
    
    if (active) {
        // Play buzzer at 0.5-sec intervals
        currentTime = micros();
        cbt = currentTime;
        
        if (currentTime - previousTime >= 500) {
            play = !play;
            buzz = !buzz;
            
            if (play) {
                //digitalWrite(buzzPin, HIGH);
                tone(buzzPin, 220);
            } else {
                digitalWrite(buzzPin, LOW);
                noTone(buzzPin);
            }
            previousTime = currentTime;
        }
        
        if (buzz) {                         // This runs constantly
            // Check if 100 ms has passed?
            if (cbt - pbt >= 250) {         // This runs every 100ms
                tone(buzzPin, freq);
                freq += 20;
                pbt = cbt;
            }
        } else {
            noTone(buzzPin);
        }
        
        data = analogRead(micPin);
        Serial.println(String(currentTime) + "," + String(data));// + "," + String(data) + "," + String(data)  + "," + String(data)  + "," + String(data));
    } else {
        currentTime = micros();
        cbt = currentTime;
        // Ensure buzzer is off
        //digitalWrite(buzzPin, LOW);
        if (buzz) {                         // This runs constantly
            // Check if 100 ms has passed?
            if (cbt - pbt >= 100) {         // This runs every 100ms
                tone(buzzPin, freq);
                freq += 20;
                pbt = cbt;
            }
        } else {
            noTone(buzzPin);
        }
    }
}

/*

A 220
B 247 + 27
C 261 + 14
D 293 + 32
E 329 + 36

*/