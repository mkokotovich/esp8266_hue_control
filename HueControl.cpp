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

    Serial.println("Connected, writing command:");

    // This will send the request to the server

    String request = "PUT /api/" + m_hueUserName + "/groups/0/action" + "\r\n" +
    "Host: " + m_bridgeIP + ":" + String(m_bridgePort) + "\r\n" +
    "User-Agent: ESP8266/1.0" + "\r\n" +
    "Connection: keep-alive" + "\r\n" +
    "Content-type: text/xml; charset=\"utf-8\"" + "\r\n" +
    "Content-Length: " + String(command.length()) + "\r\n" +
    "\r\n" +
    command + "\r\n";

    Serial.println("request: " + request);
    m_client.write((const uint8_t *)request.c_str(), request.length());

    // Read all the lines of the reply from server and print them to Serial
    // Wait up to READ_TIMEOUT ms

    int time_waited = 0;
    while (!m_client.available() && time_waited < READ_TIMEOUT)
    {
        Serial.println("No reply, have waited " + String(time_waited) + " ms");
        time_waited += READ_WAIT_INTERVAL;
        delay(READ_WAIT_INTERVAL);
    }
    while(m_client.available())
    {
      //TODO: Make the reading faster
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
    m_client.setNoDelay(true);
    Serial.println("Connected");
}
