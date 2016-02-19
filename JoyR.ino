#include <ClickEncoder.h>
#include <TimerOne.h>

ClickEncoder *encoder;
int16_t last, value;

void timerIsr() {
  encoder->service();
}

int encoderState(){ 
value += encoder->getValue();
  if (value - last == 4) { 
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

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 
  
  last = 0;
  setupUnoJoy();

}

void loop() {  
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
