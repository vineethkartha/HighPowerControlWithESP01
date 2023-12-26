#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <fauxmoESP.h>
#define ONBOARDLED 1
#define LED 2
#define RelayPin1 0
fauxmoESP fauxmo;
const char* deviceName = "Heater";// Set your Static IP address
IPAddress local_IP(192, 168, 1, 51);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

void setup() { 
  Serial.begin(9600);
  pinMode(RelayPin1, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(ONBOARDLED, OUTPUT);
  digitalWrite(ONBOARDLED,LOW);
  digitalWrite(LED,LOW);
  // This should be negated logic
  digitalWrite(RelayPin1, HIGH);

 // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

  if (connectWifi()) {
    // Setup fauxmo
     Serial.println((String)"Adding" +deviceName+  " device");
     fauxmo.setPort(80);  
     fauxmo.enable(true);
     fauxmo.addDevice(deviceName);   
  }

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    Serial.print("Device: ");Serial.print(device_name);
    Serial.print(" state");
    if(state) {
      Serial.println("ON!");
      // ESP8266 uses and inverted logic with arduino
      digitalWrite(RelayPin1, LOW);
    }
    else {
      Serial.println("off");
      digitalWrite(RelayPin1, HIGH);
    }
  });
}

void loop() {
  fauxmo.handle();
}

boolean connectWifi() {
  // Let us connect to WiFi
  WiFi.begin("B1117", "air44189");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ONBOARDLED,LOW);
    digitalWrite(LED,LOW);
    delay(500);
    Serial.print(".");
  }
  Serial.println(".......");
  Serial.println("WiFi Connected....IP Address:");
  Serial.println(WiFi.localIP());
  
  digitalWrite(LED,HIGH);
  digitalWrite(ONBOARDLED,HIGH);
  return true;
}