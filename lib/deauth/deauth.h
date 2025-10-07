#pragma once
#include <Arduino.h>

#define LED_PIN 2
#define DISASSOC_REASON 0x08
#define DISASSOC_COUNT 20

void sendDisassoc(uint8_t *bssid);