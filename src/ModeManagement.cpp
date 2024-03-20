#include "ModeManagement.h"
#include <Arduino.h>

//--- Constructeurs

ModeManagement::ModeManagement()
{    
    // Sorties de la gestion des modes
    generalMode_ = INIT;
    frontMontantReady_ = false;          
    frontMontantMaintenance_ = false;   
    frontMontantHomeDefinition_ = false; 
    frontMontantNav_ = false;           
    navigationMode_ = NAV_NOTHING;
    frontMontantNavHome_ = false;                     
    frontMontantNavHold_ = false;
    frontMontantNavStop_ = false;
    frontMontantNavBasic_ = false;
    frontMontantNavCap_ = false;
    frontMontantNavTarget_ = false;  
    dureeHome_ = 0.;
    dureeHold_ = 0.;
    dureeStop_ = 0.;
    dureeNavBasic_ = 0.;
    dureeNavCap_ = 0.;
    dureeNavTarget_ = 0.;
    posHomeOk_ = false;    
    xLatHome_ = 0.;
    yLonHome_ = 0.;
}

ModeManagement::~ModeManagement()
{
}

//--- Getters et setters

tEtatsNav ModeManagement::getMyNavigationMode()
{
    return navigationMode_;
}
tEtatsGeneral ModeManagement::getMyGeneralMode()
{
    return generalMode_;
}
bool ModeManagement::getMyFrontMontantNavHome()
{
    return frontMontantNavHome_;
}
void ModeManagement::setMyFrontMontantNavHome(bool frontMontantNavHome)
{
     frontMontantNavHome_ = frontMontantNavHome;
}
bool ModeManagement::getMyFrontMontantNavHold()
{
    return frontMontantNavHold_;
}
void ModeManagement::setMyFrontMontantNavHold(bool frontMontantNavHold)
{
     frontMontantNavHold_ = frontMontantNavHold;
}
bool ModeManagement::getMyFrontMontantNavStop()
{
    return frontMontantNavStop_;
}
void ModeManagement::setMyFrontMontantNavStop(bool frontMontantNavStop)
{
     frontMontantNavStop_ = frontMontantNavStop;
}
bool ModeManagement::getMyFrontMontantNavBasic()
{
    return frontMontantNavBasic_;
}
void ModeManagement::setMyFrontMontantNavBasic(bool frontMontantNavBasic)
{
     frontMontantNavBasic_ = frontMontantNavBasic;
}
bool ModeManagement::getMyFrontMontantNavCap()
{
    return frontMontantNavCap_;
}
void ModeManagement::setMyFrontMontantNavCap(bool frontMontantNavCap)
{
     frontMontantNavCap_ = frontMontantNavCap;
}
bool ModeManagement::getMyFrontMontantNavTarget()
{
    return frontMontantNavTarget_;
}
void ModeManagement::setMyFrontMontantNavTarget(bool frontMontantNavTarget)
{
     frontMontantNavTarget_ = frontMontantNavTarget;
}
double ModeManagement::getMyXLatHome()
{
    return xLatHome_;
}
void ModeManagement::setMyXLatHome(double xLatHome)
{
     xLatHome_ = xLatHome;
}
double ModeManagement::getMyYLonHome()
{
    return yLonHome_;
}
void ModeManagement::setMyYLonHome(double yLonHome)
{
     yLonHome = yLonHome;
}
bool ModeManagement::getMyPosHomeOk()
{
    return posHomeOk_;
}
void ModeManagement::setMyPosHomeOk(bool posHomeOk)
{
     posHomeOk_ = posHomeOk;
}

//--- Methodes fonctionnelles

// Elaboration du mode general de la bouee
//     bool frontMontantHomeMemoCmde        : Front montant de la commande memorisation du Home
//     bool frontMontantPosHomeCmde         : Front montant de la commande position du Home
//     double xLatHomeCmde, yLonHomeCmde    : Position du Home associee a la commande
//     bool frontMontantMaintenanceCmde     : Front montant de la commande Maintenance
//     bool frontMontantResumeReadyCmde     : Front montant de la commande Retour au mode operatioinnel
//     bool frontMontantHomeValidationCmde  : Front montant de la commande de validation du Home (pour le passage en Nav)
//     bool dataLinkOk              : validite de DataLink
//     double xLatGps               : Latitude de la bouee
//     double yLonGps               : Longitude de la bouee
//     bool gpsOk                   : Validite de la postion de la bouee
//     float magneticHeading        : Cap magnetique de la bouee
//     bool magneticHeadingOk       : Validite du magneticHeading magnetique de la bouee
void ModeManagement::calculateGeneralMode(
    bool frontMontantHomeMemoCmde,        
    bool frontMontantPosHomeCmde,    
    double xLatHomeCmde,                         
    double yLonHomeCmde,  
    bool frontMontantMaintenanceCmde,           
    bool frontMontantResumeReadyCmde,          
    bool frontMontantHomeValidationCmde, 
    bool dataLinkOk, 
    double xLatGps, double yLonGps, bool gpsOk,
    float magneticHeading, bool magneticHeadingOk)

