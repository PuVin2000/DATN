#include <SPI.h> // include libraries
#include <LoRa.h>
#include "Wire.h"
#include "LoraFunction.h"
#include "Arduino_FreeRTOS.h"


#define ss 10
#define rst 9
#define dio0 2

#define IN1	5
#define IN2	6
#define ENB	4

static byte MasterNode = 0x1F;
static byte Node1 = 0xBB;
static byte Node2 = 0xCC;



static String outgoing;   // outgoing message
static byte msgCount = 0; // count of outgoing messages
static String SenderNode = "";

// static int AirValue = 590;   // you need to replace this value with Value_1
// static int WaterValue = 300; // you need to replace this value with Value_2
// static int SensorPin = A0;
// static int soilMoistureValue = 0;
// static int soilmoisturepercent = 0;

//static unsigned long time;

extern bool CheckMessage1,CheckMessage2;

extern int getLightValuefromMaster;

extern float getTempFromNode1,getHumidFromNode1;
extern float getTempFromNode2,getHumidFromNode2;

//static String GetDataNode1,GetDataNode2 = "";

