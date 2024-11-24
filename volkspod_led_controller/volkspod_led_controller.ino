/* Copyright (C) 2024 Aubrey ROUET - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the GPL v2 license.
 */

// ------------------------------------------------------------- Libraries -------------------------------------------------------------

#include "Adafruit_NeoPixel.h"

// -------------------------------------------------------------- Classes --------------------------------------------------------------

class SimplifiedSwitch {
  public:
    SimplifiedSwitch(uint8_t pin, unsigned long deadTimeMs = 50)
      : _pin(pin),
        _deadTimeMs(deadTimeMs)
    {
      pinMode(
        _pin,
        pullupResistance
          ? INPUT_PULLUP
          : INPUT
      );
    }

    void update() {
      unsigned long currentTimeMs = millis();
      unsigned long timeSince
    }
  private:
    const uint8_t _pin;
    const unsigned long _safeTimeMs;

    unsigned long _latestUpdateTimeMs;
    bool _state;

    void updateState() {
      _state = digitalRead(_pin);
    }
};

// -------------------------------------------------------- Switches and Sensors -------------------------------------------------------

#define TURN_SIGNAL_LEFT_SWITCH_PIN 2
SimplifiedSwitch turnSignalLeftSwitch(TURN_SIGNAL_LEFT_SWITCH_PIN);

#define TURN_SIGNAL_RIGHT_SWITCH_PIN 3
SimplifiedSwitch turnSignalRightSwitch(TURN_SIGNAL_RIGHT_SWITCH_PIN);

#define LIGHT_ON_OFF_SWITCH_PIN 4
SimplifiedSwitch ledOnOffSwitch(LIGHT_ON_OFF_SWITCH_PIN);

#define BRAKE_SENSOR_PIN 5

// ------------------------------------------------------------- Neopixels -------------------------------------------------------------

