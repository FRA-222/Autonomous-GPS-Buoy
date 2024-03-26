#ifndef MODEMANAGEMENT_H 
#define MODEMANAGEMENT_H

#pragma once

// Modes globaux de la bouee
enum tEtatsGeneral
{
    INIT,
    READY,
    MAINTENANCE,
    HOME_DEFINITION, 
    NAV
}; 

// Modes de navigation de la bouee
enum tEtatsNav
{
    NAV_NOTHING, 
    NAV_HOME,    
    NAV_HOLD,
    NAV_STOP,
    NAV_BASIC,
    NAV_CAP,
    NAV_TARGET
}; 

class ModeManagement
{

public:

    // Constructeurs
    ModeManagement();
    ~ModeManagement();

    // Getters et Setters
    tEtatsGeneral getMyGeneralMode();
    tEtatsNav getMyNavigationMode();
    bool getMyFrontMontantNavHome();
    void setMyFrontMontantNavHome(bool);
    bool getMyFrontMontantNavHold();
    void setMyFrontMontantNavHold(bool);
    bool getMyFrontMontantNavStop();
    void setMyFrontMontantNavStop(bool );
    bool getMyFrontMontantNavBasic();
    void setMyFrontMontantNavBasic(bool);
    bool getMyFrontMontantNavCap();
    void setMyFrontMontantNavCap(bool);
    bool getMyFrontMontantNavTarget();
    void setMyFrontMontantNavTarget(bool);
    double getMyXLatHome();
    void setMyXLatHome(double);
    double getMyYLonHome();
    void setMyYLonHome(double);
    bool getMyPosHomeOk();
    void setMyPosHomeOk(bool);

    // Methodes fonctionnelles
    void calculateGeneralMode(
    bool, bool, double, double, bool, bool, bool, 
    bool, 
    double, double, bool,
    float, bool);
    void calculateNavigationMode(
        bool, bool, bool, bool, bool, bool, 
        bool, 
        double, double, bool,
        float, bool);

private:
    // Attributs primaires (sorties)
    char32_t dureeInit_;              // duree d activation du mode INIT
    char32_t dureeReady_;             // duree d activation du mode READY
    char32_t dureeMaintenance_;       // duree d activation du mode MAINTENANCE
    char32_t dureeHomeDefinition_;    // duree d activation du mode HOME_DEFINITION
    char32_t dureeNAV_;               // duree d activation du mode NAV
    char32_t dureeHome_;              // duree d activation du mode NAV_HOME
    char32_t dureeHold_;              // duree d activation du mode NAV_HOLD
    char32_t dureeStop_;              // duree d activation du mode NAV_STOP
    char32_t dureeNavBasic_;          // duree d activation du mode NAV_BASIC
    char32_t dureeNavCap_;            // duree d activation du mode NAV_CAP
    char32_t dureeNavTarget_;         // duree d activation du mode NAV_TARGET
    // Sorties
    tEtatsGeneral generalMode_;       // Mode general en cours
    bool frontMontantReady_;          // Front montant du mode READY
    bool frontMontantMaintenance_;    // Front montant du mode MAINTENANCE
    bool frontMontantHomeDefinition_; // Front montant du mode HOME_DEFINITION
    bool frontMontantNav_;            // Front montant du mode NAV
    tEtatsNav navigationMode_;        // Mode de navigation en cours
    bool frontMontantNavHome_;        // Front montant du mode NAV_HOME
    bool frontMontantNavHold_;        // Front montant du mode NAV_HOLD
    bool frontMontantNavStop_;        // Front montant du mode NAV_STOP
    bool frontMontantNavBasic_;       // Front montant du mode NAV_BASIC
    bool frontMontantNavCap_;         // Front montant du mode NAV_CAP
    bool frontMontantNavTarget_;      // Front montant du mode NAV_TARGET
    double xLatHome_;                 // Latitude du Home
    double yLonHome_;                 // Longitude du Home
    bool posHomeOk_;                  // Validite de la position du Home
};

#endif