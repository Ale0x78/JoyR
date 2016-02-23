#include <ClickEncoder.h>
#include <TimerOne.h>
#include <UnoJoy.h>
ClickEncoder *encoder;
int16_t last, value;

void timerIsr() {
  encoder->service();
}

//Pins 
int armLiftPins[] = {2,3,4,5,6};
int flyWheePins[] = {7,8,9,10};
int PID = 11;
int encoderPin[] = {12,13};
int hGoal = A3;
int lGoal = A2;
int lAutoAim = A5;
int rAutoAim = A4;
int armUp = A0;
int armDown = A1;
int armLiftValue = 0;
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
  for (int i =0; i < 5; i++){
    pinMode(armLiftPins[i], INPUT);    
  }
  for (int i =0; i < 4; i++){
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
  encoder = new ClickEncoder(encoderPin[0], NULL, encoderPin[1]);
  pinSetup();
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 
  
  last = 0;
  setupUnoJoy();

}

void loop() {  
	//dataForController_t = getBlankDataForController(); // reset everything
  	dataForController_t controllerData = getControllerData();
  	setControllerData(controllerData);
}
dataForController_t getControllerData(void){
  
  dataForController_t controllerData = getBlankDataForController();
  /* When the dataForController is initilized it is filled with junk 
     This makes sure that stuff that is not used is also not set to 
     true my an accident since C thinks evertying that is not 0 is true (stupid beared people) */
  controllerData.triangleOn = 0;
  controllerData.circleOn = 0;
  controllerData.squareOn = 0;
  controllerData.crossOn = 0;
  controllerData.dpadUpOn = 0;
  controllerData.dpadDownOn = 0;
  controllerData.dpadLeftOn = 0;
  controllerData.dpadRightOn = 0;
  controllerData.homeOn = 0;
  controllerData.startOn = 0;
  controllerData.selectOn = 0;
  controllerData.leftStickX = 0;
  controllerData.rightStickX = 0;
  controllerData.l10n = 0;
  controllerData.l20n = 0;
  controllerData.l30n = 0;
  controllerData.r10n = 0;
  controllerData.r20n = 0;
  controllerData.r30n = 0;


 int ES = encoderState();

  /* Referance
   *  Circle - Encoder tick up
   *  Cross - Encoder tick down
   *  Square  - PID
   *  Triangle  - FlyWhere 0
   *  dpadDown - Low Goal
   *  dpadRight - Auto Aim right
   *  dpadLeft - Auto Aim left
   *  dpadUp - High Goal
   *  home  - FlyWhere 1
   *  start - FlyWhere 2
   *  select - FlyWhere 3
   *  LeftStick (axies) - Arm lift down/up
   *  RightStickX (axies) - Arm lift 
   *  RightStick 42 - armLiftPins 5
   *  RightStick 84 - armLiftPins 4
   *  RightStick 126 - armLiftPins 3
   *  RightStick 168 - armLiftPins 2
   *  RightStick 210 - armLiftPins 1
   *  RightStick 252 - armLiftPins 0
   */
  if(ES == -1){
    controllerData.circleOn = true;
  }else if(ES == 1){
    controllerData.crossOn = true;
  }

    controllerData.Squareon = !digitalRead(PID);
    controllerData.triangleOn = !digitalRead(flyWheePins[0]);
    controllerData.dpadDownOn = !digitalRead(lGoal);
    controllerData.dpadUpOn = !digitalRead(hGoal);
    controllerData.dpadLeftOn = !digitalRead(lAutoAim);
    controllerData.dpadRightOn = !digitalRead(rAutoAim);
    controllerData.homeOn = !digitalRead(flyWheePins[1]);
    controllerData.startOn = !digitalRead(flyWheePins[2]);
    controllerData.selectOn = !digitalRead(flyWheePins[3]);
    armLiftValue = 0;



    if   (digitalRead(armLiftPins[0])) {
      armLiftValue = 42;
    }


    else if   (digitalRead(armLiftPins[1])) {
      armLiftValue = 84;
    }


    else if   (digitalRead(armLiftPins[2])) {
      armLiftValue = 126;
    }


    else if   (digitalRead(armLiftPins[3])) {
      armLiftValue = 168;
    }


    else if   (digitalRead(armLiftPins[4])) {
      armLiftValue = 210;
    }


    else if   (digitalRead(armLiftPins[5])) {
      armLiftValue = 252;
    }



    if(digitalRead(armUp) == HIGH){
      controllerData.leftStickX = 0;
    }else if(digitalRead(armDown) == HIGH){
      controllerData.leftStickX = 255;
    }else { 
      controllerData.leftStickX = 128;
    }

 




  return controllerData;
}
