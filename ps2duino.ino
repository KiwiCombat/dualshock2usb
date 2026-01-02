/*
* 
* author	kiwibombat
* Copyright (c) 2025 kiwicombat
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/

#include <XInput.h>

#include <PS2X_lib.h>

#include <Arduino.h>

const boolean UseLeftJoystick = true;
const boolean InvertLeftYAxis = false;
const boolean UseRightJoystick = true;
const boolean InvertRightYAxis = false;

const boolean UseTriggerButtons = false;

const int ADC_Max = 1023;

//buttom
const int Pin_ButtonA = PSB_CROSS; //PSB_CROSS
const int Pin_ButtonB = PSB_CIRCLE;
const int Pin_ButtonX = PSB_SQUARE;
const int Pin_ButtonY = PSB_TRIANGLE;
const int Pin_ButtonLB = PSB_L1;
const int Pin_ButtonRB = PSB_R1;
const int Pin_ButtonL3 = PSB_L3;
const int Pin_buttonR3 = PSB_R3;
const int Pin_ButtonBack = PSB_SELECT;
const int Pin_ButtonStart = PSB_START;
const int Pin_DpadUp = PSB_PAD_UP;
const int Pin_DpadDown = PSB_PAD_DOWN;
const int Pin_DpadLeft = PSB_PAD_LEFT;
const int Pin_DpadRight = PSB_PAD_RIGHT;

//stick pins
const int Pin_RightJoyX = PSS_RX;
const int Pin_RightJoyY = PSS_RY;
const int Pin_LeftJoyX = PSS_LX;
const int Pin_LeftJoyY = PSS_LY;

//trigure pin?
const int Pin_TriggerL = PSAB_L2; //PSAB - analog button output
const int Pin_TriggerR = PSAB_R2;


const int NumButtons = 14;
const int Buttons[NumButtons] = {
  BUTTON_A,
  BUTTON_B,
  BUTTON_X,
  BUTTON_Y,
  BUTTON_LB,
  BUTTON_RB,
  BUTTON_BACK,
  BUTTON_START,
  BUTTON_L3,
  BUTTON_R3,
  DPAD_UP,
  DPAD_DOWN,
  DPAD_LEFT,
  DPAD_RIGHT,
};

//don't think i need these but they're here anyways
#define PS2_DAT 9
#define PS2_CMD 12
#define PS2_SEL 10
#define PS2_CLK 11
#define pressures true
#define rumble true


PS2X ps2x;

//these just stop all the errors idk they're in the example sketch too
int error = 0;
byte type = 0;
byte vibrate = 0;

void setup() {
Serial.begin(57600);

  XInput.setReceiveCallback(rumbleCallback);

  XInput.setTriggerRange(0, ADC_Max);
  XInput.begin();

delay(350); //example code has delay to allow wireless ps2 modules time to start up, will keep even though i dont have any wireless

//setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
error = ps2x.config_gamepad(11, 12, 9, 10, true, true); 

if (error == 0)
  Serial.println("controller found and configured succesfully");

  else if(error == 1)
     Serial.println("controller not found");

  else if(error == 2)
   Serial.println("controller found but not accepting commands");

  else if(error == 3)
    Serial.println("controller not entering pressures mode");
 
 type = ps2x.readType(); 
   switch(type) {
        case 0:
          Serial.println("unknown controller type");
          break;
        case 1:
          Serial.println("dualShock controller found");
          break;
        case 3:
          Serial.print("wireless dualShock controller found ");
          break;
  }
//guitar hero controller type not present because ion wanna do allat
}


void rumbleCallback(uint8_t packetType) {
  
  //big motor rumble
  if (packetType == (uint8_t) XInputReceiveType::Rumble) {
    uint8_t rumbleValue = XInput.getRumbleLeft();
    analogWrite(A2, rumbleValue);
    }
    //small motor rumble
    uint8_t rumbleR = XInput.getRumbleRight();
      if (rumbleR > 0) {
        bool rr = true;
        ps2x.read_gamepad(rr, A2);
      }
      else {
        bool rr = false;
        ps2x.read_gamepad(rr, A2);
      }
//XInput.send();
}


void loop() {

  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */ 
//skip if error
if(error == 1)
  return;

ps2x.read_gamepad(); //moved stuff but probably wont work but it doesnt give me errors

  boolean buttonA = digitalRead(PSB_CROSS); //i dont think i need to make 16 billion differnt consts if setting buttonA to the ps2x button directly will work
	boolean buttonB = digitalRead(Pin_ButtonB);
	boolean buttonX = digitalRead(Pin_ButtonX);
	boolean buttonY = digitalRead(Pin_ButtonY);

	boolean buttonLB = digitalRead(Pin_ButtonLB);
	boolean buttonRB = digitalRead(Pin_ButtonRB);

	boolean buttonBack  = digitalRead(Pin_ButtonBack);
	boolean buttonStart = digitalRead(Pin_ButtonStart);

	boolean buttonL3 = digitalRead(Pin_ButtonL3);
	boolean buttonR3 = digitalRead(Pin_buttonR3);

	boolean dpadUp    = digitalRead(Pin_DpadUp);
	boolean dpadDown  = digitalRead(Pin_DpadDown);
	boolean dpadLeft  = digitalRead(Pin_DpadLeft);
	boolean dpadRight = digitalRead(Pin_DpadRight);

	XInput.setButton(BUTTON_A, buttonA); //maybe i could even move PSB_CROSS to here, but i cant test this yet
	XInput.setButton(BUTTON_B, buttonB);
	XInput.setButton(BUTTON_X, buttonX);
	XInput.setButton(BUTTON_Y, buttonY);

	XInput.setButton(BUTTON_LB, buttonLB);
	XInput.setButton(BUTTON_RB, buttonRB);

	XInput.setButton(BUTTON_BACK, buttonBack);
	XInput.setButton(BUTTON_START, buttonStart);

	XInput.setButton(BUTTON_L3, buttonL3);
	XInput.setButton(BUTTON_R3, buttonR3);

  XInput.setDpad(dpadUp, dpadDown, dpadLeft, dpadRight);

  int trigleftt = analogRead(Pin_TriggerL);
  int trigright = analogRead(Pin_TriggerR);
  XInput.setTrigger(TRIGGER_LEFT, trigleftt);
  XInput.setTrigger(TRIGGER_RIGHT, trigright);

  int rJX = analogRead(Pin_RightJoyX);
  int rJY = analogRead(Pin_RightJoyY);
  XInput.setJoystickX(JOY_RIGHT, rJX);
  XInput.setJoystickY(JOY_RIGHT, rJY);

  int lJX = analogRead(Pin_LeftJoyX);
  int lJY = analogRead(Pin_LeftJoyY);
  XInput.setJoystickX(JOY_LEFT, lJX);
  XInput.setJoystickY(JOY_LEFT, lJY);

//const int RumbleLeft = A2;
//vibrate = A2;
//if(ps2x.Button(PSB_CROSS))
//  XInput.press(BUTTON_A);

  XInput.send();
}

