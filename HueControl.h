#ifndef _HUE_CONTROL_
#define _HUE_CONTROL_

#include <WifiClient.h>

#define READ_RESPONSE 1
#define READ_TIMEOUT 1000

class HueControl {
    public:
        HueControl(const String &bridgeIP, int bridgePort, const String &hueUserName);
        ~HueControl();
        
        void turnOnLights(float dimmerPercentage=1.0);
        void turnOffLights();
        void dimLights(float percentage);

    private:
        int getBrightnessFromPercentage(float percentage);
        void setHue(const String &command);
        void connectToBridge();

        const String m_bridgeIP;
        int m_bridgePort;
        const String m_hueUserName;

        WiFiClient m_client;
};

#endif
