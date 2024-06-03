#include <Arduino.h>

#include "include.h"

// cài đặt PWM
int Value_PWM = 0;
bool SettingTempArea1, SettingHumidArea1, SettingTempArea2, SettingHumidArea2;
bool OnOffDeviceArea1, OnOffDeviceArea2;

String DataControllDeviceArea1,DataControllDeviceArea2; 

int DataLight;

float DataSettingTempArea1,DataSettingTempArea2;
float DataSettingHumidArea1,DataSettingHumidArea2;

String parentPath = "/IOT/Update";
// String childPath[3] = {"IOT/Light", "CapNhat/Area1/Temp", "CapNhat/Area1/Humid"};

int IndexMessage;

int CheckAreaNum;

bool streamTO = true;

void setup_Firebase()
{
  // put your setup code here, to run once:
  WiFi_Config();
  Firebase_Config();

  if (!Firebase.beginMultiPathStream(Firebase_DATA, parentPath))
    Serial.printf("sream begin error, %s\n\n", Firebase_DATA.errorReason().c_str());

  Firebase.setMultiPathStreamCallback(Firebase_DATA, streamCallback, streamTimeoutCallback);
}

void streamCallback(MultiPathStreamData stream)
{
  if (stream.get("/LightArea1"))
  {
    bool Sett = 1;
    DataLight = stream.value.toInt();
    //Serial.println(DataLight);
    // sendMessage(data_getString, MasterNode, Node1);
  }
  else if (stream.get("/TempArea1"))
  {
    SettingTempArea1 = 1;
    DataSettingTempArea1 = stream.value.toFloat();
    //Serial.println(DataSettingTemp);
    // sendMessage(data_getString, MasterNode, Node1);
  }
  else if (stream.get("/HumidArea1"))
  {
    SettingHumidArea1 = 1;
    DataSettingHumidArea1 = stream.value.toFloat();
    //Serial.println(DataSettingHumid);
    // sendMessage(data_getString, MasterNode, Node1);
  }
  else if (stream.get("/TempArea2"))
  {
    SettingTempArea2 = 1;
    DataSettingTempArea2 = stream.value.toFloat();
    //Serial.println(DataSettingTemp);
    // sendMessage(data_getString, MasterNode, Node1);
  }
  else if (stream.get("/HumidArea2"))
  {
    SettingHumidArea2 = 1;
    DataSettingHumidArea2 = stream.value.toFloat();
    //Serial.println(DataSettingHumid);
    // sendMessage(data_getString, MasterNode, Node1);
  }
  else if (stream.get("/DieuKhienKhuVuc1"))
  {
    OnOffDeviceArea1 = 1;
    CheckAreaNum = 1;
    DataControllDeviceArea1 = stream.value;
    //Serial.println(DataControllDeviceArea1);
  }
  else if (stream.get("/DieuKhienKhuVuc2"))
  {
    OnOffDeviceArea2 = 1;
    CheckAreaNum = 2;
    DataControllDeviceArea2 = stream.value;
    //Serial.println(DataControllDeviceArea2);
  }
}

void streamTimeoutCallback(bool timeout)
{
  if (timeout)
  {
    // Stream timeout occurred
    Serial.println("Stream timeout, resume streaming...");
  }
}

void WiFi_Config()
{
  Serial.begin(9600);
  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {

    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connecting with IP:");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void Firebase_Config()
{
  // connect Firebase-----------
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}
