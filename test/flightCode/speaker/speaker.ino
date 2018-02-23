// Speaker
// Arduino UNO
// Sparkfun MP3 shield
// Play sound from the MP3 Shield

/*
Tests: 
twoWave8:
20cm, 30, 40, 50
*/

// CONTROL DEFINES:
// #define USE_SPEAKER
#define USE_BUZZER

// ***** SHARED:
int triggerPin = 5; // Start reading
int toneDuration = 50; // How long to play for

// ***** FOR THE SPEAKER:
#ifdef USE_SPEAKER

	// ***** Includes
	#include <SPI.h>
	#include <FreeStack.h>
	#include <MinimumSerial.h>
	#include <SysCall.h>
	#include <BlockDriver.h>
	#include <SdFat.h>
	#include <SdFatConfig.h>
	#include "SFEMP3Shield.h"

	// ***** Vars
	SFEMP3Shield mp3Shield;
	SdFat sd;
	int mp3Shield_sdPin = 9;

	// ***** Funcs
	void initSpeaker() {
		// Modifications:
		if(!sd.begin(SD_SEL, SPI_FULL_SPEED)) sd.initErrorHalt();
		if(!sd.chdir("/")) sd.errorHalt("sd.chdir");

		int result = mp3Shield.begin();
		if (result != 0) {
			Serial.print(F("Error code: "));
	    	Serial.print(result);
	    	Serial.println(F(" when trying to start MP3 player"));
	    	if( result == 6 ) {
	      		Serial.println(F("Warning: patch file not found, skipping.")); // can be removed for space, if needed.
	      		Serial.println(F("Use the \"d\" command to verify SdCard can be read")); // can be removed for space, if needed.
	    	}
		}
		mp3Shield.setVolume((uint16_t)255); // 0 to 255
		mp3Shield.setMonoMode((uint16_t)3); // 0 to 3 (?)
	}
	void play() {
		Serial.println("Playing...");

		digitalWrite(triggerPin, HIGH);
		int result = mp3Shield.playTrack((byte)1); // Needs to be a byte!! // track_0003
		if (result != 0) {
			Serial.println("Error playing track");
		}

	    delay(toneDuration);

	    Serial.println("Stopping...");
		mp3Shield.stopTrack();
		digitalWrite(triggerPin, LOW);
	    delay(3000);
	}
#endif

#ifdef USE_BUZZER

	// ***** FOR THE BUZZER:

	int buzzerPin = 10; // For passive buzzer. Also used to read source wave
	int toneFreq = 800; // Hz

	void initSpeaker() {
		pinMode(triggerPin, OUTPUT);
		pinMode(buzzerPin, OUTPUT);
	}

	void play() {
		if (Serial.available()) {
			char c = Serial.read();
			switch (c) {
				case '=':
					toneFreq += 50; 
					break;
				case '-':
					toneFreq -= 50; 
					break;
				default:
					Serial.println("Unrecognized command");
			}
		}

		Serial.println("Beep @ freq = " + String(toneFreq));
		digitalWrite(triggerPin, HIGH);
		tone(buzzerPin, toneFreq);

		delay(toneDuration);

		digitalWrite(triggerPin, LOW);
		noTone(buzzerPin);
		delay(3000);
	}

#endif

// ***** MAIN CODE (tiny!)

void setup() {
	Serial.begin(115200);
	Serial.println("Starting up");
	
	initSpeaker(); // Conditionally defined
	
    delay(1000);
}

void loop() {
	play();
}

