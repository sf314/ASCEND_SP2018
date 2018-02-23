// Mic
// Wait for high from speaker, then read values from analog

/*
microphone pin: 3
trigger pin: 2
source pin: 4
*/

// Time to read two analog signals:
// 847 microseconds to milliseconds

/*
Length of cardboard tube: 32.75cm
Tone Frequency: 100Hz
Using echo off of can:
- time shift to echo: ~2ms
Straight configuration:
- time shift to sound: ~1ms
*/

// ***** Includes 

// ***** Proto
void println(String s);

int sourcePin = 1;
int triggerPin = 2;
int micPin = 3;
int ledPin = 13;

void setup() {
    SerialUSB.begin(2000000);
    pinMode(triggerPin, INPUT);
    pinMode(ledPin, OUTPUT);
    SerialUSB.println("Start");
    delay(2000);
}

long currentTime = 0;
long startTime = 0;
int data1 = 0;
int data2 = 0;

void loop() {
    
    if (analogRead(triggerPin) > 800) { // Look for buzzer on
        digitalWrite(ledPin, HIGH);
        // Tag time
        startTime = micros();
        
        // Continuously get data until it is over
        data1 = analogRead(micPin);
        while (analogRead(triggerPin) > 800) {
            currentTime = micros();
            data1 = analogRead(micPin);
            data2 = analogRead(sourcePin);
            SerialUSB.println(String(currentTime) + "," + String(data1) + "," + String(data2));
        }

        digitalWrite(ledPin, LOW);
    }
}

void version1() {
	
}


void println(String s) {
    SerialUSB.println(s);
}