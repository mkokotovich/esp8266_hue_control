#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>


// The port to listen for OTA updates
#define OTA_LISTEN_PORT          8080

ESP8266WebServer httpServer(OTA_LISTEN_PORT);
ESP8266HTTPUpdateServer httpUpdater;
unsigned long lastUpdateCheck = 0;
#define UPDATE_INTERVAL 50


void startUpdateServer(void)
{
    httpUpdater.setup(&httpServer);
    httpServer.begin();
    Serial.println("Update server started on port: " + String(OTA_LISTEN_PORT));
}


void handleOTAUpdate(void)
{
    // Handle OTA Update
    if (millis() - lastUpdateCheck > UPDATE_INTERVAL)
    {
        httpServer.handleClient();
        lastUpdateCheck = millis();
    }
}
