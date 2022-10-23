/*
 *  Project	Arduino XInput controller for Fisher Price toy controller using Arduino Micro (inspired by Rudeism's work!)
 *  @author	Jason Sckrabulis (adapted from GamepadPins.ino example code in the Arduino XInput Library for an Arduino Leonardo -- MIT license below from that project)
*/

//----------

/*
 *  Project     Arduino XInput Library
 *  @author     David Madison
 *  @link       github.com/dmadison/ArduinoXInput
 *  @license    MIT - Copyright (c) 2019 David Madison
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *  Example:      GamepadPins
 *  Description:  Uses all of the available pin inputs to build a 'complete'
 *                Xbox gamepad, with both analog joysticks, both triggers,
 *                and all of the main buttons.
 *
 *                * Joysticks should be your typical 10k dual potentiometers.
 *                  To prevent random values caused by floating inputs,
                    joysticks are disabled by default.
 *                * Triggers can be either analog (pots) or digital (buttons).
 *                  Set the 'TriggerButtons' variable to change between the two.
 *                * Buttons use the internal pull-ups and should be connected
 *                  directly to ground.
 *
 *                These pins are designed around the Leonardo's layout. You
 *                may need to change the pin numbers if you're using a
 *                different board type
 *
 */

//----------
// Libraries
#include <XInput.h>

// Setup
const boolean UseLeftJoystick   = true;  // set to true to enable left joystick
const boolean InvertLeftYAxis   = false;  // set to true to use inverted left joy Y

const boolean UseRightJoystick  = true;  // set to true to enable right joystick
const boolean InvertRightYAxis  = false;  // set to true to use inverted right joy Y

const boolean UseTriggerButtons = true;   // set to false if using analog triggers

const int ADC_Max = 1023;  // 10 bit

// Joystick Pins
const int Pin_JoyX  = A0;
const int Pin_JoyY  = A1;

// L3 & R3 on same pin
const int Pin_ButtonJoy3 = 8;

// Button Pins
const int Pin_ButtonA = 0;   // 'D' button
const int Pin_ButtonB = 1;   // 'C' button
const int Pin_ButtonX_Back = 2;   // 'A' & 'BACK' button
const int Pin_ButtonY_Start = 3;   // 'B' & 'START' button

// Switch pin
const int Pin_LRswitch = 6;

// Shoulder buttons
// Trigger Pins
const int Pin_TriggerL = A4;
const int Pin_TriggerR = A5;

// Bumper pins
const int Pin_ButtonLB = 4;
const int Pin_ButtonRB = 5;

// Directional Pad Pins
const int Pin_DpadUp    = 10;   // '2' button
const int Pin_DpadDown  = 11;   // '4' button
const int Pin_DpadLeft  = 12;   // '1' button
const int Pin_DpadRight = 13;   // '3' button

void setup() {
	// If using buttons for the triggers, use internal pull-up resistors
	if (UseTriggerButtons == true) {
		pinMode(Pin_TriggerL, INPUT_PULLUP);
		pinMode(Pin_TriggerR, INPUT_PULLUP);
	}
	// If using potentiometers for the triggers, set range
	else {
		XInput.setTriggerRange(0, ADC_Max);
	}

	// Set buttons as inputs, using internal pull-up resistors
	pinMode(Pin_ButtonA, INPUT_PULLUP);
	pinMode(Pin_ButtonB, INPUT_PULLUP);
	pinMode(Pin_ButtonX_Back, INPUT_PULLUP);
	pinMode(Pin_ButtonY_Start, INPUT_PULLUP);

	pinMode(Pin_ButtonLB, INPUT_PULLUP);
	pinMode(Pin_ButtonRB, INPUT_PULLUP);
	
	pinMode(Pin_LRSwitch, INPUT_PULLUP);

	pinMode(Pin_ButtonJoy3, INPUT_PULLUP);

	pinMode(Pin_DpadUp, INPUT_PULLUP);
	pinMode(Pin_DpadDown, INPUT_PULLUP);
	pinMode(Pin_DpadLeft, INPUT_PULLUP);
	pinMode(Pin_DpadRight, INPUT_PULLUP);

	XInput.setJoystickRange(0, ADC_Max);  // Set joystick range to the ADC
	XInput.setAutoSend(false);  // Wait for all controls before sending

	XInput.begin();
}

