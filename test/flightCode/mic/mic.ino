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

// ***** Includes 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SD.h>
#include <SPI.h>
#define ARR_MAX_SIZE 200

// #include <vector> // C++

// ***** Vars
int sourcePin = 0;
int triggerPin = 1;
int micPin = 2;
int ledPin = 13;
int sdPin = 10;

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
    SerialUSB.begin(2000000);
    Serial.begin(96000); // For comms with master controller
    SPI.begin();
    
    pinMode(triggerPin, INPUT);
    pinMode(ledPin, OUTPUT);
    SerialUSB.println("Start");

    int tries = 0; // SD card?
    while (!SD.begin(sdPin) && tries < 4) {
        SerialUSB.println("Could not init SD card!");
        tries++;
        delay(1000);
    }

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

    // dataFile = SD.open(fileName, FILE_WRITE);
    // dataFile.println("Hey," + String(count));
    // dataFile.close();

    // count++;
    // delay(1000);
    
    if (analogRead(triggerPin) > 800) { // Look for buzzer on
        SerialUSB.println("Using filename: " + String(fileName));
        digitalWrite(ledPin, HIGH);

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

        // Continuously get data until it is over
        // USE ARRAY INSTEAD
        int index = 0;
        while (analogRead(triggerPin) > 800 && index < ARR_MAX_SIZE) {
            data[index].time = micros();
            data[index].data1 = analogRead(micPin);
            data[index].data1 = analogRead(sourcePin);
            data[index].valid = true;
            index++;
        }
        
        // Write data to SD
        dataFile = SD.open(fileName, FILE_WRITE);
        for (int i = 0; i < ARR_MAX_SIZE; i++) {
            if (!data[i].valid) { break; } // That's all the good data
            
            currentTime = data[i].time;
            data1 = data[i].data1;
            data2 = data[i].data2;
            String dataString = String(count) + "," + String(currentTime) + "," + String(data1) + "," + String(data2);
            dataFile.println(dataString);
            SerialUSB.println(dataString);
        }
        dataFile.close();

        digitalWrite(ledPin, LOW);
        ones++;
        count++;
    }
}
