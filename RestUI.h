// Needs to be from https://github.com/mkokotovich/aREST until pull request is merged in
#include <aREST.h>
// Needs to be from https://github.com/mkokotovich/aREST_UI until pull request is merged in
#include <aREST_UI.h>

#define READ_TIMEOUT 10000 //10 seconds

// Create aREST instance
aREST_UI rest = aREST_UI();

// The port to listen for UI
#define UI_LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(UI_LISTEN_PORT);

// Declared in .ino
extern bool switchEnabled;
extern bool dimmerEnabled;

// Rest UI variables
String switch_enabled_status = "Enabled";
String dimmer_enabled_status = "Enabled";

// Rest UI functions
int enableSwitch(String command) {
  switchEnabled = true;
  switch_enabled_status = "Enabled";
}

int disableSwitch(String command) {
  switchEnabled = false;
  switch_enabled_status = "Disabled";
}

int enableDimmer(String command) {
  dimmerEnabled = true;
  dimmer_enabled_status = "Enabled";
}

int disableDimmer(String command) {
  dimmerEnabled = false;
  dimmer_enabled_status = "Disabled";
}


void setupRestUI()
{
    // Give name and ID to device
    rest.set_id("1");
    rest.set_name("esp8266");
    rest.title("Hue Control Configuration Page");

    // Display the switch enabled status
    rest.variable_label("switch_enabled_status", "Switch is", &switch_enabled_status);

    // Register the functions with the rest UI
    rest.function_button("enableSwitch", "Enable switch", enableSwitch);
    rest.function_button("disableSwitch", "Disable switch", disableSwitch);

    // Display the dimmer enabled status
    rest.variable_label("dimmer_enabled_status", "Dimmer is", &dimmer_enabled_status);

    // Register the functions with the rest UI
    rest.function_button("enableDimmer", "Enable dimmer", enableDimmer);
    rest.function_button("disableDimmer", "Disable dimmer", disableDimmer);
}

void startRestUIServer(void)
{
  // Start the server
  server.begin();
  Serial.println("Rest UI server started on port: " + String(UI_LISTEN_PORT));
}

void handleRestUI(void)
{
    // Handle REST calls
    WiFiClient client = server.available();
    if (!client)
    {
        return;
    }
    delay(50); //devices seems to choke if we read too fast, even with below wait
    unsigned long startMillis = millis();
    while (!client.available() && (millis() - startMillis) < READ_TIMEOUT)
    {
        yield();
    }
    if (!client.available())
    {
        Serial.println("Read timeout reached, dropping client");
        client.stop();
        return;
    }

    rest.handle(client);
}
