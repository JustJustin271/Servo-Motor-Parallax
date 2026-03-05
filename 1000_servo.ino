//Servo library
#include <Servo.h>

//Pin Connections
//13 wasn't used due to the bootloader from Arduino when powered
const int motorPin = 12;
const int clockPin = 11;      // Clockwise
const int countPin = 10;      // Counter-Clockwise
const int emergencyPin = 9;  //Pin to emergency stop to minimize damage

//Speeds for the motor
const int stopMode = 90;         
const int clockSpeed = 10;     
const int countSpeed = 170; 

//90 is a full stop
//Reminder that the motor is lowering with the force of gravity
//Formula: |90 - speed|/90 ---> percentage of max speed
//Example: |90 - 60|/90 ---> 30/90 ---> 33% of max speed


//Time durations for the motor
unsigned long totalRun = 35250; //Total time the motor will be on if the button is pressed 


unsigned long accTime = 1000;  //Time for acceleration (Milliseconds)
unsigned long decTime = 1000;  //Time for deceleration
unsigned long endCurrentTime = totalRun - decTime; 

//The motor itself
Servo parallaxServo;
//Motor, at max speed, is about 50 - 60 RPM in optimal conditions

void setup() {
  pinMode(clockPin, INPUT_PULLUP); //INPUT_PULLUP is used to be aware of static electricity in the air
  pinMode(countPin, INPUT_PULLUP); //Will be turned on only when pressed
  pinMode(emergencyPin, INPUT_PULLUP);
  
  parallaxServo.attach(motorPin); //Attaches the motor to the pin (12)
}

void loop() {

  if(digitalRead(emergencyPin) == LOW) { //Low was when the button is pressed, current goes through
    // This creates a low-voltage area from the pin
    parallaxServo.write(stopMode); //StopMode is 90, stopping the motor
    delay(15);
  }

  
  if(digitalRead(clockPin) == LOW){
    unsigned long startTime = millis();
    //Begins the clock

   while (true) { 
    unsigned long elapsedTime = millis() - startTime;
     //Takes in the relative time

    //If the button is ever pressed, a full absolute stop will be executed with priority
    if(digitalRead(emergencyPin) == LOW) {
      parallaxServo.write(stopMode);
      delay(15);
      break;
    }
    
    else if(elapsedTime <= accTime) { //If time is less than the time to accelerate, begin acceleration process
      int clockAccSpeed = map(elapsedTime, 0, accTime, stopMode, clockSpeed); //Scales the speed with the time from stop to speed
      //Takes in the elapsed time 
      //Structure: map(value, fromLow, fromHigh, toLow, toHigh)
      //Takes in the value of time, starting at 0 to acceleration, scaled from stop to go!
      //At 0, it  will result in a stop, and at accTime - 1, you will get full speed
      parallaxServo.write(clockAccSpeed); 
      //Runs that time for the speed acceleration
    } 
      
    else if ((elapsedTime > accTime) && (elapsedTime < endCurrentTime)) { //If time is between acceleration & deceleration period
      //Perform at normal speed
      parallaxServo.write(clockSpeed);
    }
    else if(elapsedTime >= endCurrentTime && elapsedTime <= totalRun) { //If time is passed main period so , deceleration begins
      int clockDecSpeed = map(elapsedTime, endCurrentTime, totalRun, clockSpeed, stopMode); //Goes from normal speed to stop
      //Structure: map(value, fromLow, fromHigh, toLow, toHigh)
      //Goes in from full speed to stop
      parallaxServo.write(clockDecSpeed);
    }
    else {
      parallaxServo.write(stopMode); //When done, stop
      break;
    }
    delay(1); //Time for the motor to get itself together, only 1/1000th of a second
   }
  }


//Same logic, just counterclockwise
    
   if(digitalRead(countPin) == LOW){
    unsigned long startTime = millis();
     //Begins timer

   while (true) { 
    unsigned long elapsedTime = millis() - startTime;
     //Records relative elapsed time

    if(digitalRead(emergencyPin) == LOW) {
      parallaxServo.write(stopMode);
      delay(15);
      break;
    }
    
    else if(elapsedTime <= accTime) {
      int countAccSpeed = map(elapsedTime, 0, accTime, stopMode, countSpeed);
      //Begins the acceleration
      //Structure: map(value, fromLow, fromHigh, toLow, toHigh)
      parallaxServo.write(countAccSpeed);
    } 
      
    else if ((elapsedTime > accTime) && (elapsedTime < endCurrentTime)) {
      //Will run at normal speed when in the middle interval
      parallaxServo.write(countSpeed);
    }
    else if(elapsedTime >= endCurrentTime && elapsedTime <= totalRun) {
      int countDecSpeed = map(elapsedTime, endCurrentTime, totalRun, countSpeed, stopMode);
      //Structure: map(value, fromLow, fromHigh, toLow, toHigh)
      parallaxServo.write(countDecSpeed);
    }
    else {
      parallaxServo.write(stopMode);
      break;
    }
    delay(1); //Makes sure the motor can handle it
   }
  }
}

//March 5th, 2026
//Servo_Motor_Code v.6
//Code for 1000 gram weight :)
