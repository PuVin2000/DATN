#include <Arduino.h>
#include "include.h"

int getLightValuefromMaster;
int MapData;
void TempHumid();
void Slave1toMaster(float HumidArea1, float TempArea1);
void Slave2toMaster(float HumidArea2, float TempArea2);
unsigned long time_1;

String Mymessage = "";

void setup()
{
  Serial.begin(9600); // initialize serial
  while (!Serial)
    ;
  setup_Lora_SlaveNano();

  Serial.println("DHTxx test!");

  dht.begin();
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(3, 1);
  digitalWrite(4, 1);
  digitalWrite(5, 0);
  digitalWrite(6, 1);
}

void loop()
{

  if ((unsigned long)(millis() - time_1) > 2000)
  {
    time_1 = millis();
    TempHumid();
    
  }

  // if(CheckMessage1 == 1){
  //   String e = String(getSettingTempValuefromMaster) + "+" + String(getSettingHumidValuefromMaster);
  //   CheckMessage1 = 0;
  // }
  // onReceive(LoRa.parsePacket());

  // if (CheckMessage == 1)
  // {
  //   MapData = map(getLightValuefromMaster, 0, 100, 0, 255);
  //   Serial.println(getLightValuefromMaster);
  //   analogWrite(3, MapData);
  // }
  // CheckMessage = 0;


}

void TempHumid()
{
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  if (getSettingHumidValuefromMaster == 0)
    getSettingHumidValuefromMaster = 65;
  else if (getSettingTempValuefromMaster == 0)
    getSettingTempValuefromMaster = 35;

  // if (h > getSettingHumidValuefromMaster)
  // {
  //   digitalWrite(4, 0);
  //   digitalWrite(5, 1);
  // }

  if (t > getSettingTempValuefromMaster)
  {
    digitalWrite(4, 1);
    digitalWrite(5, 0);
  }
  else
  {
    digitalWrite(4, 0);
    digitalWrite(5, 1);
  }
  //Slave1toMaster(h, t);
  Slave2toMaster(h, t);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  
}

void Slave1toMaster(float HumidArea1, float TempArea1)
{
  String Info =String(HumidArea1) + "," + String(TempArea1);
  sendMessage(Info, Node1, MasterNode);
  //sendMessage(Info, Node1, Node2);
  Serial.println(Info);
}

void Slave2toMaster(float HumidArea2, float TempArea2)
{
  String Info =String(HumidArea2) + "," + String(TempArea2);
;
  sendMessage(Info, Node2, MasterNode);
 // sendMessage(Info, Node2, Node1);
  Serial.println(Info);
}





// Compute heat index in Fahrenheit (the default)
  // float hif = dht.computeHeatIndex(f, h);
  // // Compute heat index in Celsius (isFahreheit = false)
  // float hic = dht.computeHeatIndex(t, h, false);

  // Serial.print("Humidity: ");
  // Serial.print(h);
  // Serial.print(" %t");
  // Serial.print("Temperature: ");
  // Serial.print(t);
  // Serial.print(" *C ");
  // Serial.print(f);
  // Serial.print(" *Ft");
  // Serial.print("Heat index: ");
  // Serial.print(hic);
  // Serial.print(" *C ");
  // Serial.print(hif);
  // Serial.println(" *F");
