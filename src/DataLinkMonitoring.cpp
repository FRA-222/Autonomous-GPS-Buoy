#include "DataLinkMonitoring.h"
#include "Arduino.h"

//--- Constructeurs

DataLinkMonitoring::DataLinkMonitoring ()
{
    // Constructeur par defaut
    dataLinkOk_ = false;
    heureDL_ = 0.;
}

//--- Getters et Setters

bool DataLinkMonitoring::getMyDataLinkOk()
{
    return dataLinkOk_;
}

void DataLinkMonitoring::setMyDataLinkOk(bool dataLinkOk)
{
     dataLinkOk_ = dataLinkOk;
}

//--- Methodes fonctionnelles

void DataLinkMonitoring::monitorDataLink()
{
    // Surveillance a mettre en place
    // Pour l instant, la validite est forcee a vrai
    dataLinkOk_ = true;
}
