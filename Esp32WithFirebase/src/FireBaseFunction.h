#include <FirebaseESP32.h>

#define WIFI_NAME "Mi phone"
#define WIFI_PASSWORD "0984647944"

#define FIREBASE_HOST "da2-songnganh-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "IHiS7PEKiGM7ZuQ2gjvwc6XItORKOtFQJK0duLz3"

extern bool SettingTempArea1, SettingHumidArea1, SettingTempArea2, SettingHumidArea2;
extern bool OnOffDeviceArea1, OnOffDeviceArea2;

extern int DataLight;

extern float DataSettingTempArea1,DataSettingTempArea2;
extern float DataSettingHumidArea1,DataSettingHumidArea2;

extern int IndexMessage;

extern int CheckAreaNum;

static FirebaseData Firebase_DATA;
static FirebaseData Firebase_DATA1;

static String ControllDeviceArea1, ControllDeviceArea2;

void setup_Firebase();
void streamCallback(MultiPathStreamData stream);
void streamTimeoutCallback(bool timeout);
void WiFi_Config();
void Firebase_Config();
void PWM_Config();

void update_Humid(int *pHumid);
