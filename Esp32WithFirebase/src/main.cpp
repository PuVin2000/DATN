#include <Arduino.h>
#include "include.h"

unsigned long time1, time2, time3, time4;

void setup()
{
  Serial.begin(9600); // initialize serial
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

  pinMode(FORWARD_SUNNET1, INPUT_PULLUP);
  pinMode(RESERVED_SUNNET1, INPUT_PULLUP);

  pinMode(FORWARD_SUNNET2, INPUT_PULLUP);
  pinMode(RESERVED_SUNNET2, INPUT_PULLUP);

  pinMode(LE1, OUTPUT);
  pinMode(LE2, OUTPUT);
  pinMode(LE3, OUTPUT);

  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);

  digitalWrite(LE1, LOW);
  digitalWrite(LE2, LOW);
  digitalWrite(LE3, LOW);

  while (!Serial)
    ;
  setup_Firebase();
  Wire.begin();
}

void loop()
{
  if ((unsigned long)(millis() - time1) > 2000)
  {
    time1 = millis();
    TestHumidArea1 = 67;
  }
  if ((unsigned long)(millis() - time2) > 4000)
  {
    time2 = millis();
    TestHumidArea1 = 60;
  }
  if ((unsigned long)(millis() - time3) > 3000)
  {
    time3 = millis();
    TestHumidArea2 = 68;
  }
  if ((unsigned long)(millis() - time4) > 5000)
  {
    time4 = millis();
    TestHumidArea2 = 62;
  }

  AutoDevice(DataControllDeviceArea1, 1, TestTempArea1, TestHumidArea1);
  AutoDevice(DataControllDeviceArea2, 2, TestTempArea2, TestHumidArea2);

  // Wire.beginTransmission(SLAVE_ADDR); // Bắt đầu giao tiếp với Slave
  // Wire.write(str1.length());          // Gửi độ dài của chuỗi thứ nhất
  // Wire.write(str1.c_str());           // Gửi chuỗi kí tự thứ nhất
  // Wire.write(str2.length());          // Gửi độ dài của chuỗi thứ hai
  // Wire.write(str2.c_str());           // Gửi chuỗi kí tự thứ hai
  // Wire.endTransmission();             // Kết thúc giao tiếp

  // delay(1000); // Đợi 1 giây

  // if ((unsigned long)(millis() - time1) > 2000)
  // {
  //   time1 = millis();
  //   // sendMessage(Mymessage, Node1, Node2);
  //   // sendMessage("0", Node1, MasterNode);
  //   Serial.println(123);
  //   Serial.println(ControllDeviceArea1);
  //   Serial.println(456);
  //   Serial.println(ControllDeviceArea2);
  //   Serial.println(789);
  // }
  if (OnOffDeviceArea1 == 1)
  {
    Serial.println(DataControllDeviceArea1);
    OnDevicesManual(DataControllDeviceArea1, 1);
    OffDevicesManual(DataControllDeviceArea1, 1);
    ControllManualSunNet(DataControllDeviceArea1, 1);
    OnOffDeviceArea1 = 0;
  }

  else if (OnOffDeviceArea2 == 1)
  {
    Serial.println(DataControllDeviceArea2);
    OnDevicesManual(DataControllDeviceArea2, 2);
    OffDevicesManual(DataControllDeviceArea2, 2);
    ControllManualSunNet(DataControllDeviceArea2, 2);
    OnOffDeviceArea2 = 0;
  }
  if (CheckSunNetArea1)
  {
    ButtonStopSunNet(1);
    ButtonCloseSunNet(DataControllDeviceArea1, 1);
    ButtonOpenSunNet(DataControllDeviceArea1, 1);
  }
  ReadStatusSunNet(1);
  if (CheckSunNetArea2)
  {
    ButtonStopSunNet(2);
    ButtonCloseSunNet(DataControllDeviceArea2, 2);
    ButtonOpenSunNet(DataControllDeviceArea2, 2);
  }
  ReadStatusSunNet(2);
  // #if 1


  // #endif

  // OnOffDeviceArea1,OnOffDeviceArea2=0;
  I2CData();
}
