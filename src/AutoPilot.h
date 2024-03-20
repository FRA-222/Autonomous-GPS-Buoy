#ifndef AUTOPILOT_H
#define AUTOPILOT_H
#include "Arduino.h"

enum tEtatsControl
{
    CONTROL_NOTHING,
    HOLD_POSITION,
    GO_TO_POSITION
}; // Modes de pilotage

#pragma once

class AutoPilot 
{
    public:
    // Constructeurs
    AutoPilot();
    AutoPilot (float, float, float, float, float, float, float);
    // Getters et Setters
    float getMyThrottle();
    void setMyThrottle(float);
    float getMyRudder();
    void setMyRudder(float);
    float getMyTrueHeading();
    void setMyTrueHeading(float);

    // Methodes fonctionnelles
    void setup();
    void calculerCommandesMoteur(tEtatsNav, float, float, float, float, bool);

    private:
    // Attributs primaires (sorties)
    float signalPwmMoteur1_;
    float signalPwmMoteur2_;
    // Attributs internes
    float commandeMoteur1_;     // Commande moteur1 (entre -100 et 100)
    float commandeMoteur2_;     // Commande moteur2 (entre -100 et 100)
    float throttle_;            // Poussee commandee
    float rudder_;              // Direction commandee
    float trueHeading_;         // Cap commande 
    float pousseeForcee_;       // manette des gaz forcee (resynchro de la commande)
    bool pousseeForceeOk_;      // Validite du forcage de la manette des gaz
    float orientationForcee_;   // cmde orientation forcee (resynchro de la commande)
    bool orientationForceeOk_;  // Validite du forcage de la commande d orientationForcee
};

#endif