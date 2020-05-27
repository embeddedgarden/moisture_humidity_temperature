#include <DHT.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>

#define MOISTURE_PIN A0
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

int moisture;
float humidity;
float temperature;

char ssid[] = "";        
char pass[] = ""; 

int  status = WL_IDLE_STATUS;
char server[] = "192.168.1.26";

WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  while(status != WL_CONNECTED){
    /* Uncomment to debug
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);
    */
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }

  IPAddress ip = WiFi.gatewayIP();
  IPAddress gateway = WiFi.gatewayIP();
}

void loop() {
  // put your main code here, to run repeatedly:
  moisture = analogRead(MOISTURE_PIN);
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  StaticJsonDocument<200> doc;
  doc["moisture"] = moisture;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  String postData;
  serializeJson(doc, postData);
  
  if(client.connect(server, 5000)){   
    client.println("POST /api/v1/sensor HTTP/1.1");
    client.println("Host: 192.168.1.26:5000");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println("Content-Type: application/json");
    client.println();
    client.println(postData);    
  }

  if(client.connected()){
    /* Uncomment to debug
    String line = client.readStringUntil('\n');
    Serial.println(line);
    */
    client.stop();
  }

  delay(60000);  
  
}
