// Stephen Flores
// Read and save amplitudes of microphone to SD card
// Flow: Every 2 seconds: Sample for 500ms. Save to SD card. Wait for rest of time.
// Commit data outside 
// Use the due
// Temp sensed once in a while
// Tone only compiles on AVR, not on ARM!!!

// *** Defines (comment out the SD one to not use it)
    #define PRINT_SERIAL 1
    #define USE_SD

// *** Libs 
    #include <Wire.h>
    #include <SD.h>

// *** Objs 
    typedef struct {
        unsigned long t;
        int amp;
        int temp;
    } DataPoint_t;

// *** Time vars
    unsigned long currentTime = 0;
    unsigned long previousTime = 0;
    int delayTime = 2000; // enter sample window every delayTime ms
    int sampleWindow = 500; // sample data for sampleWindow ms

// *** Other variables
    int micPin = 0;
    int sdPin = 8;
    int tonePin = 1;
    int toneFreq = 440;
    int tempPin = 2;
    #ifdef USE_SD
        File dataFile;
        String fileName = "test1.csv";
    #endif

void setup() {
    Serial.begin(9600);
    pinMode(micPin, OUTPUT);
    pinMode(tempPin, OUTPUT);
    
    #ifdef USE_SD
    if (!SD.begin(sdPin)) {
        Serial.println("*** ERR: SD initialization failed");
    }
    #endif
    
    delay(1000);
    
    // Beep to tell that you're on
    tone(tonePin, 220); delay(100); noTone(tonePin);
    tone(tonePin, 220); delay(100); noTone(tonePin);
    tone(tonePin, 440); delay(100); noTone(tonePin);
    
    delay(1000);
}



void loop() {
    currentTime = millis();
    
    if (currentTime - previousTime >= delayTime) {
        // Time vars
        unsigned long c = millis(); unsigned long p = c;
        
        // Get place to save data
        DataPoint_t arr[500]; // Max 500, likely won't need more than that, eh?
        int index = 0;
        int temp = analogRead(tempPin);
        
        // Activate buzzer
        tone(tonePin, toneFreq);
        
        // Sample for (sW) milliseconds
        while (c - p >= sampleWindow) {
            // Get data
            int dat = analogRead(micPin);
            
            // Save it to array
            arr[index].t = c;
            arr[index].amp = dat;
            arr[index].temp = temp;
            
            // Update time
            c = millis(); index++;
        }
        
        // Deactivate buzzer
        noTone(tonePin);
        
        // Save (and maybe print) all data of array
        String dataString;
        for (int i = 0; i < 500; i++) {
            dataString = String(arr[i].t) + "," + String(i) + "," + String(arr[i].amp + "," + String(temp));
            
            #ifdef USE_SD
                dataFile = SD.open(fileName, FILE_WRITE);
                dataFile.println(dataString);
                dataFile.close();
            #endif
            
            if (PRINT_SERIAL) {
                Serial.println(dataString);
            }
        }
        
        // Update time
        currentTime = millis(); previousTime = currentTime;
    }
}