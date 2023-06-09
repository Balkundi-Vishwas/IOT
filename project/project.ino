//Include the library files
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3A0W_KY-J"
#define BLYNK_TEMPLATE_NAME "SmartIrrigation"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <iostream>
using namespace std;

char auth[] = "PPZZU9hVH7JZwwKtJsaRK51mppIrrSR9";//Enter your Auth token
char ssid[] = "Galaxy";//Enter your WIFI name
char pass[] = "12345678";//Enter your WIFI password

BlynkTimer timer;
bool Relay = 0;
bool eventTrigger=false;
//Define component pins
#define sensor A0
#define waterPump D3

void setup() {
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, LOW);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  //Call the function
  timer.setInterval(100L, soilMoistureSensor);
}

//Get the button value
BLYNK_WRITE(V2) {
  Relay = param.asInt();

  if (Relay == 1) {
    Serial.println("Turned on");
    digitalWrite(waterPump, HIGH);
  } else {
    Serial.println("Turned off");

    digitalWrite(waterPump, LOW);
  }
}

//Get the soil moisture values
void soilMoistureSensor() {
  int value = analogRead(sensor);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;
  value =2*value;
  //delay(1000);
    if (value <= 75) {
    Serial.println("Turned on");
    digitalWrite(waterPump, HIGH);
    Blynk.virtualWrite(V5, 1);
    Blynk.virtualWrite(V4, 0);
    if(eventTrigger== false){
      eventTrigger=true;
     // Blynk.notify("Water has turned on");
     Blynk.logEvent("alert","Water has turned on");
    }
    //Blynk.logEvent("alert","Temp above 30 degree");
  } else {
    Serial.println("Turned off");
    // Blynk.notify("Water has turned offf");
    eventTrigger=false;
    digitalWrite(waterPump, LOW);
    Blynk.virtualWrite(V5, 0);
    Blynk.virtualWrite(V4, 1);
  }
  Serial.println(value);
  cout<<"hello";
  Blynk.virtualWrite(V1, value);

}

void loop() {
  //Serial.println("Running.....");
  soilMoistureSensor();
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}