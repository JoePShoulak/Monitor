#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "CircularBuffer.h"

#include <MenuItem.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET);

#define DEFAULT_BAUDRATE 9600
#define DEFAULT_MODE monitor

#define BUTTON1_PIN 11
#define BUTTON2_PIN 12

enum Mode {
  monitor,
  transmit,
  recieve
};

int baudRate = DEFAULT_BAUDRATE;
Mode mode = DEFAULT_MODE;
CircularBuffer<String, 7> messages;

long baudRateOptions[] = { 9600, 115200 };
MenuItem menuBaudRate(baudRateOptions, 2, "BR:");

void displaySettings() {
  display.setCursor(0, 0);

  // BaudRate
  display.print(menuBaudRate.getText());
  // display.print("BR:");
  // display.print(baudRate);
  // display.print(" ");

  // Mode
  switch (mode) {
    case monitor:
      display.print("MON ");
      break;
    case transmit:
      display.print("TX  ");
      break;
    case recieve:
      display.print("RX  ");
      break;
    default:
      display.print("ERR ");
      break;
  }

  display.drawFastHLine(0, 10, SCREEN_WIDTH, SSD1306_WHITE);
}

void addData(String datum) {
  messages.append(datum.trim());
}

void displayData() {
  display.setCursor(0, 12);

  for (auto& msg : messages) {
    display.println(msg);
  }
}

void update() {
  display.clearDisplay();

  displaySettings();
  displayData();

  display.display();
}

void setup() {
  // Button test, pay no mind
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(baudRate);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("screen allocation failed");
    while (true)
      ;
  }

  display.setTextColor(SSD1306_WHITE);

  // menuBaudRate.cycle();
  // menuBaudRate.cycle();
  update();
}

void loop() {
  if (Serial.available()) {
    addData(Serial.readString());
    update();
  }

  // Button test, pay no mind
  bool b1 = digitalRead(BUTTON1_PIN);
  bool b2 = digitalRead(BUTTON2_PIN);
  digitalWrite(LED_BUILTIN, b1 ^ b2);
}
