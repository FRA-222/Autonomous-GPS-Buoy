#if !(defined(BOARD_HAS_WIFI) || defined(BOARD_HAS_GSM) || defined(BOARD_HAS_LORA) || \
      defined(BOARD_HAS_NB) || defined(BOARD_HAS_ETHERNET) || defined(BOARD_HAS_CATM1_NBIOT))
  #error "Please check Arduino IoT Cloud supported boards list: https://github.com/arduino-libraries/ArduinoIoTCloud/#what"
#endif

#if defined(BOARD_HAS_SECRET_KEY)
  #define BOARD_ID "c5f017ad-b97e-4469-97d2-d4e0cc446925"
#endif

void onDataLinkReadyChange();
void onGpsReadyChange();
void onMagnetReadyChange();
void onHomeMemorisationCommandChange();
void onBatteryStatusChange();
void onDashboardNavigationModeCdeChange();
void onGpsBuoyLocationChange();
void onSpeedChange();
void onOrientationChange();
void onCapChange();



//Variables liées au Dashboard de commande des Bouées
//Le Datalink est il opérationnel
bool dataLinkReady;
//Le GPS répond il ?
bool gpsReady;
//Le compas magnétique répond il ?
bool magnetReady;
//Commande pour mémoriser la position GPS Home
bool homeMemorisationCommand;
//Statut de charge de la batterie de la Bouée, entre 1 et 100%
int batteryStatus;
//Modes de Navigation décidée sur l'interface
// 1 - HOME
// 2 - HOLD
// 3 - STOP
// 4 - NAV BASIC
// 5 - NAV CAP
// 6 - NAV TARGET
int dashboardNavigationModeCde;
//Coordonnées GPS à conserver par la Bouée Autonome, dans le mode Hold
CloudLocation gpsBuoyLocation;
//Puissance pour les moteurs en mode manuel : -100 à +100
int speed;
//Orientation pour déplacer la Bouée : -100 à +100 (Gauche à Droite)
int orientation;
//Cap : 0 est le Nord - Valeurs entre 0 et 360 degrés
int cap;
//Etats de la Bouée qui remonte vers le Dashboard
bool dashboardEtatHome;
bool dashboardEtatHold;
bool dashboardEtatStop;
bool dashboardEtatNavBasic;
bool dashboardEtatNavCap;
bool dashboardEtatNavTarget;


void initProperties() {
#if defined(BOARD_HAS_SECRET_KEY)
  ArduinoCloud.setBoardId(BOARD_ID);
  ArduinoCloud.setSecretDeviceKey(SECRET_DEVICE_KEY);
#endif
#if defined(BOARD_HAS_WIFI) || defined(BOARD_HAS_GSM) || defined(BOARD_HAS_NB) || defined(BOARD_HAS_ETHERNET) || defined(BOARD_HAS_CATM1_NBIOT)
  ArduinoCloud.addProperty(cap, READWRITE, ON_CHANGE, onCapChange);
  ArduinoCloud.addProperty(dashboardNavigationModeCde, READWRITE, ON_CHANGE, onDashboardNavigationModeCdeChange);
  ArduinoCloud.addProperty(orientation, READWRITE, ON_CHANGE, onOrientationChange);
  ArduinoCloud.addProperty(speed, READWRITE, ON_CHANGE, onSpeedChange);
  ArduinoCloud.addProperty(gpsBuoyLocation, READWRITE, ON_CHANGE, onGpsBuoyLocationChange);
  ArduinoCloud.addProperty(dataLinkReady, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(gpsReady, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(homeMemorisationCommand, READWRITE, ON_CHANGE, onHomeMemorisationCommandChange);
  ArduinoCloud.addProperty(magnetReady, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(batteryStatus, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(dashboardEtatHome, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(dashboardEtatHold, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(dashboardEtatStop, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(dashboardEtatNavBasic, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(dashboardEtatNavCap, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(dashboardEtatNavTarget, READ, ON_CHANGE, NULL);
  
#elif defined(BOARD_HAS_LORA)
  ArduinoCloud.addProperty(led, 1, READWRITE, ON_CHANGE, onLedChange);
  ArduinoCloud.addProperty(potentiometer, 2, READ, ON_CHANGE);
  ArduinoCloud.addProperty(seconds, 3, READ, 5 * MINUTES);
#endif
}

#if defined(BOARD_HAS_ETHERNET)
  /* DHCP mode */
  //EthernetConnectionHandler ArduinoIoTPreferredConnection;
  /* Manual mode. It will fallback in DHCP mode if SECRET_OPTIONAL_IP is invalid or equal to "0.0.0.0" */
  EthernetConnectionHandler ArduinoIoTPreferredConnection(SECRET_OPTIONAL_IP, SECRET_OPTIONAL_DNS, SECRET_OPTIONAL_GATEWAY, SECRET_OPTIONAL_NETMASK);
#elif defined(BOARD_HAS_WIFI)
  WiFiConnectionHandler ArduinoIoTPreferredConnection(SECRET_SSID, SECRET_PASS);
#elif defined(BOARD_HAS_GSM)
  GSMConnectionHandler ArduinoIoTPreferredConnection(SECRET_PIN, SECRET_APN, SECRET_LOGIN, SECRET_PASS);
#elif defined(BOARD_HAS_LORA)
  LoRaConnectionHandler ArduinoIoTPreferredConnection(SECRET_APP_EUI, SECRET_APP_KEY, _lora_band::EU868, NULL, _lora_class::CLASS_A);
#elif defined(BOARD_HAS_NB)
  NBConnectionHandler ArduinoIoTPreferredConnection(SECRET_PIN, SECRET_APN, SECRET_LOGIN, SECRET_PASS);
#elif defined(BOARD_HAS_CATM1_NBIOT)
  CatM1ConnectionHandler ArduinoIoTPreferredConnection(SECRET_PIN, SECRET_APN, SECRET_LOGIN, SECRET_PASS);
#endif
