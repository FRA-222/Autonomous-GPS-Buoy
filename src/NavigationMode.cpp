#include "NavigationMode.h"
#include <Arduino.h>

bool NavigationMode::homeMemorisationComPrecedente = false; // Etat de la commande de memorisation du Home du cycle precedent
int NavigationMode::navicationModePrecedent = 0;            // Mode de navigation demande au cycle precedent

tEtats NavigationMode::etatPrecedent = WAIT_HOME;    // Etat de l'automate du cycle précédent
tSousEtats NavigationMode::sousEtatPrecedent = RIEN; // Sous etat de l'automate du cycle précédent
char32_t NavigationMode::dureeWaitHome = 0;          // duree d activation du mode WAIT_HOME
char32_t NavigationMode::dureeHome = 0;              // duree d activation du mode HOME
char32_t NavigationMode::dureeHold = 0;              // duree d activation du mode HOLD
char32_t NavigationMode::dureeStop = 0;              // duree d activation du mode STOP
char32_t NavigationMode::dureeNavBasic = 0;          // duree d activation du mode NAV_BASIC
char32_t NavigationMode::dureeNavCap = 0;            // duree d activation du mode NAV_CAP
char32_t NavigationMode::dureeNavTarget = 0;         // duree d activation du mode NAV_TARGET

// Sorties de la gestion des modes
double NavigationMode::xLatHome = 0.;               // Latitude du Home
double NavigationMode::yLonHome = 0.;               // Longitude du Home
bool NavigationMode::posHomeOk = false;           // Validite de la position du Home
double NavigationMode::xLatCons = 0.;               // Latitude de consigne
double NavigationMode::yLonCons = 0.;               // Longitude de consigne
float NavigationMode::capCons = 0.;                // Cap de consigne
float NavigationMode::pousseeForcee = 0.;          // manette des gaz forcee (resynchro de la commande)
float NavigationMode::orientationForcee = 0.;      // cmdeorientation forcee (resynchro de la commande)

NavigationMode::NavigationMode()
{    
    // Sorties de la gestion des modes
    posHomeOk = false;           // Validite de la position du Home
    xLatCons = 0.;               // Latitude de consigne
    yLonCons = 0.;               // Longitude de consigne
    posConsOk = false;           // Validite de la consigne en position
    
    capConsOk = false;           // Validite de la consigne en cap
   
    pousseeForceeOk = false;     // Validite du forcage de la manette des gaz
    
    orientationForceeOk = false; // Validite du forcage de la commande d orientationForcee
}

NavigationMode::~NavigationMode()
{
}

tEtats NavigationMode::getEtat()
{
    return etat;
}

void NavigationMode::switchEtat(int navigationMode)
{
    // Memorisation des commandes en entree de l automate
    // homeMemorisationComPrecedente = homeMemorisationCom;
    navicationModePrecedent = navigationMode;
    // Memorisation de l etat de l automate
    etatPrecedent = etat;
}

