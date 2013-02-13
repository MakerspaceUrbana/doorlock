
// RFID reader for Arduino 
// Wiring version by BARRAGAN <http://people.interaction-ivrea.it/h.barragan> 
// Modified for Arudino by djmatic

#include <SD.h>

int  val = 0; 
char code[11]; 
int bytesread = 0; 


void setup() { 
    digitalWrite(2, LOW);                  // Activate the RFID reader
    Serial.begin(2400); // RFID reader SOUT pin connected to Serial RX pin at 2400bps 
    Serial.print("Initializing SD card...");
    pinMode(10, OUTPUT);
    if (!SD.begin(4)) {
        Serial.println("initialization failed!");
        return;
    }
    Serial.println("Initialization done.");
    pinMode(2,OUTPUT);   // Set digital pin 2 as OUTPUT to connect it to the RFID /ENABLE pin 
}  

void loop() { 

}

code = getKey();

char getKey(void) {
    if(Serial.available() > 0) {
        if((val = Serial.read()) == 10) {
            bytesread = 0;
            while(bytesread < 10) {
                if(Serial.available() > 0) {
                    val = Serial.read();
                    if((val == 10)||(val == 13)) {
                        break;
                    }
   
if(Serial.available() > 0) {
    



char getCodeByte(void)  {
    char val;
    val = Serial.read();
    if((val == 10) || (val == 13))  {
        return 0
    }
    return val;
}   




// extra stuff
// digitalWrite(2, HIGH);             // deactivate RFID reader 
