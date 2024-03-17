#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

void displaySettings() {
  display.setCursor(0, 0);

  // BaudRate
  display.print("BR:");
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

  display.drawFastHLine(0, 8, SCREEN_WIDTH, SSD1306_WHITE);
}

void update(String message = "") {
  display.clearDisplay();

  displaySettings();

  display.setCursor(0, 10);
  display.println(message);

  display.display();
}

void setup() {
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
  display.println("1");
  display.println("2");
  display.println("3");
  display.println("4");
  display.println("5");
  display.println("6");
  display.println("7");
  display.display();
}

void loop() {
  if (Serial.available())
    update(Serial.readString());
}
