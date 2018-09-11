#include <Adafruit_SSD1306.h>

#define OLED_RESET 2 //D4
Adafruit_SSD1306 display(OLED_RESET);

void oled_setup() {
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c); // initialize with the I2C addr 0x3D (for the 128x64)  
  display.display();
  delay(2000);
  display.clearDisplay();
 
 // set text color / Textfarbe setzen
 display.setTextColor(WHITE);
 // set text size / Textgroesse setzen
 display.setTextSize(2);
 // set text cursor position / Textstartposition einstellen
 display.setCursor(1,0);
 // show text / Text anzeigen
 display.println("Boot...");
 display.display();
}


void oled_loop() {
  display.clearDisplay();
   // set text color / Textfarbe setzen
 display.setTextColor(WHITE);
 // set text size / Textgroesse setzen
 display.setTextSize(2);
 // set text cursor position / Textstartposition einstellen
 display.setCursor(1,0);
 // show text / Text anzeigen
 display.println("xx %");
 display.display();
}

