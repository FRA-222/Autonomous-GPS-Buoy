#include "arduino_secrets.h"
#include "ModeManagement.h"
#include "AutoPilot.h"
#include "Arduino.h"

// Variables pour la gestion des deux Moteurs
int directionPinM1 = 12;
int pwmPinM1 = 3;
int brakePinM1 = 9;

int directionPinM2 = 13;
int pwmPinM2 = 11;
int brakePinM2 = 8;

//--- Constructeurs

AutoPilot::AutoPilot ()
{
    signalPwmMoteur1_ = 0.;
    signalPwmMoteur2_ = 0.;
    commandeMoteur1_ = 0.;
    commandeMoteur2_ = 0.;
    throttle_ = 0.;
    rudder_ = 0.;
    trueHeading_ = 0.;
    pousseeForcee_ = 0.;     
    pousseeForceeOk_ = false;          
    orientationForcee_ = 0.;
    orientationForceeOk_ = false;
}
AutoPilot::AutoPilot (float signalPwmMoteur1, float signalPwmMoteur2,
                    float commandeMoteur1, float commandeMoteur2,
                    float throttle, float rudder, float trueHeading)
{
    signalPwmMoteur1_ = signalPwmMoteur1;
    signalPwmMoteur2_ = signalPwmMoteur2;
    commandeMoteur1_ = commandeMoteur1;
    commandeMoteur2_ = commandeMoteur2;
    throttle_ = throttle;
    rudder_ = rudder;
    trueHeading_ = trueHeading;
}

//--- Getters et setters

float AutoPilot::getMyThrottle()
{
    return throttle_;
}
void AutoPilot::setMyThrottle(float throttle)
{
     throttle_ = throttle;
}
float AutoPilot::getMyRudder()
{
    return rudder_;
}
void AutoPilot::setMyRudder(float rudder)
{
     rudder_ = rudder;
}
float AutoPilot::getMyTrueHeading()
{
    return trueHeading_;
}
void AutoPilot::setMyTrueHeading(float trueHeading)
{
     trueHeading_ = trueHeading;
}

//--- Methodes fonctionnelles


void AutoPilot::setup()
{
  // Definit les PIN pour les moteurs A et B
  pinMode(directionPinM1, OUTPUT);
  pinMode(pwmPinM1, OUTPUT);
  pinMode(brakePinM1, OUTPUT);
  pinMode(directionPinM2, OUTPUT);
  pinMode(pwmPinM2, OUTPUT);
  pinMode(brakePinM2, OUTPUT);
}

void AutoPilot::calculerCommandesMoteur(tEtatsNav navigationMode, 
float throttleCmde, float rudderCmde, float trueHeadingCmde,
float yawRate, bool imuOk)

{
  switch (navigationMode)
  {
  case NAV_NOTHING:
    commandeMoteur1_ = 0.;
    commandeMoteur2_ = 0.;
    analogWrite(pwmPinM1, commandeMoteur1_);
    analogWrite(pwmPinM2, commandeMoteur2_);
    throttle_ = 0.;
    rudder_ = 0.;
    trueHeading_ = 0.;
    break;

  case NAV_HOME:
    commandeMoteur1_ = 0.;
    commandeMoteur2_ = 0.;
    analogWrite(pwmPinM1, commandeMoteur1_);
    analogWrite(pwmPinM2, commandeMoteur2_);
    throttle_ = 0.;
    rudder_ = 0.;
    trueHeading_ = 0.;
    break;

  case NAV_HOLD:
    commandeMoteur1_ = 0.;
    commandeMoteur2_ = 0.;
    analogWrite(pwmPinM1, commandeMoteur1_);
    analogWrite(pwmPinM2, commandeMoteur2_);
    throttle_ = 0.;
    rudder_ = 0.;
    trueHeading_ = 0.;
    break;

  case NAV_STOP:
    
    // activate breaks
    digitalWrite(brakePinM1, HIGH);
    digitalWrite(brakePinM2, HIGH);

    // set work duty for the motor to 0 (off)
    commandeMoteur1_ = 0.;
    commandeMoteur2_ = 0.;
    analogWrite(pwmPinM1, commandeMoteur1_);
    analogWrite(pwmPinM2, commandeMoteur2_);

    throttle_ = 0.;
    rudder_ = 0.;
    trueHeading_ = 0.;

    break;

  case NAV_BASIC:
   
    // Démarre en marche avant
    digitalWrite(directionPinM1, HIGH);
    digitalWrite(directionPinM2, HIGH);

    // release breaks
    digitalWrite(brakePinM1, LOW);
    digitalWrite(brakePinM2, LOW);

    // set work duty for the motor
    if (rudderCmde > 40)
    {
      commandeMoteur1_ = throttleCmde / rudderCmde;
      commandeMoteur2_ = throttleCmde;
    }

    if (rudderCmde < -40)
    {
      commandeMoteur1_ = throttleCmde;
      commandeMoteur2_ = throttleCmde / abs(rudderCmde);
    }

    if (rudderCmde > -40 && rudderCmde < 40)
    {
      commandeMoteur1_ = throttleCmde;
      commandeMoteur2_ = throttleCmde;
    }

    analogWrite(pwmPinM1, commandeMoteur1_);
    analogWrite(pwmPinM2, commandeMoteur2_);

    throttle_ = throttleCmde;
    rudder_ = rudderCmde;
    trueHeading_ = 0.;

    break;

  case NAV_CAP:
    commandeMoteur1_ = 0.;
    commandeMoteur2_ = 0.;
    analogWrite(pwmPinM1, commandeMoteur1_);
    analogWrite(pwmPinM2, commandeMoteur2_);
    throttle_ = 0.;
    rudder_ = 0.;
    trueHeading_ = trueHeadingCmde;
    break;

  case NAV_TARGET:
    commandeMoteur1_ = 0.;
    commandeMoteur2_ = 0.;
    analogWrite(pwmPinM1, commandeMoteur1_);
    analogWrite(pwmPinM2, commandeMoteur2_);
    throttle_ = 0.;
    rudder_ = 0.;
    trueHeading_ = 0.;
    break;

  default:
    commandeMoteur1_ = 0.;
    commandeMoteur2_ = 0.;
    analogWrite(pwmPinM1, commandeMoteur1_);
    analogWrite(pwmPinM2, commandeMoteur2_);
    throttle_ = 0.;
    rudder_ = 0.;
    trueHeading_ = 0.;
    break;
  }
}