#ifndef NAVIGATIONMODE_H
#define NAVIGATIONMODE_H

#pragma once

// Variables internes de la gestion des modes
enum tEtats
{
    WAIT_HOME,
    HOME,
    HOLD,
    STOP,
    NAV_BASIC,
    NAV_CAP,
    NAV_TARGET
}; // Modes de navigation
enum tSousEtats
{
    RIEN,
    HOLD_POSITION,
    GO_TO_POSITION
};                                                           // Sous modes de navigation
const char16_t DELAI_RAFRAICHISSEMENT_DL = (char16_t)30000U; // duree maxi rafraichissement DL

class NavigationMode
{

public:
    //---Surveillance dataLinkReady
    static char32_t heureDL; // Heure dernier rafraichissement DL
    bool dlOk;               // Validite de DataLink

    //---Acquisition et surveillance capteurs
    // GPS
    double xLat; // Latitude de la bouee
    double yLon; // Longitude de la bouee
    bool posOk;  // Validite de la postion de la bouee
    // Boussole
    float cap;     // Cap magnetique de la bouee
    bool magnetOk; // Validite du cap magnetique de la bouee

    //---Gestion des modes de navigation
    // Commandes en provenance de la station sol
    static bool homeMemorisationComPrecedente; // Etat de la commande de memorisation du Home du cycle precedent
    static int navicationModePrecedent;        // Mode de navigation demande au cycle precedent
    bool frontMontantDefinirHome;              // Front montant de la commande definition du Home
    bool frontMontantHome;                     // Front montant de la commande Home
    bool frontMontantHold;                     // Front montant de la commande Hold
    bool frontMontantStop;                     // Front montant de la commande Stop
    bool frontMontantNavBasic;                 // Front montant de la commande Navigation Basique
    bool frontMontantNavCap;                   // Front montant de la commande Navigation en Cap
    bool frontMontantNavTarget;                // Front montant de la commande Navigation sur une position Target
    double xLatTarget;                         // Latitude demandee associee a la commande Navigation sur une position Target
    double yLonTarget;                         // Longitude demandee associee a la commande Navigation sur une position Target

    static tEtats etatPrecedent;         // Etat de l'automate du cycle précédent
    static tSousEtats sousEtatPrecedent; // Sous etat de l'automate du cycle précédent
    static char32_t dureeWaitHome;       // duree d activation du mode WAIT_HOME
    static char32_t dureeHome;           // duree d activation du mode HOME
    static char32_t dureeHold;           // duree d activation du mode HOLD
    static char32_t dureeStop;           // duree d activation du mode STOP
    static char32_t dureeNavBasic;       // duree d activation du mode NAV_BASIC
    static char32_t dureeNavCap;         // duree d activation du mode NAV_CAP
    static char32_t dureeNavTarget;      // duree d activation du mode NAV_TARGET

    // Sorties de la gestion des modes
    tEtats etat;                    // Mode de navigation en cours
    tSousEtats sousEtat;            // Sous mode de navigation en cours
    static double xLatHome;         // Latitude du Home
    static double yLonHome;         // Longitude du Home
    static bool posHomeOk;          // Validite de la position du Home
    static double xLatCons;         // Latitude de consigne
    static double yLonCons;         // Longitude de consigne
    bool posConsOk;                 // Validite de la consigne en position
    static float capCons;           // Cap de consigne
    bool capConsOk;                 // Validite de la consigne en cap
    static float pousseeForcee;     // manette des gaz forcee (resynchro de la commande)
    bool pousseeForceeOk;           // Validite du forcage de la manette des gaz
    static float orientationForcee; // cmdeorientation forcee (resynchro de la commande)
    bool orientationForceeOk;       // Validite du forcage de la commande d orientationForcee

    NavigationMode();
    ~NavigationMode();
    void calculateMode(int navigationMode);
    tEtats getEtat();
    void switchEtat(int navigationMode);

    

private:
    


};

#endif