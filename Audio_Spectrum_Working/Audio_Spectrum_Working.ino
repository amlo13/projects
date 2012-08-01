/*
 * ForgeFun.com
 * Author: Calvin Leung (calvin@forgefun.com)
 * Date: 03 May 12
 * Arduino Uno R3
 *
 * Code written for MSGEQ7 Spectrum Analyzer
 * Full guide can be found at http://forgefun.com
 * 
 * Most of this is sourced from J. Skoba's guide at:
 * http://nuewire.com/info-archive/msgeq7-by-j-skoba/
 * and n00bsys0p's guide at:
 * http://n00bsys0p.co.uk/tags/display-filter
 *
 * Other parts sourced from various other places
 * online. If you recognise your code, THANK YOU!
 *
 * Many thanks to the Arduino community and open source community!
 */

int analogPin = 0; // MSGEQ7 OUT 3
int strobePin = 2; // MSGEQ7 STROBE 4
int resetPin = 4; // MSGEQ7 RESET 7 
int spectrumValue[7];
 
// MSGEQ7 OUT pin produces values around 50-80
// when there is no input, so use this value to
// filter out a lot of the chaff.
int filterValue = 80;
 

// LEDs pins, connected to the PWM pins on the Arduino
int ledPin1 = 3;
int ledPin2 = 5;
int ledPin3 = 6;
 
int ledPin4 = 9;
int ledPin5 = 10;
int ledPin6 = 11;
 
void setup()
{
  Serial.begin(9600);
  // Read from MSGEQ7 OUT
  pinMode(analogPin, INPUT);
  // Write to MSGEQ7 STROBE and RESET
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
 
  // Set analogPin's reference voltage
  analogReference(DEFAULT); // 5V
 
  // Set startup values for pins
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
}
 
void loop()
{
  // Set reset pin low to enable strobe
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
 
  // Get all 7 spectrum values from the MSGEQ7
  for (int i = 0; i < 7; i++)
  {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30); // Allow output to settle
 
    spectrumValue[i] = analogRead(analogPin);
 
    // Constrain any value above 1023 or below filterValue
    spectrumValue[i] = constrain(spectrumValue[i], filterValue, 1023);
 
    // Remap the value to a number between 0 and 255
    spectrumValue[i] = map(spectrumValue[i], filterValue, 1023, 0, 255);
 
    // Remove serial stuff after debugging
    Serial.print(spectrumValue[i]);
    Serial.print(" ");
 
    digitalWrite(strobePin, HIGH);
   }
 
   Serial.println();
 
   // Write the PWM values to the LEDs
   analogWrite(ledPin1, spectrumValue[0]); // 63Hz
   analogWrite(ledPin2, spectrumValue[1]); // 160Hz
   analogWrite(ledPin2, spectrumValue[2]); // 400Hz
   analogWrite(ledPin3, spectrumValue[3]); // 1000Hz
   analogWrite(ledPin4, spectrumValue[4]); // 2500Hz
   analogWrite(ledPin5, spectrumValue[5]); // 6250Hz
   analogWrite(ledPin6, spectrumValue[6]); // 16000Hz
}
