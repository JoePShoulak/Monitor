#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "CircularBuffer.h"

#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

enum Mode {
  monitor,
  transmit,
  recieve
};

int baudRate = 9600;
Mode mode = monitor;
CircularBuffer<const char*, 7> options;

void displaySettings() {
  display.setCursor(0, 0);

  // BaudRate
  display.print("BR ");
  display.print(baudRate);
  display.print(" ");

  // Mode
  switch (mode) {
    case monitor:
      display.print("MON");
      break;
    case transmit:
      display.print("TX");
      break;
    case recieve:
      display.print("RX");
      break;
    default:
      display.print("ERR");
      break;
  }

  display.drawLine(0, 8, SCREEN_WIDTH, 8, SSD1306_WHITE);
}

void update() {
  //Check if user moved the cursor?
  //You can move it up or down with options.up() and options.down() respectively.

  display.clearDisplay();

  displaySettings();


  //Print all options again
  display.setCursor(0, 10);
  for (int i = 0; i < options.length; ++i) {
    if (i == options.index()) {
      //Maybe append something? Or change how this line displays?
    }
    display.println(options[i]);
  }

  //Alternatively, you could print the options, then set cursor, and print some line or something
  display.setCursor(0, 10);
  for (auto option : options) {
    display.println(option);
  }
  display.setCursor(0, 10 * options.index()); //assuming rows are 10 px?
  display.print(">");

  display.display();
}

void setup() {
  options.append("1");
  options.append("2");
  options.append("3");
  options.append("4");
  options.append("5");
  options.append("6");
  options.append("7");

  Serial.begin(baudRate);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 allocation failed");
    while (true)
      ;
  }

  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  update();
  display.setCursor(0, 10);
  for (int i = 0; i < options.length; ++i) {
    if (i == options.index()) {
      //Maybe append something? Or change how this line displays?
    }
    display.println(options[i]);
  }
  display.display();
}

void loop() {
  if (Serial.available())
    update(Serial.readString());
}