#define FRONT_NEOPIXEL_PIN 6
#define FRONT_NEOPIXEL_PIXEL_COUNT 60
Adafruit_NeoPixel frontNeopixel(FRONT_NEOPIXEL_PIXEL_COUNT, FRONT_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

#define BACK_NEOPIXEL_PIN 7
#define BACK_NEOPIXEL_PIXEL_COUNT 37 // 16 + 12 + 8 + 1
Adafruit_NeoPixel backNeopixel(BACK_NEOPIXEL_PIXEL_COUNT, BACK_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// Index pour chaque section de l'anneau arrière
const int indexAnneauExterieurStart = 0;
const int indexAnneauExterieurEnd = 15;
const int indexAnneauMilieuStart = 16;
const int indexAnneauMilieuEnd = 27;
const int indexAnneauInterieurStart = 28;
const int indexAnneauInterieurEnd = 35;
const int indexLedCentrale = 36;

// --------------------------------------------------------------- Setup ---------------------------------------------------------------

void setup() {
  pinMode(boutonClignoGauche, INPUT);
  pinMode(boutonClignoDroit, INPUT);
  pinMode(boutonPleinPhare, INPUT);
  pinMode(boutonFrein, INPUT);

  anneauAvant.begin();
  anneauArriere.begin();
  eteindreAnneaux();
}

// ------------------------------------------------------------- Main Loop -------------------------------------------------------------

void loop() {
  bool freinActif = digitalRead(boutonFrein) == HIGH;
  bool pleinPhareActif = digitalRead(boutonPleinPhare) == HIGH;
  bool clignoGaucheActif = digitalRead(boutonClignoGauche) == HIGH;
  bool clignoDroitActif = digitalRead(boutonClignoDroit) == HIGH;

  static bool pleinPhareInit = false;

  if (pleinPhareActif && !pleinPhareInit) {
    // Animation de démarrage lors de l'activation des pleins phares
    allumerPleinPhareProgressivement();
    pleinPhareInit = true; // Indique que l'animation de démarrage est terminée
  }

  if (freinActif && pleinPhareActif) {
    allumerPleinPhareAvecFrein();
  } else if (freinActif) {
    allumerFrein();
  } else if (clignoGaucheActif) {
    clignoterGauche();
  } else if (clignoDroitActif) {
    clignoterDroit();
  } else if (pleinPhareActif) {
    effetActivationPleinPhare();
  } else {
    eteindreAnneaux();
    pleinPhareInit = false; // Réinitialise l'animation de démarrage
  }
}

void allumerPleinPhareProgressivement() {
  // Allumer les LEDs avant séquentiellement
  for (int i = 0; i < numLEDsAvant; i++) {
    anneauAvant.setPixelColor(i, anneauAvant.Color(255, 255, 255));
    anneauAvant.show();
    delay(10); // Contrôle la vitesse d'allumage
  }

  // Allumer les LEDs arrière (milieu + intérieur + centrale) séquentiellement
  for (int i = indexAnneauMilieuStart; i <= indexAnneauInterieurEnd; i++) {
    anneauArriere.setPixelColor(i, anneauArriere.Color(255, 255, 255));
    anneauArriere.show();
    delay(10);
  }

  // Allumer la LED centrale à la fin
  anneauArriere.setPixelColor(indexLedCentrale, anneauArriere.Color(255, 255, 255));
  anneauArriere.show();
  delay(10);
}


void allumerFrein() {
  for (int i = indexAnneauMilieuStart; i <= indexAnneauInterieurEnd; i++) {
    anneauArriere.setPixelColor(i, anneauArriere.Color(255, 0, 0));
  }
  anneauArriere.setPixelColor(indexLedCentrale, anneauArriere.Color(255, 0, 0));
  anneauArriere.show();
}

void effetActivationPleinPhare() {
  static int brightness = 35;        // Luminosité initiale (jamais 0)
  static int step = 5;               // Pas d'incrémentation/décrémentation
  static bool increasing = true;     // Direction de l'animation

  // Animation progressive pour les LEDs avant
  for (int i = 0; i < numLEDsAvant; i++) {
    anneauAvant.setPixelColor(i, anneauAvant.Color(brightness, brightness, brightness));
  }
  anneauAvant.show();

  // Animation progressive pour les LEDs arrière
  for (int i = indexAnneauMilieuStart; i <= indexAnneauInterieurEnd; i++) {
    anneauArriere.setPixelColor(i, anneauArriere.Color(brightness, brightness, brightness));
  }
  anneauArriere.setPixelColor(indexLedCentrale, anneauArriere.Color(brightness, brightness, brightness));
  anneauArriere.show();

  // Mise à jour de la luminosité pour la prochaine boucle
  if (increasing) {
    brightness += step;
    if (brightness >= 255) {
      increasing = false; // Commence à décroître
    }
  } else {
    brightness -= step;
    if (brightness <= 35) { // Ne descend pas en dessous de 50
      increasing = true; // Recommence à croître
    }
  }

  delay(30); // Contrôle la vitesse de l'animation
}

void allumerPleinPhareAvecFrein() {
  // LEDs avant restent en blanc
  for (int i = 0; i < numLEDsAvant; i++) {
    anneauAvant.setPixelColor(i, anneauAvant.Color(255, 255, 255));
  }
  anneauAvant.show();

  // LEDs arrière : superposition du rouge (frein)
  for (int i = indexAnneauMilieuStart; i <= indexAnneauInterieurEnd; i++) {
    anneauArriere.setPixelColor(i, anneauArriere.Color(255, 0, 0));
  }
  anneauArriere.setPixelColor(indexLedCentrale, anneauArriere.Color(255, 0, 0));
  anneauArriere.show();
}

void clignoterGauche() {
  while (digitalRead(boutonClignoGauche) == HIGH) {
    // LEDs avant gauche
    for (int i = 0; i < 15; i++) {
      anneauAvant.setPixelColor(i, anneauAvant.Color(255, 165, 0));
      anneauAvant.setPixelColor(29 - i, anneauAvant.Color(255, 165, 0));
      anneauAvant.show();
      delay(50);
    }

    // LEDs arrière gauche
    for (int i = 0; i < 4; i++) {
      anneauArriere.setPixelColor(i, anneauArriere.Color(255, 165, 0));
      anneauArriere.setPixelColor(7 - i, anneauArriere.Color(255, 165, 0));
      anneauArriere.show();
      delay(50);
    }

    delay(300); // Pause entre les cycles

    // Si les pleins phares sont activés, on met les clignotants en blanc, sinon on les éteint
    if (digitalRead(boutonPleinPhare) == HIGH) {
      // Mettre les clignotants en blanc
      for (int i = 0; i < 15; i++) {
        anneauAvant.setPixelColor(i, anneauAvant.Color(255, 255, 255));
        anneauAvant.setPixelColor(29 - i, anneauAvant.Color(255, 255, 255));
      }
      for (int i = 0; i < 4; i++) {
        anneauArriere.setPixelColor(i, anneauArriere.Color(0, 0, 0));
        anneauArriere.setPixelColor(7 - i, anneauArriere.Color(0, 0, 0));
      }
      anneauAvant.show();
      anneauArriere.show();
    } else {
      eteindreClignotants();
    }
  }
}

void clignoterDroit() {
  while (digitalRead(boutonClignoDroit) == HIGH) {
    // LEDs avant droite
    for (int i = 0; i < 15; i++) {
      anneauAvant.setPixelColor(30 + i, anneauAvant.Color(255, 165, 0));
      anneauAvant.setPixelColor(59 - i, anneauAvant.Color(255, 165, 0));
      anneauAvant.show();
      delay(50);
    }

    // LEDs arrière droite
    for (int i = 0; i < 4; i++) {
      anneauArriere.setPixelColor(8 + i, anneauArriere.Color(255, 165, 0));
      anneauArriere.setPixelColor(15 - i, anneauArriere.Color(255, 165, 0));
      anneauArriere.show();
      delay(50);
    }

    delay(300); // Pause entre les cycles

    // Si les pleins phares sont activés, on met les clignotants en blanc, sinon on les éteint
    if (digitalRead(boutonPleinPhare) == HIGH) {
      // Mettre les clignotants en blanc
      for (int i = 0; i < 15; i++) {
        anneauAvant.setPixelColor(30 + i, anneauAvant.Color(255, 255, 255));
        anneauAvant.setPixelColor(59 - i, anneauAvant.Color(255, 255, 255));
      }
      for (int i = 0; i < 4; i++) {
        anneauArriere.setPixelColor(8 + i, anneauArriere.Color(0, 0, 0));
        anneauArriere.setPixelColor(15 - i, anneauArriere.Color(0, 0, 0));
      }
      anneauAvant.show();
      anneauArriere.show();
    } else {
      eteindreClignotants();
    }
  }
}

void eteindreClignotants() {
  for (int i = 0; i < numLEDsAvant; i++) {
    anneauAvant.setPixelColor(i, 0);
  }
  anneauAvant.show();

  for (int i = indexAnneauExterieurStart; i <= indexAnneauExterieurEnd; i++) {
    anneauArriere.setPixelColor(i, 0);
  }
  anneauArriere.show();
}

void eteindreAnneaux() {
  for (int i = 0; i < numLEDsAvant; i++) {
    anneauAvant.setPixelColor(i, 0);
  }
  for (int i = 0; i < numLEDsArriere; i++) {
    anneauArriere.setPixelColor(i, 0);
  }
  anneauAvant.show();
  anneauArriere.show();
}
