#include "HueControl.h"

HueControl::HueControl(const String &bridgeIP, int bridgePort, const String &hueUserName) :
    m_bridgeIP(bridgeIP),
    m_bridgePort(bridgePort),
    m_hueUserName(hueUserName)
{
}

HueControl::~HueControl()
{
    m_client.stop();
}

void HueControl::turnOnLights(int percentage)
{
    int brightness = getBrightnessFromPercentage(percentage);
    setHue("{\"on\":true, \"bri\":" + String(brightness) + "}");
}

void HueControl::turnOffLights()
{
    setHue("{\"on\":false}");
}

void HueControl::dimLights(int percentage)
{
    int brightness = getBrightnessFromPercentage(percentage);
    setHue("{\"bri\":" + String(brightness) + "}");
}

int HueControl::getBrightnessFromPercentage(int percentage)
{
    return ceil(255*((float)percentage/100));
}

void HueControl::setHue(const String &command)
{
    connectToBridge();

    String request = "PUT /api/" + m_hueUserName + "/groups/0/action" + "\r\n" +
    "Host: " + m_bridgeIP + ":" + String(m_bridgePort) + "\r\n" +
    "User-Agent: ESP8266/1.0" + "\r\n" +
    "Connection: close" + "\r\n" +
    "Content-type: text/xml; charset=\"utf-8\"" + "\r\n" +
    "Content-Length: " + String(command.length()) + "\r\n" +
    "\r\n" +
    command + "\r\n";
    
    Serial.println("Writing request: \n" + request);
    
    m_client.write((const uint8_t *)request.c_str(), request.length());

#if READ_RESPONSE
    delay(50); //hue seems to choke if we read too fast, even with below wait
    int time_waited = 0;
    unsigned long startMillis = millis();
    while (!m_client.available() && (time_waited = (millis() - startMillis)) < READ_TIMEOUT)
    {
        Serial.println("No reply, have waited " + String(time_waited) + " ms");
        yield();
    }
    char buf[1024];
    while(m_client.available())
    {
        memset(buf, 0, sizeof(buf));
        int bytes = m_client.read((uint8_t*)buf, sizeof(buf));
        Serial.print(buf);
    }
#endif

    m_client.stop();
    Serial.println("done");
}

void HueControl::connectToBridge()
{
    Serial.println("Connecting to " + m_bridgeIP);

    if (!m_client.connect(m_bridgeIP.c_str(), m_bridgePort))
    {
        Serial.println("Connection failed");
        return;
    }
    m_client.setNoDelay(true);
    Serial.println("Connected");
}
