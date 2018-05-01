// Mic
// Wait for high from speaker, then read values from analog

/*
microphone pin: 2
trigger pin: 1
source pin: 0
SD CS pin: 10
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

2/27: Changes
Update to send current ping # when asked. 
Use data arrays instead! And write stuff afterward!
*/

/*
SD: 
nominal datastring is 14-16 bytes in size
chipSelect = 10
MOSI: pin 11
MISO: pin 12
SCK: pin 13
*/

/* ****** IMPORTANT NOTE: CHANGING FROM SPARKFUN SAMD TO ARDUINO DUE
Changes:
SAMD -> DUE
Serial1 -> Serial1 (19,18) (MISO,MOSI)
SerialUSB -> Serial
sourcePin = 0 -> A0
selectPin = 1 -> A1 // Remove pinmode input designation
micPin = 2 -> A2

1000 logs: 7.43ms 
*/

// ***** Includes 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SD.h>
#include <SPI.h>
#define ARR_MAX_SIZE 1000

// #include <vector> // C++

// ***** Vars
int sourcePin = A0; 
int selectPin = A1; 
int micPin = A2; 

int ledPin = 13;
int sdPin = 10; // DUE = 4; SAMD = 10;

// ***** SD
File dataFile;
char fileName[13] = "ping0000.csv"; // number on indexes 4 - 7

// ***** Data stuff
typedef struct Data {
    //Data(long t, int d1, int d2); // Init
    long time;
    int data1;
    int data2;
    bool valid;
} Data_t;
Data_t data[ARR_MAX_SIZE];

void setup() {
    Serial.begin(115200); // 2M baud
    Serial1.begin(9600); // For comms with master controller
    SPI.begin();
    
    // pinMode(selectPin, INPUT);
    pinMode(ledPin, OUTPUT);
    Serial.println("Starting Due...");

    int tries = 0; // SD card?
    while (!SD.begin(sdPin)) {
        Serial.print("Could not init SD card! ");
        Serial.println("Tries: " + String(tries));
        tries++;
        digitalWrite(ledPin, HIGH); delay(750); // Blink with 75% duty cycle over 1 sec
        digitalWrite(ledPin, LOW); delay(250);
    }
    
    // Signal on!
    digitalWrite(ledPin, HIGH); delay(250); digitalWrite(ledPin, LOW); delay(250);
    digitalWrite(ledPin, HIGH); delay(250); digitalWrite(ledPin, LOW); delay(250);
    digitalWrite(ledPin, HIGH); delay(250); digitalWrite(ledPin, LOW); delay(250);
    
    delay(2000);
}

long currentTime = 0;
int data1 = 0;
int data2 = 0;

int ones = 1;
int tens = 0;
int hunds = 0;
int thous = 0;

int count = 0;
void loop() {
    
    // Also respond to asynchronous calls?
    if (Serial1.available()) {
        char c = Serial1.read();
        switch (c) {
            case 's':
                Serial1.print(String(count));
                break;
        }
    }
    
    // Measure data and send response if selected
    if (analogRead(selectPin) > 800) { // Look for buzzer on
        Serial.println("Using filename: " + String(fileName));

        ones = ones % 10;
        if (ones == 0) {
            tens++;
        }

        tens = tens % 10;
        if (tens == 0 && ones == 0) {
            hunds++;
        }

        hunds = hunds % 10;
        if (hunds == 0 && tens == 0 && ones == 0) {
            thous++;
        }

        thous = thous % 10;

        fileName[4] = thous + '0'; // Maybe?
        fileName[5] = hunds + '0';
        fileName[6] = tens + '0';
        fileName[7] = ones + '0';
        
        // Init data arr to be invalid. 
        for (int i = 0; i < ARR_MAX_SIZE; i++) {
            data[i].valid = false;
        }

        // Continuously get data until it is over.
        // USE ARRAY INSTEAD
        int index = 0;
        while (analogRead(selectPin) > 800 && index < ARR_MAX_SIZE) {
            data[index].time = micros();
            data[index].data1 = analogRead(micPin);
            data[index].data2 = analogRead(sourcePin);
            data[index].valid = true;
            
            // dt with everything: -1270- Due: 
            // dt without anything: Due: 
            index++;
        }
        
        // Write data to SD  MAY TAKE MORE THAN 3 SECONDS
        digitalWrite(ledPin, HIGH);
        dataFile = SD.open(fileName, FILE_WRITE);
        for (int i = 0; i < ARR_MAX_SIZE; i++) {
            if (!data[i].valid) { break; } // That's all the good data
            
            currentTime = data[i].time;
            data1 = data[i].data1;
            data2 = data[i].data2;
            String dataString = String(count) + "," + String(currentTime) + "," + String(data1) + "," + String(data2);
            dataFile.println(dataString);
            // Serial.println(dataString);
        }
        dataFile.close();
        digitalWrite(ledPin, LOW);
        
        // Send current count to MEGA
        if (!Serial1.available()) {
            Serial1.begin(9600);
            Serial.println("Had to restart Serial1!");
            
        }
        Serial1.print(String(count));

        digitalWrite(ledPin, LOW);
        ones++;
        count++;
    }
}
