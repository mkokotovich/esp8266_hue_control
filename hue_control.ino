#include <ESP8266WiFi.h>
#include "private.h"
#include "HueControl.h"
#include "AnalogReader.h"

#define DIMMER_DELAY 4

const char* ssid     = private_ssid;
const char* password = private_password;

const char bridge_ip[] = "192.168.86.100";  // Hue bridge IP
const String hueUsername = private_hueUsername;
const int bridge_port = 80;

const int LIGHT_LED_PIN = D0;
const int WORKING_LED_PIN = D4;
const int switchPin = D5;
const int dimmerPin = A0;

int dimmerValue = 0;
unsigned long lastDimmerRead = 0;
unsigned long lastDimmerPrint = 0;
int prevSwitchState = 0;
int currSwitchState = 0;

HueControl *hue = NULL;
AnalogReader *dimmer = NULL;

void turnOnLights(int dimmerValue)
{
  digitalWrite(LIGHT_LED_PIN, LOW);
  digitalWrite(WORKING_LED_PIN, LOW);
  hue->turnOnLights(ceil((float)dimmerValue/1024));
  digitalWrite(WORKING_LED_PIN, HIGH);
}

void turnOffLights()
{
  digitalWrite(LIGHT_LED_PIN, HIGH);
  digitalWrite(WORKING_LED_PIN, LOW);
  hue->turnOffLights();
  digitalWrite(WORKING_LED_PIN, HIGH);
}

void dimLights(int analogVal)
{
  digitalWrite(WORKING_LED_PIN, LOW);
  hue->dimLights(ceil((float)analogVal/1024));
  digitalWrite(WORKING_LED_PIN, HIGH);
}

void setup() {
  Serial.begin(115200);
  delay(10);

  //Set switch pin to input
  pinMode(switchPin, INPUT_PULLUP);
  // LED to mirror status of light 
  pinMode(LIGHT_LED_PIN, OUTPUT);
  // LED to indicate communications on network
  pinMode(WORKING_LED_PIN, OUTPUT);
  pinMode(dimmerPin, INPUT);
  
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  hue = new HueControl(String(bridge_ip), bridge_port, hueUsername);
  dimmer = new AnalogReader(0, 1024, 20, 985, 50, 2);

  //to indicate first time through loop
  prevSwitchState = -1;
}

void loop() {
  // Reset device if wifi is disconnected
  if (WiFi.status() == WL_DISCONNECTED)
  {
    Serial.println("Wifi diconnected, reset board");
    ESP.reset();
  }

  //Read dimmer, but not too often as that crashes wifi
  if (millis() - lastDimmerRead > DIMMER_DELAY)
  {
    dimmerValue = dimmer->addReading(analogRead(dimmerPin));
    if (millis() - lastDimmerPrint > 1000)
    {
      Serial.println(dimmerValue);
      lastDimmerPrint = millis();
    }
    lastDimmerRead = millis();

    // If light is on, update brightness
    if (prevSwitchState == LOW)
    {
        dimLights(dimmerValue);
    }
  }

  //Read switch
  currSwitchState = digitalRead(switchPin);
  if (currSwitchState != prevSwitchState && prevSwitchState != -1)
  {
    // LOW means switch is closed (light is on) due to the PULLUP resister
    if (currSwitchState == LOW)
    {
      turnOnLights(dimmerValue);
    }
    else
    {
      turnOffLights();
    }
  }
  prevSwitchState = currSwitchState;

}
