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
#include "thingProperties.h"
#include <Arduino_MKRGPS.h>
#include "Arduino_NineAxesMotion.h" //Contains the bridge code between the API and the Arduino Environment
#include <Wire.h>

//#include "Arduino_LED_Matrix.h"

//Définition des variables locales
//static int const LED_BUILTIN = 2;
//ArduinoLEDMatrix matrix;

//CloudLocation gpsHomeLocation;

// Définition de l'image n°1 pour afficher une flêche
byte picture1[8][12] = {

  { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0 },

  { 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0 },

  { 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0 },

  { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }

};

// Définition de l'image n°2
byte picture2[8][12] = {

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 

};

//Variables pour la gestion des deux Moteurs
int directionPinM1 = 12;
int pwmPinM1 = 3;
int brakePinM1 = 9;

bool directionStateM1;

int directionPinM2 = 13;
int pwmPinM2 = 11;
int brakePinM2 = 8;

bool directionStateM2;

float vitesseM1;
float vitesseM2;


//Variables pour la Gestion de l'IMU
NineAxesMotion mySensor;                 //Object that for the sensor
unsigned long lastStreamTime = 0;     //To store the last streamed time stamp
const int streamPeriod = 40;          //To stream at 25Hz without using additional timers (time period(ms) =1000/frequency(Hz))
bool updateSensorData = true;         //Flag to update the sensor data. Default is true to perform the first read before the first stream




void setup() {
  /* Initialize serial and wait up to 5 seconds for port to open */
  Serial.begin(9600);
  for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime > 5000); ) { }

  
  /* This function takes care of connecting your sketch variables to the ArduinoIoTCloud object */
  initProperties();

  /* Initialize Arduino IoT Cloud library */
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(DBG_INFO);
  ArduinoCloud.printDebugInfo();

  //Initialisation de la Matrice d'affichage
  //matrix.begin();
  
  //Initialisation temporaire des coordonnées GPS - A supprimer
  gpsBuoyLocation = Location(48.759539 , 2.429735);

  //Initialise le mode de Navigation à 0, le temps que l'état initial soit défini
  navigationMode = 1;


  // If you are using the MKR GPS as shield, change the next line to pass
  // the GPS_MODE_SHIELD parameter to the GPS.begin(...)
  if (!GPS.begin(GPS_MODE_SHIELD)) {
    Serial.println("Failed to initialize GPS!");
    while (1);
    gpsReady = false;
  }
  else  
    gpsReady = true;


  //Remplacer en cible par les test Bousole Magnétique, DataLink et batteryStatus
  dataLinkReady = true;
  magnetReady = false;
  batteryStatus = 45;

  //Met à 0 les commandes manuelles
  speed = 0;
  orientation = 0;
  
  //Initialise le CAP lorsque la bousole magnétique fonctionnera
  cap = 45;
  
  
  //Définit les PIN pour les moteurs A et B
  pinMode(directionPinM1, OUTPUT);
  pinMode(pwmPinM1, OUTPUT);
  pinMode(brakePinM1, OUTPUT);
  pinMode(directionPinM2, OUTPUT);
  pinMode(pwmPinM2, OUTPUT);
  pinMode(brakePinM2, OUTPUT);


  //Initialisation IMU
  Wire.begin();                    //Initialize I2C communication to the let the library communicate with the sensor.
  //Sensor Initialization
  mySensor.initSensor();          //The I2C Address can be changed here inside this function in the library
  mySensor.setOperationMode(OPERATION_MODE_NDOF);   //Can be configured to other operation modes as desired
  mySensor.setUpdateMode(MANUAL);	//The default is AUTO. Changing to manual requires calling the relevant update functions prior to calling the read functions
  //Setting to MANUAL requires lesser reads to the sensor
  mySensor.updateAccelConfig();
  updateSensorData = true;

  magnetReady = true;

  Serial.println();
  Serial.println("Default accelerometer configuration settings...");
  Serial.print("Range: ");
  Serial.println(mySensor.readAccelRange());
  Serial.print("Bandwidth: ");
  Serial.println(mySensor.readAccelBandwidth());
  Serial.print("Power Mode: ");
  Serial.println(mySensor.readAccelPowerMode());
  Serial.println("Streaming in ...");	//Countdown
  Serial.print("3...");
  delay(1000);	//Wait for a second
  Serial.print("2...");
  delay(1000);	//Wait for a second
  Serial.println("1...");
  delay(1000);	//Wait for a second

}

