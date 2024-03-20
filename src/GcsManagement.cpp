#include "ModeManagement.h"
#include "Navigation.h"
#include "GcsManagement.h"
#include "arduino_secrets.h"
#include "thingProperties.h"

//--- Constructeurs

// Constructeur par defaut
GcsManagement::GcsManagement()
{
    pastPosHomeCommand_ = false;
    pastMaintenanceCommand_ = false;
    pastResumeReadyCommand_ = false;
    pastHomeMemorisationCommand_ = false;
    pastHomeValidationCommand_ = false;
    pastDashboardNavigationModeCde_ = false;

    // Sorties
    frontMontantPosHomeCmde_ = false;
    xLatHomeCmde_ = 0.;
    yLonHomeCmde_ = 0.;
    frontMontantHomeMemoCmde_ = false;
    frontMontantMaintenanceCmde_ = false;
    frontMontantResumeReadyCmde_ = false;
    frontMontantHomeValidationCmde_ = false;
    frontMontantNavHomeCmde_ = false;
    frontMontantNavHoldCmde_ = false;
    frontMontantNavStopCmde_ = false;
    frontMontantNavBasicCmde_ = false;
    frontMontantNavCapCmde_ = false;
    frontMontantNavTargetCmde_ = false;
    xLatTargetCmde_ = 0.;
    yLonTargetCmde_ = 0.;
    throttleCmde_ = 0.;
    rudderCmde_ = 0.;
    trueHeadingCmde_ = 0.;
}

GcsManagement::~GcsManagement()
{

}

//--- Getters et setters
double GcsManagement::getMyXLatHomeCmde()
{
    return xLatHomeCmde_;
}
void GcsManagement::setMyXLatHomeCmde(double xLatHomeCmde)
{
     xLatHomeCmde_ = xLatHomeCmde;
}
double GcsManagement::getMyYLonHomeCmde()
{
    return yLonHomeCmde_;
}
void GcsManagement::setMyYLonHomeCmde(double yLonHomeCmde)
{
     yLonHomeCmde_ = yLonHomeCmde;
}
bool GcsManagement::getMyFrontMontantPosHomeCmde()
{
    return frontMontantPosHomeCmde_;
}
void GcsManagement::setMyFrontMontantPosHomeCmde(bool frontMontantPosHomeCmde)
{
     frontMontantPosHomeCmde_ = frontMontantPosHomeCmde;
}
bool GcsManagement::getMyFrontMontantHomeMemoCmde()
{
    return frontMontantHomeMemoCmde_;
}
void GcsManagement::setMyFrontMontantHomeMemoCmde(bool frontMontantHomeMemoCmde)
{
     frontMontantHomeMemoCmde_ = frontMontantHomeMemoCmde;
}
bool GcsManagement::getMyFrontMontantMaintenanceCmde()
{
    return frontMontantMaintenanceCmde_;
}
void GcsManagement::setMyFrontMontantMaintenanceCmde(bool frontMontantMaintenanceCmde)
{
     frontMontantMaintenanceCmde_ = frontMontantMaintenanceCmde;
}
bool GcsManagement::getMyFrontMontantResumeReadyCmde()
{
    return frontMontantResumeReadyCmde_;
}
void GcsManagement::setMyFrontMontantResumeReadyCmde(bool frontMontantResumeReadyCmde)
{
     frontMontantResumeReadyCmde_ = frontMontantResumeReadyCmde;
}
bool GcsManagement::getMyFrontMontantHomeValidationCmde()
{
    return frontMontantHomeValidationCmde_;
}
void GcsManagement::setMyFrontMontantHomeValidationCmde(bool frontMontantHomeValidationCmde)
{
     frontMontantHomeValidationCmde_ = frontMontantHomeValidationCmde;
}
bool GcsManagement::getMyFrontMontantNavHomeCmde()
{
    return frontMontantNavHomeCmde_;
}
void GcsManagement::setMyFrontMontantNavHomeCmde(bool frontMontantNavHomeCmde)
{
     frontMontantNavHomeCmde_ = frontMontantNavHomeCmde;
}
bool GcsManagement::getMyFrontMontantNavHoldCmde()
{
    return frontMontantNavHoldCmde_;
}
void GcsManagement::setMyFrontMontantNavHoldCmde(bool frontMontantNavHoldCmde)
{
     frontMontantNavHoldCmde_ = frontMontantNavHoldCmde;
}
bool GcsManagement::getMyFrontMontantNavStopCmde()
{
    return frontMontantNavStopCmde_;
}
void GcsManagement::setMyFrontMontantNavStopCmde(bool frontMontantNavStopCmde)
{
     frontMontantNavStopCmde_ = frontMontantNavStopCmde;
}
bool GcsManagement::getMyFrontMontantNavBasicCmde()
{
    return frontMontantNavBasicCmde_;
}
void GcsManagement::setMyFrontMontantNavBasicCmde(bool frontMontantNavBasicCmde)
{
     frontMontantNavBasicCmde_ = frontMontantNavBasicCmde;
}
bool GcsManagement::getMyFrontMontantNavCapCmde()
{
    return frontMontantNavCapCmde_;
}
void GcsManagement::setMyFrontMontantNavCapCmde(bool frontMontantNavCapCmde)
{
     frontMontantNavCapCmde_ = frontMontantNavCapCmde;
}
bool GcsManagement::getMyFrontMontantNavTargetCmde()
{
    return frontMontantNavTargetCmde_;
}
void GcsManagement::setMyFrontMontantNavTargetCmde(bool frontMontantNavTargetCmde)
{
     frontMontantNavTargetCmde_ = frontMontantNavTargetCmde;
}
double GcsManagement::getMyXLatTargetCmde()
{
    return xLatTargetCmde_;
}
void GcsManagement::setMyXLatTargetCmde(double xLatTargetCmde)
{
     xLatTargetCmde_ = xLatTargetCmde;
}
double GcsManagement::getMyYLonTargetCmde()
{
    return yLonTargetCmde_;
}
void GcsManagement::setMyYLonTargetCmde(double yLonTargetCmde)
{
     yLonTargetCmde_ = yLonTargetCmde;
}
float GcsManagement::getMyThrottleCmde()
{
    return throttleCmde_;
}
void GcsManagement::setMyThrottleCmde(float throttleCmde)
{
     throttleCmde_ = throttleCmde;
}
float GcsManagement::getMyRudderCmde()
{
    return rudderCmde_;
}
void GcsManagement::setMyRudderCmde(float rudderCmde)
{
     rudderCmde_ = rudderCmde;
}
float GcsManagement::getMyTrueHeadingCmde()
{
    return trueHeadingCmde_;
}
void GcsManagement::setMyTrueHeadingCmde(float trueHeadingCmde)
{
     trueHeadingCmde_ = trueHeadingCmde;
}


