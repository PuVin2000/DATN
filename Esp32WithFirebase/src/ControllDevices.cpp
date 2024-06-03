#include "Arduino.h"
#include "include.h"

unsigned long timeStopButtonSunNet1, timeStopButtonSunNet2, timeOpenButtonSunNet1, timeOpenButtonSunNet2, timeCloseButtonSunNet1, timeCloseButtonSunNet2;
//
float CompareTempValueArea1, CompareHumidValueArea1;
float CompareTempValueArea2, CompareHumidValueArea2;

float CompareValueTemp[] = {0, CompareTempValueArea1, CompareTempValueArea2};
float CompareValueHumid[] = {0, CompareHumidValueArea1, CompareHumidValueArea2}; //

bool CheckStatusBomNuoc = 0;
bool CheckCloseSunNet, CheckOpenSunNet = 1;

bool CheckStatusButtonCloseSunNet, CheckStatusButtonOpenSunNet;

int NumberTimeCheck;
//
bool CheckSunNetArea1, CheckSunNetArea2;
bool CheckSunNetArea[] = {0, CheckSunNetArea1, CheckSunNetArea2}; //

float TestTempArea1, TestTempArea2, TestHumidArea1, TestHumidArea2;
//
bool StatusAutoMayBom1, StatusAutoMayBom2, StatusAutoSunNet1, StatusAutoSunNet2;
bool AutoMayBomArea[] = {0, StatusAutoMayBom1, StatusAutoMayBom2};
bool AutoSunNetArea[] = {0, StatusAutoSunNet1, StatusAutoSunNet2}; //
//
bool CloseSunNetStatusArea1, OpenSunNetStatusArea1, CloseSunNetStatusArea2, OpenSunNetStatusArea2;
bool ForwardReservedSunNet[][2] = {
    {},
    {CloseSunNetStatusArea1, OpenSunNetStatusArea1},
    {CloseSunNetStatusArea2, OpenSunNetStatusArea2}}; //

int ReadStatusSunNetToFireBase[] = {0, 0, 0}; //
bool ReadStopSunNet[] = {0, 0, 0};

float FloatValueHumid1, FloatValueTemp1, FloatValueHumid2, FloatValueTemp2;

unsigned long time123;

