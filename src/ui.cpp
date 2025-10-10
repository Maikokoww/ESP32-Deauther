#include "ui.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_ADDR     0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
String oledBuffer = "";

void ui_init() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED init failed");
    return;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.display();
}

void ui_print(const String& msg) {
  Serial.print(msg); // mirror to UART

  oledBuffer += msg;
  int lines = 0;
  for (char c : oledBuffer) if (c == '\n') lines++;

  if (lines > 6) {
    int skip = 0;
    for (int i = 0; i < oledBuffer.length(); i++) {
      if (oledBuffer[i] == '\n') {
        skip = i + 1;
        lines--;
        if (lines <= 6) break;
      }
    }
    oledBuffer = oledBuffer.substring(skip);
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(oledBuffer);
  display.display();
}

void ui_println(const String& msg) {
  ui_print(msg + "\n");
}

void ui_clear() {
  oledBuffer = "";
  display.clearDisplay();
  display.setCursor(0, 0);
  display.display();
}