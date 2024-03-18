// Default Libs
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Eventually.h>

// Custom Libs
#include <MenuItem.h>
#include "CircularBuffer.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET);

#define DEFAULT_BAUDRATE 9600
#define DEFAULT_MODE monitor

#define SELECT_BTN 12
#define CYCLE_BTN 11

enum Mode {
  monitor,
  transmit,
  recieve
};

int baudRate = DEFAULT_BAUDRATE;
Mode mode = DEFAULT_MODE;
CircularBuffer<String, 7> messages;
EvtManager mgr(true);  // true to manage memory

long baudRateOptions[] = { 9600, 115200 };
MenuItem menuBaudRate(baudRateOptions, 2, "BR:");

void displaySettings() {
  display.setCursor(0, 0);

  // BaudRate
  display.print(menuBaudRate.getText());

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
  datum.trim();
  messages.append(datum);
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

bool cycleBaud() {
  menuBaudRate.cycle();
  update();
}

void setup() {
  pinMode(SELECT_BTN, INPUT);
  pinMode(CYCLE_BTN, INPUT);

  Serial.begin(baudRate);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Screen init failed");
    while (true)
      ;
  }

  display.setTextColor(SSD1306_WHITE);

  update();

  mgr.addListener(new EvtPinListener(CYCLE_BTN, 100, (EvtAction)cycleBaud));
}

void loop() {
  mgr.loopIteration();
  
  if (Serial.available()) {
    addData(Serial.readString());
    update();
  }
}
