#include <XInput.h>

const boolean UseLeftJoystick = true;
const boolean InvertLeftYAxis = false;
const boolean UseRightJoystick = true;
const boolean InvertRightYAxis = false;

const boolean UseTriggerButtons = false;

const int ADC_Max = 1023;

const int Pin_LeftJoyX = PSS_LX;
//^^ doesnt work (obviously)

#include <PS2X_lib.h>

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

void loop() {

  /* You must Read Gamepad to get new values and set vibration values
     ps2x.read_gamepad(small motor on/off, larger motor strenght from 0-255)
     if you don't enable the rumble, use ps2x.read_gamepad(); with no values
     You should call this at least once a second
   */ 
if(error == 1)
  return;

if(ps2x.Button(PSB_CROSS))
  XInput.press(BUTTON_A);

}
