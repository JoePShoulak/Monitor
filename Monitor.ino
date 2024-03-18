#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Eventually.h>
#include "MenuItem.h"
#include "CircularBuffer.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_RESET -1
#define SCREEN_ADDRESS 0x3C
#define DEFAULT_MODE monitor
#define DEFAULT_CHANNEL 256
#define SELECT_BTN 12
#define CYCLE_BTN 11

enum Mode {
  monitor,
  transmit,
  recieve
};

long baudRateOptions[] = { 9600, 115200 };
MenuItem menuBaudRate(baudRateOptions, 2, "BR:");

Mode mode = DEFAULT_MODE;
CircularBuffer<String, 6> messages;
EvtManager mgr(true);  // true to manage memory
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET);

bool cycleButtonAction() {
  // TODO: This should change which menu item is selected, not just cyclke the baudrate
  menuBaudRate.cycle();
  Serial.end();
  Serial.begin(menuBaudRate.getValue());
  updateDisplay();

  return true;
}

EvtPinListener cycleButtonListener(CYCLE_BTN, 100, (EvtAction)cycleButtonAction);

void displaySettings() {
  display.setCursor(0, 0);

  display.print(menuBaudRate.getText());

  if (mode == monitor)
   display.print("MON ");
  else if (mode == transmit)
   display.print("TX  ");
  else if (mode == recieve)
   display.print("RX  ");
  else
   display.print("ERR ");

  display.print("CH:");
  display.print(DEFAULT_CHANNEL);
  display.print(" ");

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

void updateDisplay() {
  display.clearDisplay();
  displaySettings();
  displayData();
  display.display();
}

void setup() {
  pinMode(SELECT_BTN, INPUT);
  pinMode(CYCLE_BTN, INPUT);

  mgr.addListener(&cycleButtonListener);

  Serial.begin(menuBaudRate.getValue());

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("Screen init failed");
    while (true)
      ;
  }

  display.setTextColor(SSD1306_WHITE);
  updateDisplay();
}

void loop() {
  mgr.loopIteration();

  if (Serial.available()) {
    addData(Serial.readString());
    updateDisplay();
  }
}
