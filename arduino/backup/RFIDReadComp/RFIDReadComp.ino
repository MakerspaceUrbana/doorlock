
// RFID reader for Arduino 
// Wiring version by BARRAGAN <http://people.interaction-ivrea.it/h.barragan> 
// Modified for Arudino by djmatic


int  val = 0; 
char code[11]; 
int bytesread = 0; 

#include <SD.h>

File myFile;

void setup() { 

Serial.begin(2400); // RFID reader SOUT pin connected to Serial RX pin at 2400bps 

Serial.print("Initializing SD card...");
pinMode(10, OUTPUT);

if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
    }
Serial.println("Initialization done.");

pinMode(2,OUTPUT);   // Set digital pin 2 as OUTPUT to connect it to the RFID /ENABLE pin 
digitalWrite(2, LOW);                  // Activate the RFID reader
}  

 void loop() { 

  if(Serial.available() > 0) {          // if data available from reader 
    if((val = Serial.read()) == 10) {   // check for header 
      bytesread = 0; 
      while(bytesread<10) {              // read 10 digit code 
        if( Serial.available() > 0) { 
          val = Serial.read(); 
          if((val == 10)||(val == 13)) { // if header or stop bytes before the 10 digit reading 
            break;                       // stop reading 
          } 
          code[bytesread] = val;         // add the digit           
          bytesread++;                   // ready to read next digit  
        } 
      } 
      code[10]='\0';
      
      if(bytesread == 10) {              // if 10 digit read is complete 
        Serial.print("TAG code is: ");   // possibly a good TAG 
        Serial.println(code);            // print the TAG code 
	
	char fbuffer[20];
	int foo;
	char mycmp[] = "0600966908";
	myFile = SD.open("KEYLIST.TXT");
	if (myFile) {
	   Serial.println("KEYLIST.TXT:");
	   
	   while (myFile.available()){
	     foo = myFile.readBytesUntil('\n', fbuffer, 20);
	     fbuffer[foo] = '\0';
	     Serial.write("Bytes read: ");
	     Serial.write(fbuffer);

	     if (strncmp(code, fbuffer, 10) == 0 ) {
	         Serial.write("  THE ONE!!!!\r\n");
		 break;
	     }
	     else {
	     	  Serial.write("  You lose");
	     }
	     Serial.write("\r\n");
	   }
        }
	else {
	     Serial.println("error opening file");
	}
	
	// close the file:
	myFile.close();
      }

      
    
      bytesread = 0; 
      digitalWrite(2, HIGH);                  // deactivate the RFID reader for a moment so it will not flood
           delay(1500);                       // wait for a bit 
           digitalWrite(2, LOW);                  // Activate the RFID reader
    } 
  } 
} 

// extra stuff
// digitalWrite(2, HIGH);             // deactivate RFID reader 
