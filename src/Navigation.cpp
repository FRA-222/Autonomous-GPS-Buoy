
#include "Arduino.h"
#include "ModeManagement.h"
#include "Navigation.h"

//--- Constructeurs

Navigation::Navigation()
{
    xLatHold_ = 0.;     
    yLonHold_ = 0.;       
    posHoldOk_ = false;  
    locationCons_ = NO_LOCATION_CONS;          
    xLatCons_ = 0.;     
    yLonCons_ = 0.;       
    posConsOk_ = false;              
    capCons_ = 0.;          
    capConsOk_ = false;    

}

//--- Getters et setters

tEtatsLocationCons Navigation::getMyLocationCons()
{
    return locationCons_;
}
double Navigation::getMyXLatCons()
{
    return xLatCons_;
}
void Navigation::setMyXLatCons(double xLatCons)
{
     xLatCons_ = xLatCons;
}
double Navigation::getMyYLonCons()
{
    return yLonCons_;
}
void Navigation::setMyYLonCons(double yLonCons)
{
     yLonCons = yLonCons;
}
bool Navigation::getMyPosConsOk()
{
    return posConsOk_;
}
void Navigation::setMyPosConsOk(bool posConsOk)
{
     posConsOk_ = posConsOk;
}

//--- Methodes fonctionnelles

void Navigation::calculateNavTarget(
    tEtatsNav navigationMode,    
    bool frontMontantNavHome,              
    bool frontMontantNavHold,           
    bool frontMontantNavStop,          
    bool frontMontantNavBasic,      
    bool frontMontantNavCap,           
    bool frontMontantNavTarget,           
    double xLatTarget, double yLonTarget,
    double xLatHome, double yLonHome, bool posHomeOk, 
    double xLatGps, double yLonGps, bool gpsOk,
    float magneticHeading, bool magneticHeadingOk)

{

    float pastCapCons;     // Cap de consigne du cycle precedent
    bool pastCapConsOk;    // Validite du cap de consigne du cycle precedent
    pastCapCons = capCons_;
    pastCapConsOk = capConsOk_;

    //---Calcul des consignes de navigation
    switch (navigationMode)
    {
    case NAV_NOTHING:
        // Mise a zero des consignes
        locationCons_ = NO_LOCATION_CONS;
        xLatCons_ = 0.;
        yLonCons_ = 0.;
        posConsOk_ = false;
        capCons_ = 0.;
        capConsOk_ = false;
        break;
    case NAV_HOME:
        if (posHomeOk)
        {
            // La consigne de nav est la position du HOME
            locationCons_ = HOME_CONS;
            xLatCons_ = xLatHome;
            yLonCons_ = yLonHome;
            posConsOk_ = posHomeOk;
        }
        else
        {
            // Mise a zero des consignes
            locationCons_ = NO_LOCATION_CONS;
            xLatCons_ = 0.;
            yLonCons_ = 0.;
            posConsOk_ = false;
        }
        capCons_ = 0.;
        capConsOk_ = false;
        break;
    case NAV_HOLD:
        if (frontMontantNavHold)
        {
            // Le mode de navigation vien de passer a NAV_HOLD
            if (gpsOk)
            {
                // Memorisation de la position de la bouee comme position de consigne
                xLatHold_ = xLatGps;
                yLonHold_ = yLonGps;
                posHoldOk_ = true;
            }
            else
            {
                // Mise a zero de la consigne de HOLD
                // (branche a priori non passante car ModeManagement tetste la validite du GPS)
                xLatHold_ = 0.;
                yLonHold_ = 0.;
                posHoldOk_ = false;
            }
        }
        // La consigne de nav est la position du HOLD
        locationCons_ = HOLD_CONS;
        xLatCons_ = xLatHold_;
        yLonCons_ = yLonHold_;
        posConsOk_ = posHoldOk_;
        capCons_ = 0.;
        capConsOk_ = false;
        break;
    case NAV_STOP:
        if (frontMontantNavStop)
        {
            // Le mode de navigation vient de passer a NAV_STOP
        }
        locationCons_ = NO_LOCATION_CONS;
        xLatCons_ = 0.;
        yLonCons_ = 0.;
        posConsOk_ = false;
        capCons_ = 0.;
        capConsOk_ = false;
        break;
    case NAV_BASIC:
        if (frontMontantNavBasic)
        {
            // Le mode de navigation vient de passer a NAV_BASIC
        }
        locationCons_ = NO_LOCATION_CONS;
        xLatCons_ = 0.;
        yLonCons_ = 0.;
        posConsOk_ = false;
        capCons_ = 0.;
        capConsOk_ = false;
        break;
    case NAV_CAP:
        locationCons_ = NO_LOCATION_CONS;
        xLatCons_ = 0.;
        yLonCons_ = 0.;
        posConsOk_ = false;
        if (frontMontantNavCap)
        {
            // Le mode de navigation vient de passer a NAV_CAP
            if (magneticHeadingOk)
            {
                // Memorisation du cap de la bouee comme cap de consigne
                capCons_ = magneticHeading;
                capConsOk_ = true;
            }
            else
            {
                // Mise a zero de la consigne de cap
                // (branche a priori non passante car ModeManagement tetste la validite du GPS)
                capCons_ = 0.;
                capConsOk_ = false;
            }
        }
        else
        {
            capCons_ = pastCapCons;
            capConsOk_ = pastCapConsOk;
        }
        break;
    case NAV_TARGET:
        locationCons_ = TARGET_CONS;
        xLatCons_ = xLatTarget;
        yLonCons_ = yLonTarget;
        posConsOk_ = true;
        capCons_ = 0.;
        capConsOk_ = false;
        break;
    default:
        locationCons_ = NO_LOCATION_CONS;
        xLatCons_ = 0.;
        yLonCons_ = 0.;
        posConsOk_ = false;
        capCons_ = 0.;
        capConsOk_ = false;
    }
}
