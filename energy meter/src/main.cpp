#include "ACS712.h"
#include "ZMPT101B.h"
#include "database.hpp"


//user dfeined
database obj; 

void setup() {
// 115200
  Serial.begin(115200); 

  obj.setup_meter();
  obj.connectToWiFi();
  obj.connectToDataBase();


}

void loop() { 

  obj.energy_meter_values();
  obj.send_data(POST_DATA);
  delay(1000);
}
