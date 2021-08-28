/*
6/25/2021 ActuatorOne Rev2 by Tim "@burnermaster" Tagman for
pneumatic ignition actuator live marketing demo model constructed by 
Richard Gardner and Tim Tagman
Constructed for Fireye, Inc. Derry New Hampshire USA
Fireye, Inc.
A Carrier Company
*/

#include "Arduino.h"


int SOLState;  //solenoid output OFF state
int SOLPin=2;  //solenoid connected to pin 2
int buttonLED=3;  //button LED connected to pin 3
int value;  //floating value of button LED brightness
long time=0;  //time in millis for buttonLED brightness value calculation
int period=2500; //value for time multiplier for buttonLED brightness calculation            
int buttonPin=6; //button input connected to pin 6
int buttonNew; //floating holder for current button state
int buttonOld=1; //value for button press high
int debounce=100; //debounce to eliminate multiple or false button press sensing
int ignLED=7; //ignition LED connected to pin 7
int startDelay=2000; //delay to start sequence after button is pushed
int strokeDelay=3000; //delay to let actuator travel to max stroke
int ignitionTiming=25; //how fast the ignition LED blinks in milliseconds
int ignBlinks=10; //number of blinks
int ignMidDelay=100; //delay in between blink bursts of 3
int j;  //holder for counting number of blinks for loop
int c;  //holder for counting number of complete cycles to perform
int ignEndDelay=2000;  //delay at end of ignition flashesd before actuator retracts
int cycles=4; //how many complete cycles the actuator performs after long button press
int longCycleDelay=10000;  //time in between complete actuator cycles after long press

const int ShortPressTime = 1000;  //time in milliseconds for press calculation
const int LongPressTime = 2500;  //time in milliseconds for press calculation  

int lastState=LOW;  //the previous state from the pin
int currentState;  //the current reading from the pin
unsigned long pressedTime = 0; //pressed time starts at zero
unsigned long releasedTime = 0; //released time starts at zero

long pressDuration = releasedTime-pressedTime; //difference in time button was pressed

void setup() {
 Serial.begin(9600); //starting the serial monitor at 9600 baud rate
 pinMode(SOLPin,OUTPUT); //defining what the pins do
 pinMode(buttonPin,INPUT);
 pinMode(ignLED,OUTPUT);
 pinMode(buttonLED,OUTPUT);

}

void loop() {
  time=millis();
  value=128+127*cos(2*PI/period*time);
  analogWrite(buttonLED,value);  //makes button LED glow or “breathe”
  //Serial.println(value);  //prints 0-255 brightness of buttonLED

buttonNew=digitalRead(buttonPin);  //this is reading button state for operation of button LED only

  if (buttonOld==LOW && buttonNew==HIGH){  //turns button LED off after press or during actuator program(s)
    analogWrite(buttonLED,0);

  }
currentState = digitalRead(buttonPin);  //watching buttonPin for change of state(press)

if(lastState==LOW && currentState==HIGH)  //calculates time for press determination
	pressedTime = millis();
else if(lastState ==HIGH && currentState ==LOW) {
	releasedTime = millis();

	long pressDuration = releasedTime-pressedTime; //not sure if I need this here

	if (pressDuration < ShortPressTime)  //under the short press time, do the following:
	Serial.println ("Now executing short press program");
	delay(startDelay); //sanity delay after button is pressed
	digitalWrite(SOLPin, HIGH); //solenoid is activated
	if(SOLPin==HIGH)
	SOLState=1; //solenoid state is ON
	Serial.print("Solenoid State is:");
	Serial.println(SOLState); //prints state to serial monitor
	buttonOld=buttonNew; //current button state for button LED state
}

	if (SOLState==1){
		delay(strokeDelay); //time to allow actuator to fully extend before "ignition"
	
	for (j=1; j<ignBlinks; j=j+1){ //ignition
		digitalWrite(ignLED, HIGH); //LED ON
		delay(ignitionTiming); //LED ON time
		digitalWrite(ignLED, LOW); //LED OFF
		delay(ignitionTiming); //LED OFF TIME
		digitalWrite(ignLED, HIGH); //LED ON
		delay(ignitionTiming); //LED ON time
		digitalWrite(ignLED, LOW); //LED OFF
		delay(ignitionTiming);//LED OFF TIME
		digitalWrite(ignLED, HIGH); //LED ON
		delay(ignitionTiming);//LED ON time
		digitalWrite(ignLED, LOW); //LED OFF
		delay(ignMidDelay);//LED OFF TIME
}
	{
    	delay(ignEndDelay); //delay time between completed ignition blinks and solenoid retract
    	digitalWrite(SOLPin,LOW); //deactivate actuator solenoid
		SOLState=0; }//sets solenoid state as Home or off
	}	




    if (pressDuration > LongPressTime)  {//same as short press except for repeat cycle loop
		Serial.println("Now executing long press program");
	}
	
	for (c=1; c < cycles; c=c+1) {  //repeats entire cycle x amount of times (int cycles)
	
	if (SOLState==0){ 
	delay(startDelay);
	digitalWrite(SOLPin, HIGH);
	SOLState=1;
	Serial.println(SOLState);
	buttonOld=buttonNew;}
}

   if (SOLState==1){
		delay(strokeDelay);
   }
	 for (j = 1; j < ignBlinks; j = j + 1) {

   
		digitalWrite(ignLED, HIGH);
		delay(ignitionTiming);
		digitalWrite(ignLED, LOW);
		delay(ignitionTiming);
		digitalWrite(ignLED, HIGH);
		delay(ignitionTiming);
		digitalWrite(ignLED, LOW);
		delay(ignitionTiming);
		digitalWrite(ignLED, HIGH);
		delay(ignitionTiming);
		digitalWrite(ignLED, LOW);
		delay(ignMidDelay); 
	}
   
   

	
 		delay(ignEndDelay);
 		digitalWrite(SOLPin,LOW);
 		SOLState=0;
	
	}

		//THE END
