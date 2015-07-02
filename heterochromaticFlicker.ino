
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
uint8_t dacpinmap[2] = {9,11};
int ledOut_ch1Pin = dacpinmap[0];
int ledOut_ch2Pin = dacpinmap[1];
int flickerPeriod = 500; // ms

//   Arduino UNO pins for DIGITAL IN (could be used also for OUTPUT)
//   check correspondence if you have some other board
uint8_t pinmap[3] = {2,4,7};
int digIn_ch1Pin = pinmap[0];
int digIn_ch2Pin = pinmap[1];
int digIn_ch3Pin = pinmap[2];

  // values to be read
  int digitalValues[3] = {0,0,0};
  
  // threshold (counts per second) to evaluate
  // whether the button is really pressed 
  int pressThreshold = 1; 
  
  // additionally we want to limit the possible 
  // frequency of "confirm presses" as it not possible
  // in the study paradigm really to confirm the flicker
  // fusion every second
  #define noOfHistorysSamples 6
  int confirmButtonHistory[noOfHistorysSamples];
  
  
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
  
    // Digital INPUT PINs  
    // as default all these are input but for 
    // keeping the code intuitive as possible
    // for beginners, they are defined
    
      // Note also that digital pins can be used
      // in "output" mode for example opening shutters,
      // relays, etc.
      pinMode(digIn_ch1Pin, INPUT);
      pinMode(digIn_ch2Pin, INPUT);
      pinMode(digIn_ch3Pin, INPUT);
}


void loop() {
  
  // RESET on each execution of the each loop the
  // variables for reading joystick and button

    // i.e. the number of each channel pressed during
    // the 1 second period (2xflickerPeriod) 
    int count_joystickUP = 0;
    int count_joystickDown = 0;
    int count_buttonPressed = 0;
  
    // if pressed or not
    int boolean_joystickUP = LOW;
    int boolean_joystickDown = LOW;
    int boolean_buttonPressed = LOW;
    int boolean_buttonPressedAfterHistoryCheck = LOW;
    
    // IF Light Intensities or the flicker frequency is changed
    // from the GUI  
    if(Serial.available() >= 2){
      
      // The cases are given from the .py file (or some other frontend)
      switch( byte( Serial.read() )) {
        case 'b': // LIGHT 1 ('blue')
          ledOut_ch1 = Serial.read();
          break;
        case 'g': // LIGHT 2 ('green')
          ledOut_ch2 = Serial.read();
          break;   
        case 'f': // Flicker Period changed
          flickerPeriod = Serial.read();
          break;   
      }
    }
  
  for (int i = 1; i <= 2; i++) {      
     int numberOfDigitalSamples = 10; 
     
     for (int j = 1; j <= numberOfDigitalSamples; j++) {      
     // Read the digital inputs
     digitalValues[0] = digitalRead(pinmap[0]);  // read input value
     digitalValues[1] = digitalRead(pinmap[1]);  // read input value
     digitalValues[2] = digitalRead(pinmap[2]);  // read input value
     
       // if you need to "filter" spurious button noise
       // check LadyAda's tutorial on digital inputs 
       // and button debouncing:
       // http://www.ladyada.net/learn/arduino/lesson5.html
       
       // We can now simply calculate the number of HIGH signals
       // from each channel, and if the channels are very noisy
       // we can use some threshold values to determine whether
       // the button/joystick was actually pressed by the subject
       if (digitalValues[0] == HIGH) {
         count_joystickUP++;
       }
       
       if (digitalValues[1] == HIGH) {
         count_joystickDown++;
       }
       
       if (digitalValues[2] == HIGH) {
         count_buttonPressed++;
       }       
       
       // delay in each loop, in milliseconds
       delay(flickerPeriod / numberOfDigitalSamples); // e.g. 500 ms
       
       // write the PWM values now to all the channels   
       if (i == 1) {
         analogWrite(ledOut_ch1Pin, ledOut_ch1); 
         analogWrite(ledOut_ch2Pin, 0);
       }
       else {
         analogWrite(ledOut_ch1Pin, 0); 
         analogWrite(ledOut_ch2Pin, ledOut_ch2);
       }     
 
              
     }   
  }  
  
  // Evaluate whether the buttons/joysticks are really pressed
  
    if (count_joystickUP >= pressThreshold) {
      boolean_joystickUP = HIGH;
      ledOut_ch1++;
    }
    
    // Evaluate whether the buttons are really pressed
    if (count_joystickDown >= pressThreshold) {
      boolean_joystickDown = HIGH;
      ledOut_ch1--;
    }
    
    // Evaluate whether the buttons are really pressed
    if (count_buttonPressed >= pressThreshold) {      
      boolean_buttonPressed = HIGH;
    }
    
      // Add the newly read boolean_buttonPressed after
      // making "room" for it using a temporary array
        int tempHistory[noOfHistorysSamples];
        for (int k = 1; k <= noOfHistorysSamples-1; k++) {      
          tempHistory[k-1] = confirmButtonHistory[k];
        }
        tempHistory[noOfHistorysSamples-1] = boolean_buttonPressed;
        
        for (int k = 0; k < noOfHistorysSamples; k++) {      
          confirmButtonHistory[k] = tempHistory[k];
        }
        
        
        // calculate the number of presses in the history
        int buttonCount = 0;
        for (int l = 0; l < noOfHistorysSamples; l++) {
          if (confirmButtonHistory[l] == HIGH) {
            buttonCount++;
          }
        }
        
    // now after HISTORY CHECK if only one button press
    // is found, then we can say that sufficient time has
    // passed since the previous button press
    if (buttonCount == 1 && boolean_buttonPressed == HIGH) {
      boolean_buttonPressedAfterHistoryCheck = HIGH;
    }
    
    
 
    // If the subject has acted somehow, serial line is printed
    if (boolean_buttonPressedAfterHistoryCheck == HIGH || boolean_joystickUP == HIGH || boolean_joystickDown == HIGH) {  
  
      // Upon action we want to know to which intensity
      // values the user confirmed the fusion,
      // so we return then
      
      Serial.print(ledOut_ch1);
      Serial.print("\t");    // prints a tab
      Serial.print(ledOut_ch2);
      Serial.print("\t");    // prints a tab
      
  
    // Boolean indicator whether confirm button is pressed
    
    if (boolean_buttonPressedAfterHistoryCheck == HIGH) {
      Serial.print(1); 
      Serial.print("\n");    // prints a line change   
    }
    
    else {
      Serial.print(0); 
      Serial.print("\n");    // prints a line change  
    }
      
    
    
    
    }
    
    // Advanced Debug if you want
      /*  
      Serial.print(count_joystickUP);
      Serial.print("\t");    // prints a tab
      Serial.print(count_joystickDown);
      Serial.print("\t");    // prints a tab
      Serial.print(count_buttonPressed);
      Serial.print("\t");    // prints a tab      
      
      Serial.print(boolean_joystickUP);
      Serial.print("\t");    // prints a tab
      Serial.print(boolean_joystickDown);
      Serial.print("\t");    // prints a tab
      Serial.print(boolean_buttonPressed);
      Serial.print("\t");    // prints a tab  
      
      Serial.print(buttonCount);
      Serial.print("\t");    // prints a tab  
      Serial.print(boolean_buttonPressedAfterHistoryCheck);
      Serial.print("\t");    // prints a tab      
      
      Serial.print("\n");    // prints a line change
      */
}
