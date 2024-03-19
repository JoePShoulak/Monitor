#include <Wire.h>
#include <Eventually.h>
#include <U8x8lib.h>
#include "CircularBuffer.h"
// #include "MenuItem.h"

#define SELECT_BTN 12
#define CYCLE_BTN 11

CircularBuffer<String, 6> messages;
CircularBuffer<String, 4> menuItemBuffer = { "NONE", "BAUDRATE", "MODE"};
CircularBuffer<long, 2> baudRateBuffer = { 9600, 115200 };
CircularBuffer<String, 3> modeBuffer = { "MN", "TX", "RX" };
CircularBuffer<int, 3> channelBuffer = { 1, 2, 3 };

EvtManager evtMgr(true);  // true to manage memory
U8X8_SSD1306_128X64_NONAME_HW_I2C display(U8X8_PIN_NONE);

bool selectButtonAction() {
  menuItemBuffer.next();
  drawMenu();

  return true;
}

bool cycleButtonAction() {
  String selection = menuItemBuffer.current();

  if (selection == "NONE")
    return true;

  if (selection == "BAUDRATE") {
    baudRateBuffer.next();
  } else if (selection == "MODE") {
    modeBuffer.next();
  } else if (selection == "CHANNEL") {
    channelBuffer.next();
  }

  drawMenu();

  return true;
}

EvtPinListener selectButtonListener(SELECT_BTN, 100, (EvtAction)selectButtonAction);
EvtPinListener cycleButtonListener(CYCLE_BTN, 100, (EvtAction)cycleButtonAction);

void updateMonitor(String datum) {
  datum.trim();
  messages.append(datum);

  int i = 2;

  for (auto& msg : messages)
    display.drawString(0, i++, msg.c_str());
}

template<typename T, int N>
void printMenuItem(int x, const CircularBuffer<T, N>& buffer, String name, String prefix = "") {
  display.setInverseFont(menuItemBuffer.current() == name);

  if (prefix)
    display.drawString(x++, 0, prefix.c_str());

  display.drawString(x, 0, String(buffer.current()).c_str());
}

void drawMenu() {
  display.drawString(0, 0, "                ");

  printMenuItem(0, baudRateBuffer, "BAUDRATE", "B");
  printMenuItem(8, modeBuffer, "MODE");
  printMenuItem(12, channelBuffer, "CHANNEL", "C");

  display.setInverseFont(0);
  display.drawString(0, 1, "----------------");
}

void setup() {
  display.begin();
  display.setPowerSave(0);
  display.setFont(u8x8_font_chroma48medium8_r);

  pinMode(SELECT_BTN, INPUT);
  pinMode(CYCLE_BTN, INPUT);

  evtMgr.addListener(&selectButtonListener);
  evtMgr.addListener(&cycleButtonListener);

  Serial.begin(baudRateBuffer.current());

  drawMenu();
}

void loop() {
  evtMgr.loopIteration();

  if (Serial.available())
    updateMonitor(Serial.readString());
}
