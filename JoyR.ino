#include <ClickEncoder.h>
#include <TimerOne.h>

ClickEncoder *encoder;
int16_t last, value;

void timerIsr() {
  encoder->service();
}

//Pins 
int armLiftPins[] = {2,3,4,5,6}
int flyWheePins[] = {7,8,9,10}
int PID = 11;
int encoder[] = {12,13}
int hGoal = A3;
int lGoal = A2;
int lAutoAim = A5;
int rAutoAim = A4;


/* Referance 

The digital pins are counting from the lowest button to the highest
The analog pins are as follows
High Goal A3
Low  Goal A2
Auto Aim right A4
Auto Aim left A5
arm lift A1 (lower) A0 (higher)
*/
void pinSetup(){
  for (int i =0; i < armLiftPins.length(); i++){
    pinMode(armLiftPins[i], INPUT);    
  }
  for (int i =0; i < flyWheePins.length(); i++){
    pinMode(flyWheePins[i], INPUT);    
  }
  pinMode(PID, INPUT);    
}

int encoderState(){ 
value += encoder->getValue();
  if (value - last != 0) { 
                        // Each tick the value changes 4 times, 
                        //Sometimes it does 3 if you shake 
                        //it voilently and thurn really slowly
  if (value > last) {
    last = value;
    return 1;

  }
  else if (value < last) {
    last = value;
    return -1;

  }
  }
  return 0;
}

void setup() {
  Serial.begin(9600);
  encoder = new ClickEncoder(A1, A0, A2);
  pinSetup();
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 
  
  last = 0;
  setupUnoJoy();

}

void loop() {  
	dataForController_t = getBlankDataForController(); // reset everything
  	dataForController_t controllerData = getControllerData();
  	setControllerData(controllerData);
}
dataForController_t getControllerData(void){
  
  dataForController_t controllerData = getBlankDataForController();
  /* When the dataForController is initilized it is filled with junk 
     This makes sure that stuff that is not used is also not set to 
     true my an accident since C thinks evertying that is not 0 is false (stupid beared people) */
  controllerData.triangleOn = false;
  controllerData.circleOn = false;
  controllerData.squareOn = false;
  controllerData.crossOn = false;
  controllerData.dpadUpOn = false;
  controllerData.dpadDownOn = false;
  controllerData.dpadLeftOn = false;
  controllerData.dpadRightOn = false;

  ES = encoderState();

  if(ES == -1){
    controllerData.circleOn = true;
  }else if(ES == 1){
    controllerData.crossOn = true;
  }


  
  controllerData.startOn = !digitalRead(StartPin);




  return controllerData;
}