// Setup de la gestion du dashboard
void GcsManagement::setup()
{
    /* This function takes care of connecting your sketch variables to the ArduinoIoTCloud object */
    initProperties();

    /* Initialize Arduino IoT Cloud library */
    ArduinoCloud.begin(ArduinoIoTPreferredConnection);

    setDebugMessageLevel(DBG_INFO);
    ArduinoCloud.printDebugInfo();

    /*// If you are using the MKR GPS as shield, change the next line to pass
    // the GPS_MODE_SHIELD parameter to the GPS.begin(...)
    if (!GPS.begin(GPS_MODE_UART))
    {
        Serial.println("Failed to initialize GPS!");
        while (1);
        gpsReady = false;
    }
    else
    {
        gpsReady = true;
    }*/

    gpsReady = true;

    // Initialisation temporaire des coordonnées GPS - A supprimer
    gpsBuoyLocation = Location(48.759539, 2.429735);

    // Initialise le mode de Navigation à 0, le temps que l'état initial soit défini
    dashboardNavigationModeCde = 1;

    // Remplacer en cible par les test Bousole Magnétique, DataLink et batteryStatus
    batteryStatus = 45;

    // Met à 0 les commandes manuelles
    speed = 0;
    orientation = 0;

    // Initialise le CAP lorsque la bousole magnétique fonctionnera
    cap = 45;

}

