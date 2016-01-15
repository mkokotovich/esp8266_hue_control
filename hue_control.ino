#include <ESP8266WiFi.h>
#include "private.h"
#include "HueControl.h"

const char* ssid     = private_ssid;
const char* password = private_password;

const char bridge_ip[] = "192.168.86.100";  // Hue bridge IP
const String hueUsername = private_hueUsername;
const int bridge_port = 80;

const int LIGHT_LED_PIN = D0;
const int WORKING_LED_PIN = D4;
const int switchPin = D5;
int prevSwitchState = 0;
int currSwitchState = 0;

HueControl *hue = NULL;

void turnOnLights()
{
  digitalWrite(LIGHT_LED_PIN, LOW);
  digitalWrite(WORKING_LED_PIN, LOW);
  hue->turnOnLights();
  digitalWrite(WORKING_LED_PIN, HIGH);
}

void turnOffLights()
{
  digitalWrite(LIGHT_LED_PIN, HIGH);
  digitalWrite(WORKING_LED_PIN, LOW);
  hue->turnOffLights();
  digitalWrite(WORKING_LED_PIN, HIGH);
}

void dimLights(float percentage)
{
  int brightness = ceil(255*(percentage/100));
  //setHue("{\"bri\":" + String(brightness) + "}");
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
}

void loop() {
  // Reset device if wifi is disconnected
  if (WiFi.status() == WL_DISCONNECTED)
  {
    ESP.reset();
  }

  //Read switch
  currSwitchState = digitalRead(switchPin);
  if (currSwitchState != prevSwitchState)
  {
    // LOW means switch is closed due to the PULLUP resister
    prevSwitchState = currSwitchState;
    if (currSwitchState == LOW)
    {
      turnOnLights();
    }
    else
    {
      turnOffLights();
    }
  }

  //Read dimmer
  //TODO
}
