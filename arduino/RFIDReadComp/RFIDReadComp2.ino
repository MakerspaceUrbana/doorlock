
// RFID reader for Arduino
// Wiring version by BARRAGAN <http://people.interaction-ivrea.it/h.barragan>
// Modified for Arudino by djmatic



#include <SD.h>
#define READER_CONTROL_PIN 2
char code[11];

File myFile;

// Incase we need it???
int readCard();
void activateReader();
void deactivateReader();
int checkKey();

void setup() {

	Serial.begin(2400); // RFID reader SOUT pin connected to Serial RX pin at 2400bps

	Serial.print("Initializing SD card...");
	pinMode(10, OUTPUT);

	if (!SD.begin(4)) {
		Serial.println("initialization failed!");
		return;
	}
	Serial.println("Initialization done.");

	pinMode(READER_CONTROL_PIN,OUTPUT);   // Set digital pin 2 as OUTPUT to connect it to the RFID /ENABLE pin
	digitalWrite(READER_CONTROL_PIN, LOW);                  // Activate the RFID reader
}

int checkKey() {
	// TODO: this function should call checkKey_network, then checkKey_file,
	// and so on, to check all the places
	//return checkKey_file();
}
int checkKey_file(){
	//Serial.print("TAG code is: ");   // possibly a good TAG
	//Serial.println(code);            // print the TAG code

	char fbuffer[20];
	int res;
	int foo;
	char mycmp[] = "0600966908";
	myFile = SD.open("KEYLIST.TXT");
	if (myFile) {
		//Serial.println("KEYLIST.TXT:");

		while (myFile.available()){
			foo = myFile.readBytesUntil('\n', fbuffer, 20);
			fbuffer[foo] = '\0';
			//Serial.write("Bytes read: ");
			//Serial.write(fbuffer);
            //Serial.write("\r\n");

			if (strncmp(code, fbuffer, 10) == 0 ) {
				//Serial.write("  THE ONE!!!!\r\n");
				res =  1;
                break;
			} else {
				//Serial.write("  You lose\r\n");
				res = 0;
			}
		}
	} else {
		//Serial.println("error opening file");
		res =  -1;
	}
	// close the file:
	myFile.close();
    Serial.println(res);
	return res;
}

void deactivateReader() {
	// deactivate the RFID reader
	digitalWrite(READER_CONTROL_PIN, HIGH);
}
void activateReader() {
	// Activate the RFID reader
	digitalWrite(READER_CONTROL_PIN, LOW);
}

int readCard() {
	int  val = 0;
	int nread = 0 ;
	if(Serial.available() > 0) {          // if data available from reader
		if((val = Serial.read()) == 10) {   // check for header
			nread = 0;
			while(nread<10) {              // read 10 digit code
				if( Serial.available() > 0) {
					val = Serial.read();
					if((val == 10)||(val == 13)) { // if header or stop bytes before the 10 digit reading
						break;                       // stop reading
					}
				code[nread] = val;         // add the digit
				nread++;                   // ready to read next digit
				}
			}
			code[10]='\0';
            Serial.println(code);

		}
	}
	return nread;
}

void loop() {
	activateReader();
	int bytesread = 0;
	bytesread = readCard();
	if(bytesread == 10) {              // if 10 digit read is complete
		// Deactivate and wait for a bit to avoid collisions/flooding
		deactivateReader();
		int keyOK = checkKey_file();
		if (!keyOK) {
			Serial.println("Key Invalid"); // in reality blink lights meaning NO
		} else if ( keyOK == -1 ) {
			Serial.println("there was an error"); //in reality blink lights meaning "Problem"
		} else {
		// remaining values mean ALL is well, carry on
		    Serial.println("opening door");
        }
        delay(1500); // stop flooding
	} else {
		//either do nothing or bling problem...
	}
}

