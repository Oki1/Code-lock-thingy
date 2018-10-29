//This script sends button code over serial port, detects button presses and turns servo accordingly
//This is meant to use as a lock, but please don't. It's vulnerable as fuck. The serial port message isn't even encrypted.
//And I know. It's ugly.
//If you have any questions send them to oki.archer@gmail.com
//I will probably anwser in a few days if I check my email :D
//OKI
/**    if(myservo.read() >= 0 and myservo.read() <= 45){
        openDoor();
    }
    else if(myservo.read() <= 130 and myservo.read() > 45){
        closeDoor();
    }**/

//pin numbers
#include "Servo.h"
Servo myservo;
//LEDs
int REDLEDPIN = 11;
int GREENLEDPIN = 12;
//Buttons
int sizee = 6;
int BUTTONPINS[6] = {2, 3, 4, 5, 6, 7};

//Button states
int ButtonStates[6];
int lastButtonStates[6];

//codes
int CODELENGTH = 6;
int realcode[6] = {6, 6, 6, 6, 6, 6};
int code [6];
void closeDoor(){  //Turns servo to closed position
    myservo.write(0);
    delay(1000);
    Serial.println("closed");
}
void openDoor(){   //Turns servo to open position
    myservo.write(90);
    delay(1000);
    Serial.println("opened");
}
void setup() {
  myservo.attach(9); //attaches servo
  randomSeed(analogRead(0)); //makes sure random is truly random https://www.arduino.cc/reference/en/language/functions/random-numbers/random/ for more
  //begins serial. duhh
  Serial.begin(9600);

  //sets up LED pins
  pinMode(REDLEDPIN, OUTPUT);
  pinMode(GREENLEDPIN, OUTPUT);
  //sets up button pins
  for (int x = 0; x < sizee; x++) {
    pinMode(BUTTONPINS[x], INPUT);
  }

  //Makes sure LEDs are not on
  digitalWrite(REDLEDPIN, LOW);
  digitalWrite(GREENLEDPIN, LOW);
  //Generates code
  
}
int DetectButtonPress() {  //Detects button press
  //Renews the buttons
  for(int x = 0; x < sizee; x++){
    lastButtonStates[x] = ButtonStates[x];
    ButtonStates[x] = digitalRead(BUTTONPINS[x]);
  }
  //returns the presed button(if nothing is pressed returns one number higher than the total number of buttons)
  for(int x = 0; x < sizee; x++){
    if(lastButtonStates[x] == 1 && ButtonStates[x] == 0){
        return(x+1); //returns the button
        break;
    }
  }
  return(sizee+1); //if nothing is pressed the function
}


void loop(){
    int change = DetectButtonPress();
    if(change != sizee + 1){
        Serial.println(change);
    }
}

//BEAUTIFUL right XD