// Elaboration des commandes attendues par la gestion des modes
// a partir des commandes du dashboard
void GcsManagement::adaptCommand()
{
      ArduinoCloud.update();
    //---Lecture des entrees de l automate

/*
    frontMontantPosHomeCmde_ = posHomeCommand && !pastPosHomeCommand_;
    if (frontMontantPosHomeCmde_)
    {
        xLatHomeCmde_ = xLatHomeCommand;
        yLonHomeCmde_ = yLonHomeCommand;
    }
    frontMontantMaintenanceCmde_ = maintenanceCommand && !pastMaintenanceCommand_;
    frontMontantResumeReadyCmde_ = resumeReadyCommand && !pastResumeReadyCommand_;
    frontMontantHomeMemoCmde_ = homeMemorisationCommand && !pastHomeMemorisationCommand_;
    frontMontantHomeValidationCmde_ = homeValidationCommand && !pastHomeValidationCommand_;
    frontMontantPosHomeCmde_ = posHomeCommand && !pastPosHomeCommand_;
    if (frontMontantPosHomeCmde_)
    {
        xLatHomeCmde_ = xLatHomeCommand;
        yLonHomeCmde_ = yLonHomeCommand;
    }
*/
    // A remplacer par les lignes de commentaire au dessus
    frontMontantPosHomeCmde_ = false;
    frontMontantMaintenanceCmde_ = false;
    frontMontantResumeReadyCmde_ = false;
    frontMontantHomeMemoCmde_ = homeMemorisationCommand && !pastHomeMemorisationCommand_;
    if (pastHomeMemorisationCommand_ && !pastHomeMemorisationCommand_)
    {
        //simulation d une commande de validation du Home apres l avoir memorise
        frontMontantHomeValidationCmde_ = true;
    }
    frontMontantPosHomeCmde_ = false;

    frontMontantNavHomeCmde_ = false;
    if (dashboardNavigationModeCde != pastDashboardNavigationModeCde_ && dashboardNavigationModeCde == 1)
    {
        frontMontantNavHomeCmde_ = true;
    }
    frontMontantNavHoldCmde_ = false;
    if (dashboardNavigationModeCde != pastDashboardNavigationModeCde_ && dashboardNavigationModeCde == 2)
    {
        frontMontantNavHoldCmde_ = true;
    }
    frontMontantNavStopCmde_ = false;
    if (dashboardNavigationModeCde != pastDashboardNavigationModeCde_ && dashboardNavigationModeCde == 3)
    {
        frontMontantNavStopCmde_ = true;
    }
    frontMontantNavBasicCmde_ = false;
    if (dashboardNavigationModeCde != pastDashboardNavigationModeCde_ && dashboardNavigationModeCde == 4)
    {
        frontMontantNavBasicCmde_ = true;
    }
    frontMontantNavCapCmde_ = false;
    if (dashboardNavigationModeCde != pastDashboardNavigationModeCde_ && dashboardNavigationModeCde == 5)
    {
        frontMontantNavCapCmde_ = true;
    }
    frontMontantNavTargetCmde_ = false;
    if (dashboardNavigationModeCde != pastDashboardNavigationModeCde_ && dashboardNavigationModeCde == 6)
    {
        frontMontantNavTargetCmde_ = true;
    }
    /*
    if (frontMontantNavTargetCmde_)
    {
        xLatTargetCmde_ = xLatTargetCommand;
        yLonTargetCmde_ = yLonTargetCommand;
    }
    */
       // A remplacer par les lignes de commentaire au dessus
        xLatTargetCmde_ = 48.893524;
        yLonTargetCmde_ = 2.167674;

    throttleCmde_ = speed;
    rudderCmde_ = orientation;
    trueHeadingCmde_ = cap;

}

