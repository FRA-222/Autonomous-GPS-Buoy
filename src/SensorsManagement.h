#ifndef SENSORSMANAGEMENT_H
#define SENSORSMANAGEMENT_H
#include "Arduino.h"

#pragma once

class SensorsManagement
{
    public:
    // Constructeurs
    SensorsManagement(); // Constructeur par defaut
    // Getters et Setters
    double getMyXLatGps();
    void setMyXLatGps(double);
    double getMyYLonGps();
    void setMyYLonGps(double);
    bool getMyGpsOk();
    void setMyGpsOk(bool);
    float getMyNorthSpeedGps();
    void setMyNorthSpeedGps(float);
    float getMyEastSpeedGps();
    void setMyEastSpeedGps(float);
    bool getMySpeedGpsOk();
    void setMySpeedGpsOk(bool);
    float getMyYawRate();
    void setMyYawRate(float);
    bool getMyImuOk();
    void setMyImuOk(bool);
    float getMyMagneticHeading();
    void setMyMagneticHeading(float);
    bool getMyMagneticHeadingOk();
    void setMyMagneticHeadingOk(bool);
    // Methodes fonctionnelles
    void setup();
    void manageSensors(); // Gere les capteurs avant leur utilisation

    private:
    // Attributs primaires (sorties)
    // GPS
    double xLatGps_;                // Latitude de la bouee
    double yLonGps_;                // Longitude de la bouee
    bool gpsOk_;                    // Validite de la postion de la bouee
    float northSpeedGps_;           // Vitesse Nord de la bouee
    float eastSpeedGps_;            // Vitesse Est de la bouee
    float speedGps_;                // Module de la vitesse de la bouee 
    bool speedGpsOk_;               // Validite des vitesses GPS
    // IMU
    float yawRate_;                 // Vitesse de lacet de la bouee
    bool imuOk_;                    // Validite des infos IMU
    // Boussole
    float magneticHeading_;         // Cap de la bouee
    bool magneticHeadingOk_;        // Validite du cap magnetique de la bouee
};

#endif