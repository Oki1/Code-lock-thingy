//This script sends button code over serial port, detects button presses and turns servo accordingly
//This is meant to use as a lock, but please don't. It's vulnerable as fuck. The serial port message isn't even encrypted.
//And I know. It's ugly.
//If you have any questions send them to oki.archer@gmail.com
//I will probably anwser in a few days if I check my email :D
//OKI

//P.S Theres a lot of commented debugging. I had to rewrite the whole script and i really didn't wanna go trought the trouble of deleting it
/*    if(myservo.read() >= 0 and myservo.read() <= 45){
        openDoor();
    }
    else if(myservo.read() <= 130 and myservo.read() > 45){
        closeDoor();
    }*/
/*void reset(){   //resets everything. Runs when the code is wrong
    
    for(int x = 0; x < CODELENGTH; x++){  //clears code
        code[x] = 0;
        //realcode[x] = 0;
    }
    codeDone = false;               //resets code done
    right = false;
    digitalWrite(REDLEDPIN, HIGH);
    delay(1000);
    digitalWrite(REDLEDPIN, LOW);
    generateNewCode();
}*/
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
int realcode[6] = {};
int code [6];

//bools
bool codeDone = false;
bool right  = false;
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
  generateNewCode();
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
void generateNewCode(){  //generates new code
    String temp = ""; //temporary string so I can print the whole code in one go
    for(int x = 0; x < CODELENGTH; x++){  //generates each number of code separatly <<spelling?
        realcode[x] = random(1, sizee);
        temp += realcode[x];
    }
    Serial.println(temp);
}

//ˇˇfound this little script on http://forum.arduino.cc/index.php/topic,42205.0.html
void reset() // Restarts program from beginning but does not reset the peripherals and registers
{
    asm volatile ("  jmp 0");  
}  
void addPress(int button){ //adds button press to the code array
    for(int x = 0; x < CODELENGTH; x ++){
        if(code[x] == 0){
            code[x] = button;
            break;
        }

    }
    if(code[CODELENGTH-1] != 0){ //if the last code number isn't zero
        codeDone = true;
    }
}
bool detectIfRight(){ //detects if code is right
    //vvvTESTINGvvv
/*    for(int x = 0; x < CODELENGTH; x ++){
        Serial.print(code[x]);
    }
    Serial.println("\n\n");
    */
    for(int x = 0; x < CODELENGTH; x ++){
        if((code[x] == realcode[x]) == false){
            right = false;
            reset();
            break;
            Serial.println("This shouldn't ever run. Crap");
        }
        
    }
    right = true;
}
void lastStep(){  //what happenes when you enter the write code
    Serial.println("right done shit");
}

void loop(){
    if(!codeDone){   //if the code isn't done. for example the code is 123120
        int change = DetectButtonPress();
        if(change != sizee + 1){  //if a button was pressed
            addPress(change);     //add that button to the code array
        }
    }
    else{   //if code is done. for example the code is 123123
        if(right == false){
            detectIfRight();
        }else if (right == true){
            lastStep();
        }
    }
}


//BEAUTIFUL right XD
