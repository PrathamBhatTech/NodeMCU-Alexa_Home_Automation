#include <ESP8266WiFi.h>
#include <Espalexa.h>


// define the GPIO connected with Relays

#define RelayPin1 5  //D1

//direct callback functions
void device1Changed(uint8_t brightness);

// device names
String Device_1_Name = "Table Lamp";

Espalexa espalexa;

// prototypes
boolean connectWifi();

// WiFi Credentials
const char* ssid = "Wifi_SSID";
const char* password = "Wifi_Password";

boolean wifiConnected = false;

void setup()
{
  Serial.begin(115200);

  pinMode(RelayPin1, OUTPUT);

  // Initialise wifi connection
  wifiConnected = connectWifi();

  if (wifiConnected)
  {
    // Define your devices here.
    espalexa.addDevice(Device_1_Name, device1Changed);

    espalexa.begin();
  }
  else
  {
    while (1)
    {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }
}

void loop()
{
  espalexa.loop();
  delay(1);
}

//our callback functions
void device1Changed(uint8_t brightness){

  //Control the device
  if (brightness == 255)
    {
      digitalWrite(RelayPin1, HIGH);
      Serial.println("Device1 ON");
    }
  else if (brightness == 0)
  {
    digitalWrite(RelayPin1, LOW);
    Serial.println("Device1 OFF");
  }
  else
  {
    int brigh_perc = (brightness/255.)*100;
    analogWrite(RelayPin1, brightness);
    Serial.print("Device1 Brightness: ");
    Serial.print(brigh_perc);
    Serial.println("%");
    
  }
}


// connect to wifi  returns true if successful or false if not
boolean connectWifi()
{
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state) {
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}
