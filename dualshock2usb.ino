/*
* 
* author	kiwicombat
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

#include <PsxControllerBitBang.h>
#include <PsxNewLib.h>
#include <XInput.h>

const boolean UseLeftJoystick = true;
const boolean InvertLeftYAxis = false;
const boolean UseRightJoystick = true;
const boolean InvertRightYAxis = false;

const boolean UseTriggerButtons = false;

const int ADC_Max = 1023;

boolean haveController = false;

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

PsxControllerBitBang<10, 12, 9, 11> psx, psxCtrl;  //PIN_PS2_ATT, PIN_PS2_CMD, PIN_PS2_DAT, PIN_PS2_CLK

void setup() {
  Serial.begin(57600);
  psxCtrl.begin();

  XInput.setReceiveCallback(rumbleCallback);
  XInput.setTriggerRange(0, 1023);
  XInput.begin();
}

void rumbleCallback(uint8_t packetType) {

  //big motor rumble
  if (packetType == (uint8_t)XInputReceiveType::Rumble) {
    uint8_t rumbleValue = XInput.getRumbleLeft();
    analogWrite(A2, rumbleValue);
  }
  //small motor rumble
  uint8_t rumbleR = XInput.getRumbleRight();
  if (rumbleR > 0) {
    bool rr = true;
    psxCtrl.setRumble(0xff, 0xFF);  //need to convert rumble Value (0-255) to hexadecimal (0x00-0xFF)
  } else {
    bool rr = false;
    psxCtrl.setRumble(0x00, 0x00);
  }
}

void loop() {

  //debug rubbish
  if (!haveController) {
    if (psx.begin()) {
      Serial.println("controller found");
      if (!psx.enterConfigMode()) {
        Serial.println("cannon anter config mode");
      } else {
        if (!psx.exitConfigMode()) {
          Serial.println("unable to exit config mode");
        }
      }
      haveController = true;
    }
  } else {
    if (!psx.read()) {
      Serial.println("controller loss");
      haveController = false;
    }
    if (psxCtrl.enterConfigMode() && psxCtrl.enableAnalogSticks(true, true)) {
      psxCtrl.enableRumble();
      psxCtrl.exitConfigMode();
      Serial.println("Axis sticks enabled! Rumble enabled!");
    } else {
      Serial.println("Failed to enable axis sticks!");
    }
  }

  psxCtrl.read();
  psx.read();
  byte x, y;

  boolean buttonA = digitalRead(PSB_CROSS);
  boolean buttonB = digitalRead(PSB_CIRCLE);
  boolean buttonX = digitalRead(PSB_SQUARE);
  boolean buttonY = digitalRead(PSB_TRIANGLE);

  boolean buttonLB = digitalRead(PSB_L1);
  boolean buttonRB = digitalRead(PSB_R1);

  boolean buttonBack = digitalRead(PSB_SELECT);
  boolean buttonStart = digitalRead(PSB_START);

  boolean buttonL3 = digitalRead(PSB_L3);
  boolean buttonR3 = digitalRead(PSB_R3);

  boolean dpadUp = digitalRead(PSB_PAD_UP);
  boolean dpadDown = digitalRead(PSB_PAD_DOWN);
  boolean dpadLeft = digitalRead(PSB_PAD_LEFT);
  boolean dpadRight = digitalRead(PSB_PAD_RIGHT);

  XInput.setButton(BUTTON_A, PSB_CROSS);
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

  int trigleftt = analogRead(PSAB_L2);
  int trigright = analogRead(PSAB_R2);
  XInput.setTrigger(TRIGGER_LEFT, trigleftt);
  XInput.setTrigger(TRIGGER_RIGHT, trigright);

  if (psx.getRightAnalog(x, y)) {
    XInput.setJoystickX(JOY_RIGHT, x);
    XInput.setJoystickY(JOY_RIGHT, y);
  }

  if (psx.getLeftAnalog(x, y)) {
    XInput.setJoystickX(JOY_LEFT, x);
    XInput.setJoystickY(JOY_LEFT, y);
  }


  XInput.send();
}
