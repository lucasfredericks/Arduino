
// We'll use SoftwareSerial to communicate with the XBee:
#include <SoftwareSerial.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>


const int XBeeSleep = 2; // Connect to XBee DTR for hibernation mode


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


volatile int MeasurementID = 1;
volatile char XBeeIDH[8];
volatile char XBeeIDL[8];

void sleepNow()
{
  /* Now is the time to set the sleep mode. In the Atmega8 datasheet
     http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
     there is a list of sleep modes which explains which clocks and
     wake up sources are available in which sleep modes.

     In the avr/sleep.h file, the call names of these sleep modus are to be found:

     The 5 different modes are:
         SLEEP_MODE_IDLE         -the least power savings
         SLEEP_MODE_ADC
         SLEEP_MODE_PWR_SAVE
         SLEEP_MODE_STANDBY
         SLEEP_MODE_PWR_DOWN     -the most power savings

      the power reduction management <avr/power.h>  is described in
      http://www.nongnu.org/avr-libc/user-manual/group__avr__power.html
  */

  set_sleep_mode(SLEEP_MODE_PWR_SAVE); // Sleep mode is set here

  sleep_enable();                      // Enables the sleep bit in the mcucr register
  // so sleep is possible. just a safety pin
  sleep_mode();                        // Here the device is actually put to sleep!!
  // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
  sleep_disable();                     // First thing after waking from sleep:
  // disable sleep...
}


void setup()
{
  analogReference(INTERNAL);
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.
  voltage = 0;
  lastVoltage = 0;
  XBee.begin(9600);
  Serial.begin(9600);

  // set digital pins for low power consumption
  //  for (int i = 0; i < 14; i++) {
  //    pinMode(i, OUTPUT);
  //    digitalWrite(i, LOW);
  //  }

  pinMode(XBeeSleep, OUTPUT);
  digitalWrite(XBeeSleep, 1); // Disable XBee
  digitalWrite(XBeeSleep, 0); // Enable XBee
  
}

void loop()
{

  voltage = analogRead(A1);
  voltage = (voltage / 1024.0) * 1.1;
  voltage = voltage / denominator;
  if (voltage != lastVoltage) {

    digitalWrite(XBeeSleep, 0); // Enable XBee
    // send voltage for display on Serial Monitor
    Serial.print("WM3/");
    Serial.print(voltage);
    Serial.println(",");
    XBee.write(Serial.read());
    lastVoltage = voltage;
    digitalWrite(XBeeSleep, 1); // Disable XBee

  }
  delay(250);
}