void I2CData()
{
    Wire.requestFrom(SLAVE_ADDR, 17); // Yêu cầu nhận dữ liệu từ Slave (tối đa 64 byte)
    if (Wire.available())
    {
        int length1 = Wire.read();                           // Đọc độ dài của chuỗi thứ nhất
        String receivedStr1 = Wire.readStringUntil(length1); // Đọc chuỗi kí tự thứ nhất
        String DataGet = getValue(receivedStr1, ',', 0);

        if (DataGet == "Area1")
        {
            String StringGetHumid = getValue(receivedStr1, ',', 1);
            FloatValueHumid1 = StringGetHumid.toFloat();

            String StringGetTemp = getValue(receivedStr1, ',', 2);
            FloatValueTemp1 = StringGetTemp.toFloat();
            if (CompareValueHumid[1] != FloatValueHumid1)
            {
                CompareValueHumid[1] = FloatValueHumid1;
                Firebase.setFloat(Firebase_DATA, "/IOT/Area1/Getting/Humid", CompareValueHumid[1]);
            }
            else if (CompareValueTemp[1] != FloatValueTemp1)
            {
                CompareValueTemp[1] = FloatValueTemp1;
                Firebase.setFloat(Firebase_DATA, "/IOT/Area1/Getting/Temp", CompareValueTemp[1]);
            }
            // int length2 = Wire.read();                           // Đọc độ dài của chuỗi thứ hai
            // String receivedStr2 = Wire.readStringUntil(length2); // Đọc chuỗi kí tự thứ hai
            Serial.print("Received strings: ");
            Serial.print(DataGet);
            Serial.print(',');
            Serial.print(CompareValueHumid[1]);
            Serial.print(',');
            Serial.print(CompareValueTemp[1]);
            Serial.println();
            // Serial.print(", ");
            // Serial.println(receivedStr2);
        }
        else if (DataGet == "Area2")
        {
            String StringGetHumid = getValue(receivedStr1, ',', 1);
            FloatValueHumid1 = StringGetHumid.toFloat();

            String StringGetTemp = getValue(receivedStr1, ',', 2);
            FloatValueTemp1 = StringGetTemp.toFloat();
            if (CompareValueHumid[2] != FloatValueHumid1)
            {
                CompareValueHumid[2] = FloatValueHumid1;
                Firebase.setFloat(Firebase_DATA, "/IOT/Area2/Getting/Humid", CompareValueHumid[2]);
            }
            else if (CompareValueTemp[2] != FloatValueTemp1)
            {
                CompareValueTemp[2] = FloatValueTemp1;
                Firebase.setFloat(Firebase_DATA, "/IOT/Area2/Getting/Temp", CompareValueTemp[2]);
            }

            Serial.print("Received strings: ");
            Serial.print(DataGet);
            Serial.print(',');
            Serial.print(CompareValueHumid[2]);
            Serial.print(',');
            Serial.print(CompareValueTemp[2]);
            Serial.println();
        }
    }
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;
    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

// Controll Device
void OnDevicesManual(String DataControll, int AreaNum)
{
    int lengthStringArray = sizeof buttonOFFDevice[AreaNum] / sizeof(String);
    for (int i = 0; i < lengthStringArray; i++)
    {
        if (DataControll == buttonONDevice[AreaNum][i])
        {
            ButtonOnDevices(AreaNum, i);
            if (i == 0)
                AutoMayBomArea[AreaNum] = false;
        }
    }
}

void OffDevicesManual(String DataControll, int AreaNum)
{
    int lengthStringArray = sizeof buttonOFFDevice[AreaNum] / sizeof(String);
    for (int i = 0; i < lengthStringArray; i++)
    {
        if (DataControll == buttonOFFDevice[AreaNum][i])
        {
            ButtonOffDevices(AreaNum, i);
            if (i == 0)
                AutoMayBomArea[AreaNum] = false;
        }
    }
}
// StatusButtonDevice
void ButtonOffDevices(int LEindex, int PinControll)
{
    for (int count = 0; count < sizeof StatusPinWriteArea[LEindex] / sizeof(bool); count++)
    {
        digitalWrite(PinWrtie[count], StatusPinWriteArea[LEindex][count]);
    }
    StatusPinWriteArea[LEindex][PinControll] = LOW;
    digitalWrite(PinWrtie[PinControll], StatusPinWriteArea[LEindex][PinControll]);
    digitalWrite(LEWrtie[LEindex], HIGH);
    digitalWrite(LEWrtie[LEindex], LOW);
}

void ButtonOnDevices(int LEindex, int PinControll)
{
    for (int count = 0; count < sizeof StatusPinWriteArea[LEindex] / sizeof(bool); count++)
    {
        digitalWrite(PinWrtie[count], StatusPinWriteArea[LEindex][count]);
    }
    StatusPinWriteArea[LEindex][PinControll] = HIGH;
    digitalWrite(PinWrtie[PinControll], StatusPinWriteArea[LEindex][PinControll]);
    digitalWrite(LEWrtie[LEindex], HIGH);
    digitalWrite(LEWrtie[LEindex], LOW);
}

// Controll Auto
void AutoDevice(String DataControll, int AreaNum, float TempArea, float HumidArea)
{
    // CompareValueTemp[AreaNum] = TempArea;
    // CompareValueHumid[AreaNum] = HumidArea;

    if (DataControll == buttonAUTODevice[1][AreaNum])
    {
        AutoMayBomArea[AreaNum] = true;
    }
    else if (DataControll == buttonAUTODevice[2][AreaNum])
    {
        AutoSunNetArea[AreaNum] = true;
        if (AreaNum == 1)
            CheckSunNetArea1 = 1;
        else if (AreaNum == 2)
            CheckSunNetArea2 = 1;
    }

    if (AutoMayBomArea[AreaNum] == true)
    {
        Serial.println(FloatValueHumid1);
        Serial.println(DataSettingHumidArea1);
        if (FloatValueHumid1 < DataSettingHumidArea1)
        {
            CheckStatusBomNuoc = true;
            ButtonOnDevices(AreaNum, 0);
        }
        else if (FloatValueHumid1 > DataSettingHumidArea1)
        {
            if (CheckStatusBomNuoc == 1)
            {
                ButtonOffDevices(AreaNum, 0);
                CheckStatusBomNuoc = 0;
            }
        }
    }

    if (AutoSunNetArea[AreaNum] == true)
    {
        Serial.println(DataSettingTempArea1);
        if (FloatValueTemp1 > DataSettingTempArea1)
        {
            Serial.println("abcadas");
            Serial.println(CheckCloseSunNet);
            Serial.println(CheckOpenSunNet);
            if (CheckCloseSunNet == 1)
            {
                CheckStatusButtonCloseSunNet = true;
                CheckOpenSunNet = 1;
                CheckCloseSunNet = 0;
                StatusSunNetIsCloseOrOpen(AreaNum);
            }
        }
        else if (FloatValueTemp1 < DataSettingTempArea1)
        {
            Serial.println(CheckCloseSunNet);
            Serial.println(CheckOpenSunNet);
            if (CheckOpenSunNet == 1)
            {
                CheckStatusButtonOpenSunNet = true;
                CheckOpenSunNet = 0;
                CheckCloseSunNet = 1;
                StatusSunNetIsCloseOrOpen(AreaNum);
            }
        }
    }
}

// Controll SunNet
void ControllManualSunNet(String DataControll, int AreaNum)
{
    for (int count = 0; count < sizeof StatusPinWriteArea[3] / sizeof(bool); count++)
    {
        digitalWrite(PinWrtie[count], StatusPinWriteArea[3][count]);
    }

    if (AreaNum == 1)
        CheckSunNetArea1 = 1;
    else if (AreaNum == 2)
        CheckSunNetArea2 = 1;

    if (DataControll == OpenSunNetArea[AreaNum][0])
    {
        CheckStatusButtonOpenSunNet = true;
        AutoSunNetArea[AreaNum] = 0;
    }
    if (DataControll == CloseSunNetArea[AreaNum][0])
    {
        CheckStatusButtonCloseSunNet = true;
        AutoSunNetArea[AreaNum] = 0;
    }

    StatusSunNetIsCloseOrOpen(AreaNum);
}

void StatusSunNetIsCloseOrOpen(int AreaNum)
{
    ForwardReservedSunNet[AreaNum][0] = digitalRead(SunNetStatusControll[AreaNum][0]);
    ForwardReservedSunNet[AreaNum][1] = digitalRead(SunNetStatusControll[AreaNum][1]);

    if (ForwardReservedSunNet[AreaNum][0] == 1 && ForwardReservedSunNet[AreaNum][1] == 1)
    {
        // 0 is Overload,1 Is Stop,2 Is Closing, 3 Is CLosed, 4 Is Opening,5 IS Opened

        NumberTimeCheck = 3;
        if (CheckStatusButtonOpenSunNet == true)
        {
            Serial.println("Need to Reserved");
            timeOpenButtonSunNet1 = millis();
        }
        else if (CheckStatusButtonCloseSunNet == true)
        {
            Serial.println("Need to Forward");
            timeCloseButtonSunNet1 = millis();
        }
    }
    else if (ForwardReservedSunNet[AreaNum][0] == 0 && ForwardReservedSunNet[AreaNum][1] == 1) // Forward is Close
    {
        Serial.println("Forward is Closing");
        // Serial.println("Forward is Close");
        if (CheckStatusButtonOpenSunNet == true)
        {
            NumberTimeCheck = 1;
            // CheckStatusButtonCloseSunNet = 0;
            timeStopButtonSunNet1 = millis();
        }
    }
    else if (ForwardReservedSunNet[AreaNum][0] == 1 && ForwardReservedSunNet[AreaNum][1] == 0) // reserved is Open
    {
        Serial.println("reserved is Open");
        if (CheckStatusButtonCloseSunNet == true)
        {
            NumberTimeCheck = 1;
            timeStopButtonSunNet1 = millis();
        }
    }
}

// StatusButtonSunNet
void ButtonStopSunNet(int AreaNum)
{
    int StopSunNetPin;
    if (AreaNum == 1)
    {
        StopSunNetPin = 2;
    }
    else if (AreaNum == 2)
    {
        StopSunNetPin = 5;
    }

    if (NumberTimeCheck == 1)
    {
        if ((unsigned long)(millis() - timeStopButtonSunNet1) > 2000)
        {
            ReadStatusSunNetToFireBase[AreaNum] = 1; // 4 Is Stopping
            Firebase.setInt(Firebase_DATA, PathSunNetFireBase[AreaNum][0], ReadStatusSunNetToFireBase[AreaNum]);

            Serial.println("OK1a");
            StatusPinWriteArea[3][StopSunNetPin] = HIGH;
            digitalWrite(PinWrtie[StopSunNetPin], StatusPinWriteArea[3][StopSunNetPin]);
            digitalWrite(LEWrtie[3], HIGH);
            digitalWrite(LEWrtie[3], LOW);
            timeStopButtonSunNet2 = millis();
            NumberTimeCheck = 2;
        }
    }
    if (NumberTimeCheck == 2)
    {
        if ((unsigned long)(millis() - timeStopButtonSunNet2) > 2000)
        {
            Serial.println("OK1b");
            StatusPinWriteArea[3][StopSunNetPin] = LOW;
            digitalWrite(PinWrtie[StopSunNetPin], StatusPinWriteArea[3][StopSunNetPin]);
            digitalWrite(LEWrtie[3], HIGH);
            digitalWrite(LEWrtie[3], LOW);
            timeOpenButtonSunNet1 = millis();
            timeCloseButtonSunNet1 = millis();
            NumberTimeCheck = 3;
        }
    }
}

void ButtonCloseSunNet(String DataControll, int AreaNum)
{
    if (DataControll == CloseSunNetArea[AreaNum][0] || DataControll == buttonAUTODevice[2][AreaNum])
    {
        CloseSunNet(AreaNum);
    }
}

void ButtonOpenSunNet(String DataControll, int AreaNum)
{
    if (DataControll == OpenSunNetArea[AreaNum][0] || DataControll == buttonAUTODevice[2][AreaNum])
    {
        OpenSunNet(AreaNum);
    }
}

void ReadStatusSunNet(int AreaNum)
{
    ForwardReservedSunNet[AreaNum][0] = digitalRead(SunNetStatusControll[AreaNum][0]);
    ForwardReservedSunNet[AreaNum][1] = digitalRead(SunNetStatusControll[AreaNum][1]);

    if (ForwardReservedSunNet[AreaNum][0] == 1 && ForwardReservedSunNet[AreaNum][1] == 1)
    {
        if (ReadStopSunNet[AreaNum] == 1)
        {
            Firebase.setInt(Firebase_DATA, PathSunNetFireBase[AreaNum][0], ReadStatusSunNetToFireBase[AreaNum]);
            ReadStopSunNet[AreaNum] = 0;
        }
    }
}

void CloseSunNet(int AreaNum)
{
    int CloseSunNetPin, StopSunNetPin;
    if (AreaNum == 1)
    {
        CloseSunNetPin = 0;
        StopSunNetPin = 2;
    }
    else if (AreaNum == 2)
    {
        CloseSunNetPin = 3;
        StopSunNetPin = 5;
    }
    if (CheckStatusButtonCloseSunNet == 1)
    {
        if (NumberTimeCheck == 3)
        {
            if ((unsigned long)(millis() - timeCloseButtonSunNet1) > 5000)
            {
                ReadStatusSunNetToFireBase[AreaNum] = 2; // 2 Is Closing
                Firebase.setInt(Firebase_DATA, PathSunNetFireBase[AreaNum][0], ReadStatusSunNetToFireBase[AreaNum]);

                Serial.println("OK12");
                StatusPinWriteArea[3][CloseSunNetPin] = HIGH;
                digitalWrite(PinWrtie[CloseSunNetPin], StatusPinWriteArea[3][CloseSunNetPin]);
                digitalWrite(LEWrtie[3], HIGH);
                digitalWrite(LEWrtie[3], LOW);
                timeCloseButtonSunNet2 = millis();
                NumberTimeCheck = 4;
            }
        }
        if (NumberTimeCheck == 4)
        {
            if ((unsigned long)(millis() - timeCloseButtonSunNet2) > 2000)
            {
                Serial.println("OK13");
                StatusPinWriteArea[3][CloseSunNetPin] = LOW;
                digitalWrite(PinWrtie[CloseSunNetPin], StatusPinWriteArea[3][CloseSunNetPin]);
                digitalWrite(LEWrtie[3], HIGH);
                digitalWrite(LEWrtie[3], LOW);

                ReadStatusSunNetToFireBase[AreaNum] = 3;

                CheckStatusButtonCloseSunNet = 0;

                ReadStopSunNet[AreaNum] = 1;

                if (AreaNum == 1)
                    CheckSunNetArea1 = 0;
                else if (AreaNum == 2)
                    CheckSunNetArea2 = 0;
            }
        }
    }
}

void OpenSunNet(int AreaNum)
{
    int OpenSunNetPin, StopSunNetPin;
    if (AreaNum == 1)
    {
        OpenSunNetPin = 1;
        StopSunNetPin = 2;
    }
    else if (AreaNum == 2)
    {
        OpenSunNetPin = 4;
        StopSunNetPin = 5;
    }
    if (CheckStatusButtonOpenSunNet == 1)
    {
        if (NumberTimeCheck == 3)
        {
            if ((unsigned long)(millis() - timeOpenButtonSunNet1) > 5000)
            {
                ReadStatusSunNetToFireBase[AreaNum] = 4; // 4 Is Opening
                Firebase.setInt(Firebase_DATA, PathSunNetFireBase[AreaNum][0], ReadStatusSunNetToFireBase[AreaNum]);

                Serial.println("OK22");
                StatusPinWriteArea[3][OpenSunNetPin] = HIGH;
                digitalWrite(PinWrtie[OpenSunNetPin], StatusPinWriteArea[3][OpenSunNetPin]);
                digitalWrite(LEWrtie[3], HIGH);
                digitalWrite(LEWrtie[3], LOW);
                timeOpenButtonSunNet2 = millis();
                NumberTimeCheck = 4;
            }
        }

        if (NumberTimeCheck == 4)
        {
            if ((unsigned long)(millis() - timeOpenButtonSunNet2) > 2000)
            {
                Serial.println("OK23");
                StatusPinWriteArea[3][OpenSunNetPin] = LOW;
                digitalWrite(PinWrtie[OpenSunNetPin], StatusPinWriteArea[3][OpenSunNetPin]);
                digitalWrite(LEWrtie[3], HIGH);
                digitalWrite(LEWrtie[3], LOW);

                ReadStatusSunNetToFireBase[AreaNum] = 5;

                CheckStatusButtonOpenSunNet = 0;

                ReadStopSunNet[AreaNum] = 1;

                if (AreaNum == 1)
                    CheckSunNetArea1 = 0;
                else if (AreaNum == 2)
                    CheckSunNetArea2 = 0;
            }
        }
    }
}