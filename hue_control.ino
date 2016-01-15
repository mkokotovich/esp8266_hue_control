#include <ESP8266WiFi.h>
#include "private.h"

const char* ssid     = private_ssid;
const char* password = private_password;

const char bridge_ip[] = "192.168.86.100";  // Hue bridge IP
const String hueUsername = private_hueUsername;
const int bridge_port = 80;

const String hue_on="{\"on\":true, \"bri\":255}";
const String hue_off="{\"on\":false}";

const int LIGHT_LED_PIN = D0;
const int WORKING_LED_PIN = D4;
const int switchPin = D5;
int prevSwitchState = 0;
int currSwitchState = 0;

/* setHue() is our main command function, which needs to be passed a
 * properly formatted command string in JSON format (basically a Javascript style array of variables
 * and values. It then makes a simple HTTP PUT request to the Bridge at the IP specified at the start.
 */
void setHue(const String &command)
{
  Serial.print("Connecting to ");
  Serial.println(bridge_ip);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(bridge_ip, bridge_port))
  {
    Serial.println("Connection failed");
    return;
  }
  Serial.println("Connected, writing command:" + command);
  
  // This will send the request to the server

  client.println("PUT /api/" + hueUsername + "/groups/0/action");
  client.println("Host: " + String(bridge_ip) + ":" + String(bridge_port));
  client.println("User-Agent: ESP8266/1.0");
  client.println("Connection: close");
  client.println("Content-type: text/xml; charset=\"utf-8\"");
  client.println("Content-Length: " + String(command.length()));
  client.println();
  client.println(command);


  // Read all the lines of the reply from server and print them to Serial
/*
  while(client.available())
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
*/

  Serial.println("done");
  client.stop();
}

void turnOnLights()
{
  digitalWrite(LIGHT_LED_PIN, LOW);
  digitalWrite(WORKING_LED_PIN, LOW);
  setHue(hue_on);
  digitalWrite(WORKING_LED_PIN, HIGH);
}

void turnOffLights()
{
  digitalWrite(LIGHT_LED_PIN, HIGH);
  digitalWrite(WORKING_LED_PIN, LOW);
  setHue(hue_off);
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
