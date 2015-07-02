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
  

//   Arduino UNO pins for DIGITAL IN and OUT
//   check correspondence if you have some other board
int digIn_Pin1 = 2; // to read whether the mouse has collected the reward
int digIn_Pin2 = 4; // to detect when the 30 sec period starts
int digOut_Pin = 13; // to switch on the cue light

// Auxiliary variables adopted from the "Switch" -tutorial
// http://arduino.cc/en/Tutorial/Switch

  int state = LOW;      // the current state of the output pin
  int reading_Reward;           // the current reading from the input pin 1 (REWARD)
  int reading_Trial;           // the current reading from the input pin 2 (TRIAL)
  int previous_Reward = LOW;    // the previous reading from the input pin 1 (REWARD)
  int previous_Trial = LOW;    // the previous reading from the input pin 2 (TRIAL)
  
  // the follow variables are long's because the time, measured in miliseconds,
  // will quickly become a bigger number than can be stored in an int.
  long time = 0;         // the last time the output pin was toggled
  long debounce = 200;   // the debounce time, increase if the output flickers
  
  long timeInit = 0; // used to track whether 30 sec has passed
  long trialTime = 30000; // 30 sec in milliseconds

void setup() {

  // Set the baud rate  
  Serial.begin(57600);

  // set pin modes, note that same pins can be used both for in and out
    
    // Digital PINs  
    
      // IN
      pinMode(digIn_Pin1, INPUT);
      pinMode(digIn_Pin2, INPUT);
      
      // OUT
      pinMode(digOut_Pin, OUTPUT); 
      
}

void loop()
{
  reading_Reward = digitalRead(digIn_Pin1);
  reading_Trial = digitalRead(digIn_Pin2);
  
  // DEBUG for Serial Monitor
  // Serial.print("reward IN: ");
  // Serial.println(reading_Reward);
  // Serial.print("trialInit IN: ");
  // Serial.println(reading_Trial);
  
  // Print only when button is pressed
  if (reading_Reward == HIGH) {
    Serial.print("reward IN: ");
    Serial.println(reading_Reward);
  }
  
  
  // Option to simulate the physical signals from a Python GUI
  
  // temporary variables to override the debounce checking
  // for GUI presses
  int simulatedTrial = LOW;
  int simulatedReward = LOW; 
  
    if(Serial.available() >= 2){    
      switch( byte( Serial.read() )) {
        case 't': // trial 
          reading_Trial = HIGH;
          simulatedTrial = HIGH;
          break;
        case 'r': // reward
          reading_Reward = HIGH;
          simulatedReward = HIGH; 
          break;   
        }
     }  
  
  // for tutorials how to read digital inputs:
  // http://quarkstream.wordpress.com/2009/12/11/arduino-3-counting-events/
  // http://arduino.cc/en/Tutorial/Switch

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  
    // TRIAL_INIT SIGNAL
      if (simulatedTrial == HIGH) {
        timeInit = millis(); // If button pressed from GUI
        state = HIGH;      
      }
      
      else if (reading_Trial == HIGH && previous_Trial == LOW && millis() - time > debounce) {  
        time = millis();    
        timeInit = millis();
        state = HIGH;
      }
    
    // REWARD Signal
      if  (simulatedReward == HIGH) { // If button pressed from GUI
        state = LOW;
      }    
      
      else if (reading_Reward == HIGH && previous_Reward == LOW && millis() - time > debounce) {  
        time = millis();    
        state = LOW;
      }
    
    // CHECK whether trialTime (e.g. 30 sec) has passed
    if (millis() - timeInit > trialTime ) {
      state = LOW;      
    }

  // Write the digital output to the CUE LIGHT
  digitalWrite(digOut_Pin, state);

  // Store previous values
  previous_Reward = reading_Reward;
  previous_Trial = reading_Trial;  
  
  // delay in each loop, in milliseconds
  // delay(20);     

}
  
  

