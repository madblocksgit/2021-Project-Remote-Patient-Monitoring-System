/********************************************************************
 * Project        :   Remote Patient Monitoring System - Pulse Module
 * Input          :   Temperature Sensor
 * Output         :   madBlocks DB, LCD Display
 * H/W Engineer   :   Madhu Parvathaneni
 * Programmer     :   Madhu Parvathaneni
 * Client         :   Harshi
 * Date           :   13th March 2021
 ********************************************************************/
 
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Madhu P";
const char* password = "madhu2022";

//Your Domain name with URL path or IP address with path
String serverageName = "http://madblocks.tech/projects/harshi/ageread.php";
String servergenderName = "http://madblocks.tech/projects/harshi/genderread.php";
String servertempName = "http://madblocks.tech/projects/harshi/update_temp.php";

int age=0;
int gender=0;

LiquidCrystal lcd(19,18,5,17,16,4); // rs, en, d4, d5, d6, d7
int buzzer=23;
int sensor=34;


#define BUZZER_ON 1
#define BUZZER_OFF 0

#define HAND_DETECTED 0
#define HAND_NOT_DETECTED 1

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
float temp;

int flag=0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();
  lcd.print("WELCOME");
  lcd.setCursor(0,1);
  lcd.print("Place Hand");
  pinMode(buzzer,OUTPUT);
  pinMode(sensor,INPUT);
  digitalWrite(buzzer,BUZZER_OFF);
  bsp_init();   
}

void loop() {

  if(digitalRead(sensor)==HAND_DETECTED && flag==0) {
    flag=1;
    lcd.clear();
    lcd.print("Temp: ");
    digitalWrite(buzzer,BUZZER_ON);
    delay(3000);
    temp=checkBodyTemperature();
    Serial.print("Temp: ");
    Serial.println(temp);
    lcd.print(temp);
    delay(5000);
    digitalWrite(buzzer,BUZZER_OFF);
    delay(1000);
    digitalWrite(buzzer,BUZZER_ON);
    while(age==0) {
        age=age_read_from_madblocksdb();  
    }
    while(gender==0) {
       gender=gender_read_from_madblocksdb();
    }
    digitalWrite(buzzer,BUZZER_OFF);
    lcd.setCursor(0,1);
    lcd.print("Age: ");
    Serial.print("Age: ");
    Serial.print(age);
    lcd.print(age);
    lcd.setCursor(8,1);
    lcd.print(gender);
    Serial.print(", Gender: ");
    Serial.println(gender);
    delay(4000);
    int k=upload_sensory_feed(age,gender,temp);
    while(k==0) {
      k=upload_sensory_feed(age,gender,temp);
    }
    Serial.println("Data Uploaded");
    lcd.clear();
    lcd.print("Data Uploaded");
    delay(4000);
    age=0;
    gender=0;
  }
  else if(digitalRead(sensor)==HAND_NOT_DETECTED && flag==1) {
    flag=0;
    lcd.clear();
    lcd.print("Place Hand");
  }
}

void bsp_init(void) {
  mlx.begin(); 
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

float checkBodyTemperature(void) {
  float sum=0;
  for(int i=0;i<4;i++) {
    sum+=mlx.readObjectTempF();
  }
  sum=sum/4;
  if(sum>91)
    sum+=4.2;
  return(sum);
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

int upload_sensory_feed(int a, int g, int t) {
  
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = servertempName +"?age=" + String(a) + "&gender=" + String(g) + "&temp=" + String(t);
            
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
