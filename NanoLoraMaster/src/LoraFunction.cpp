#include "include.h"

bool CheckMessage1, CheckMessage2;

float getTempFromNode1,getHumidFromNode1;
float getTempFromNode2,getHumidFromNode2;

// String Mymessage = "";
void setup_Lora_SlaveNano()
{

  Serial.println("LoRa Node1");
  LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(433E6))
  {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }
  // LoRa.setTxPower(20);
  //  LoRa.onReceive(onReceive);
  //  LoRa.receive();
}
void onReceive(int packetSize)
{
  if (packetSize == 0)
    return; // if there's no packet, return
  // read packet header bytes:
  int recipient = LoRa.read(); // recipient address
  byte sender = LoRa.read();   // sender address
  if (sender == Node1)
    SenderNode = "Node 1:";
  if (sender == Node2)
    SenderNode = "Node2:";

  byte incomingMsgId = LoRa.read();  // incoming msg ID
  byte incomingLength = LoRa.read(); // incoming msg length

  char incoming[256];

  for (int i = 0; i < packetSize; i++)
  {
    incoming[i] = (char)LoRa.read();
  }

  incoming[packetSize] = '\0';
  /*  if (incomingLength != incoming.length()) {  // check length for error
     // Serial.println("error: message length does not match length");
     ;
     return;  // skip rest of function
   } */
  // if the recipient isn't this device or broadcast,
  Serial.println(recipient);
  Serial.println(sender);
  if (recipient != Node1 && recipient != MasterNode )
  {
    Serial.println("This message is not for me 1.");
    ;
    return; // skip rest of function
  }
  else if (recipient != Node2 && recipient != MasterNode)
  {
    Serial.println("This message is not for me 2.");
    ;
    return; // skip rest of function
  }
  // Serial.println(incoming);
  if (sender == Node1)
  {
    CheckMessage1 = 1;
    // String StringGetDataLight = getValue(incoming, ',', 0);
    // getLightValuefromMaster = StringGetDataLight.toInt();
    String v = incoming;
    String StringGetHumidFromNode1 = getValue(incoming, ',', 0);
    getHumidFromNode1 = StringGetHumidFromNode1.toFloat();
    Serial.println(getHumidFromNode1);

    String StringGetTempFromNode1 = getValue(incoming, ',', 1);
    getTempFromNode1 = StringGetTempFromNode1.toFloat();
    Serial.println(getTempFromNode1);
    //Serial.println(v);
  }

  if (sender == Node2)
  {
    CheckMessage2 = 1;
    String t = incoming;
    String StringGetHumidFromNode2 = getValue(incoming, ',', 0);
    getHumidFromNode2 = StringGetHumidFromNode2.toFloat();
     Serial.println(getHumidFromNode2);

    String StringGetTempFromNode2 = getValue(incoming, ',', 1);
    getTempFromNode2 = StringGetTempFromNode2.toFloat();
     Serial.println(getTempFromNode2);
  }
}
void sendMessage(String outgoing, byte MasterNode, byte otherNode)
{
  LoRa.beginPacket();            // start packet
  LoRa.write(otherNode);         // add destination address
  LoRa.write(MasterNode);        // add sender address
  LoRa.write(msgCount);          // add message ID
  LoRa.write(outgoing.length()); // add payload length
  LoRa.print(outgoing);          // add payload
  LoRa.endPacket();              // finish packet and send it
  msgCount++;                    // increment message ID
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

