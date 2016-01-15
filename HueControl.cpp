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

void HueControl::turnOnLights()
{
    setHue(hue_on);
}

void HueControl::turnOffLights()
{
    setHue(hue_off);
}

void HueControl::setHue(const String &command)
{
    reconnectToBridgeIfNeeded();

    Serial.println("Connected, writing command:" + command);

    // This will send the request to the server

    m_client.println("PUT /api/" + m_hueUserName + "/groups/0/action");
    m_client.println("Host: " + m_bridgeIP + ":" + String(m_bridgePort));
    m_client.println("User-Agent: ESP8266/1.0");
    m_client.println("Connection: keepalive");
    m_client.println("Content-type: text/xml; charset=\"utf-8\"");
    m_client.println("Content-Length: " + String(command.length()));
    m_client.println();
    m_client.println(command);


    // Read all the lines of the reply from server and print them to Serial
    while(m_client.available())
    {
        String line = m_client.readStringUntil('\r');
        Serial.print(line);
    }

    Serial.println("done");
}

void HueControl::reconnectToBridgeIfNeeded()
{
    if (!m_client.connected())
    {
        Serial.println("Client is disconnected");
        connectToBridge();
    }
}

void HueControl::connectToBridge()
{
    Serial.println("Connecting to " + m_bridgeIP);

    if (!m_client.connect(m_bridgeIP.c_str(), m_bridgePort))
    {
        Serial.println("Connection failed");
        return;
    }
    Serial.println("Connected");
}
