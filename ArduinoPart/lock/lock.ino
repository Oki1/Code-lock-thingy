//This script sends button code over serial port, detects button presses and turns servo accordingly
//This is meant to use as a lock, but please don't. It's vulnerable as fuck. The serial port message isn't even encrypted.
//And I know. It's ugly.
//OKI


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
bool first = true;
bool right = false;
bool alldone = false;
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
void generateRandomCode() { //read the function name
  String temp = "";
  for (int x = 0; x < CODELENGTH; x++) {
    realcode[x] = int(random(1, sizee));
    temp += realcode[x];
  }
  Serial.println(temp); //Sends code over Serial port. NOT ENCRYPTED!!!
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
  generateRandomCode();
}
void resetButtonStates() {  //resets button states
  for (int x = 0; x < sizee; x++) {
    lastButtonStates[x] = ButtonStates[x];
    ButtonStates[x] = digitalRead(BUTTONPINS[x]);
  }
}
int DetectChange() {  //Detects when a button is pressed
  for (int y = 0; y < sizee; y++) {
    //Serial.println(y);
    if (lastButtonStates[y] == 1 and ButtonStates[y] == 0) {
      return (y + 1);
    }
  }
  return (sizee + 1);
}
void addCode(int chang) {  //adds button to code
  for (int x = 0; x < CODELENGTH; x++){
    Serial.println(code[x]);
    if (code[x] == 0) {
      code[x] = chang;
      break;
    }
    else if (x == CODELENGTH - 1) {
      //all numbers are entered
      alldone = true;
    }
  }
}
void reset() { //resets everything
    Serial.println("resetting");
  for (int x = 0; x < CODELENGTH; x++) {
    code[x] = 0;
  }
  for (int x = 0; x < sizee; x++) {
    ButtonStates[x] = 0;
    lastButtonStates[x] = 0;
  }
  alldone = false;
  first = true;
  right = false;
  digitalWrite(GREENLEDPIN, LOW);
  generateRandomCode();
}
void Win() {  //what happenes when the code is right
    if(myservo.read() >= 0 and myservo.read() <= 45){
        openDoor();
    }
    else if(myservo.read() <= 130 and myservo.read() > 45){
        closeDoor();
    }
  Serial.println("DONE");
  digitalWrite(GREENLEDPIN, HIGH);
  delay(3000);
  reset();
}
void DetectIfRight() {  //detects if all codes are right and acts accordingly
  if (not alldone) {
    
    resetButtonStates();
    int change = DetectChange();
    if (change != sizee + 1) {
      addCode(change);
    }
  }
  else {
    if ((not right) and first) {
      for (int x = 0; x < CODELENGTH; x++) {
        if (code[x] == realcode[x]) {
          if (x == CODELENGTH - 1) {
            Win();
          }
        }
        else {
          first = false;
          digitalWrite(REDLEDPIN, HIGH);
          delay(200);
          digitalWrite(REDLEDPIN, LOW);
          delay(200);
          digitalWrite(REDLEDPIN, HIGH);
          delay(200);
          digitalWrite(REDLEDPIN, LOW);
          delay(200);
          digitalWrite(REDLEDPIN, HIGH);
          delay(200);
          digitalWrite(REDLEDPIN, LOW);
          break;
        }

      }
    }
    else if ((not right) and (not first)) {
      reset();
    }
  }
}
void loop() {  //Every part if this mess in one function
  DetectIfRight();
}
//BEAUTIFUL right XD
