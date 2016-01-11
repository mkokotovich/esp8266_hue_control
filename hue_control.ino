#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid     = "your-ssid";
const char* password = "your-password";

const char hueHubIP[] = "192.168.68.100";  // Hue hub IP
const char hueUsername[] = "newdeveloper";  // Hue username
const int hueHubPort = 80;

const int switchPin = D5;
int prevSwitchState = 0;
int currSwitchState = 0;

/* setHue() is our main command function, which needs to be passed a
 * properly formatted command string in JSON format (basically a Javascript style array of variables
 * and values. It then makes a simple HTTP PUT request to the Bridge at the IP specified at the start.
 */
void setHue(const String &command)
{
  HTTPClient http;
  String url = "/api/" + String(hueUsername) + "/groups/0/action";
  http.begin(hueHubIP, hueHubPort, url.c_str());
  
  Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.POST(command);
  if (httpCode)
  {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  }
  else
  {
      Serial.print("[HTTP] GET... failed, no connection or no HTTP server\n");
  }
  
  http.end();
}

void turnOnLights()
{
  setHue("{\"on\":true, \"bri\":255}");
}

void turnOffLights()
{
  setHue("{\"on\":false");
}

void dimLights(float percentage)
{
  int brightness = ceil(255*(percentage/100));
  setHue("{\"bri\":" + String(percentage) + "}");
}



void setup() {
  Serial.begin(115200);
  delay(10);

  //Set switch pin to input
  pinMode(switchPin, INPUT);
  
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
    if (currSwitchState == HIGH)
    {
      turnOnLights();
    }
    else
    {
      turnOffLights();
    }
    prevSwitchState = currSwitchState;
  }

  //Read dimmer
  //TODO
}
