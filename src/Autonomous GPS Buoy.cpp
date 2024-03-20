/*
  This sketch demonstrates how to exchange data between your board and the Arduino IoT Cloud.

  * Connect a potentiometer (or other analog sensor) to A0.
  * When the potentiometer (or sensor) value changes the data is sent to the Cloud.
  * When you flip the switch in the Cloud dashboard the onboard LED lights gets turned ON or OFF.

  IMPORTANT:
  This sketch works with WiFi, GSM, NB, Ethernet and Lora enabled boards supported by Arduino IoT Cloud.
  On a LoRa board, if it is configured as a class A device (default and preferred option), values from Cloud dashboard are received
  only after a value is sent to Cloud.

  The full list of compatible boards can be found here:
   - https://github.com/arduino-libraries/ArduinoIoTCloud#what
*/

#include "arduino_secrets.h"
#include <DataLinkMonitoring.h>
#include <SensorsManagement.h>
#include <ModeManagement.h>
#include <Navigation.h>
#include <AutoPilot.h>
#include <GcsManagement.h>

// #include "Arduino_LED_Matrix.h"

// Définition des variables locales
// static int const LED_BUILTIN = 2;
// ArduinoLEDMatrix matrix;

// CloudLocation gpsHomeLocation;

// Définition de l'image n°1 pour afficher une flêche
byte picture1[8][12] = {

    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},

    {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0},

    {0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0},

    {0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0},

    {0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0},

    {1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0},

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}

};

// Définition de l'image n°2
byte picture2[8][12] = {

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},

};

//------
// Creation statiques des objets de la bouee
//------

// objet pour la surveillance du datalink
DataLinkMonitoring dataLink;

// objet pour la gestion et surveillance des capteurs
SensorsManagement sensors;

// objet pour la gestion du dashboard
GcsManagement dashboard;

// objet pour la Gestion des modes de la bouee
ModeManagement mode;

// objet pour l ebaboration des consignes de navigation
Navigation navigation;

// objet pour le pilote automatique elaborant les ordres moteurs
AutoPilot autoPilot;

void setup()
{
  /* Initialize serial and wait up to 5 seconds for port to open */
  Serial.begin(9600);
  for (unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime > 5000);)
  {
  }

  // Initialisation de la Matrice d'affichage
  // matrix.begin();

  // Setup des sensors
  sensors.setup();

  // Setup du dashboard
  dashboard.setup();

  // setup de l autopilot
  autoPilot.setup();

}

void loop()
{

  // Surveillance du DataLink : la validite de DL est actuelment force a vrai
  dataLink.monitorDataLink();

  // Gestion et surveillance des capteurs : les validites capteur sont actuellement forcees a vrai
  sensors.manageSensors();

  // Adapatation des commandes du dashboard
  dashboard.adaptCommand();

  // Elaboration des modes generaux de la bouee
  mode.calculateGeneralMode(
      dashboard.getMyFrontMontantHomeMemoCmde(),        
      dashboard.getMyFrontMontantPosHomeCmde(),    
      dashboard.getMyXLatHomeCmde(),                         
      dashboard.getMyYLonHomeCmde(),  
      dashboard.getMyFrontMontantMaintenanceCmde(),           
      dashboard.getMyFrontMontantResumeReadyCmde(),          
      dashboard.getMyFrontMontantHomeValidationCmde(),
      dataLink.getMyDataLinkOk(), 
      sensors.getMyXLatGps(), sensors.getMyYLonGps(), sensors.getMyGpsOk(), 
      sensors.getMyMagneticHeading(), sensors.getMyMagneticHeadingOk());

  // Elaboration des modes de navigation de la bouee
  mode.calculateNavigationMode(
      dashboard.getMyFrontMontantNavHomeCmde(),
      dashboard.getMyFrontMontantNavHoldCmde(),
      dashboard.getMyFrontMontantNavStopCmde(),
      dashboard.getMyFrontMontantNavBasicCmde(),
      dashboard.getMyFrontMontantNavCapCmde(),
      dashboard.getMyFrontMontantNavTargetCmde(),
      dataLink.getMyDataLinkOk(), 
      sensors.getMyXLatGps(), sensors.getMyYLonGps(), sensors.getMyGpsOk(), 
      sensors.getMyMagneticHeading(), sensors.getMyMagneticHeadingOk());

  // Elaboration des consignes de navigation de la bouee
  navigation.calculateNavTarget(
      mode.getMyNavigationMode(),
      mode.getMyFrontMontantNavHome(),
      mode.getMyFrontMontantNavHold(),
      mode.getMyFrontMontantNavStop(),
      mode.getMyFrontMontantNavBasic(),
      mode.getMyFrontMontantNavCap(),
      mode.getMyFrontMontantNavTarget(),
      dashboard.getMyXLatTargetCmde(),
      dashboard.getMyYLonTargetCmde(),
      mode.getMyXLatHome(),
      mode.getMyYLonHome(),
      mode.getMyPosHomeOk(),
      sensors.getMyXLatGps(), sensors.getMyYLonGps(), sensors.getMyGpsOk(), 
      sensors.getMyMagneticHeading(), sensors.getMyMagneticHeadingOk());

  // Elaboration des commandes moteur
  autoPilot.calculerCommandesMoteur(mode.getMyNavigationMode(), 
  dashboard.getMyThrottleCmde(), dashboard.getMyThrottleCmde(), dashboard.getMyTrueHeadingCmde(),
  sensors.getMyYawRate(), sensors.getMyImuOk());

  // Resynchro des commandes et etats du dashboard
  dashboard.synchronizeCommand(
      dataLink.getMyDataLinkOk(),
      sensors.getMyGpsOk(), sensors.getMyMagneticHeadingOk(), 
      mode.getMyGeneralMode(), mode.getMyNavigationMode(),
      navigation.getMyLocationCons(), navigation.getMyXLatCons(), navigation.getMyYLonCons(), navigation.getMyPosConsOk(),
      autoPilot.getMyThrottle(), autoPilot.getMyRudder(), autoPilot.getMyTrueHeading());
  
}
