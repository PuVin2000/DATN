#include <WiFi.h>
#include <FirebaseESP32.h>

#include "FreeRTOSConfig.h"
#include "soc/rtc_wdt.h"

#include "FireBaseFunction.h"
#include "ControllDevices.h"

#include "Wire.h"

#include <stdio.h>
#include <string.h>
#include "driver/i2c.h"

#define ss 5   // GPIO 5
#define rst 14 // GPIO 14
#define dio0 2 // GPIO 2

// khai báo static or const để không sinh lỗi multiple defination of

void receiveEvent(int bytes);