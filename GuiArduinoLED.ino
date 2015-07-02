//   Sketch for Arduino UNO, to control LEDs and monitor inputs
//   Copyright (C) 2012 Petteri Teikari <petteri.teikari@gmail.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program; if not, write to the Free Software
//   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  // It takes about 100 microseconds (0.0001 s) to read an analog input, 
  // so the maximum reading rate is about 10,000 times a second.
  // In order to include the overhead of the code, 200μs. was decided by balau82
  // http://balau82.wordpress.com/2011/03/26/capturing-an-analog-signal-with-arduino-and-python/
  // if ANALOG INPUT want to be implemented
  
//   Arduino UNO pins for PWM OUTPUT
//   check correspondence if you have some other board
//   6 x PWM Outputs
uint8_t dacpinmap[6] = {3,5,6,9,10,11};
int ledOut_ch1Pin = dacpinmap[0];
int ledOut_ch2Pin = dacpinmap[1];
int ledOut_ch3Pin = dacpinmap[2];
int ledOut_ch4Pin = dacpinmap[3];
int ledOut_ch5Pin = dacpinmap[4];
int ledOut_ch6Pin = dacpinmap[5];

// INIT VARIABLES
int h_int;
float h;

//   init the output channel values
int ledOut_ch1=0, ledOut_ch2=0, ledOut_ch3=0, ledOut_ch4=0, ledOut_ch5=0, ledOut_ch6=0;


void setup() {

  // Set the baud rate  
  Serial.begin(57600);

  // set pin modes, note that same pins can be used both for in and out
  
    // PWM PINs
    pinMode(ledOut_ch1Pin, OUTPUT);
    pinMode(ledOut_ch2Pin, OUTPUT);
    pinMode(ledOut_ch3Pin, OUTPUT);
    pinMode(ledOut_ch4Pin, OUTPUT);
    pinMode(ledOut_ch5Pin, OUTPUT);
    pinMode(ledOut_ch6Pin, OUTPUT);

}


void loop() {
  
  if(Serial.available() >= 2){
    
    // The cases are given from the .py file (or some other frontend)
    switch( byte( Serial.read() )) {
      case 'r':
        ledOut_ch1 = Serial.read();
        break;
      case 'g':
        ledOut_ch2 = Serial.read();
        break;   
      case 'b':
        ledOut_ch3 = Serial.read();
        break;
      case 'w':
        ledOut_ch4 = Serial.read();
        break;
      case 'v':
        ledOut_ch5 = Serial.read();
      case 'y':
        ledOut_ch6 = Serial.read();
      }
   }    
   
   // write the PWM values now to all the channels   
   analogWrite(ledOut_ch1Pin, ledOut_ch1); 
   analogWrite(ledOut_ch2Pin, ledOut_ch2);
   analogWrite(ledOut_ch3Pin, ledOut_ch3);
   analogWrite(ledOut_ch4Pin, ledOut_ch4); 
   analogWrite(ledOut_ch5Pin, ledOut_ch5);
   analogWrite(ledOut_ch6Pin, ledOut_ch6);
   
   // delay in each loop, in milliseconds
   delay(20); // going too low maybe lead to unstable behavior
              // default value 20 ms worked well
   
}
