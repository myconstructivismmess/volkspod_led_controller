// +------------------------------------------------------------------------------+
// | filename: volkspod_led_controller.ino                                        |
// | filepath: volkspod_led_controller/volkspod_led_controller.ino                |
// | project name: volkspod_led_controller                                        |
// | authors: Aubrey ROUET <aubrey.rouet@gmail.com>                               |
// | license: MIT                                                                 |
// |                                                                              |
// | Copyright (c) 2025 Aubrey ROUET                                              |
// |                                                                              |
// | Permission is hereby granted, free of charge, to any person obtaining a copy |
// | of this software and associated documentation files (the "Software"), to     |
// | deal in the Software without restriction, including without limitation the   |
// | rights to use, copy, modify, merge, publish, distribute, sublicense, and/or  |
// | sell copies of the Software, and to permit persons to whom the Software is   |
// | furnished to do so, subject to the following conditions:                     |
// |                                                                              |
// | The above copyright notice and this permission notice shall be included in   |
// | all copies or substantial portions of the Software.                          |
// |                                                                              |
// | THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR   |
// | IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,     |
// | FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE  |
// | AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER       |
// | LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      |
// | FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS |
// | IN THE SOFTWARE.                                                             |
// +------------------------------------------------------------------------------+



// ----- External Libraries -------------------------------------------------------

#include <Adafruit_NeoPixel.h>



// ----- Internal Libraries -------------------------------------------------------

#include "simplified_switch.h"
#include "neopixel_manager.h"



// ----- Settings -----------------------------------------------------------------

// Debug Settings
#define DEBUG


// Experimentation Sensors And Switches Settings
#define BRAKE_SENSOR_PIN 12
#define TURN_SIGNAL_LEFT_SWITCH_PIN 11
#define TURN_SIGNAL_RIGHT_SWITCH_PIN 10
#define LIGHT_ON_OFF_SWITCH_PIN 9

// Sensors And Switches Settings
//#define BRAKE_SENSOR_PIN 5
//#define TURN_SIGNAL_LEFT_SWITCH_PIN 2
//#define TURN_SIGNAL_RIGHT_SWITCH_PIN 3
//#define LIGHT_ON_OFF_SWITCH_PIN 4


// Experimentation Neopixel Settings
#define FRONT_NEOPIXEL_PIN 6
#define FRONT_NEOPIXEL_PIXEL_COUNT 60
#define BACK_NEOPIXEL_PIN 5
#define BACK_NEOPIXEL_PIXEL_COUNT 37 // 16 + 12 + 8 + 1

// Neopixel Settings
//#define FRONT_NEOPIXEL_PIN 6
//#define FRONT_NEOPIXEL_PIXEL_COUNT 60
//#define BACK_NEOPIXEL_PIN 7
//#define BACK_NEOPIXEL_PIXEL_COUNT 37 // 16 + 12 + 8 + 1



// ----- Switches and Sensors -----------------------------------------------------

SimplifiedSwitch brakeSensor(BRAKE_SENSOR_PIN);
SimplifiedSwitch turnSignalLeftSwitch(TURN_SIGNAL_LEFT_SWITCH_PIN);
SimplifiedSwitch turnSignalRightSwitch(TURN_SIGNAL_RIGHT_SWITCH_PIN);
SimplifiedSwitch ledOnOffSwitch(LIGHT_ON_OFF_SWITCH_PIN);



// ----- Neopixels ----------------------------------------------------------------

Adafruit_NeoPixel frontNeopixel(
    FRONT_NEOPIXEL_PIXEL_COUNT,
    FRONT_NEOPIXEL_PIN,
    NEO_GRB + NEO_KHZ800
);
NeopixelManager frontNeopixelManager(&frontNeopixel);

Adafruit_NeoPixel backNeopixel(
    BACK_NEOPIXEL_PIXEL_COUNT,
    BACK_NEOPIXEL_PIN,
    NEO_GRB + NEO_KHZ800
);
NeopixelManager backNeopixelManager(&backNeopixel);



// ----- Front Neopixel Layers ----------------------------------------------------

