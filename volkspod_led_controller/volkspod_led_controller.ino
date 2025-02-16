// +------------------------------------------------------------------------------+
// | filename: volkspod_led_controller.ino                                        |
// | filepath: volkspod_led_controller/volkspod_led_controller.ino                |
// | project name: volkspod_led_controller                                        |
// | authors: Aubrey ROUET <aubrey.rouet@gmail.com>                               |
// | license: Proprietary                                                         |
// |                                                                              |
// | Copyright (c) 2025 Aubrey ROUET                                              |
// +------------------------------------------------------------------------------+

// ----- External Libraries -------------------------------------------------------

#include <Adafruit_NeoPixel.h>

// ----- Internal Libraries -------------------------------------------------------

#include "simplified_switch.h"
#include "neopixel_manager.h"
#include "color.h"

#include "led_on_off_layer.h"
#include "back_brake_layer.h"

// ----- Settings -----------------------------------------------------------------

// Debug Settings
#define DEBUG

// Color Settings
const Color white = Color{255, 255, 255};
const Color brakeColor = Color{255, 0, 0};

// Animation Settings
#define LED_ON_OFF_ANIMATION_DURATION 600
#define BRAKE_ANIMATION_SETUP_DURATION 600
#define BRAKE_ANIMATION_BREATHING_DURATION 800
#define BRAKE_ANIMATION_BREATHING_DOWN_POWER_RATIO 0.3

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
#define BACK_NEOPIXEL_RING_2_START_INDEX 16
#define BACK_NEOPIXEL_RING_3_START_INDEX 28
#define BACK_NEOPIXEL_RING_4_START_INDEX 36

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

// ----- Front Neopixel Layers ----------------------------------------------------

LedOnOffLayer ledOnOffFrontLayer(
    0,
    FRONT_NEOPIXEL_PIXEL_COUNT - 1,
    LED_ON_OFF_ANIMATION_DURATION,
    white
);


#define FRONT_NEOPIXEL_LAYERS_COUNT 1
NeopixelLayer* frontNeopixelLayers[FRONT_NEOPIXEL_LAYERS_COUNT] = {
    &ledOnOffFrontLayer
};

// ----- Back Neopixel Layers -----------------------------------------------------

LedOnOffLayer ledOnOffBackLayer1(
    0,
    BACK_NEOPIXEL_RING_2_START_INDEX - 1,
    LED_ON_OFF_ANIMATION_DURATION,
    white
);
LedOnOffLayer ledOnOffBackLayer2(
    BACK_NEOPIXEL_RING_2_START_INDEX,
    BACK_NEOPIXEL_RING_3_START_INDEX - 1,
    LED_ON_OFF_ANIMATION_DURATION,
    white
);
LedOnOffLayer ledOnOffBackLayer3(
    BACK_NEOPIXEL_RING_3_START_INDEX,
    BACK_NEOPIXEL_RING_4_START_INDEX - 1,
    LED_ON_OFF_ANIMATION_DURATION,
    white
);
LedOnOffLayer ledOnOffBackLayer4(
    BACK_NEOPIXEL_RING_4_START_INDEX,
    BACK_NEOPIXEL_PIXEL_COUNT - 1,
    LED_ON_OFF_ANIMATION_DURATION,
    white
);

BackBrakeLayer brakeBackLayer(
    BACK_NEOPIXEL_RING_2_START_INDEX,
    BRAKE_ANIMATION_SETUP_DURATION,
    BRAKE_ANIMATION_BREATHING_DURATION,
    BRAKE_ANIMATION_BREATHING_DOWN_POWER_RATIO,
    brakeColor
);

#define BACK_NEOPIXEL_LAYERS_COUNT 5
NeopixelLayer* backNeopixelLayers[BACK_NEOPIXEL_LAYERS_COUNT] = {
    &ledOnOffBackLayer1,
    &ledOnOffBackLayer2,
    &ledOnOffBackLayer3,
    &ledOnOffBackLayer4,
    &brakeBackLayer
};

// ----- Neopixels ----------------------------------------------------------------

Adafruit_NeoPixel frontNeopixel(
    FRONT_NEOPIXEL_PIXEL_COUNT,
    FRONT_NEOPIXEL_PIN,
    NEO_GRB + NEO_KHZ800
);
NeopixelManager frontNeopixelManager(
    &frontNeopixel,
    &frontNeopixelLayers[0],
    FRONT_NEOPIXEL_LAYERS_COUNT
);

Adafruit_NeoPixel backNeopixel(
    BACK_NEOPIXEL_PIXEL_COUNT,
    BACK_NEOPIXEL_PIN,
    NEO_GRB + NEO_KHZ800
);
NeopixelManager backNeopixelManager(
    &backNeopixel,
    &backNeopixelLayers[0],
    BACK_NEOPIXEL_LAYERS_COUNT
);

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

    Serial.println("______________________________________");
    Serial.println("Volkspod Led Controller: Hello ! ;)");
    Serial.println();
    Serial.flush();

#endif

    frontNeopixelManager.begin();
    backNeopixelManager.begin();

#ifdef DEBUG

    Serial.println("Volkspod Led Controller: Neopixel Managers Initialized !");
    Serial.println();
    
    print_buttons_state();

    Serial.flush();

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

    ledOnOffFrontLayer.update(currentTimeMs);

    ledOnOffBackLayer1.update(currentTimeMs);
    ledOnOffBackLayer2.update(currentTimeMs);
    ledOnOffBackLayer3.update(currentTimeMs);
    ledOnOffBackLayer4.update(currentTimeMs);
    brakeBackLayer.update(currentTimeMs);
}

// ----- Events -------------------------------------------------------------------

void onBrakeSensorChange() {

#ifdef DEBUG

    print_buttons_state();

#endif

    if (brakeSensor.getState()) {
        // Brake is pressed
        brakeBackLayer.enable();
    } else {
        // Brake is released
        brakeBackLayer.disable();
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
        ledOnOffFrontLayer.enable();

        ledOnOffBackLayer1.enable();
        ledOnOffBackLayer2.enable();
        ledOnOffBackLayer3.enable();
        ledOnOffBackLayer4.enable();
    } else {
        // LED is off
        ledOnOffFrontLayer.disable();

        ledOnOffBackLayer1.disable();
        ledOnOffBackLayer2.disable();
        ledOnOffBackLayer3.disable();
        ledOnOffBackLayer4.disable();
    }
}

// --------------------------------------------------------------------------------
