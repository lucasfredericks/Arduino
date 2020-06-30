/*****************************************************************
  XBee_Serial_Passthrough.ino

  Set up a software serial port to pass data between an XBee Shield
  and the serial monitor.

  Hardware Hookup:
  The XBee Shield makes all of the connections you'll need
  between Arduino and XBee. If you have the shield make
  sure the SWITCH IS IN THE "DLINE" POSITION. That will connect
  the XBee's DOUT and DIN pins to Arduino pins 2 and 3.

*****************************************************************/
// We'll use SoftwareSerial to communicate with the XBee:
#include <SoftwareSerial.h>
// XBee's DOUT (TX) is connected to pin 2 (Arduino's Software RX)
// XBee's DIN (RX) is connected to pin 3 (Arduino's Software TX)
SoftwareSerial XBee(2, 3); // RX, TX

const int XBeeSleep = 2; // Connect to XBee DTR for hibernation mode
const int ledPin = 13;

void setup()
{
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  XBee.begin(9600);
  Serial.begin(9600);
  pinMode(XBeeSleep, OUTPUT);
}


void loop() {
  digitalWrite(XBeeSleep, 0);           // Enable XBee
  digitalWrite(ledPin, 1);              // Turn on Notification LED
  delay(12);                            // Give XBee time to boot up
  Serial.println("Xbee on");
  XBee.write(Serial.read());
  digitalWrite(ledPin, 0);              // Turn off Notification LED
  
  delay(2000);                          // 4 second LED blink, good for wireless programming
  digitalWrite(XBeeSleep, 1); // Disable XBee
  //Serial.println("Xbee off");
  delay(2000);
  //XBee.write(Serial.read());
}
