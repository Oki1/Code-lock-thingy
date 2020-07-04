//This script sends button code over serial port, detects button presses and turns servo accordingly
//This is meant to use as a lock, but please don't. It's vulnerable as fuck. The serial port message isn't even encrypted.
//And I know. It's ugly.
//If you have any questions send them to oki.archer@gmail.com
//I will probably anwser in a few days if I check my email :D
//OKI

//P.S Theres a lot of commented debugging. I had to rewrite the whole script and i really didn't wanna go trought the trouble of deleting it

// define if you want to print to console
//#define DEBUG

#ifdef DEBUG
#define LOG(x) Serial.println(x)
#else
#define LOG(x)
#endif

//pin numbers
#include "Servo.h"
Servo myservo;

//LEDs
#define REDLEDPIN 11
#define GREENLEDPIN 12

//Buttons
#define sizee 6
int BUTTONPINS[] = {2, 3, 4, 5, 6, 7};

//Button states
int ButtonStates[sizee];
int lastButtonStates[sizee];

//codes
#define CODELENGTH sizee;
int realcode[sizee] = {};
int code [sizee];

//bools
bool codeDone = false;

void closeDoor() {  //Turns servo to closed position
    myservo.write(0);
    delay(1000);
    LOG("closed");
}
void openDoor() {   //Turns servo to open position
    myservo.write(90);
    delay(1000);
    LOG("opened");
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
    for(int x = 0; x < sizee; x++) {
        lastButtonStates[x] = ButtonStates[x];
        ButtonStates[x] = digitalRead(BUTTONPINS[x]);
    }
    //returns the presed button(if nothing is pressed returns one number higher than the total number of buttons)
    for(int x = 0; x < sizee; x++) {
        if(lastButtonStates[x] == 1 && ButtonStates[x] == 0) {
            return ++x; //returns the button
        }
    }
    return 0; //if nothing is pressed the function
}
void generateNewCode() {  //generates new code
    String temp = ""; //temporary string so I can print the whole code in one go
    for(int x = 0; x < CODELENGTH; x++) {  //generates each number of code separatly <<spelling?
        realcode[x] = random(1, sizee);
        temp += realcode[x];
    }
    LOG(temp);
}

//ˇˇfound this little script on http://forum.arduino.cc/index.php/topic,42205.0.html
void reset() { // Restarts program from beginning but does not reset the peripherals and registers
    for(int i = 0; i < 3; i++) { //<<DAMN UGLY light blink
        digitalWrite(REDLEDPIN, HIGH);
        delay(200);
        digitalWrite(REDLEDPIN, LOW);
        delay(200);
    }
    asm volatile ("  jmp 0");  
}  
void addPress(int button) { //adds button press to the code array
    for(int x = 0; x < CODELENGTH; x++) {
        if(!code[x]) {
            code[x] = button;
            break;
        }
    }
    if(code[CODELENGTH - 1]) { //if the last code number isn't zero
        codeDone = true;
    }
}
bool detectIfRight() { //detects if code is right
    //vvvTESTINGvvv
/*    for(int x = 0; x < CODELENGTH; x ++){
        Serial.print(code[x]);
    }
    LOG("\n\n");
    */
    
    for(int x = 0; x < CODELENGTH; x++) {
        if(code[x] != realcode[x]) {
            return false;
        }
    }
    return true;
}
void lastStep() {  //what happenes when you enter the write code
    digitalWrite(GREENLEDPIN, HIGH);
    if(DetectButtonPress() != 7) {
        if(myservo.read() >= 0 && myservo.read() <= 45) {
            openDoor();
        }
        else if(myservo.read() <= 130 && myservo.read() > 45) {
            closeDoor();
        }
    }
}

void loop() {
    if(!codeDone) {   //if the code isn't done. for example the code is 123120
        int change = DetectButtonPress();
        if(change) {  //if a button was pressed
            addPress(change);     //add that button to the code array
        }
    }
    else {   //if code is done. for example the code is 123123
        if(detectIfRight())
            lastStep();
        else {
            reset();
            LOG("This shouldn't ever run. Crap");
        }
    }
}


//BEAUTIFUL right XD
