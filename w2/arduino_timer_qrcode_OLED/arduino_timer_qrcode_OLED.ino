/**************************************************************************
  Text/ image/ QR Code display for SSD1306 OLED
  Uses Adafruit EPD library: http://librarymanager/All#Adafruit_SSD1306
  and Tom Igoe's QR Code generator for SSD1306 Display: https://github.com/tigoe/html-for-conndev/tree/main/queryString/SSD1306_QRCode_redirect
  created 4 Feb 2022
  modified 7 Feb 2022
  by I-Jon Hsieh
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "qrcode.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define IMG_HEIGHT   16
#define IMG_WIDTH    16
static const unsigned char PROGMEM img_bmp[] =
{ 0b00000111, 0b11100000,
  0b00011000, 0b00011000,
  0b00100000, 0b00000100,
  0b01000000, 0b00000010,
  0b01000000, 0b00000010,
  0b10000000, 0b00000001,
  0b10011100, 0b00111001,
  0b10000000, 0b00000001,
  0b10000000, 0b00000001,
  0b10011111, 0b11111001,
  0b10010000, 0b00001001,
  0b01001111, 0b11110010,
  0b01000111, 0b11100010,
  0b00100000, 0b00000100,
  0b00011000, 0b00011000,
  0b00000111, 0b11100000
};

int potReading;
int btnReading;
int countdown;
const int btnPin = 2;
bool starting = true;
bool selected = false;
String urlString = "https://github.com/IJon127/personal_website";

void setup() {
  Serial.begin(9600);
  pinMode(btnPin, INPUT);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Clear the buffer (the library initializes this with an Adafruit splash screen)
  display.clearDisplay();

}

void loop() {
  potReading = analogRead(A0);
  btnReading = digitalRead(btnPin);
  countdown = 20 - millis() / 1000;
  int selectedPotValue;

  if (starting) {
    introduction();
    if (btnReading == HIGH || countdown < 0) {
      selected = true;
      selectedPotValue =  potReading;
    }
  }


  if (selected || countdown < 0) {
    starting = false; //hide the intro
    if (selectedPotValue < 512) {
      displayQrCode(urlString); //show QR code
    } else {
      showImg();   //show image (face)
      display.invertDisplay(true); // Invert and restore display, pausing in-between
      delay(1000);
      display.invertDisplay(false);
      delay(1000);
    }
  }
  Serial.println(btnReading);
}




void introduction(void) {
  display.clearDisplay();

  display.setCursor(0, 0); // Start at top-left corner

  display.setTextSize(2);
  if (potReading < 512) {
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
    display.println("QRCode");
    display.setTextSize(1);
    display.println("");
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.println("Face");
  } else {
    display.setTextColor(SSD1306_WHITE);
    display.println("QRCode");
    display.setTextSize(1);
    display.println("");
    display.setTextSize(2);
    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
    display.println("Face");
  }

  display.setTextSize(1);
  display.println("");
  display.setTextColor(SSD1306_WHITE);
  display.print("sec:");
  display.println(countdown);

  display.display();
}


void showImg(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - IMG_WIDTH ) / 2,
    (display.height() - IMG_HEIGHT) / 2,
    img_bmp, IMG_WIDTH, IMG_HEIGHT, 1);
  display.display();
  delay(1000);
}



//QR Code------------------------------------------------
void displayQrCode(String message) {
  Serial.print("Message length: ");
  Serial.println(message.length());

  // Create the QR code
  QRCode qrcode;
  // See table at https://github.com/ricmoo/QRCode
  // or https://www.qrcode.com/en/about/version.html for
  // calculation of data capacity of a QR code. Current
  // settings will support a string of about 100 bytes:
  int qrVersion = 5;
  // can be ECC_LOW, ECC_MEDIUM, ECC_QUARTILE and ECC_HIGH (0-3, respectively):
  int qrErrorLevel = ECC_LOW;

  // allocate QR code memory:
  byte qrcodeBytes[qrcode_getBufferSize(qrVersion)];
  qrcode_initText(&qrcode, qrcodeBytes, qrVersion, qrErrorLevel, message.c_str());

  // QR Code block characteristics will depend on the display:
  // QR code needs a "quiet zone" of background color around it, hence the offset:
  int offset = 2;
  int blockSize = (display.height() - (offset * 2)) / qrcode.size;
 
  display.fillScreen(SSD1306_BLACK); // fill with the background color:

  // read the bytes of the QR code and set the blocks light or dark, accordingly:
  // vertical loop:
  for (byte y = 0; y < qrcode.size; y++) {
    // horizontal loop:
    for (byte x = 0; x < qrcode.size; x++) {
      // caculate the block's X and Y positions:
      int blockX = (x * blockSize) + offset;
      int blockY = (y * blockSize) + offset;
      // read the block value from the QRcode:
      int blockValue = qrcode_getModule(&qrcode, x, y);
      // set the default block color:
      int blockColor = SSD1306_BLACK;
      // if the block value is 1, set color to foreground color instead:
      if (blockValue == 1) {
        blockColor = SSD1306_WHITE;
      }
      // display the block on the screen:
      display.fillRect(blockX, blockY, blockSize, blockSize, blockColor);
    }
  }
  // print the message and display it:
  Serial.println(message);
  display.display();
}