void loop() {
	// Read pin values and store in variables
	// (Note the "!" to invert the state, because LOW = pressed)
	boolean buttonA = !digitalRead(Pin_ButtonA);
	boolean buttonB = !digitalRead(Pin_ButtonB);
	boolean buttonX_Back = !digitalRead(Pin_ButtonX_Back);
	boolean buttonY_Start = !digitalRead(Pin_ButtonY_Start);

	boolean buttonLB = !digitalRead(Pin_ButtonLB);
	boolean buttonRB = !digitalRead(Pin_ButtonRB);

	boolean switchLR = digitalRead(Pin_LRSwitch);
	
	boolean buttonLRJoy3 = !digitalRead(Pin_ButtonJoy3);

	boolean dpadUp    = !digitalRead(Pin_DpadUp);
	boolean dpadDown  = !digitalRead(Pin_DpadDown);
	boolean dpadLeft  = !digitalRead(Pin_DpadLeft);
	boolean dpadRight = !digitalRead(Pin_DpadRight);

	// Set XInput buttons that are not dependent on switch
	XInput.setButton(BUTTON_A, buttonA);
	XInput.setButton(BUTTON_B, buttonB);
	
	XInput.setButton(BUTTON_LB, buttonLB);
	XInput.setButton(BUTTON_RB, buttonRB);
	
	XInput.setButton(BUTTON_L3, buttonL3);
	XInput.setButton(BUTTON_R3, buttonR3);	
	
	// Set XInput DPAD values
	XInput.setDpad(dpadUp, dpadDown, dpadLeft, dpadRight);
	
	// Set XInput trigger values
	if (UseTriggerButtons == true) {
		// Read trigger buttons
		boolean triggerLeft  = !digitalRead(Pin_TriggerL);
		boolean triggerRight = !digitalRead(Pin_TriggerR);

		// Set the triggers as if they were buttons
		XInput.setButton(TRIGGER_LEFT, triggerLeft);
		XInput.setButton(TRIGGER_RIGHT, triggerRight);
	}
	else {
		// Read trigger potentiometer values
		int triggerLeft  = analogRead(Pin_TriggerL);
		int triggerRight = analogRead(Pin_TriggerR);

		// Set the trigger values as analog
		XInput.setTrigger(TRIGGER_LEFT, triggerLeft);
		XInput.setTrigger(TRIGGER_RIGHT, triggerRight);
	}
	
	// Set XInput buttons and joysticks that are dependent on switch
	// Switch set to numbers == default state, LOW)
	// Default state  = left joystick and normal face buttons
	if (switchLR == LOW) {
		
		// Set normal face buttons
		XInput.setButton(BUTTON_X, buttonX_Back);
		XInput.setButton(BUTTON_Y, buttonY_Start);

		// Set left joystick
		if (UseLeftJoystick == true) {
			int JoyX = analogRead(Pin_JoyX);
			int JoyY = analogRead(Pin_JoyY);

			// White lie here... most generic joysticks are typically
			// inverted by default. If the "Invert" variable is false
			// then we'll take the opposite value with 'not' (!).
			boolean invert = !InvertLeftYAxis;

			XInput.setJoystickX(JOY_LEFT, JoyX);
			XInput.setJoystickY(JOY_LEFT, JoyY, invert);
		}
	}
	else if {
		
		// Set modified face buttons
		XInput.setButton(BUTTON_BACK, buttonX_Back);
		XInput.setButton(BUTTON_START, buttonY_Start);
		
		// Set right joystick
		if (UseRightJoystick == true) {
			int JoyX = analogRead(Pin_JoyX);
			int JoyY = analogRead(Pin_JoyY);

			boolean invert = !InvertRightYAxis;

			XInput.setJoystickX(JOY_RIGHT, JoyX);
			XInput.setJoystickY(JOY_RIGHT, JoyY, invert);
		}

	// Send control data to the computer
	XInput.send();
}