{

    uint32_t _millis = millis();

    tEtatsGeneral pastGeneralMode;  //Mode general du cycle precedent
    pastGeneralMode = generalMode_;
    bool pastPosHomeOk;  //Validite de la position du HOME du cycle precedent
    pastPosHomeOk = posHomeOk_;

    frontMontantReady_ = false;          
    frontMontantMaintenance_ = false;   
    frontMontantHomeDefinition_ = false; 
    frontMontantNav_ = false;           

    //---Calcul du nouvel general Mode de l automate (avec les entres figees)
    switch (pastGeneralMode)
    {
    case INIT:
        if (!gpsOk || !magneticHeadingOk)
        {
            // Maintien dans le mode INIT, car un des capteurs est invalide (priorite 1)
            generalMode_ = pastGeneralMode;
            posHomeOk_ = false;
        }
        else
        {
            if (!dataLinkOk)
            {
                // Maintien dans le mode INIT tant que le Datalink n est pas operationnel (priorite 1)
                generalMode_ = pastGeneralMode;
                posHomeOk_ = false;
            }
            else 
            {
                // Passage en mode READY car les capteurs et le datalink sont OK
                generalMode_ = READY;
                posHomeOk_ = false;
                frontMontantReady_ = true;
            }
        }
        break;
    case READY:
        if (!dataLinkOk)
        {
            // Maintien dans le mode READY tant que le Datalink n est pas operationnel (priorite 1)
            generalMode_ = pastGeneralMode;
            posHomeOk_ = false;
        }
        else 
        {
            // Datalink operationnel => prise en compte d eventuelles commandes
            // dans l ordre de priorite MAINTENANCE, HOME_DEFINITION
            if (frontMontantMaintenanceCmde)
            {
                // Passage en mode MAINTENANCE sur commande de maintenance (priorite 1)
                generalMode_ = MAINTENANCE;
                posHomeOk_ = false;
                frontMontantMaintenance_ = true;
            }
            else
            {
                if (frontMontantHomeMemoCmde  && gpsOk)
                {
                    // Passage en mode NAV_HOME sur commande de memorisation du Home (priorite 2)
                    generalMode_ = HOME_DEFINITION;
                    frontMontantHomeDefinition_ = true;                     
                    // Stockage de la position courante de la bouee dans la position du Home
                    xLatHome_ = xLatGps;
                    yLonHome_ = yLonGps;
                    posHomeOk_ = true;
                }
                else
                {
                    if (frontMontantPosHomeCmde)
                    {
                        // Passage en mode NAV_HOME sur commande de position du Home (priorite 3)
                        generalMode_ = HOME_DEFINITION;
                        frontMontantHomeDefinition_ = true;                     
                        // Stockage de la position courante de la bouee dans la position du Home
                        xLatHome_ = xLatHomeCmde;
                        yLonHome_ = yLonHomeCmde;
                        posHomeOk_ = true;
                    }
                    else
                    {
                        // Pas de transition d etat (En attente de la commande de definition du Home)
                        generalMode_ = pastGeneralMode;
                        posHomeOk_ = pastPosHomeOk;
                    }
                }
            }
        }
        break;
    case MAINTENANCE:
        if (!dataLinkOk)
        {
            // Maintien dans le mode MAINTENANCE tant que le Datalink n est pas operationnel (priorite 1)
            generalMode_ = pastGeneralMode;
            posHomeOk_ = false;
        }
        else 
        {
            // Datalink operationnel => prise en compte d eventuelles commandes
            // dans l ordre de priorite RESUME_READY
            if (frontMontantResumeReadyCmde)
            {
                // Passage en mode READY sur commande de resume ready (priorite 1)
                generalMode_ = READY;
                posHomeOk_ = false;
                frontMontantReady_ = true;
            }
            else
            {
                // Pas de transition d etat
                generalMode_ = pastGeneralMode;
                posHomeOk_ = false;
            }
        }
        break;
    case HOME_DEFINITION:
        if (!dataLinkOk)
        {
            // Maintien dans le mode HOME_DEFINITION tant que le Datalink n est pas operationnel (priorite 1)
            generalMode_ = pastGeneralMode;
            posHomeOk_ = pastPosHomeOk;
        }
        else 
        {
            // Datalink operationnel => prise en compte d eventuelles commandes
            // dans l ordre de priorite MAINTENANCE, HOME_DEFINITION, NAV
            if (frontMontantMaintenanceCmde)
            {
                // Passage en mode MAINTENANCE sur commande de maintenance (priorite 1)
                generalMode_ = MAINTENANCE;
                posHomeOk_ = pastPosHomeOk;
                frontMontantMaintenance_ = true;
            }
            else
            {
                if (frontMontantHomeMemoCmde  && gpsOk)
                {
                    // Passage en mode NAV_HOME sur commande de memorisation du Home (priorite 2)
                    generalMode_ = pastGeneralMode;
                    // Stockage de la position courante de la bouee dans la position du Home
                    xLatHome_ = xLatGps;
                    yLonHome_ = yLonGps;
                    posHomeOk_ = true;
                }
                else
                {
                    if (frontMontantPosHomeCmde)
                    {
                        // Passage en mode NAV_HOME sur commande de position du Home (priorite 3)
                        generalMode_ = pastGeneralMode;
                        frontMontantHomeDefinition_ = false;                     
                        // Stockage de la position courante de la bouee dans la position du Home
                        xLatHome_ = xLatHomeCmde;
                        yLonHome_ = yLonHomeCmde;
                        posHomeOk_ = true;
                    }
                    else
                    {
                        if (frontMontantHomeValidationCmde)
                        {
                            // Passage en mode NAV sur commande de validation du HOme
                            generalMode_ = NAV;
                            posHomeOk_ = pastPosHomeOk;
                            frontMontantNav_ = true;
                        }
                        else
                        {
                            // Pas de transition d etat (En attente de la commande de definition ou de validation du Home)
                            generalMode_ = pastGeneralMode;
                            posHomeOk_ = pastPosHomeOk;
                        }
                    }
                }
            }
        }
        break;
    case NAV:
        generalMode_ = pastGeneralMode;
        posHomeOk_ = pastPosHomeOk;
        break;
    default:
        generalMode_ = INIT;
        posHomeOk_ = false;
    }
}

