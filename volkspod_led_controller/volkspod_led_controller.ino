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
#include "turn_signal_layer.h"
#include "back_brake_layer.h"

// ----- Settings -----------------------------------------------------------------

// Debug Settings
#define DEBUG

// Color Settings
const Color frontLedOnOffColor = Color{100, 100, 100};
const Color backLedOnOffColor = Color{100, 100, 100};
const Color turnSignalColor = Color{255, 45, 0};
const Color brakeColor = Color{255, 0, 0};

// Animation Settings
#define LED_ON_OFF_ANIMATION_DURATION 600

#define TURN_SIGNAL_ANIMATION_FRONT_LAYER_CARRET_LENGTH 5
#define TURN_SIGNAL_ANIMATION_BACK_LAYER_RING_1_CARRET_LENGTH 2
#define TURN_SIGNAL_ANIMATION_BACK_LAYER_RING_2_CARRET_LENGTH 1
#define TURN_SIGNAL_ANIMATION_DURATION 700

#define BRAKE_ANIMATION_SETUP_DURATION 600

// Experimentation Sensors And Switches Settings
// #define BRAKE_SENSOR_PIN 12
// #define TURN_SIGNAL_LEFT_SWITCH_PIN 11
// #define TURN_SIGNAL_RIGHT_SWITCH_PIN 10
// #define LIGHT_ON_OFF_SWITCH_PIN 9

// Sensors And Switches Settings
#define BRAKE_SENSOR_PIN 5
#define TURN_SIGNAL_LEFT_SWITCH_PIN 2
#define TURN_SIGNAL_RIGHT_SWITCH_PIN 3
#define LIGHT_ON_OFF_SWITCH_PIN 4

// Experimentation Neopixel Settings
// #define FRONT_NEOPIXEL_PIN 6
// #define FRONT_NEOPIXEL_PIXEL_COUNT 60
// #define BACK_NEOPIXEL_PIN 5
// #define BACK_NEOPIXEL_PIXEL_COUNT 37 // 16 + 12 + 8 + 1
#define BACK_NEOPIXEL_RING_2_START_INDEX 16
#define BACK_NEOPIXEL_RING_3_START_INDEX 28
#define BACK_NEOPIXEL_RING_4_START_INDEX 36

// Neopixel Settings
#define FRONT_NEOPIXEL_PIN 6
#define FRONT_NEOPIXEL_PIXEL_COUNT 60
#define BACK_NEOPIXEL_PIN 7
#define BACK_NEOPIXEL_PIXEL_COUNT 37 // 16 + 12 + 8 + 1

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
    frontLedOnOffColor
);

TurnSignalLayer turnLeftSignalFrontLayer1(
    0,
    14,
    TURN_SIGNAL_ANIMATION_FRONT_LAYER_CARRET_LENGTH,
    TURN_SIGNAL_ANIMATION_DURATION,
    turnSignalColor
);
TurnSignalLayer turnLeftSignalFrontLayer2(
    29,
    15,
    TURN_SIGNAL_ANIMATION_FRONT_LAYER_CARRET_LENGTH,
    TURN_SIGNAL_ANIMATION_DURATION,
    turnSignalColor
);

TurnSignalLayer turnRightSignalFrontLayer1(
    59,
    45,
    TURN_SIGNAL_ANIMATION_FRONT_LAYER_CARRET_LENGTH,
    TURN_SIGNAL_ANIMATION_DURATION,
    turnSignalColor
);
TurnSignalLayer turnRightSignalFrontLayer2(
    30,
    44,
    TURN_SIGNAL_ANIMATION_FRONT_LAYER_CARRET_LENGTH,
    TURN_SIGNAL_ANIMATION_DURATION,
    turnSignalColor
);

