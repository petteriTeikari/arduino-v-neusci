/*
 *  10bit PWM out from Arduino UNO
 *  demonstration modified from the Timer1 library example
 *  by Petteri Teikari, 2012 | petteri dot teikari at gmail dot com
 *  
 *  Timer1 library example
 *  June 2008 | jesse dot tane at gmail dot com
 */

// USE THE TimerOne LIBRARY
// http://www.arduino.cc/playground/Code/Timer1

  // Add the library manually to the sketch (Sketch -> "Add File")
  // http://arduino.cc/en/Guide/Environment#libraries
  
  // Or add to the system folder
  // http://arduino.cc/playground/Code/Library
  
  // Include
  #include "TimerOne.h"

// declare variables

  // PWM Output Pin
  int pinOut = 9;
  
  //  init the output channel value
  int led_dutyCycle_Out = 4; // from 0 to 1024 (10 bits)
                               // (led_dutyCycle_Out/1024)*100) % duty cycle
  
  // PWM parameters
  int PWM_period = 2500; // in microseconds [400 Hz]
  
void setup()
{
  
  // Set the baud rate  
  Serial.begin(57600);
  
  // Define output pin, and its mode
  pinMode(10, OUTPUT);
  
  // Initialize
  Timer1.initialize(PWM_period);         // initialize timer1, and set a 1/2 second period
                                     // Note that this breaks analogWrite() for digital pins 9 and 10 on Arduino.
                                     
  // Adjust the PWM period to be something else
  //Timer1.setPeriod(PWM_period);
    
  // Output the PWM                                   
  Timer1.pwm(pinOut, led_dutyCycle_Out); // setup pwm on pin X  

  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
}

void callback()
{
  digitalWrite(10, digitalRead(10) ^ 1);
}

void loop()
{
  if(Serial.available() >= 2){
      
      // The cases are given from the .py file (or some other frontend)
      switch( byte( Serial.read() )) {
        case 'r':    
          
          // now the intensity is encoded as two 8 bit integers
          // so we need to recombine
          
          // read the 8 bit ones
          int highByte = Serial.read();          
          int lowByte = Serial.read();
          
          // combine
          // e.g. 
          unsigned int led_dutyCycle_Out = highByte * 256 + lowByte;

          Serial.println(highByte);
          Serial.println(lowByte);        
          Serial.println(led_dutyCycle_Out);
          break;
      }
      
    // Output the PWM                                   
    Timer1.setPwmDuty(pinOut, led_dutyCycle_Out); // setup pwm on pin X      
      
       // delay in each loop, in milliseconds
      delay(20); // going too low maybe lead to unstable behavior
                 // default value 20 ms worked well
  }
  
  
}