// Elaboration du mode de navigation de la bouee
//     bool frontMontantHomeCmde        : Front montant de la commande Home
//     bool frontMontantHoldCmde        : Front montant de la commande Hold
//     bool frontMontantStopCmde        : Front montant de la commande Stop
//     bool frontMontantNavBasicCmde    : Front montant de la commande Navigation Basique
//     bool frontMontantNavCapCmde      : Front montant de la commande Navigation en Cap
//     bool frontMontantNavTargetCmde   : Front montant de la commande Navigation sur une position Target
//     double xLatTargetCmde            : Latitude demandee associee a la commande Navigation sur une position Target
//     double yLonTargetCmde            : Longitude demandee associee a la commande Navigation sur une position Target
//     bool dataLinkOk                  : validite de DataLink
//     double xLatGps                   : Latitude de la bouee
//     double yLonGps                   : Longitude de la bouee
//     bool gpsOk                       : Validite de la postion de la bouee
//     float magneticHeading            : Cap magnetique de la bouee
//     bool magneticHeadingOk           : Validite du magneticHeading magnetique de la bouee
void ModeManagement::calculateNavigationMode(
    bool frontMontantHomeCmde,              
    bool frontMontantHoldCmde,           
    bool frontMontantStopCmde,          
    bool frontMontantNavBasicCmde,      
    bool frontMontantNavCapCmde,           
    bool frontMontantNavTargetCmde,           
    bool dataLinkOk, 
    double xLatGps, double yLonGps, bool gpsOk,
    float magneticHeading, bool magneticHeadingOk)

