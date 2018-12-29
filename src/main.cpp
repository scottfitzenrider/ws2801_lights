#include "PubSubClient.h"
#include "WS2801.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

char ID[32];
byte mac[6];
ESP8266WiFiMulti WiFiMulti;

int clockPin = 12;
int dataPin = 13;
int striplen = 240;
WS2801 strip;

String topic;
WiFiClient WiFiClient;
PubSubClient client(WiFiClient);
const char *server = "raspberrypiv304";

void callback(char *topic, byte *payload, unsigned int length) {
  int n;
  byte r, g, b;
  // Serial.print("Message arrived [");
  // Serial.print(topic);
  // Serial.print("] ");
  // Serial.printf("%d %d\n", length, (int)floor(length / 3));
  strip.clearStrip();
  if (length > 0) {
    for (n = 0; n < min(striplen, (int)floor(length / 3)); n++) {
      r = payload[n * 3];
      g = payload[n * 3 + 1];
      b = payload[n * 3 + 2];
      // Serial.printf("pixel %d red=%d green=%d blue=%d datalen %d length
      // %d\n",
      // n, r, g, b, (int)(length / 3), length);
      strip.setPixel(n, r, g, b);
    }
  }
  //  strip.show();
  delay(500);
  strip.show();
  //  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(ID)) {
      Serial.println("connected");
      client.subscribe(topic.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  strip.begin(striplen, clockPin, dataPin, true);
  WiFiMulti.addAP("SFRouter_2GEXT", "flounder99");
  WiFiMulti.addAP("SFRouter", "flounder99");
  WiFiMulti.addAP("SFRouter_shed", "flounder99");
  // WiFiMulti.addAP("SFRouter-dlink", "flounder99");
  Serial.println("\n");
  Serial.print("Wait for WiFi... ");

  WiFi.macAddress(mac);
  sprintf(ID, "esp8266_%02X%02X%02X", mac[3], mac[4], mac[5]);
  topic = "/ledstring/" + String(ID) + "/data";
  Serial.printf("hostname %d\n", WiFi.hostname(ID));
  Serial.printf("hostname %s\n", WiFi.hostname().c_str());
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(ID);
  Serial.println(topic);
  client.setServer(server, 1883);
  client.setCallback(callback);
}
void loop() {
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