void loop() {

  ArduinoCloud.update();  //acquisition des commandes en provenance du Dashboard
  
  if (updateSensorData)  //Keep the updating of data as a separate task
  {
    mySensor.updateAccel();        //Update the Accelerometer data
    mySensor.updateLinearAccel();  //Update the Linear Acceleration data
    mySensor.updateGravAccel();    //Update the Gravity Acceleration data
    mySensor.updateCalibStatus();  //Update the Calibration Status
    updateSensorData = false;
  }
  if ((millis() - lastStreamTime) >= streamPeriod)
  {
    lastStreamTime = millis();

    Serial.print("Time: ");
    Serial.print(lastStreamTime);
    Serial.print("ms ");

    Serial.print("      aX: ");
    Serial.print(mySensor.readAccelerometer(X_AXIS)); //Accelerometer X-Axis data
    Serial.print("m/s2 ");

    Serial.print(" aY: ");
    Serial.print(mySensor.readAccelerometer(Y_AXIS));  //Accelerometer Y-Axis data
    Serial.print("m/s2 ");

    Serial.print(" aZ: ");
    Serial.print(mySensor.readAccelerometer(Z_AXIS));  //Accelerometer Z-Axis data
    Serial.print("m/s2 ");

    Serial.print("      lX: ");
    Serial.print(mySensor.readLinearAcceleration(X_AXIS)); //Linear Acceleration X-Axis data
    Serial.print("m/s2 ");

    Serial.print(" lY: ");
    Serial.print(mySensor.readLinearAcceleration(Y_AXIS));  //Linear Acceleration Y-Axis data
    Serial.print("m/s2 ");

    Serial.print(" lZ: ");
    Serial.print(mySensor.readLinearAcceleration(Z_AXIS));  //Linear Acceleration Z-Axis data
    Serial.print("m/s2 ");

    Serial.print("      gX: ");
    Serial.print(mySensor.readGravAcceleration(X_AXIS)); //Gravity Acceleration X-Axis data
    Serial.print("m/s2 ");

    Serial.print(" gY: ");
    Serial.print(mySensor.readGravAcceleration(Y_AXIS));  //Gravity Acceleration Y-Axis data
    Serial.print("m/s2 ");

    Serial.print(" gZ: ");
    Serial.print(mySensor.readGravAcceleration(Z_AXIS));  //Gravity Acceleration Z-Axis data
    Serial.print("m/s2 ");

    Serial.print("      C: ");
    Serial.print(mySensor.readAccelCalibStatus());  //Accelerometer Calibration Status (0 - 3)

    Serial.println();

    updateSensorData = true;
  }

  
//  Reception des commandes en provenance du Dashboard

//  Gestion des modes

  switch (navigationMode)
  {
  case 1 : //HOME
    /* code */
    break;
  
  case 2 : //HOLD
    /* code */
    break;

  case 3 : // STOP
    //activate breaks
    digitalWrite(brakePinM1, HIGH);
    digitalWrite(brakePinM2, HIGH);

    //set work duty for the motor to 0 (off)
    analogWrite(pwmPinM1, 0);
    analogWrite(pwmPinM2, 0);

    speed = 0;
    orientation = 0;

    break;

  case 4 : // NAV BASIC
    //Démarre en marche avant
    digitalWrite(directionPinM1, HIGH);
    digitalWrite(directionPinM2, HIGH);
    
    //release breaks
    digitalWrite(brakePinM1, LOW);
    digitalWrite(brakePinM2, LOW);

    //set work duty for the motor
    if (orientation > 40)
    {
      vitesseM1 = speed / orientation ;
      vitesseM2 = speed;
    }

    if (orientation < -40)
    {
      vitesseM1 = speed ;
      vitesseM2 = speed / abs(orientation);
    }

    if (orientation > -40 && orientation < 40 )
    {
      vitesseM1 = speed ;
      vitesseM2 = speed;
    }

    analogWrite(pwmPinM1, vitesseM1);
    analogWrite(pwmPinM2, vitesseM2);

    break;

  case 5 : // NAV CAP
    /* code */
    bvreak;

  case 6 : // NAV TARGET
    /* code */
    break;

  default:
    break;
  }



  /* Mise en commentaire le temps de faire fonctionner le GPS 
  // put the GPS in standby mode
  Serial.println("standby");
  GPS.standby();

  // wait for 10 seconds
  Serial.print("delay ");
  for (int i = 0; i < 10; i++) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();

  // wake up the GPS
  Serial.println("wakeup");
  GPS.wakeup();

  Serial.print("wait location ... ");

  // wait for new GPS data to become available
  unsigned long startMillis = millis();
  while (!GPS.available());
  unsigned long endMillis = millis();

  Serial.print(endMillis - startMillis);
  Serial.println(" ms");

  // read GPS values
  float latitude   = GPS.latitude();
  float longitude  = GPS.longitude();
  float altitude   = GPS.altitude();
  int   satellites = GPS.satellites();

  gpsBuoyLocation = Location(GPS.latitude() , GPS.longitude());

  // print GPS values
  Serial.println();
  Serial.print("Location: ");
  Serial.print(latitude, 7);
  Serial.print(", ");
  Serial.println(longitude, 7);

  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.println("m");

  Serial.print("Number of satellites: ");
  Serial.println(satellites);

  Serial.println();
  Fin du commentaire concernant le GPS */
}




void onHomeMemorisationCommandChange()  {
  
  if (homeMemorisationCommand)
  {
      Serial.println("onHomeMemorisationCommandChange : homeMemorisationCommand true");
      
  }    
  else
  {
      Serial.println("onHomeMemorisationCommandChange : homeMemorisationCommand false");
      
  }
}

void onNavigationModeChange()  {
  
    Serial.println("onNavigationModeChange : navigationMode : " + String(navigationMode) );
      
}

void onGpsBuoyLocationChange()  {
  
    //Serial.println("onGpsBuoyLocationChange : gpsBuoyLocation : " + gpsBuoyLocation.getValue().lat + " - " + gpsBuoyLocation.getValue().lon );
    Serial.println("onGpsBuoyLocationChange : gpsBuoyLocation : " + String(gpsBuoyLocation.getValue().lat) + " - " + String(gpsBuoyLocation.getValue().lon));
    
}

void onSpeedChange()  {
  
    Serial.println("onSpeedChange : speed : " + String(speed));
      
}

void onOrientationChange()  {
  
    Serial.println("onOrientationChange : orientation : " + String(orientation));
      
}

void onCapChange()  {
  
    Serial.println("onCapChange : cap : " + String(cap));
      
}