{

    uint32_t _millis = millis();

    tEtatsNav pastNavigationMode;  //Mode de navigation ducycle precedent
    pastNavigationMode = navigationMode_;
    bool pastPosHomeOk;  //Validite de la position du HOME du cycle precedent
    pastPosHomeOk = posHomeOk_;

    frontMontantNavHome_ = false;                     
    frontMontantNavHold_ = false;
    frontMontantNavStop_ = false;
    frontMontantNavBasic_ = false;
    frontMontantNavCap_ = false;
    frontMontantNavTarget_ = false;  

    //---Calcul du nouvel navigationMode de l automate (avec les entres figees)
    switch (pastNavigationMode)
    {
    case NAV_NOTHING:
        if (frontMontantNav_)
        {
            // Initialisation du mode de navigation en NAV_HOME 
            navigationMode_ = NAV_HOME;
        }
        else
        {
            // Pas de transition d etat (En attente de passage en NAV)
            navigationMode_ = pastNavigationMode;
        }
        break;
    case NAV_HOME:
        if (!gpsOk || !magneticHeadingOk)
        {
            // Passage en mode NAV_STOP sur perte capteur (priorite 1)
            navigationMode_ = NAV_STOP;
            frontMontantNavStop_ = true;                     
        }
        else
        {
            if (!dataLinkOk)
            {
                // Maintien dans le mode NAV_HOME en cas de perte DataLink (priorite 2)
                navigationMode_ = pastNavigationMode;
            }
            else
            {   
                // Data Link operationnel => prise en compte d un eventuelle commande
                // dans l ofre de priorite NAV_STOP, NAV_TARGET, NAV_CAP, NAV_BASIC
                // (Les capteurs sont valides a ce stade : voir test plus haut)
                if (frontMontantStopCmde) 
                {
                    // Passage en mode NAV_STOP sur commande Stop (priorite 1)
                    navigationMode_ = NAV_STOP;
                    frontMontantNavStop_ = true;                     
                } else {
                    if (frontMontantNavTargetCmde)
                    {
                        // Passage dans le mode NAV_TARGET sur commande NavTarget (priorite 3)
                        navigationMode_ = NAV_TARGET;
                        frontMontantNavTarget_ = true;                     
                    }
                    else
                    {
                        if (frontMontantNavCapCmde)
                        {
                            // Passage en mode NAV_CAP sur commande NavCap (priorite 4)
                            navigationMode_ = NAV_CAP;
                            frontMontantNavCap_ = true;                     
                        }
                        else
                        {
                            if (frontMontantNavBasicCmde)
                            {
                                // Passage en mode NAV_BASIC sur commande NavBasic (priorite 5)
                                navigationMode_ = NAV_BASIC;
                                frontMontantNavBasic_ = true;                     
                            }
                            else
                            {
                                // Pas de transition d etat
                                navigationMode_ = pastNavigationMode;
                            }
                        }
                    }
                }
            }
        }
        break;
    case NAV_HOLD:
        if (!gpsOk || !magneticHeadingOk)
        {
            // Passage en mode NAV_STOP sur perte capteur (priorite 1))
            navigationMode_ = NAV_STOP;
            frontMontantNavStop_ = true;                     
        }
        else
        {
            if (!dataLinkOk)
            {
                // Passage en mode NAV_HOME sur perte DataLink (priorite 2)
                navigationMode_ = NAV_HOME;
                frontMontantNavHome_ = true;                     
            }
            else
            {
                // Data Link operationnel => prise en compte d eventuelles commandes
                // dans l ofre de priorite NAV_STOP, NAV_HOME, NAV_TARGET, NAV_CAP, NAV_BASIC
                if (frontMontantStopCmde)
                {
                    // Passage en mode NAV_STOP sur commande Stop (priorite 1)
                    navigationMode_ = NAV_STOP;
                    frontMontantNavStop_ = true;                     
                } 
                else 
                {
                    if (frontMontantHomeCmde)
                    {
                        // Passage en mode NAV_HOME sur commande Home (priorite 2)
                        navigationMode_ = NAV_HOME;
                        frontMontantNavHome_ = true;                     
                    }
                    else
                    {
                        if (frontMontantNavTargetCmde)
                        {
                            // Passage dans le mode NAV_TARGET sur commande NavTarget (priorite 3)
                            navigationMode_ = NAV_TARGET;
                            frontMontantNavTarget_ = true;                     
                        }
                        else
                        {
                            if (frontMontantNavCapCmde)
                            {
                                // Passage en mode NAV_CAP (priorite 4)
                                navigationMode_ = NAV_CAP;
                                frontMontantNavCap_ = true;                     
                            }
                            else
                            {
                                if (frontMontantNavBasicCmde)
                                {
                                    // Passage en mode NAV_BASIC sur commande NavBasic (priorite 5)
                                    navigationMode_ = NAV_BASIC;
                                    frontMontantNavBasic_ = true;                     
                                }
                                else
                                {
                                    // Pas de transition d etat, en attente d une commande de sortie
                                    navigationMode_ = pastNavigationMode;
                                }
                            }
                        }
                    }
                }
            }
        }
        break;
    case NAV_STOP:
        if (!dataLinkOk)
        {
            // Perte DataLink
            if (gpsOk && magneticHeadingOk)
            {
                // Passage en mode NAV_HOME sur perte DataLink (priorite 1)
                navigationMode_ = NAV_HOME;
                frontMontantNavHome_ = true;                     
            }
            else
            {
                // Maintien dans l etat NAV_STOP sur panne capteur
                navigationMode_ = pastNavigationMode;
            }
        }
        else
        {
            // DataLInk operationnel => prise en compte d une eventuelle commande
            //dans l ordre de priorite NAV_HOME, NAV_HOLD, NAV_CAP, NAV_BASIC
            if (gpsOk && magneticHeadingOk && frontMontantHomeCmde)
            {
                // Passage en mode NAV_HOME sur commande Home (priorite 1)
                navigationMode_ = NAV_HOME;
                frontMontantNavHome_ = true;                     
            }
            else
            {
                if (gpsOk && magneticHeadingOk && frontMontantHoldCmde)
                {
                    // Passage en mode NAV_HOME sur commande Home (priorite 2)
                    navigationMode_ = NAV_HOLD;
                    frontMontantNavHold_ = true;                     
                }
                else
                {
                    if (magneticHeadingOk && frontMontantNavCapCmde)
                    {
                        // Passage en mode NAV_CAP sur commande NavCap (priorite 3)
                        navigationMode_ = NAV_CAP;
                        frontMontantNavCap_ = true;                     
                    }
                    else
                    {
                        if (frontMontantNavBasicCmde)
                        {
                            // Passage en mode NAV_BASIC sur commande NavBasic (priorite 4)
                            navigationMode_ = NAV_BASIC;
                            frontMontantNavBasic_ = true;                     
                        }
                        else
                        {
                            // Maintien dans l etat NAV_STOP en attente d une commande de sortie
                            navigationMode_ = pastNavigationMode;
                        }
                    }
                }
            }
        }
        break;
    case NAV_BASIC:
        if (!dataLinkOk)
        {
            // Perte DataLink
            if (gpsOk && magneticHeadingOk)
            {
                // Passage en mode NAV_HOME sur perte DataLink (priorite 2)
                navigationMode_ = NAV_HOME;
                frontMontantNavHome_ = true;                     
            }
            else
            {
                // Passage en mode NAV_STOP sur perte Datalink et perte capteurs (priorite 1)
                navigationMode_ = NAV_STOP;
                frontMontantNavStop_ = true;                     
            }
        }
        else
        {
            // Data Link operationnel => prise en compte d une eventuelle commande
            // dans l ordre de priorite NAV_STOP, NAV_HOME, NAV_HOLD, NAV_CAP
            if (frontMontantStopCmde)
            {
                // Passage en mode NAV_STOP sur commande Stop (priorite 1)
                navigationMode_ = NAV_STOP;
                frontMontantNavStop_ = true;                     
            }
            else
            {
                if (gpsOk && magneticHeadingOk && frontMontantHomeCmde)
                {
                        // Passage en mode NAV_HOME sur commande Home (priorite 2)
                        navigationMode_ = NAV_HOME;
                        frontMontantNavHome_ = true;                     
                }
                else
                {
                    if (gpsOk && magneticHeadingOk && frontMontantHoldCmde)
                    {
                        // Passage en mode NAV_HOLD sur commande Hold (priorite 3)
                        navigationMode_ = NAV_HOLD;
                        frontMontantNavHold_ = true;                     
                    }
                    else
                    {
                        if (magneticHeadingOk && frontMontantNavCapCmde)
                        {
                            // Passage en mode NAV_CAP sur commande NavCap (priorite 4)
                            navigationMode_ = NAV_CAP;
                            frontMontantNavCap_ = true;                     
                        }
                        else
                        {
                            // Pas de transition d etat, en attente d une commande de sortie
                            navigationMode_ = pastNavigationMode;
                        }
                    }
                }
            }
        }
        break;
    case NAV_CAP:
        if (!dataLinkOk)
        {
            // Perte DataLink
            if (gpsOk && magneticHeadingOk)
            {
                // Passage en mode NAV_HOME sur perte DataLink (priorite 2)
                navigationMode_ = NAV_HOME;
                frontMontantNavHome_ = true;                     
            }
            else
            {
                // Passage en mode NAV_STOP sur perte Datalink et perte capteurs (priorite 1)
                navigationMode_ = NAV_STOP;
                frontMontantNavStop_ = true;                     
            }
        }
        else
        {
            // DataLink operationnel => prise en compte d une eventiuelle commande
            // dans l ordre de priorite NAV_STOP, NAV_HOME, NAV_HOLD, NAV_BASIC
            if (frontMontantStopCmde)
            {
                // Passage en mode NAV_STOP sur commande Stop (priorite 1)
                navigationMode_ = NAV_STOP;
                frontMontantNavStop_ = true;                     
            } 
            else
            {
                if (gpsOk && magneticHeadingOk && frontMontantHomeCmde)
                {
                    // Passage en mode NAV_HOME sur commande Home (priorite 2)
                    navigationMode_ = NAV_HOME;
                    frontMontantNavHome_ = true;                     
                }
                else
                {
                    if (gpsOk && magneticHeadingOk && frontMontantHoldCmde)
                    {
                        // Passage en mode NAV_HOLD sur commande Hold (priorite 3)
                        navigationMode_ = NAV_HOLD;
                        frontMontantNavHold_ = true;                     
                    }
                    else
                    {
                        if (!magneticHeadingOk || frontMontantNavBasicCmde)
                        {
                            // Passage en mode NAV_BASIC sur commande NavCap ou perte boussole (priorite 4)
                            navigationMode_ = NAV_BASIC;
                            frontMontantNavBasic_ = true;                     
                        }
                        else
                        {
                            // Pas de transition d etat, en attente de commande de sortie
                            navigationMode_ = pastNavigationMode;
                        }
                    }
                }
            }
        }
        break;
    case NAV_TARGET:
        if (!gpsOk || !magneticHeadingOk)
        {
            // Passage en mode NAV_STOP sur perte capteur (priorite 1)
            navigationMode_ = NAV_STOP;
            frontMontantNavStop_ = true;                     
        }
        else
        {
            if (!dataLinkOk)
            {
                // Passage en mode NAV_HOME sur perte DataLink (priorite 2)
                navigationMode_ = NAV_HOME;
                frontMontantNavHome_ = true;                     
            } 
            else 
            {
                //Data Link operationnel => prise en compte d un eventuelle commande
                // dans l ofre de priorite NAV_STOP, NAV_HOME, NAV_CAP, NAV_BASIC
                if (frontMontantStopCmde)
                {
                    // Passage en mode NAV_STOP sur perte capteur ou commande Stop (priorite 1)
                    navigationMode_ = NAV_STOP;
                    frontMontantNavStop_ = true;                     
                }
                else
                {
                    if (gpsOk && magneticHeadingOk  && frontMontantHomeCmde)
                    {
                        // Passage en mode NAV_HOME commande Home (priorite 2)
                        navigationMode_ = NAV_HOME;
                        frontMontantNavHome_ = true;                     
                    }
                    else
                        {
                        if (magneticHeadingOk  && frontMontantNavCapCmde)
                        {
                            // Passage en mode NAV_CAP (priorite 3)
                            navigationMode_ = NAV_CAP;
                            frontMontantNavCap_ = true;                     
                        }
                        else
                        {
                            if (frontMontantNavBasicCmde)
                            {
                                // Passage en mode NAV_BASIC sur commande NavBasic (priorite 4)
                                navigationMode_ = NAV_BASIC;
                                frontMontantNavBasic_ = true;                     
                            }
                            else
                            {
                                // Pas de transition d etat, en attente de commande de sortie
                                navigationMode_ = pastNavigationMode;
                            }
                        }
                    }
                }
            }
        }
        break;
    default:
        navigationMode_ = NAV_NOTHING;
    }
}
