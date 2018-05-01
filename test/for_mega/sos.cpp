// Speed of Sound implementation file for Mega
#include "sos.h"


void SOS_setup() {
    // Start serial interface with the Due on serial 3: pins (15,14) (rx,tx)
    // Will receive data from Due here (as string)
    Serial3.begin(9600);
    pinMode(SOS_selectPin, OUTPUT);
    digitalWrite(SOS_selectPin, LOW);
}

int SOS_main() {
    // Assumed to run infinitely, in loop()
    // Every 30 seconds, ping the thing
    

    // Only run if 30 seconds have passed
    SOS_currentTime = millis();
    if (SOS_currentTime - SOS_prevTime > 30000) {
        String dataString = "";
        
        // Select the Due
        digitalWrite(SOS_selectPin, HIGH);
        
        // Play the tone on pin a15
        tone(SOS_buzzerPin, toneFreq);
        delay(50);
        noTone(SOS_buzzerPin);
        
        // Deselect the Due
        digitalWrite(SOS_selectPin, LOW);
        
        // Read serial return data from Due
        if (Serial3.available()) {
            // If available, read
            while (Serial3.available()) {
                char c = Serial3.read();
                dataString += String(c);
            }
            // Serial.println(returnString);
        } else {
            // If unavailable, try to reconnect
            Serial3.begin(9600);
            int ct = 0;
            while (!Serial3.available() && ct < 20) {
                delay(20);
                ct++;
            }
            //if (ct == 20) { Serial.println("Due data not available");};
        }
        
        // Return new value of count if it was updated
        if (dataString != "") { // Nothin
            SOS_count = dataString.toInt(); // Update
        }
    }
    
    
    return SOS_count;
}