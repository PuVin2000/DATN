#include "Arduino.h"

#define SLAVE_ADDR 0x08
// extern double DataControllDeviceArea1,DataControllDeviceArea2;

#define LE1 15
#define LE2 23
#define LE3 25

#define D0 19
#define D1 18
#define D2 5
#define D3 17

#define D4 16
#define D5 4
#define D6 0
#define D7 2

#define FORWARD_SUNNET1 34
#define RESERVED_SUNNET1 35

#define FORWARD_SUNNET2 32
#define RESERVED_SUNNET2 33

extern float CompareTempValueArea1, CompareHumidValueArea1;
extern float CompareTempValueArea2, CompareHumidValueArea2;

extern String DataControllDeviceArea1, DataControllDeviceArea2;

extern bool CheckSunNetArea1, CheckSunNetArea2;

const String buttonONDevice[][7] = {{"0"}, {"KHUVUC1_MAYBOM_ON", "KHUVUC1_QUATTHONGGIO1_ON", "KHUVUC1_QUATTHONGGIO2_ON", "KHUVUC1_QUATTHONGGIO3_ON", "KHUVUC1_QUATTHONGGIO4_ON", "KHUVUC1_QUATTHONGGIO5_ON", "KHUVUC1_QUATTHONGGIO6_ON"}, {"KHUVUC2_MAYBOM_ON", "KHUVUC2_QUATTHONGGIO1_ON", "KHUVUC2_QUATTHONGGIO2_ON", "KHUVUC2_QUATTHONGGIO3_ON", "KHUVUC2_QUATTHONGGIO4_ON", "KHUVUC2_QUATTHONGGIO5_ON", "KHUVUC2_QUATTHONGGIO6_ON"}};

const String buttonOFFDevice[][7] = {{"0"}, {"KHUVUC1_MAYBOM_OFF", "KHUVUC1_QUATTHONGGIO1_OFF", "KHUVUC1_QUATTHONGGIO2_OFF", "KHUVUC1_QUATTHONGGIO3_OFF", "KHUVUC1_QUATTHONGGIO4_OFF", "KHUVUC1_QUATTHONGGIO5_OFF", "KHUVUC1_QUATTHONGGIO6_OFF"}, {"KHUVUC2_MAYBOM_OFF", "KHUVUC2_QUATTHONGGIO1_OFF", "KHUVUC2_QUATTHONGGIO2_OFF", "KHUVUC2_QUATTHONGGIO3_OFF", "KHUVUC2_QUATTHONGGIO4_OFF", "KHUVUC2_QUATTHONGGIO5_OFF", "KHUVUC2_QUATTHONGGIO6_OFF"}};

const String buttonAUTODevice[][3] = {{"0"}, {"0", "KHUVUC1_MAYBOM_AUTO", "KHUVUC2_MAYBOM_AUTO"}, {"0", "KHUVUC1_LUOICHANNANG_AUTO", "KHUVUC2_LUOICHANNANG_AUTO"}};

const String OpenSunNetArea[][1] = {{"0"}, {"KHUVUC1_LUOICHANNANG_OPEN"}, {"KHUVUC2_LUOICHANNANG_OPEN"}};
const String CloseSunNetArea[][1] = {{"0"}, {"KHUVUC1_LUOICHANNANG_CLOSE"}, {"KHUVUC2_LUOICHANNANG_CLOSE"}};

const String PathSunNetFireBase[][1] = {{"0"}, {"/IOT/Area1/Getting/SunNet"}, {"/IOT/Area2/Getting/SunNet"}};

extern float TestTempArea1, TestTempArea2, TestHumidArea1, TestHumidArea2;

const int PinWrtie[] = {D0, D1, D2, D3, D4, D5, D6, D7};
const int LEWrtie[] = {0, LE1, LE2, LE3};

const int SunNetStatusControll[][2] = {{0},
                                       {FORWARD_SUNNET1, RESERVED_SUNNET1},
                                       {FORWARD_SUNNET2, RESERVED_SUNNET2}};

static bool StatusPinWriteArea[][8] = {{},
                                       {0, 0, 0, 0, 0, 0, 0, 0},
                                       {0, 0, 0, 0, 0, 0, 0, 0},
                                       {0, 0, 0, 0, 0, 0, 0, 0}};

extern bool StatusAutoMayBom1, StatusAutoMayBom2, StatusAutoSunNet1, StatusAutoSunNet2;

extern int CheckStatusSunNetToFireBase;

// OnOffManual
void OnDevicesManual(String DataControll, int AreaNum);
void OffDevicesManual(String DataControll, int AreaNum);

void ButtonOnDevices(int LEindex, int PinControll);
void ButtonOffDevices(int LEindex, int PinControll);

// ControllAuto
extern bool CheckStatusBomNuoc, CheckStatusSunNet;

extern bool CheckStatusButtonCloseSunNet, CheckStatusButtonOpenSunNet;
void AutoDevice(String DataControll, int AreaNum, float TempArea, float HumidArea);

// ControllSunNet
void ControllManualSunNet(String DataControll, int SunNetArea);

void ButtonStopSunNet(int SunNetArea);
void ButtonCloseSunNet(String DataControll, int AreaNum);
void ButtonOpenSunNet(String DataControll, int AreaNum);

void I2CData();
String getValue(String data, char separator, int index);

void SetupButton(String DataControll, int AreaNum);

void StatusSunNetIsCloseOrOpen(int AreaNum);

void ReadStatusSunNet(int AreaNum);

void CloseSunNet(int AreaNum);
void OpenSunNet(int AreaNum);
extern bool CloseSunNetStatusArea1, OpenSunNetStatusArea1, CloseSunNetStatusArea2, OpenSunNetStatusArea2;
