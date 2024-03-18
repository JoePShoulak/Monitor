#include <Wire.h>
#include <Eventually.h>
#include "CircularBuffer.h"
#include <U8x8lib.h>
// #include "MenuItem.h"

#define SELECT_BTN 12
#define CYCLE_BTN 11

EvtManager mgr(true);  // true to manage memory

CircularBuffer<String, 4> menuItem;
CircularBuffer<String, 6> messages;
CircularBuffer<long, 2> baudRate;
CircularBuffer<String, 3> mode;
CircularBuffer<int, 3> channel;

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

  if (selection == "BR") {
    baudRate.next();
    // TODO: Change the serial port
  } else if (selection == "MODE") {
    mode.next();
    // TODO: Change the mode}
  } else if (selection == "CH") {
    channel.next();
    // TODO: Change the channel
  }

  drawMenu();

  return true;
}

EvtPinListener selectButtonListener(SELECT_BTN, 100, (EvtAction)selectButtonAction);
EvtPinListener cycleButtonListener(CYCLE_BTN, 100, (EvtAction)cycleButtonAction);

void addData(String datum) {
  datum.trim();
  messages.append(datum);
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

  printMenuItem(0, baudRate, "BR", "B");
  printMenuItem(8, mode, "MODE");
  printMenuItem(12, channel, "CH", "C");

  display.setInverseFont(0);
  display.drawString(0, 1, "----------------");
}

void displayData() {
  display.setCursor(0, 12);

  int i = 2;

  for (auto& msg : messages) {
    display.drawString(0, i++, msg.c_str());
  }
}

void setup() {
  display.begin();
  display.setPowerSave(0);
  display.setFont(u8x8_font_chroma48medium8_r);

  pinMode(SELECT_BTN, INPUT);
  pinMode(CYCLE_BTN, INPUT);

  mgr.addListener(&selectButtonListener);
  mgr.addListener(&cycleButtonListener);

  baudRate.append(9600);
  baudRate.append(115200);

  mode.append("MN");
  mode.append("TX");
  mode.append("RX");

  channel.append(1);
  channel.append(2);
  channel.append(3);

  menuItem.append("NONE");
  menuItem.append("BR");
  menuItem.append("MODE");
  menuItem.append("CH");

  Serial.begin(baudRate.current());

  drawMenu();
}

void loop() {
  mgr.loopIteration();

  if (Serial.available()) {
    addData(Serial.readString());
    displayData();
  }
}
