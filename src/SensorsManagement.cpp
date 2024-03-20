#include "SensorsManagement.h"
#include "Arduino.h"
#include <Arduino_MKRGPS.h>
#include "Arduino_NineAxesMotion.h" //Contains the bridge code between the API and the Arduino Environment
#include <Wire.h>

// Variables pour la Gestion de l'IMU
NineAxesMotion mySensor;          // Object that for the sensor
unsigned long lastStreamTime = 0; // To store the last streamed time stamp
const int streamPeriod = 40;      // To stream at 25Hz without using additional timers (time period(ms) =1000/frequency(Hz))
bool updateSensorData = true;     // Flag to update the sensor data. Default is true to perform the first read before the first stream

//--- Constructeurs

SensorsManagement::SensorsManagement ()
{
  // Constructeur par defaut
  xLatGps_ = 0.;
  yLonGps_ = 0.;
  gpsOk_ = false;
  northSpeedGps_ = 0.;
  eastSpeedGps_ = 0.;
  speedGps_ = 0.;
  speedGpsOk_ = false;
  yawRate_ = 0.;
  imuOk_ = false;
  magneticHeading_ = 0.;
  magneticHeadingOk_ = false;
}

//--- Getters et setters

double SensorsManagement::getMyXLatGps()
{
    return xLatGps_;
}
void SensorsManagement::setMyXLatGps(double xLatGps)
{
     xLatGps_ = xLatGps;
}
double SensorsManagement::getMyYLonGps()
{
    return yLonGps_;
}
void SensorsManagement::setMyYLonGps(double yLonGps)
{
     yLonGps_ = yLonGps;
}
bool SensorsManagement::getMyGpsOk()
{
    return gpsOk_;
}
void SensorsManagement::setMyGpsOk(bool gpsOk)
{
     gpsOk_ = gpsOk;
}
float SensorsManagement::getMyNorthSpeedGps()
{
    return northSpeedGps_;
}
void SensorsManagement::setMyNorthSpeedGps(float northSpeedGps)
{
     northSpeedGps_ = northSpeedGps;
}
float SensorsManagement::getMyEastSpeedGps()
{
    return eastSpeedGps_;
}
void SensorsManagement::setMyEastSpeedGps(float eastSpeedGps)
{
     eastSpeedGps_ = eastSpeedGps;
}
bool SensorsManagement::getMySpeedGpsOk()
{
    return speedGpsOk_;
}
void SensorsManagement::setMySpeedGpsOk(bool speedGpsOk)
{
     speedGpsOk_ = speedGpsOk;
}
float SensorsManagement::getMyYawRate()
{
    return yawRate_;
}
void SensorsManagement::setMyYawRate(float yawRate)
{
     yawRate_ = yawRate;
}
bool SensorsManagement::getMyImuOk()
{
    return imuOk_;
}
void SensorsManagement::setMyImuOk(bool imuOk)
{
     imuOk_ = imuOk;
}
float SensorsManagement::getMyMagneticHeading()
{
    return magneticHeading_;
}
void SensorsManagement::setMyMagneticHeading(float magneticHeading)
{
     magneticHeading_ = magneticHeading;
}
bool SensorsManagement::getMyMagneticHeadingOk()
{
    return magneticHeadingOk_;
}
void SensorsManagement::setMyMagneticHeadingOk(bool magneticHeadingOk)
{
     magneticHeadingOk_ = magneticHeadingOk;
}

//--- Methodes fonctionnelles
// Setup de la gestion du dashboard
void SensorsManagement::setup()
{
  /* //Initialisation IMU
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
   */
}
void SensorsManagement::manageSensors()
{

  /*
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
  */
  /* Mise en commentaire le temps de faire fonctionner le GPS
  // put the GPS in standby mode
  //Serial.println("standby");
  //GPS.standby();

  // wait for 10 seconds
  //Serial.print("delay ");
  //for (int i = 0; i < 10; i++) {
  //  delay(1000);
  //  Serial.print(".");
  //}
  //Serial.println();

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

  // Gere les capteurs avant leur utilisation
  // Pour l instant, les validites sont forcees a vrai
  xLatGps_ = 0.;
  yLonGps_ = 0.;
  gpsOk_ = true;
  magneticHeading_ = 45.;
  magneticHeadingOk_ = true;
}