#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
 
#define DS18B20 D2          //DS18B20 is connected to Pin D2
#define led 2//ESP-12LED

String apiKey = "BF655K54FB3J20WE";     //  Enter my Write API key from ThingSpeak
const char* ssid =  "1327";     // Enter my WiFi Network's SSID
const char* pass =  "13271327"; // Enter my WiFi Network's Password
const char* server = "api.thingspeak.com";
float temp;
 
OneWire ourWire(DS18B20);
DallasTemperature sensor(&ourWire);
WiFiClient client;
 
void setup() 
{
       Serial.begin(9600);//board rate
       delay(1000);
       sensor.begin();
 
       Serial.print("Connecting to: ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            digitalWrite(led,HIGH);
            delay(100);
            digitalWrite(led,LOW);
            Serial.print("*");
     }
      Serial.println("");
      Serial.println("***WiFi connected***");
 
}
 
void loop() 
{
  
      sensor.requestTemperatures();
      temp = sensor.getTempCByIndex(0);
 
      if (client.connect(server,80) && temp != 85.0 && temp!=-127.0)//Prevent sending trash data
      {  
       String sendData = apiKey+"&field1="+String(temp)+"\r\n\r\n";

       client.print("POST /update HTTP/1.1\n");
       client.print("Host: api.thingspeak.com\n");
       client.print("Connection: close\n");
       client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
       client.print("Content-Type: application/x-www-form-urlencoded\n");
       client.print("Content-Length: ");
       client.print(sendData.length());
       client.print("\n\n");
       client.print(sendData);
       
        Serial.print("Temperature: ");
        Serial.print(temp);
        Serial.println("deg C. Connecting to Thingspeak..");
       
      }
      
      client.stop();

      Serial.println("Sending....");
  
 delay(60000);//wait 1 minute
}
