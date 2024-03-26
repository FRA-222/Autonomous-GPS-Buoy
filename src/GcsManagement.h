#ifndef CCSMANAGEMENT_H
#define CCSMANAGEMENT_H

#pragma once

class GcsManagement
{
public:

    // Constructeurs
    GcsManagement();
    ~GcsManagement();

    // Getters et Setters
    bool getMyFrontMontantPosHomeCmde();
    void setMyFrontMontantPosHomeCmde(bool);
    double getMyXLatHomeCmde();
    void setMyXLatHomeCmde(double);
    double getMyYLonHomeCmde();
    void setMyYLonHomeCmde(double);
    bool getMyFrontMontantHomeMemoCmde();
    void setMyFrontMontantHomeMemoCmde(bool);
    bool getMyFrontMontantMaintenanceCmde();
    void setMyFrontMontantMaintenanceCmde(bool);
    bool getMyFrontMontantResumeReadyCmde();
    void setMyFrontMontantResumeReadyCmde(bool);
    bool getMyFrontMontantHomeValidationCmde();
    void setMyFrontMontantHomeValidationCmde(bool);
    bool getMyFrontMontantNavHomeCmde();
    void setMyFrontMontantNavHomeCmde(bool);
    bool getMyFrontMontantNavHoldCmde();
    void setMyFrontMontantNavHoldCmde(bool);
    bool getMyFrontMontantNavStopCmde();
    void setMyFrontMontantNavStopCmde(bool );
    bool getMyFrontMontantNavBasicCmde();
    void setMyFrontMontantNavBasicCmde(bool);
    bool getMyFrontMontantNavCapCmde();
    void setMyFrontMontantNavCapCmde(bool);
    bool getMyFrontMontantNavTargetCmde();
    void setMyFrontMontantNavTargetCmde(bool);
    double getMyXLatTargetCmde();
    void setMyXLatTargetCmde(double);
    double getMyYLonTargetCmde();
    void setMyYLonTargetCmde(double);
    float getMyThrottleCmde();
    void setMyThrottleCmde(float);
    float getMyRudderCmde();
    void setMyRudderCmde(float);
    float getMyTrueHeadingCmde();
    void setMyTrueHeadingCmde(float);
    // Methodes fonctionnelles
    void setup();
    void adaptCommand();
    void synchronizeCommand(bool, bool, bool, 
        tEtatsGeneral, tEtatsNav, 
        tEtatsLocationCons, double, double, bool,
        float, float, float);
    void onHomeMemorisationCommandChange();
    void onDashboardNavigationModeCdeChange();
    void onGpsBuoyLocationChange();
    void onSpeedChange();
    void onOrientationChange();
    void onCapChange();

private:
    bool pastPosHomeCommand_;                  // Etat de la commande de position du Home au cycle precedent
    bool pastMaintenanceCommand_;              // Etat de la commande de passage en maintenance au cycle precedent
    bool pastResumeReadyCommand_;              // Etat de la commande de retour en mode operationnel au cycle precedent
    bool pastHomeMemorisationCommand_;         // Etat de la commande de memorisation du Home du cycle precedent
    bool pastHomeValidationCommand_;           // Etat de la commande de validation du Home au cycle precedent
    int pastDashboardNavigationModeCde_;       // Mode de navigation demande au cycle precedent

    // Sorties
    bool frontMontantPosHomeCmde_;             // Front montant de la commande de position du Home
    double xLatHomeCmde_;                      // Latitude demandee associee a la commande de position du Home
    double yLonHomeCmde_;                      // Longitude demandee associee a la commande de position du Home
    bool frontMontantHomeMemoCmde_;            // Front montant de la commande de memorisation du Home
    bool frontMontantMaintenanceCmde_;         // Front montant de la commande de passage en maintenance  
    bool frontMontantResumeReadyCmde_;         // Front montant de la commande de retour en mode operationnel 
    bool frontMontantHomeValidationCmde_;      // Front montant de la commande de validation du Home (pour passer en Nav)
    bool frontMontantNavHomeCmde_;             // Front montant de la commande Navigation Home
    bool frontMontantNavHoldCmde_;             // Front montant de la commande Navigation Hold
    bool frontMontantNavStopCmde_;             // Front montant de la commande Navigation Stop
    bool frontMontantNavBasicCmde_;            // Front montant de la commande Navigation Basique
    bool frontMontantNavCapCmde_;              // Front montant de la commande Navigation en Cap
    bool frontMontantNavTargetCmde_;           // Front montant de la commande Navigation sur une position Target
    double xLatTargetCmde_;                    // Latitude demandee associee a la commande Navigation sur une position Target
    double yLonTargetCmde_;                    // Longitude demandee associee a la commande Navigation sur une position Target
    float throttleCmde_;                       // Commande nanuelle de la vitesse de la bouee (manette des gaz)
    float rudderCmde_;                         // Commande manuelle de la direction de la bouee (gouvernail)
    float trueHeadingCmde_;                    // Commande manuelle de cap dans le mode NAV_CAP

};

#endif