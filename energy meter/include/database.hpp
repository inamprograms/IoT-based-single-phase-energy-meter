#ifndef DATABASE_HPP
#define DATABASE_HPP
#define WIFI_SSID "EPTeckS"
#define WIFI_PASSWORD "epteck@123S"
#define HOST "172.168.168.79"  
#define PORT 80
#define POST_DATA 0
#define PUT_DATA 1
#define CNIC "123456"
#define URI "/energyMeter/energy_meter.php"

#define CONTENT_TYPE "application/json"

#include<stdint.h>



class database
{
private:
   
public:
    database();
    ~database();

    void connectToWiFi();
    void connectToDataBase();
    void send_data(uint8_t request);
    void retrieve_data();
    void energy_meter_values();
    void setup_meter();

};







#endif