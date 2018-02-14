// Yea or na?
/*
This is a test of the SAMD board 
*/


void setup() {
    SerialUSB.begin(115200);
    pinMode(13, OUTPUT);
}

bool go = true;
void loop() {
	// Listen for serial stuff
	if (SerialUSB.available()) {
		char c = SerialUSB.read();
		switch (c) {
			case 'a':
				go = true;
				break;
			case 'b':
				go = false;
				break;
			default:
				SerialUSB.println("Wrong!");
		}
	}

    if (go) {
    	SerialUSB.println("Hoi");
	    digitalWrite(13, HIGH);
	    delay(500);
	    digitalWrite(13, LOW);
	    delay(500);
    }
}