#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

int amp_pin = A1;

float pin_average(int input_name, int num_samp, int dwell) {
  // initialize local counting vars
  uint8_t sample_count = 0;
  int sum = 0;
  // Loop num_samp times, measuring pin and adding val to sum var
  while (sample_count < num_samp) {
        sum += analogRead(input_name);
        sample_count++;
        delay(dwell);
    }
  // average the measurements
  float avg_ain = (float)sum / (float)num_samp;

  return avg_ain;
}

void setup() {
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
    // init done
    display.display();
    // Clear the buffer.
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Hello Ammeter!");
    display.display();

    pinMode(amp_pin, INPUT);

}

void loop() {
  float zero_v = 2.48;
  float slope = 0.136;  // V per amp
  float amp_ain = pin_average(amp_pin, 10, 50);
  float v_amp = 4.9 * amp_ain / 1024.0;
  float delta_v = zero_v - v_amp;
  float current = delta_v / slope;
  

  display.clearDisplay();
  display.setCursor(0,0);
  display.print(F("Pin Counts: ")); display.println(amp_ain);
  display.print(F("Amp Volts: ")); display.println(v_amp);
  display.print(F("Delta V: ")); display.println(delta_v);
  display.print(F("Pred. Amps: ")); display.println(current);
  display.display();
  delay(1000);
}
