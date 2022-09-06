#include <ModbusMaster.h>
#include <SoftwareSerial.h>

#define RdWrPin 9

ModbusMaster node;
SoftwareSerial nexSerial(2,3);

void preTransmission(){
  digitalWrite(RdWrPin,HIGH);
}

void postTransmission(){
  digitalWrite(RdWrPin,LOW);
}

void setup()
{
  pinMode(RdWrPin,OUTPUT);
  Serial.begin(9600);
  nexSerial.begin(9600);
  digitalWrite(RdWrPin,LOW);

  node.begin(3,nexSerial); //(slave ID,serial Port)
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

void loop()
{
  uint8_t result;
  uint16_t data[6];

  result = node.readHoldingRegisters(0x4001,6);
  Serial.print("\nResult: ");
  getResultMsg(result);

  if(result == node.ku8MBSuccess){
    Serial.print("V1 L-L: ");
    Serial.println(node.getResponseBuffer(1));
  }

  delay(1000);
}

bool getResultMsg(uint8_t result)
{
  String tmpstr2;

  switch (result) {
    case node.ku8MBSuccess:
      return true;
      break;
    case node.ku8MBIllegalFunction:
      tmpstr2 = "Illegal Function";
      break;
    case node.ku8MBIllegalDataAddress:
      tmpstr2 = "Illegal Data Address";
      break;
    case node.ku8MBIllegalDataValue:
      tmpstr2 = "Illegal Data Value";
      break;
    case node.ku8MBSlaveDeviceFailure:
      tmpstr2 = "Slave Device Failure";
      break;
    case node.ku8MBInvalidSlaveID:
      tmpstr2 = "Invalid Slave ID";
      break;
    case node.ku8MBInvalidFunction:
      tmpstr2 = "Invalid Function";
      break;
    case node.ku8MBResponseTimedOut:
      tmpstr2 = "Response Timed Out";
      break;
    case node.ku8MBInvalidCRC:
      tmpstr2 = "Invalid CRC";
      break;
    default:
      tmpstr2 = "Unknown error: " + String(result);
      break;
  }
  Serial.println(tmpstr2);
  return false;
}