#define FRONT_NEOPIXEL_LAYERS_COUNT 5
NeopixelLayer* frontNeopixelLayers[FRONT_NEOPIXEL_LAYERS_COUNT] = {
    &ledOnOffFrontLayer,
    &turnLeftSignalFrontLayer1,
    &turnLeftSignalFrontLayer2,
    &turnRightSignalFrontLayer1,
    &turnRightSignalFrontLayer2
};

// ----- Back Neopixel Layers -----------------------------------------------------

LedOnOffLayer ledOnOffBackLayer1(
    0,
    BACK_NEOPIXEL_RING_2_START_INDEX - 1,
    LED_ON_OFF_ANIMATION_DURATION,
    backLedOnOffColor
);
LedOnOffLayer ledOnOffBackLayer2(
    BACK_NEOPIXEL_RING_2_START_INDEX,
    BACK_NEOPIXEL_RING_3_START_INDEX - 1,
    LED_ON_OFF_ANIMATION_DURATION,
    backLedOnOffColor
);
// LedOnOffLayer ledOnOffBackLayer3(
//     BACK_NEOPIXEL_RING_3_START_INDEX,
//     BACK_NEOPIXEL_RING_4_START_INDEX - 1,
//     LED_ON_OFF_ANIMATION_DURATION,
//     backLedOnOffColor
// );
// LedOnOffLayer ledOnOffBackLayer4(
//     BACK_NEOPIXEL_RING_4_START_INDEX,
//     BACK_NEOPIXEL_PIXEL_COUNT - 1,
//     LED_ON_OFF_ANIMATION_DURATION,
//     backLedOnOffColor
// );

BackBrakeLayer brakeBackLayer(
    BACK_NEOPIXEL_RING_2_START_INDEX,
    BRAKE_ANIMATION_SETUP_DURATION,
    brakeColor
);

TurnSignalLayer turnRightSignalBackLayer1(
    1,
    4,
    TURN_SIGNAL_ANIMATION_BACK_LAYER_RING_1_CARRET_LENGTH,
    TURN_SIGNAL_ANIMATION_DURATION,
    turnSignalColor
);
TurnSignalLayer turnRightSignalBackLayer2(
    7,
    4,
    TURN_SIGNAL_ANIMATION_BACK_LAYER_RING_1_CARRET_LENGTH,
    TURN_SIGNAL_ANIMATION_DURATION,
    turnSignalColor
);
TurnSignalLayer turnRightSignalBackLayer3(
    17,
    19,
    TURN_SIGNAL_ANIMATION_BACK_LAYER_RING_2_CARRET_LENGTH,
    TURN_SIGNAL_ANIMATION_DURATION,
    turnSignalColor
);
TurnSignalLayer turnRightSignalBackLayer4(
    21,
    19,
    TURN_SIGNAL_ANIMATION_BACK_LAYER_RING_2_CARRET_LENGTH,
    TURN_SIGNAL_ANIMATION_DURATION,
    turnSignalColor
);

TurnSignalLayer turnLeftSignalBackLayer1(
    15,
    12,
    TURN_SIGNAL_ANIMATION_BACK_LAYER_RING_1_CARRET_LENGTH,
    TURN_SIGNAL_ANIMATION_DURATION,
    turnSignalColor
);
TurnSignalLayer turnLeftSignalBackLayer2(
    9,
    12,
    TURN_SIGNAL_ANIMATION_BACK_LAYER_RING_1_CARRET_LENGTH,
    TURN_SIGNAL_ANIMATION_DURATION,
    turnSignalColor
);
TurnSignalLayer turnLeftSignalBackLayer3(
    27,
    25,
    TURN_SIGNAL_ANIMATION_BACK_LAYER_RING_2_CARRET_LENGTH,
    TURN_SIGNAL_ANIMATION_DURATION,
    turnSignalColor
);
TurnSignalLayer turnLeftSignalBackLayer4(
    23,
    25,
    TURN_SIGNAL_ANIMATION_BACK_LAYER_RING_2_CARRET_LENGTH,
    TURN_SIGNAL_ANIMATION_DURATION,
    turnSignalColor
);