//NeopixelLayer ledOnOffFrontLayer;



// ----- Back Neopixel Layers -----------------------------------------------------

//NeopixelLayer ledOnOffBackLayer;



// ----- Debug Functions ----------------------------------------------------------

#ifdef DEBUG

void print_buttons_state() {
    Serial.print("-");
    if (brakeSensor.getState()) {
        Serial.print("---");
    } else {
        Serial.print("   ");
    }
    Serial.print("- ");

    Serial.print("-");
    if (turnSignalLeftSwitch.getState()) {
        Serial.print("---");
    } else {
        Serial.print("   ");
    }
    Serial.print("- ");

    Serial.print("-");
    if (turnSignalRightSwitch.getState()) {
        Serial.print("---");
    } else {
        Serial.print("   ");
    }
    Serial.print("- ");

    Serial.print("-");
    if (ledOnOffSwitch.getState()) {
        Serial.print("---");
    } else {
        Serial.print("   ");
    }
    Serial.println("-");

    
    Serial.println("BRAKE LEFT  RIGHT LIGHT");

    
    Serial.println();
}

#endif



// ----- Setup --------------------------------------------------------------------

void setup() {

#ifdef DEBUG

    Serial.begin(115200);
    
    Serial.println("Volkspod Led Controller: Hello ! ;)");
    Serial.println();

#endif

    frontNeopixelManager.begin();
    backNeopixelManager.begin();

#ifdef DEBUG

    Serial.println("Volkspod Led Controller: Neopixel Managers Initialized !");
    Serial.println();
    
    print_buttons_state();

#endif

}



// ----- Update Loop --------------------------------------------------------------

void loop() {
    // Update Time
    unsigned long currentTimeMs = millis();
  
    // Update Buttons
    brakeSensor.update(currentTimeMs);
    if (brakeSensor.getStateUpdated()) {
        brakeSensor.resetStateUpdated();
        onBrakeSensorChange();
    }
    
    turnSignalLeftSwitch.update(currentTimeMs);
    if (turnSignalLeftSwitch.getStateUpdated()) {
        turnSignalLeftSwitch.resetStateUpdated();
        onTurnSignalLeftSwitchChange();
    }

    turnSignalRightSwitch.update(currentTimeMs);
    if (turnSignalRightSwitch.getStateUpdated()) {
        turnSignalRightSwitch.resetStateUpdated();
        onTurnSignalRightSwitchChange();
    }
    
    ledOnOffSwitch.update(currentTimeMs);
    if (ledOnOffSwitch.getStateUpdated()) {
        ledOnOffSwitch.resetStateUpdated();
        onLedOnOffSwitchChange();
    }

    // Rendering
    render(currentTimeMs);
}



// ----- Rendering ----------------------------------------------------------------

unsigned long lastRenderingUpdateTime = millis();
void render(unsigned long currentTimeMs) {
    // Rendering Front Neopixel (In reverse order, From top to bottom)
//    frontNeopixelManager.select();
    
//    ledOnOffFrontLayer.draw();

    // Rendering Back Neopixel (In reverse order, From top to bottom)
//    backNeopixelManager.select();
}



// ----- Events -------------------------------------------------------------------

void onBrakeSensorChange() {

#ifdef DEBUG

    print_buttons_state();

#endif

    if (brakeSensor.getState()) {
        // Brake is pressed
    } else {
        // Brake is released
    }
}

void onTurnSignalLeftSwitchChange() {

#ifdef DEBUG

    print_buttons_state();

#endif

    if (turnSignalLeftSwitch.getState()) {
        // Turn signal left is on
    } else {
        // Turn signal left is off
    }
}

void onTurnSignalRightSwitchChange() {

#ifdef DEBUG

    print_buttons_state();

#endif

    if (turnSignalRightSwitch.getState()) {
        // Turn signal right is on
    } else {
        // Turn signal right is off
    }
}

void onLedOnOffSwitchChange() {

#ifdef DEBUG

    print_buttons_state();

#endif

    if (ledOnOffSwitch.getState()) {
        // LED is on
    } else {
        // LED is off
    }
}



// --------------------------------------------------------------------------------
