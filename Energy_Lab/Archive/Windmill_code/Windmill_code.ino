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
//
int NUM_SAMPLES = 20;

float voltage = 0.0;            // calculated voltage
float R1 = 300000.0;    //resistor 1 value (ohms)
float R2 = 51000.0;    //resistor 2 value (ohms)
float denominator = (R2) / (R1 + R2);
float sample = 0;
float lastVoltage = 0;


void setup()
{
  analogReference(INTERNAL);
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  voltage = analogRead(A1);
  lastVoltage = analogRead(A1);
  XBee.begin(9600);
  Serial.begin(9600);
}

void loop()
{

  voltage = analogRead(A1);
  voltage = (voltage / 1024.0) * 1.1;
  voltage = voltage / denominator;
  if (voltage != lastVoltage) {

    // send voltage for display on Serial Monitor
    Serial.print("WM4/");
    Serial.print(voltage);
    Serial.println(","); 
    lastVoltage = voltage;

    if (Serial.available())
    { // If data comes in from serial monitor, send it out to XBee
      XBee.write(Serial.read());
    }
  }
  delay(100);
}
