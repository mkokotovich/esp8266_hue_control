#ifndef _HUE_CONTROL_
#define _HUE_CONTROL_

#include <WifiClient.h>

#define READ_TIMEOUT 1000
#define READ_WAIT_INTERVAL 50

class HueControl {
    public:
        HueControl(const String &bridgeIP, int bridgePort, const String &hueUserName);
        ~HueControl();
        
        void turnOnLights();
        void turnOffLights();

    private:
        void setHue(const String &command);
        void reconnectToBridgeIfNeeded();
        void connectToBridge();

        const String m_bridgeIP;
        int m_bridgePort;
        const String m_hueUserName;

        WiFiClient m_client;

        const String hue_on="{\"on\":true, \"bri\":255}";
        const String hue_off="{\"on\":false}";

};

#endif
