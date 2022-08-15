//ESP8266 with schneider meter EM6436
#include <ModbusRTU.h>
#include <SoftwareSerial.h>


float InttoFloat(uint16_t Data0,uint16_t Data1) {
  float x;
  unsigned long *p;
  p = (unsigned long*)&x;
  *p = (unsigned long)Data0 << 16 | Data1; //Big-endian
  return(x);
}
SoftwareSerial S(D3, D2);


ModbusRTU mb;

bool cb(Modbus::ResultCode event, uint16_t transactionId, void* data) { // Callback to monitor errors
  if (event != Modbus::EX_SUCCESS) {
    Serial.print("Request result: 0x");
    Serial.print(event, HEX);
  }
  return true;
}

void setup() {
  Serial.begin(9600);
  S.begin(9600, SWSERIAL_8E1);
  mb.begin(&S,D0); // RE/DE connected to D0 of ESP8266
  mb.master();
}

uint16_t val[2];
void loop() {
  
  if (!mb.slave()) {
    mb.readHreg(1, 3926, val, 2, cb); // Slave id is 1 and register address is 3926 and 
    //we are reading 2 bytes from the register and saving in val
    while(mb.slave()) { // Check if transaction is active
      mb.task();
      delay(100);
    }
    Serial.println("Register Values ");
    Serial.println(val[0]);
    Serial.println(val[1]);
  float voltage= InttoFloat(val[1],val[0]);
  Serial.println("Voltage= ");
  Serial.print(voltage);
  Serial.println("V");

      }
  delay(1000);
}
