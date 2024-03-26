#ifndef DATALINKMONITORING_H
#define DATALINKMONITORING_H
#include "Arduino.h"

#pragma once

const char16_t DELAI_RAFRAICHISSEMENT_DL = (char16_t)30000U; // duree maxi rafraichissement DL

class DataLinkMonitoring
{
    public:
    // Constructeurs
    DataLinkMonitoring(); // Constructeur par defaut 
    // Getters et Setters
    bool getMyDataLinkOk();
    void setMyDataLinkOk(bool);
    // Methodes fonctionnelles
    void monitorDataLink(); // Surveille la liaison montante du DataLink

    private:
    // Attributs primaires (sorties)
    bool dataLinkOk_;  // Validite du DataLink
    // Attributs internes
    char32_t heureDL_; // Heure dernier rafraichissement DL
};

#endif