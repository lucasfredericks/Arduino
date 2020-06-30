// We'll use SoftwareSerial to communicate with the XBee:
#include <SoftwareSerial.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/io.h>
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
volatile int f_timer = 0;



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
  watchdogSetup();

  // set digital pins for low power consumption
  //  for (int i = 0; i < 14; i++) {
  //    pinMode(i, OUTPUT);
  //    digitalWrite(i, LOW);
  //  }

  pinMode(XBeeSleep, OUTPUT);
  digitalWrite(XBeeSleep, 1); // Disable XBee

}

void watchdogSetup(void) //this sets up the watchdog timer. It is configured to reset the entire arduino if the watchdog is not reset for 8 seconds in order to mitigate crashes.
{
  cli(); // disable all interrupts
  wdt_reset(); // reset the WDT timer
  /*
    WDTCSR configuration:
    WDIE = 1: Interrupt Enable
    WDE = 1 :Reset Enable
    WDP3 = 0 :For 2000ms Time-out
    WDP2 = 1 :For 2000ms Time-out
    WDP1 = 1 :For 2000ms Time-out
    WDP0 = 1 :For 2000ms Time-out
  */
  // Enter Watchdog Configuration mode:
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  // Set Watchdog settings:
  WDTCSR = (1 << WDIE) | (1 << WDE) | (1 << WDP3) | (0 << WDP2) | (0 << WDP1) | (1 << WDP0);
  sei();
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
    lastVoltage = voltage;
    digitalWrite(XBeeSleep, 1); // Disable XBee

  }
  wdt_reset();                         // feed (reset) the watchdog
  delay(200);


}
