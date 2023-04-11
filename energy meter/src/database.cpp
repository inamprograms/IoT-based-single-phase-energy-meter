#include"database.hpp"
#include<WiFi.h>
#include<stdint.h>
#include "ACS712.h"
#include "ZMPT101B.h"

int I = 0;
int V = 0;
int P = 0;

ACS712 sensor(ACS712_30A, 33);
ZMPT101B voltageSensor(32);

WiFiClient client;
database::database()
{
}
database::~database()
{
}

void database::connectToWiFi(){

    WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
    Serial.println("Connecting to WiFi Netmork...");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print("<");
        delay(1000);
        Serial.println("<Connecting...>");
    }
    Serial.println("Connected to WiFi Network");
    Serial.print("IP : ");
    Serial.println(WiFi.localIP());
}

void database::connectToDataBase(){

    while (!client.connect(HOST,PORT))
    {
        Serial.println("Connection failed.");
        return;
    }
    Serial.println("Connected With database.");
    
}

void database::send_data(uint8_t request){

    char payload[500];
    char packet[1000];
    int contentLength;

    switch (request)
    {
    case POST_DATA:

        contentLength = sprintf(payload, "{\"current\" : %d, \"voltage\" : %d, \"power\" : %d}",I,V,P);
        sprintf(packet, "POST %s HTTP/1.1\r\nHost: %s\r\nContent-type:%s\r\nContent-length:%i\r\n\r\n%s",URI,HOST,CONTENT_TYPE,contentLength,payload);

        Serial.println(packet);
        client.print(packet);

        while (client.available())
        {
            String line = client.readStringUntil('\r');
            Serial.print(line);
        }
        Serial.println();
        Serial.println("Closing connection");
    
        break;
    case PUT_DATA:

        contentLength = sprintf(payload, "{\"current\": %i, \"voltage\":%i,\"power\": %i,\"cnic\": %s}",I,V,P,CNIC);
        sprintf(packet,"PUT %s HTTP/1.1\r\nHost:%s\r\nContent-Type:%s\r\nContent-Length:%i\r\n\r\n%s",URI,HOST,CONTENT_TYPE,contentLength,payload);

        Serial.println(packet);
        client.print(packet);

        while (client.available())
        {
            String line = client.readStringUntil('\r');
            Serial.print(line);
        }
        Serial.println();
        Serial.println("Closing connection");
        break;
    
    default:
        break;
    }
}

void database::retrieve_data(){

    char payload[500];
    char packet[1000];
    

    Serial.println("reading data...");
    sprintf(packet, "GET %s?cnic=%sHTTP/1.1\r\nHost:%s\r\n\r\n\r\n",URI,CNIC,HOST);
    Serial.println(packet);
    client.print(packet);

    String line;
    line = client.readString();
    Serial.println(line);
    Serial.println();

    for (int i = 0; i < line.length(); i++)
    {
        if (line[i]=='{')
        {
            int startIndex = i;
            for (int j = startIndex; j < line.length(); j++)
            {
                payload[j-i] = line[j];
                if (line[j] == '}')
                {
                    payload[j-i+1] = '\0';
                    break;
                }
                
            }
            
        }
        
    }

    Serial.println("Closing connection");
    Serial.println(payload);
}

void database::energy_meter_values(){


  I = sensor.getCurrentAC();
  V = voltageSensor.getVoltageAC();
  P = V*I;
  
}

void database::setup_meter(){
  sensor.setZeroPoint(1876);
  voltageSensor.setZeroPoint(1875);
  voltageSensor.setSensitivity(0.001);
}