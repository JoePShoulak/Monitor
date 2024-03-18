#include <Wire.h>
#include <Eventually.h>
#include <U8x8lib.h>
#include "CircularBuffer.h"
// #include "MenuItem.h"

#define SELECT_BTN 12
#define CYCLE_BTN 11

CircularBuffer<String, 4> menuItem;
CircularBuffer<String, 6> messages;
CircularBuffer<long, 2> baudRate;
CircularBuffer<String, 3> mode;
CircularBuffer<int, 3> channel;

String menuItemOptions[] = { "NONE", "BAUDRATE", "MODE", "CHANNEL" };
long baudRateOptions[] = { 9600, 115200 };
String modeOptions[] = { "MN", "TX", "RX" };
int channelOptions[] = { 1, 2, 3 };

EvtManager evtMgr(true);  // true to manage memory
U8X8_SSD1306_128X64_NONAME_HW_I2C display(U8X8_PIN_NONE);

bool selectButtonAction() {
  menuItem.next();
  drawMenu();

  return true;
}

bool cycleButtonAction() {
  String selection = menuItem.current();

  if (selection == "NONE")
    return true;

  if (selection == "BAUDRATE") {
    baudRate.next();
    // TODO: Change the serial port
  } else if (selection == "MODE") {
    mode.next();
    // TODO: Change the mode}
  } else if (selection == "CHANNEL") {
    channel.next();
    // TODO: Change the channel
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
  display.setInverseFont(menuItem.current() == name);

  if (prefix)
    display.drawString(x++, 0, prefix.c_str());

  display.drawString(x, 0, String(buffer.current()).c_str());
}

void drawMenu() {
  display.drawString(0, 0, "                ");

  printMenuItem(0, baudRate, "BAUDRATE", "B");
  printMenuItem(8, mode, "MODE");
  printMenuItem(12, channel, "CHANNEL", "C");

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

  for (int i = 0; i < baudRate.length; i++)
    baudRate.append(baudRateOptions[i]);

  for (int i = 0; i < mode.length; i++)
    mode.append(modeOptions[i]);

  for (int i = 0; i < channel.length; i++)
    channel.append(channelOptions[i]);

  for (int i = 0; i < menuItem.length; i++)
    menuItem.append(menuItemOptions[i]);

  Serial.begin(baudRate.current());

  drawMenu();
}

void loop() {
  evtMgr.loopIteration();

  if (Serial.available())
    updateMonitor(Serial.readString());
}