void NavigationMode::calculateMode(int navigationMode)
{
    //---Lecture des entrees de l automate
    // frontMontantDefinirHome = homeMemorisationCom && !homeMemorisationComPrecedente;
    frontMontantHome = false;
    if (navigationMode != navicationModePrecedent && navigationMode == 1)
    {
        frontMontantHome = true;
    }
    frontMontantHold = false;
    if (navigationMode != navicationModePrecedent && navigationMode == 2)
    {
        frontMontantHold = true;
    }
    frontMontantStop = false;
    if (navigationMode != navicationModePrecedent && navigationMode == 3)
    {
        frontMontantStop = true;
    }
    frontMontantNavBasic = false;
    if (navigationMode != navicationModePrecedent && navigationMode == 4)
    {
        frontMontantNavBasic = true;
    }
    frontMontantNavCap = false;
    if (navigationMode != navicationModePrecedent && navigationMode == 5)
    {
        frontMontantNavCap = true;
    }
    frontMontantNavTarget = false;
    if (navigationMode != navicationModePrecedent && navigationMode == 6)
    {
        frontMontantNavTarget = true;
    }

    uint32_t _millis = millis();

    //---Calcul du nouvel etat de l automate (avec les entres figees)
    switch (etatPrecedent)
    {
    case WAIT_HOME:
        if (!dlOk || !posOk || !magnetOk)
        {
            // Maintien dans le mode WAIT_HOME, car un des capteurs est invalide (priorite 1)
            etat = etatPrecedent;
            // Mise a zero des consignes
            posConsOk = false;
            capConsOk = false;
            pousseeForceeOk = false;
            orientationForcee = false;
        }
        else
        {
            if (frontMontantDefinirHome)
            {
                // Passage en mode HOME sur commande de memorisation du Home (priorite 2)
                etat = HOME;
                // Stockage de la position courante de la bouee dans la position du Home
                xLatHome = xLat;
                yLonHome = yLon;
                posHomeOk = true;
                // Stockage de la position du Home dans la position de consigne
                xLatCons = xLatHome;
                yLonCons = yLonHome;
                posConsOk = true;
                capConsOk = false;
                pousseeForceeOk = false;
                orientationForceeOk = false;
            }
            else
            {
                // Pas de transition d etat (En attente de la definition du Home)
                etat = etatPrecedent;
                // Maintien a zero des consignes
                posConsOk = false;
                capConsOk = false;
                pousseeForceeOk = false;
                orientationForceeOk = false;
            }
        }
        break;
    case HOME:
        if (!posOk || !magnetOk || frontMontantStop)
        {
            // Passage en mode STOP sur perte capteur ou commande Stop (priorite 1)
            etat = STOP;
            // Mise à zero des consignes
            posConsOk = false;
            capConsOk = false;
            pousseeForceeOk = false;
            orientationForceeOk = false;
        }
        else
        {
            if (!dlOk)
            {
                // Maintien dans le mode HOME en cas de perte DataLink (priorite 2)
                etat = etatPrecedent;
                // Maintien de la validite de la position de consigne
                posConsOk = true;
                capConsOk = false;
                pousseeForceeOk = false;
                orientationForceeOk = false;
            }
            else
            {
                if (frontMontantNavTarget)
                {
                    // Passage dans le mode NAV_TARGET sur commande NavTarget (priorite 3)
                    etat = NAV_TARGET;
                    // Stockage de la position Target dans la position de consigne
                    xLatCons = xLatTarget;
                    yLonCons = yLonTarget;
                    posConsOk = true;
                    capConsOk = false;
                    pousseeForceeOk = false;
                    orientationForceeOk = false;
                }
                else
                {
                    if (frontMontantNavCap)
                    {
                        // Passage en mode NAV_CAP sur commande NavCap (priorite 4)
                        etat = NAV_CAP;
                        // Stockage du cap courant de la bouee dans le cap de consigne
                        // et forcage des commandes des gaz et orientation a zero
                        posConsOk = false;
                        capCons = cap;
                        capConsOk = true;
                        pousseeForcee = 0.;
                        pousseeForceeOk = true;
                        orientationForcee = 0.;
                        orientationForceeOk = true;
                    }
                    else
                    {
                        if (frontMontantNavBasic)
                        {
                            // Passage en mode NAV_BASIC sur commande NavBasic (priorite 5)
                            etat = NAV_BASIC;
                            // Forcage des commandes des gaz et orientation a zero
                            posConsOk = false;
                            capConsOk = false;
                            pousseeForcee = 0.;
                            pousseeForceeOk = true;
                            orientationForcee = 0.;
                            orientationForceeOk = true;
                        }
                        else
                        {
                            // Pas de transition d etat
                            etat = etatPrecedent;
                            // Maintien de la validite de la position de consigne
                            posConsOk = true;
                            capConsOk = false;
                            pousseeForceeOk = false;
                            orientationForceeOk = false;
                        }
                    }
                }
            }
        }
        break;
    case HOLD:
        if (!posOk || !magnetOk || frontMontantStop)
        {
            // Passage en mode STOP sur perte capteur ou commande Stop (priorite 1)
            etat = STOP;
            // Mise a zero des consignes
            posConsOk = false;
            capConsOk = false;
            pousseeForceeOk = false;
            orientationForceeOk = false;
        }
        else
        {
            if (!dlOk || frontMontantHome)
            {
                // Passage en mode HOME sur perte DataLink ou commande Home (priorite 2)
                etat = HOME;
                // Stockage de la position du Home dans la position de consigne
                xLatCons = xLatHome;
                yLonCons = yLonHome;
                posConsOk = true;
                capConsOk = false;
                pousseeForceeOk = false;
                orientationForceeOk = false;
            }
            else
            {
                if (frontMontantNavTarget)
                {
                    // Passage dans le mode NAV_TARGET sur commande NavTarget (priorite 3)
                    etat = NAV_TARGET;
                    // Stockage de la position Target dans la position de consigne
                    xLatCons = xLatTarget;
                    yLonCons = yLonTarget;
                    posConsOk = true;
                    capConsOk = false;
                    pousseeForceeOk = false;
                    orientationForceeOk = false;
                }
                else
                {
                    if (frontMontantNavCap)
                    {
                        // Passage en mode NAV_CAP (priorite 4)
                        etat = NAV_CAP;
                        // Stockage du cap courant de la bouee dans le cap de consigne
                        // et forcage des commandes des gaz et orientation a zero
                        posConsOk = false;
                        capCons = cap;
                        capConsOk = true;
                        pousseeForcee = 0.;
                        pousseeForceeOk = true;
                        orientationForcee = 0.;
                        orientationForceeOk = true;
                    }
                    else
                    {
                        if (frontMontantNavBasic)
                        {
                            // Passage en mode NAV_BASIC sur commande NavBasic (priorite 5)
                            etat = NAV_BASIC;
                            // Forcage des commandes des gaz et orientation a zero
                            posConsOk = false;
                            capConsOk = false;
                            pousseeForcee = 0.;
                            pousseeForceeOk = true;
                            orientationForcee = 0.;
                            orientationForceeOk = true;
                        }
                        else
                        {
                            // Pas de transition d etat, en attente d une commande de sortie
                            etat = etatPrecedent;
                            // Maintien de la validite de la position de consigne
                            posConsOk = true;
                            capConsOk = false;
                            pousseeForceeOk = false;
                            orientationForceeOk = false;
                        }
                    }
                }
            }
        }
        break;
    case STOP:
        if (!dlOk)
        {
            // Perte DataLink
            if (posOk && magnetOk)
            {
                // Passage en mode HOME sur perte DataLink (priorite 1)
                etat = HOME;
                // Stockage de la position Home dans la position de consigne
                xLatCons = xLatHome;
                yLonCons = yLonHome;
                posConsOk = true;
                capConsOk = false;
                pousseeForceeOk = false;
                orientationForceeOk = false;
            }
            else
            {
                // Maintien dans l etat STOP sur panne capteur
                etat = etatPrecedent;
                // Maintien a zero des consignes
                posConsOk = false;
                capConsOk = false;
                pousseeForceeOk = false;
                orientationForceeOk = false;
            }
        }
        else
        {
            // DataLInk operationnel
            if (posOk && magnetOk)
            {
                if (frontMontantHome)
                {
                    // Passage en mode HOME sur commande Home (priorite 1)
                    etat = HOLD;
                    // Stockage de la position du Home dans la position de consigne
                    xLatCons = xLatHome;
                    yLonCons = yLonHome;
                    posConsOk = true;
                    capConsOk = false;
                    pousseeForceeOk = false;
                    orientationForceeOk = false;
                }
                else
                {
                    if (frontMontantHold)
                    {
                        // Passage en mode HOME sur commande Home (priorite 2)
                        etat = HOLD;
                        // Stockage de la position courante de la bouee dans la position de consigne
                        xLatCons = xLat;
                        yLonCons = yLon;
                        posConsOk = true;
                        capConsOk = false;
                        pousseeForceeOk = false;
                        orientationForceeOk = false;
                    }
                    else
                    {
                        // Maintien dans l etat STOP en attente d une commande de sortie
                        etat = etatPrecedent;
                        // Maintien a zero des consignes
                        posConsOk = false;
                        capConsOk = false;
                        pousseeForceeOk = false;
                        orientationForceeOk = false;
                    }
                }
            }
            else
            {
                if (magnetOk && frontMontantNavCap)
                {
                    // Passage en mode NAV_CAP sur commande NavCap (priorite 3)
                    etat = NAV_CAP;
                    // Stockage du cap courant de la bouee dans le cap de consigne
                    posConsOk = false;
                    capCons = cap;
                    capConsOk = true;
                    pousseeForceeOk = false;
                    orientationForceeOk = false;
                }
                else
                {
                    if (frontMontantNavBasic)
                    {
                        // Passage en mode NAV_BASIC sur commande NavBasic (priorite 4)
                        etat = NAV_BASIC;
                        // Forcage des commandes des gaz et orientation a zero
                        posConsOk = false;
                        capConsOk = false;
                        pousseeForcee = 0.;
                        pousseeForceeOk = true;
                        orientationForcee = 0.;
                        orientationForceeOk = true;
                    }
                    else
                    {
                        // Maintien dans l etat STOP en attente d une commande de sortie
                        etat = etatPrecedent;
                        // Maintien a zero des consignes
                        posConsOk = false;
                        capConsOk = false;
                        pousseeForceeOk = false;
                        orientationForceeOk = false;
                    }
                }
            }
        }
        break;
    case NAV_BASIC:
        if (frontMontantStop)
        {
            // Passage en mode STOP sur commande Stop (priorite 1)
            etat = STOP;
            // Mise à zero des consignes
            posConsOk = false;
            capConsOk = false;
            pousseeForceeOk = false;
            orientationForceeOk = false;
        }
        else
        {
            if (!dlOk)
            {
                // Perte DataLink
                if (posOk && magnetOk)
                {
                    // Passage en mode HOME sur perte DataLink (priorite 2)
                    etat = HOME;
                    // Stockage de la position du Home dans la position de consigne
                    xLatCons = xLatHome;
                    yLonCons = yLonHome;
                    posConsOk = true;
                    capConsOk = false;
                    pousseeForceeOk = false;
                    orientationForceeOk = false;
                }
                else
                {
                    // Passage en mode STOP sur perte Datalink et perte capteurs (priorite 1)
                    etat = STOP;
                    // Mise à zero des consignes
                    posConsOk = false;
                    capConsOk = false;
                    pousseeForceeOk = false;
                    orientationForceeOk = false;
                }
            }
            else
            {
                // DataLink operationnel
                if (frontMontantHome)
                {
                    if (posOk && magnetOk)
                    {
                        // Passage en mode HOME sur commande Home (priorite 2)
                        etat = HOME;
                        // Stockage de la position du Home dans la position de consigne
                        xLatCons = xLatHome;
                        yLonCons = yLonHome;
                        posConsOk = true;
                        capConsOk = false;
                        pousseeForceeOk = false;
                        orientationForceeOk = false;
                    }
                    else
                    {
                        // Pas de transition d etat, car les capteurs ne permettent pas de passer en mode Home
                        etat = etatPrecedent;
                        // Maintien des consignes a zero
                        posConsOk = false;
                        capConsOk = false;
                        pousseeForceeOk = false;
                        orientationForceeOk = false;
                    }
                }
                else
                {
                    // Attente de commandes
                    if (posOk && magnetOk && frontMontantHold)
                    {
                        // Passage en mode HOLD sur commande Hold (priorite 3)
                        etat = HOLD;
                        // Stockage de la position courante de la bouee dans la position de consigne
                        xLatCons = xLat;
                        yLonCons = yLon;
                        posConsOk = true;
                        capConsOk = false;
                        pousseeForceeOk = false;
                        orientationForceeOk = false;
                    }
                    else
                    {
                        if (magnetOk && frontMontantNavCap)
                        {
                            // Passage en mode NAV_CAP sur commande NavCap (priorite 4)
                            etat = NAV_CAP;
                            // Stockage du cap courant de la bouee dans le cap de consigne
                            posConsOk = false;
                            capCons = cap;
                            capConsOk = true;
                            pousseeForceeOk = false;
                            orientationForceeOk = false;
                        }
                        else
                        {
                            // Pas de transition d etat, en attente d une commande de sortie
                            etat = etatPrecedent;
                            // Maintien des consignes a zero
                            posConsOk = false;
                            capConsOk = false;
                            pousseeForceeOk = false;
                            orientationForceeOk = false;
                        }
                    }
                }
            }
        }
        break;
    case NAV_CAP:
        if (frontMontantStop)
        {
            // Passage en mode STOP sur commande Stop (priorite 1)
            etat = STOP;
            // Mise A zero des consignes
            posConsOk = false;
            capConsOk = false;
            pousseeForceeOk = false;
            orientationForceeOk = false;
        }
        else
        {
            if (!dlOk)
            {
                // Perte DataLink
                if (posOk && magnetOk)
                {
                    // Passage en mode HOME sur perte DataLink (priorite 2)
                    etat = HOME;
                    // Stockage de la position du Home dans la position de consigne
                    xLatCons = xLatHome;
                    yLonCons = yLonHome;
                    posConsOk = true;
                    capConsOk = false;
                    pousseeForceeOk = false;
                    orientationForceeOk = false;
                }
                else
                {
                    // Passage en mode STOP sur perte Datalink et perte capteurs (priorite 1)
                    etat = STOP;
                    // Mise à zero des consignes
                    posConsOk = false;
                    capConsOk = false;
                    pousseeForceeOk = false;
                    orientationForceeOk = false;
                }
            }
            else
            {
                // DataLink operationnel
                if (frontMontantHome)
                {
                    if (posOk && magnetOk)
                    {
                        // Passage en mode HOME sur commande Home (priorite 2)
                        etat = HOME;
                        // Stockage de la position du Home dans la position de consigne
                        xLatCons = xLatHome;
                        yLonCons = yLonHome;
                        posConsOk = true;
                        capConsOk = false;
                        pousseeForceeOk = false;
                        orientationForceeOk = false;
                    }
                    else
                    {
                        // Pas de transition d etat, car les capteurs ne permettent pas de passer en mode Home
                        etat = etatPrecedent;
                        // Maintien des consignes a zero
                        posConsOk = false;
                        capConsOk = false;
                        pousseeForceeOk = false;
                        orientationForceeOk = false;
                    }
                }
                else
                {
                    // Attente de commande
                    if (posOk && magnetOk && frontMontantHold)
                    {
                        // Passage en mode HOLD sur commande Hold (priorite 3)
                        etat = HOLD;
                        // Stockage de la position de la position courante de la bouee dans la position de consigne
                        xLatCons = xLat;
                        yLonCons = yLon;
                        posConsOk = true;
                        capConsOk = false;
                        pousseeForceeOk = false;
                        orientationForceeOk = false;
                    }
                    else
                    {
                        if (!magnetOk || frontMontantNavBasic)
                        {
                            // Passage en mode NAV_BASIC sur commande NavCap ou perte boussole (priorite 4)
                            etat = NAV_BASIC;
                            // Maintien des consignes a zero
                            posConsOk = false;
                            capConsOk = false;
                            pousseeForceeOk = false;
                            orientationForceeOk = false;
                        }
                        else
                        {
                            // Pas de transition d etat, en attente de commande de sortie
                            etat = etatPrecedent;
                            // Maintien des consignes a zero
                            posConsOk = false;
                            capConsOk = false;
                            pousseeForceeOk = false;
                            orientationForceeOk = false;
                        }
                    }
                }
            }
        }
        break;
    case NAV_TARGET:
        if (!posOk || !magnetOk || frontMontantStop)
        {
            // Passage en mode STOP sur perte capteur ou commande Stop (priorite 1)
            etat = STOP;
            // Mise a zero des consignes
            posConsOk = false;
            capConsOk = false;
            pousseeForceeOk = false;
            orientationForceeOk = false;
        }
        else
        {
            if (!dlOk || frontMontantHome)
            {
                // Passage en mode HOME sur perte DataLink ou commande Home (priorite 2)
                etat = HOME;
                // Stockage de la position du Home dans la position de consigne
                xLatCons = xLatHome;
                yLonCons = yLonHome;
                posConsOk = true;
                capConsOk = false;
                pousseeForceeOk = false;
                orientationForceeOk = false;
            }
            else
            {
                if (frontMontantNavCap)
                {
                    // Passage en mode NAV_CAP (priorite 3)
                    etat = NAV_CAP;
                    // Stockage du cap courant de la bouee dans le cap de consigne
                    // et forcage des commandes des gaz et orientation a zero
                    posConsOk = false;
                    capCons = cap;
                    capConsOk = true;
                    pousseeForcee = 0.;
                    pousseeForceeOk = true;
                    orientationForcee = 0.;
                    orientationForceeOk = true;
                }
                else
                {
                    if (frontMontantNavBasic)
                    {
                        // Passage en mode NAV_BASIC sur commande NavBasic (priorite 4)
                        etat = NAV_BASIC;
                        // Forcage des commandes des gaz et orientation a zero
                        posConsOk = false;
                        capConsOk = false;
                        pousseeForcee = 0.;
                        pousseeForceeOk = true;
                        orientationForcee = 0.;
                        orientationForceeOk = true;
                    }
                    else
                    {
                        // Pas de transition d etat, en attente de commande de sortie
                        etat = etatPrecedent;
                        // Maintien de la validite de la position de consigne
                        posConsOk = true;
                        capConsOk = false;
                        pousseeForceeOk = false;
                        orientationForceeOk = false;
                    }
                }
            }
        }
        break;
    default:
        etat = WAIT_HOME;
    }
}
