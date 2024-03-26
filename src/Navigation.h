#ifndef NAVIGATION_H
#define NAVIGATION_H
#include "Arduino.h"

#pragma once

// Modes de navigation de la bouee
enum tEtatsLocationCons
{
    NO_LOCATION_CONS, 
    HOME_CONS,    
    HOLD_CONS,
    TARGET_CONS
}; 

class Navigation 
{
    public:
    // Constructeurs
    Navigation(); // Constructeur par defaut

    // Getters et Setters
    tEtatsLocationCons getMyLocationCons();
    double getMyXLatCons();
    void setMyXLatCons(double);
    double getMyYLonCons();
    void setMyYLonCons(double);
    bool getMyPosConsOk();
    void setMyPosConsOk(bool);

    // Methodes fonctionnelles
    void calculateNavTarget(
    tEtatsNav, bool, bool, bool, bool, bool, bool,           
    double, double, 
    double, double, bool, 
    double, double, bool,
    float, bool);

    private:
    double xLatHold_;           // Latitude du point HOLD
    double yLonHold_;           // Longitude du point HOLD
    bool posHoldOk_;            // Validite de la position du point HOLD
    // Sorties de la navigation
    tEtatsLocationCons locationCons_;   // Type de consigne en position 
    double xLatCons_;                   // Latitude de la consigne en position
    double yLonCons_;                   // Longitude de la consigne en position
    bool posConsOk_;                    // Validite de la consigne en position
    float capCons_;                     // Cap de consigne
    bool capConsOk_;                    // Validite de la consigne en cap

};

#endif