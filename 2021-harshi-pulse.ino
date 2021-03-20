/********************************************************************
 * Project        :   Remote Patient Monitoring System - Pulse Module
 * Input          :   Pressure Sensor
 * Output         :   madBlocks DB
 * H/W Engineer   :   Madhu Parvathaneni
 * Programmer     :   Madhu Parvathaneni
 * Client         :   Harshi
 * Date           :   13th March 2021
 ********************************************************************/

#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Madhu P";
const char* password = "madhu2022";

//Your Domain name with URL path or IP address with path
String serverageName = "http://madblocks.tech/XXXXXXX/XXXXXXXX/XXXXXX";
String servergenderName = "http://madblocks.tech/XXXXXXX/XXXXXXXX/XXXXXX";
String serverpulseName = "http://madblocks.tech/XXXXXXX/XXXXXXXX/XXXXXX";

int age=0;
int gender=0;
int statusLED=2;

#define LED_ON 1
#define LED_OFF 0

void setup() { 
  Serial.begin(9600);
  pinMode(statusLED,OUTPUT);
  digitalWrite(statusLED,LED_OFF);
  delay(2000);
  Serial.println("Welcome");
  bsp_init();
  digitalWrite(statusLED,LED_ON);
}

void loop() {
  digitalWrite(statusLED,LED_ON);
  while(!Serial.available());
  while(Serial.available()) {
    int sys=Serial.parseInt();
    int dia=Serial.parseInt();
    int pulse=Serial.parseInt();
    digitalWrite(statusLED,LED_OFF);

    if(sys>0 && dia>0 && pulse>0) {

      Serial.print("Sys: ");
      Serial.print(sys);
      Serial.print(", Dia: ");
      Serial.print(dia);
      Serial.print(", Pulse: ");
      Serial.println(pulse);
      delay(1000);
      while(age==0) {
        age=age_read_from_madblocksdb();  
      }
      while(gender==0) {
        gender=gender_read_from_madblocksdb();
      }
      Serial.print("Age: ");
      Serial.print(age);
      Serial.print(", Gender: ");
      Serial.println(gender);
      int k=upload_sensory_feed(age,gender,sys,dia,pulse);
      while(k==0) {
        k=upload_sensory_feed(age,gender,sys,dia,pulse);
      }
      Serial.println("Data Uploaded");
      age=0;
      gender=0;
    }
  }
}

void bsp_init(void) {
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

int age_read_from_madblocksdb(void) {
  
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverageName;
            
      http.begin(serverPath.c_str());
       
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        //Serial.print("HTTP Response code: ");
        //Serial.println(httpResponseCode);
        String payload = http.getString();
        return(payload.toInt());
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        return(0);
        
      }
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
}

int gender_read_from_madblocksdb(void) {
  
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = servergenderName;
            
      http.begin(serverPath.c_str());
       
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        //Serial.print("HTTP Response code: ");
        //Serial.println(httpResponseCode);
        String payload = http.getString();
        return(payload.toInt());
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        return(0);
        
      }
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
}

int upload_sensory_feed(int a, int g, int s, int d, int p) {
  
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverpulseName +"?age=" + String(a) + "&gender=" + String(g) + "&sys=" + String(s) + "&dia=" + String(d)+ "&pulse=" + String(p);
            
      http.begin(serverPath.c_str());
       
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        //Serial.print("HTTP Response code: ");
        //Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        return(1);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        return(0);
        
      }
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
}
