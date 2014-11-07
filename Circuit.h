#ifndef _CIRCUIT_
#define _CIRCUIT_
#include <elapsedMillis.h>

// #define DEBUG

#define HEADROOM 180
#define HYSTERESIS 2000    // in mS units, 2000 = 2.0 seconds

#define OCCUPIED LOW
#define EMPTY    ~OCCUPIED

#include <Arduino.h>

class Circuit {
public:
    Circuit(void) {};
    void init(int number, int ir, int out, int sensor) {
        num = number;
        irPin = ir;
        outPin = out;
        sensorPin = sensor;
        detected = false;
        delaytime = 0;
        pinMode(irPin,      OUTPUT);
        pinMode(outPin,     OUTPUT);
        pinMode(sensorPin,  INPUT);
#ifdef DEBUG
        digitalWrite(outPin,EMPTY); delay(100); digitalWrite(outPin,OCCUPIED); delay(100); 
#endif
        digitalWrite(outPin,EMPTY); // Leave the detection pin 
};
    
    int check(void) {
        digitalWrite(irPin, 0);            // turn off IR transmitter
        delay(5);
        int r1 =  analogRead(sensorPin);   // read ambient light intensity
        digitalWrite(irPin, 1);            // turn ON IR source
        delay(5);
        int r2 = analogRead(sensorPin);    // see if anything is reflecting
        digitalWrite(irPin, 0);            // make sure things are turned off when done
     
        if ((r2 - r1) > HEADROOM) {        // if a major positive difference, something has been detected...
            delaytime = 0;  // expiration timer is reset every time detection is seen
            if (detected == false) { // newly triggered
#ifdef DEBUG
                Serial.print("ON  ");
                Serial.print(num, DEC);
                Serial.print(": ambient=");
                Serial.print(r1, DEC);
                Serial.print(", reflected=");
                Serial.print(r2, DEC);
                Serial.print(", diff=");
                Serial.println(r2-r1, DEC);
#endif
            }
            detected = true;
        }
        if (detected) {
            if (delaytime < HYSTERESIS) {
                digitalWrite(outPin, OCCUPIED); 
            } else {
#ifdef DEBUG
                Serial.print("OFF "); 
                Serial.println(num, DEC);
#endif
                digitalWrite(outPin, EMPTY); 
                detected = false;
            }
        }
        return detected ? 1 : 0;
    };
    
private:  
    int num;
    elapsedMillis delaytime;
    int detected;
    int irPin;
    int outPin;
    int sensorPin;
};

#endif