// Synchronisation des commandes et etats du dashboard
// et memorisation de l etat des commandes pour le cycle suivant
void GcsManagement::synchronizeCommand(bool dataLinkOk, bool gpsOk, bool magneticHeadingOk,
    tEtatsGeneral generalMode, tEtatsNav navigationMode, 
    tEtatsLocationCons locationCons, double xLatCons, double yLonCons, bool posConsOk, 
    float throttle, float rudder, float trueHeading)
{
    // Indique les états des capteurs au dashboard
    dataLinkReady = false;
    if (dataLinkOk) dataLinkReady = true;
    gpsReady = false;
    if (gpsOk) gpsReady = true;
    magnetReady = false;
    if (magneticHeadingOk) magnetReady = true;

    // Recopie de mode general de la bouee

    //Fait tomber la commande du dashboard apres prise en compte 
    //dans la gestion des modes de navigation
    homeMemorisationCommand = false;

    switch (navigationMode)
    {
    case NAV_NOTHING:
        Serial.println("WAIT_HOME ");

        //Remonte l'état au dashboard
        dashboardNavigationModeCde = NAV_NOTHING;
        dashboardEtatHome = true;
        dashboardEtatHold = false;
        dashboardEtatStop = false;
        dashboardEtatNavBasic = false;
        dashboardEtatNavCap = false;
        dashboardEtatNavTarget = false;
        break;

    case NAV_HOME:
        Serial.println("NAV_HOME");

        //Remonte l'état au dashboard
        dashboardNavigationModeCde = NAV_HOME;
        dashboardEtatHome = true;
        dashboardEtatHold = false;
        dashboardEtatStop = false;
        dashboardEtatNavBasic = false;
        dashboardEtatNavCap = false;
        dashboardEtatNavTarget = false;
        break;

    case NAV_HOLD:
        Serial.println("NAV_HOLD");

        //Remonte l'état au dashboard
        dashboardNavigationModeCde = NAV_HOLD;
        dashboardEtatHome = false;
        dashboardEtatHold = true;
        dashboardEtatStop = false;
        dashboardEtatNavBasic = false;
        dashboardEtatNavCap = false;
        dashboardEtatNavTarget = false;
        break;

    case NAV_STOP:
        Serial.println("NAV_STOP");

        //Remonte l'état au dashboard
        dashboardNavigationModeCde = NAV_STOP;
        dashboardEtatHome = false;
        dashboardEtatHold = false;
        dashboardEtatStop = true;
        dashboardEtatNavBasic = false;
        dashboardEtatNavCap = false;
        dashboardEtatNavTarget = false;

        break;

    case NAV_BASIC:
        Serial.println("NAV_BASIC");
        
        //Remonte l'état au dashboard
        dashboardNavigationModeCde = NAV_BASIC;
        dashboardEtatHome = false;
        dashboardEtatHold = false;
        dashboardEtatStop = false;
        dashboardEtatNavBasic = true;
        dashboardEtatNavCap = false;
        dashboardEtatNavTarget = false;
        break;

    case NAV_CAP:
        Serial.println("NAV_CAP");

        //Remonte l'état au dashboard
        dashboardNavigationModeCde = NAV_CAP;
        dashboardEtatHome = false;
        dashboardEtatHold = false;
        dashboardEtatStop = false;
        dashboardEtatNavBasic = false;
        dashboardEtatNavCap = true;
        dashboardEtatNavTarget = false;
        break;

    case NAV_TARGET:
        Serial.println("NAV_TARGET");

        //Remonte l'état au dashboard
        dashboardNavigationModeCde = NAV_TARGET;
        dashboardEtatHome = false;
        dashboardEtatHold = false;
        dashboardEtatStop = false;
        dashboardEtatNavBasic = false;
        dashboardEtatNavCap = false;
        dashboardEtatNavTarget = true;
        break;

    default:
        break;
    }
    // Recopie des consignes de navigation


    // Resynchro des commandes manuelles
    speed = throttle;
    orientation = rudder;
    cap = trueHeading;

    // Memorisation des commandes apres resynchro du dashboard
    pastHomeMemorisationCommand_ = homeMemorisationCommand;
    pastDashboardNavigationModeCde_ = dashboardNavigationModeCde;
}
void onHomeMemorisationCommandChange()
{
  if (homeMemorisationCommand)
  {
    Serial.println("onHomeMemorisationCommandChange : homeMemorisationCommand true");
  }
  else
  {
    Serial.println("onHomeMemorisationCommandChange : homeMemorisationCommand false");
  }
}

void onDashboardNavigationModeCdeChange()
{

  Serial.println("onNavigationModeChange : navigationMode : " + String(dashboardNavigationModeCde));
}

void onGpsBuoyLocationChange()
{

  // Serial.println("onGpsBuoyLocationChange : gpsBuoyLocation : " + gpsBuoyLocation.getValue().lat + " - " + gpsBuoyLocation.getValue().lon );
  Serial.println("onGpsBuoyLocationChange : gpsBuoyLocation : " + String(gpsBuoyLocation.getValue().lat) + " - " + String(gpsBuoyLocation.getValue().lon));
}

void onSpeedChange()
{

  Serial.println("onSpeedChange : speed : " + String(speed));
}

void onOrientationChange()
{

  Serial.println("onOrientationChange : orientation : " + String(orientation));
}

void onCapChange()
{

  Serial.println("onCapChange : cap : " + String(cap));
}
