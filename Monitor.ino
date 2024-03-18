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
#define DEFAULT_CHANNEL 256
#define SELECT_BTN 12
#define CYCLE_BTN 11

int channel = DEFAULT_CHANNEL;
EvtManager mgr(true);  // true to manage memory

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET);

CircularBuffer<long, 2> baudRates;
CircularBuffer<String, 6> messages;
CircularBuffer<String, 3> modes;

bool selectButtonAction() {
  // TODO: implement
  return true;
}

bool cycleButtonAction() {
  // TODO: This should change which menu item is selected, not just cycle the baudrate
  baudRates.next();
  Serial.end();
  Serial.begin(baudRates.current());
  updateDisplay();

  return true;
}

EvtPinListener selectButtonListener(SELECT_BTN, 100, (EvtAction)selectButtonAction);
EvtPinListener cycleButtonListener(CYCLE_BTN, 100, (EvtAction)cycleButtonAction);

void displaySettings() {
  int x;
  int y;

  int oX;
  int oY;

  unsigned int h;
  unsigned int w;

  display.setCursor(0, 0);

  display.print("BR:");
  String bR = String(baudRates.current());
  x = display.getCursorX();
  y = display.getCursorY();
  display.print(bR);
  display.print(" ");


  display.getTextBounds(bR, x, y, &oX, &oY, &w, &h);

  if (modes.current() == "monitor")
    display.print("MON");
  else if (modes.current() == "transmit")
    display.print("TX");
  else if (modes.current() == "recieve")
    display.print("RX");
  else
    display.print("ERR");

  display.print(" ");

  display.print(channel);
  display.print(" ");

  display.drawFastHLine(x, 8, w, SSD1306_WHITE);
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

  baudRates.append(9600);
  baudRates.append(115200);
  
  modes.append("monitor");
  modes.append("transmit");
  modes.append("recieve");

  mgr.addListener(&selectButtonListener);
  mgr.addListener(&cycleButtonListener);

  Serial.begin(baudRates.current());

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
