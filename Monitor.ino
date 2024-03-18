#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Eventually.h>
#include "CircularBuffer.h"
// #include "MenuItem.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_RESET -1
#define SCREEN_ADDRESS 0x3C
#define DEFAULT_CHANNEL 256
#define SELECT_BTN 12
#define CYCLE_BTN 11

int channel = DEFAULT_CHANNEL;  // 1-125
EvtManager mgr(true);           // true to manage memory

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET);

CircularBuffer<long, 2> baudRates;
CircularBuffer<String, 6> messages;
CircularBuffer<String, 3> modes;
CircularBuffer<String, 3> menuItems;

bool selectButtonAction() {
  // TODO: implement
  return true;
}

bool cycleButtonAction() {
  // TODO: This should change which menu item is selected, not just cycle the baudrate
  baudRates.next();
  Serial.println(baudRates.current());
  // Serial.end();
  // Serial.begin(baudRates.current());
  // updateDisplay();

  return true;
}

EvtPinListener selectButtonListener(SELECT_BTN, 100, (EvtAction)selectButtonAction);
EvtPinListener cycleButtonListener(CYCLE_BTN, 100, (EvtAction)cycleButtonAction);

void displaySettings() {
  display.setCursor(0, 0);

  display.print("BR:");
  display.print(baudRates.current());
  display.print(" ");

  display.print(modes.current());

  display.print(" ");

  display.print(channel);
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
  Serial.begin(9600);
  Serial.println("\n=== New Session ===\n");

  // if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
  //   Serial.println("Screen init failed");
  //   while (true)
  //     ;
  // }

  pinMode(SELECT_BTN, INPUT);
  pinMode(CYCLE_BTN, INPUT);
  mgr.addListener(&selectButtonListener);
  mgr.addListener(&cycleButtonListener);

  baudRates.append(9600);
  baudRates.append(115200);

  Serial.println(baudRates.current());

  modes.append("MON");
  modes.append("TX");
  modes.append("RX");

  Serial.println(modes.current());

  menuItems.append("baudRate");
  menuItems.append("mode");
  menuItems.append("channel");

  Serial.println(menuItems.current());

  Serial.println("Buffers Populated");

  // display.setTextColor(SSD1306_WHITE);
  // updateDisplay();
}

void loop() {
  mgr.loopIteration();

  if (Serial.available()) {
    addData(Serial.readString());
    // updateDisplay();
  }
}
