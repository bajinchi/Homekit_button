#include <ESP8266WiFi.h>          
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>          
#include <EEPROM.h>

IPAddress server(192, 168, 0, 56); // IP de la raspberry Pi
const char* host = "Button1"; // nombre del entorno

int pulsador = 0;
boolean estado_pulsador = LOW;
boolean estado_button=0;



#define BUFFER_SIZE 100

WiFiClient wclient;
PubSubClient client(wclient, server);

void callback(const MQTT::Publish& pub) {
  Serial.println (pub.payload_string());
    if(pub.payload_string() == "")
    {

    }
    
}

void setup() 
{
  pinMode(pulsador,INPUT);
  Serial.begin(115200);
  EEPROM.begin(512);
  delay(10);
  Serial.println();
  Serial.println();
  client.set_callback(callback);
    if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(WiFi.SSID());
    Serial.println("...");
    WiFi.mode(WIFI_STA);
    WiFi.begin();
    if (WiFi.waitForConnectResult() != WL_CONNECTED){
      Serial.println("WiFi not connected");
      WiFiManager wifiManager;  
      if (!wifiManager.startConfigPortal("Button1Setup")) { // SSID 
        Serial.println("failed to connect and hit timeout");
        delay(3000);
        ESP.reset();
        delay(5000);
        }
      Serial.print("connected to ");
      Serial.println(WiFi.SSID());
      EEPROM.commit();
      delay(2000);
    }
    else
   {
     Serial.println("WiFi connected");                       
   }
  }   
  estado_button=EEPROM.read(0);

  delay(200);
}


void loop() {
estado_pulsador=digitalRead(pulsador);
if (WiFi.status() == WL_CONNECTED)
{
if (estado_pulsador==LOW)
{
  if (estado_button==LOW)
  {
    estado_button=HIGH;
  }
  else
  {
    estado_button=LOW;
  }
  client.publish("Button1",String(estado_button));  
  Serial.print("Button1, ");
  Serial.println(estado_button);
  EEPROM.write(0,estado_button);
  EEPROM.commit();
  delay(500);
}
{
  if (!client.connected()) 
  {
    if (client.connect("ESP8266: Button1"))  
    {   
      //client.subscribe("Button1");
    }
  }
    client.loop();
  }
  delay(1);
} 
 delay(50); 
}