#define BACK_NEOPIXEL_LAYERS_COUNT 11
NeopixelLayer* backNeopixelLayers[BACK_NEOPIXEL_LAYERS_COUNT] = {
    &ledOnOffBackLayer1,
    &ledOnOffBackLayer2,
    // &ledOnOffBackLayer3,
    // &ledOnOffBackLayer4,
    &brakeBackLayer,
    &turnLeftSignalBackLayer1,
    &turnLeftSignalBackLayer2,
    &turnLeftSignalBackLayer3,
    &turnLeftSignalBackLayer4,
    &turnRightSignalBackLayer1,
    &turnRightSignalBackLayer2,
    &turnRightSignalBackLayer3,
    &turnRightSignalBackLayer4
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

    // Update Front Layers
    ledOnOffFrontLayer.update(currentTimeMs);
    turnLeftSignalFrontLayer1.update(currentTimeMs);
    turnLeftSignalFrontLayer2.update(currentTimeMs);
    turnRightSignalFrontLayer1.update(currentTimeMs);
    turnRightSignalFrontLayer2.update(currentTimeMs);

    // Update Back Layers
    ledOnOffBackLayer1.update(currentTimeMs);
    ledOnOffBackLayer2.update(currentTimeMs);
    // ledOnOffBackLayer3.update(currentTimeMs);
    // ledOnOffBackLayer4.update(currentTimeMs);

    brakeBackLayer.update(currentTimeMs);

    turnLeftSignalBackLayer1.update(currentTimeMs);
    turnLeftSignalBackLayer2.update(currentTimeMs);
    turnLeftSignalBackLayer3.update(currentTimeMs);
    turnLeftSignalBackLayer4.update(currentTimeMs);

    turnRightSignalBackLayer1.update(currentTimeMs);
    turnRightSignalBackLayer2.update(currentTimeMs);
    turnRightSignalBackLayer3.update(currentTimeMs);
    turnRightSignalBackLayer4.update(currentTimeMs);
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
        turnLeftSignalFrontLayer1.enable();
        turnLeftSignalFrontLayer2.enable();

        turnLeftSignalBackLayer1.enable();
        turnLeftSignalBackLayer2.enable();
        turnLeftSignalBackLayer3.enable();
        turnLeftSignalBackLayer4.enable();
    } else {
        // Turn signal left is off
        turnLeftSignalFrontLayer1.disable();
        turnLeftSignalFrontLayer2.disable();

        turnLeftSignalBackLayer1.disable();
        turnLeftSignalBackLayer2.disable();
        turnLeftSignalBackLayer3.disable();
        turnLeftSignalBackLayer4.disable();
    }
}

void onTurnSignalRightSwitchChange() {

#ifdef DEBUG

    print_buttons_state();

#endif

    if (turnSignalRightSwitch.getState()) {
        // Turn signal right is on
        turnRightSignalFrontLayer1.enable();
        turnRightSignalFrontLayer2.enable();

        turnRightSignalBackLayer1.enable();
        turnRightSignalBackLayer2.enable();
        turnRightSignalBackLayer3.enable();
        turnRightSignalBackLayer4.enable();
    } else {
        // Turn signal right is off
        turnRightSignalFrontLayer1.disable();
        turnRightSignalFrontLayer2.disable();

        turnRightSignalBackLayer1.disable();
        turnRightSignalBackLayer2.disable();
        turnRightSignalBackLayer3.disable();
        turnRightSignalBackLayer4.disable();
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
        // ledOnOffBackLayer3.enable();
        // ledOnOffBackLayer4.enable();
    } else {
        // LED is off
        ledOnOffFrontLayer.disable();

        ledOnOffBackLayer1.disable();
        ledOnOffBackLayer2.disable();
        // ledOnOffBackLayer3.disable();
        // ledOnOffBackLayer4.disable();
    }
}

// --------------------------------------------------------------------------------
