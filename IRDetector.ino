/*
 Arduino based proximity sensor
 Based very loosely on work done by Dustin Andrews 2012
 License: Creative Commons Unported http://creativecommons.org/licenses/by/3.0/
 */
#include <elapsedMillis.h>

//#define DEBUG
#ifdef DEBUG
elapsedMillis t1;
#endif

#include "Circuit.h"

Circuit circuit[4];

void setup() 
{       
#ifdef DEBUG
    // Configure the serial port
    Serial.begin(115200);
    Serial.println("IR Detector");  
    t1 = 0; 
#endif

    analogReference(EXTERNAL);
    circuit[0].init(0,2,6,14);
    circuit[1].init(1,3,7,15);
    circuit[2].init(2,4,8,16);
    circuit[3].init(3,5,9,17);
    pinMode(10,      OUTPUT); digitalWrite(10, LOW);
    pinMode(11,      OUTPUT); digitalWrite(11, LOW);
    pinMode(12,      OUTPUT); digitalWrite(12, LOW);
    pinMode(13,      OUTPUT); digitalWrite(13, LOW);
}

void loop() 
{
#ifdef DEBUG
    if (t1 > 100) { // delay between checks to keep serial debug stream reasonable
        for (int x = 0; x < 4; x++) {
            circuit[x].check();
        }
        t1 = 0;
    }
#else // no delay needed
    for (int x = 0; x < 4; x++) {
        circuit[x].check();
    }
#endif
}

