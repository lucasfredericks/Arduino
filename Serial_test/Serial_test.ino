#include <SoftwareSerial.h>
int i;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  i = 0;


}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("hello world <");
  Serial.print(i);
  Serial.println(">");
  i++;
  delay (1000);
}
