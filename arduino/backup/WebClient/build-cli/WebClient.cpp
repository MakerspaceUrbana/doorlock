#include <Arduino.h>
/*
  Web client
 
 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 18 Dec 2009
 modified 9 Apr 2012
 by David A. Mellis
 
 */

#include <SPI.h>
#include <Ethernet.h>
void endit();

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// IPAddress server(173,194,33,104); // Google
IPAddress server(192,168,0,51);

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

char code[500];
int charPos;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  while (1) {
    if (client.connect(server, 80)) {
      Serial.println("connected");
      // Make a HTTP request:
      client.println("GET /door/1/list/index.json");
      client.println("");
      break;
    } 
    else {
      // kf you didn't get a connection to the server:
      Serial.println("connection failed");
      Serial.println("reconnecting...");
    }
    charPos = 0;
  }
}

void loop()
{
  // if there are incoming bytes available 
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
    code[charPos] = c;
    charPos++;
  }


  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    endit();
  } else {
      //Serial.print(".");
  }
}

void endit()
{
  Serial.println();
  Serial.println("disconnecting.");
  Serial.print("CharPos is: ");
  Serial.println(charPos);
  client.stop();
  Serial.println("Code is:...");
  Serial.print(code);
  Serial.println("bye");
  // do nothing forevermore:
  for(;;)
    ;
}
