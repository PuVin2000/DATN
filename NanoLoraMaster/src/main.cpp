#include <Arduino.h>
#include "include.h"

void receiveEvent(int bytes);

#define SLAVE_ADDR 0x08 // Replace with the I2C address of the Arduino Nano

// volatile uint8_t data1 = getSettingHumidValuefromMaster;
// volatile uint8_t data2 = getSettingTempValuefromMaster; // Initialize the second byte with a value

String receivedStr1, receivedStr2; // Biến lưu trữ hai chuỗi kí tự nhận được

void receiveData(int numBytes);
void sendData();

void requestEvent();

void setup()
{
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  digitalWrite(3, 0);
  digitalWrite(4, 0);
  digitalWrite(5, 1);
  digitalWrite(6, 1);

  Wire.begin(SLAVE_ADDR); // Initialize I2C as slave
  // Wire.onReceive(receiveData);  // Đăng ký hàm xử lý sự kiện nhận dữ liệu
  Wire.onRequest(requestEvent); // Đăng ký hàm xử lý sự kiện yêu cầu dữ liệu
  Serial.begin(9600);
  while (!Serial)
    ;
  setup_Lora_SlaveNano();

  //Wire.onReceive(requestEvent);
}

void loop()
{
  onReceive(LoRa.parsePacket());
  //requestEvent();
}

void receiveData(int numBytes)
{
  receivedStr1 = "";
  receivedStr2 = "";
  while (Wire.available())
  {
    int length = Wire.read(); // Đọc độ dài chuỗi kí tự
    String str = "";
    for (int i = 0; i < length; i++)
    {
      str += (char)Wire.read(); // Đọc từng kí tự của chuỗi
    }
    if (receivedStr1.length() == 0)
    {
      receivedStr1 = str; // Lưu chuỗi kí tự thứ nhất
    }
    else
    {
      receivedStr2 = str; // Lưu chuỗi kí tự thứ hai
    }
  }
  Serial.print("Received strings: ");
  Serial.print(receivedStr1);
  Serial.print(", ");
  Serial.println(receivedStr2);
}

void requestEvent()
{
  //Serial.println(1);
  if (CheckMessage2 == 1)
  {
    // String GetDataNode1 = "Area1," + String(getHumidFromNode1) + "," + String(getTempFromNode1);
    // Wire.write(GetDataNode1.length()); // Gửi độ dài của chuỗi kí tự thứ nhất
    // Wire.write(GetDataNode1.c_str());  // Gửi chuỗi kí tự thứ nhất đã nhận được
    // Serial.println(GetDataNode1);

    String GetDataNode2 = "Area2," + String(getHumidFromNode2) + "," + String(getTempFromNode2);
    Wire.write(GetDataNode2.length()); // Gửi độ dài của chuỗi kí tự thứ nhất
    Wire.write(GetDataNode2.c_str());  // Gửi chuỗi kí tự thứ nhất đã nhận được
    Serial.println(GetDataNode2);      // Gửi kí tự ngắt dòng
    CheckMessage2 = 0;
  }
  else if (CheckMessage1 == 1)
  {
    // String GetDataNode2 = "Area2," + String(getHumidFromNode2) + "," + String(getTempFromNode2);
    // Wire.write(GetDataNode2.length()); // Gửi độ dài của chuỗi kí tự thứ nhất
    // Wire.write(GetDataNode2.c_str());  // Gửi chuỗi kí tự thứ nhất đã nhận được
    // Serial.println(GetDataNode2);

    String GetDataNode1 = "Area1," + String(getHumidFromNode1) + "," + String(getTempFromNode1);
    Wire.write(GetDataNode1.length()); // Gửi độ dài của chuỗi kí tự thứ nhất
    Wire.write(GetDataNode1.c_str());  // Gửi chuỗi kí tự thứ nhất đã nhận được
    Serial.println(GetDataNode1);
    CheckMessage1 = 0;
  }
}
