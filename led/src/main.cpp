#include "Arduino.h"

#include <PololuLedStrip.h>

// Create an ledStrip object and specify the pin it will use.
PololuLedStrip<12> ledStrip;

// Create a buffer for holding the colors (3 bytes per color).
#define LED_COUNT 60
rgb_color colors[LED_COUNT];

#define FIRST_LED 13
#define X_PIXEL_COUNT 10
#define Y_PIXEL_COUNT 3

#define PULSE_FREQUENCY 0.5 // Hz
#define SECTION_FREQUENCY 10 // Hz

// Converts a color from HSV to RGB.
// h is hue, as a number between 0 and 360.
// Actual color
// s is the saturation, as a number between 0 and 255.
// Color vibrancy
// v is the value, as a number between 0 and 255.
//dim and brightness
rgb_color hsvToRgb(uint16_t h, uint8_t s, uint8_t v)
{
    uint8_t f = (h % 60) * 255 / 60;
    uint8_t p = (255 - s) * (uint16_t)v / 255;
    uint8_t q = (255 - f * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t t = (255 - (255 - f) * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t r = 0, g = 0, b = 0;
    switch((h / 60) % 6){
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }
    return rgb_color(r, g, b);
}

int arrayIndexFromScreenCoordinates(int x, int y)
{

  int delta = 0;
  if (y % 2 == 0) {
    delta = y * X_PIXEL_COUNT + x;
  } else {
    delta = (y + 1) * X_PIXEL_COUNT - 1 - x;
  }

   return (FIRST_LED + delta);
}

void sectionFromCoordinate(int arr[], int x, int y)
{
  int section_x = 0;
  if (x > floor(X_PIXEL_COUNT/2)) {
    section_x = 1;
  }
  // Serial.print("section_x inside =");
  // Serial.print(section_x);
  arr[0] = section_x;

  int section_y = 0;
  if (y > floor(Y_PIXEL_COUNT/2)) {
    section_y = 1;
  }
  // Serial.print("section_y inside =");
  // Serial.print(section_y);
  arr[1] = section_y;
}

float currentColorValue () {
  unsigned long system_time_s = millis()/1000;
  float colorValue = 0.5 * sin(PULSE_FREQUENCY * system_time_s) + 0.5;
  // Serial.print("value=");
  // Serial.print(colorValue);
  return colorValue;
}

float currentSection () {
  unsigned long system_time_s = millis()/1000;
  float sectionFloat = 2 * sin(SECTION_FREQUENCY * system_time_s) + 2;
  // Serial.print("value=");
  // Serial.print(colorValue);
  return floor(sectionFloat);
}

void setup()
{
  Serial.begin(9600);
  Serial.println("My Sketch has started");

// int x = 9;
// int y = 1;
// int section[] = { 0, 0 };
// sectionFromCoordinate(section, x, y);
//
//  Serial.print("x=");
//  Serial.print(x);
//  Serial.print("\n");
//  Serial.print("y=");
//  Serial.print(y);
//  Serial.print("\n");
//  Serial.print("sec_x=");
//  Serial.print(section[0]);
//  Serial.print("\n");
//  Serial.print("sec_y=");
//  Serial.print(section[1]);
//  Serial.print("\n");
}


void loop()
{
  // Update the colors.
  //Controls the speed of the traverse
  // uint16_t time = millis() >> 2;

float value = currentColorValue();
int current_section_to_illuminate = currentSection();
// float value = 0;
for(uint16_t y = 0; y < Y_PIXEL_COUNT; y++)
 {
   for(uint16_t x = 0; x < X_PIXEL_COUNT; x++)
   {
     uint8_t hue = 255;
     float saturation = 1.0;

     int arrayIndex = arrayIndexFromScreenCoordinates(x,y);
     float temp_value = 0;
     int section[] = { 0, 0 };
     sectionFromCoordinate(section, x, y);

    //  Serial.print("array_index=");
    //  Serial.print(arrayIndex);
    //  Serial.print("\n");
    // Serial.print("x=");
    // Serial.print(x);
    // Serial.print("\n");
    // Serial.print("y=");
    // Serial.print(y);
    // Serial.print("\n");
    // Serial.print("sec_x=");
    // Serial.print(section[0]);
    // Serial.print("\n");
    // Serial.print("sec_y=");
    // Serial.print(section[1]);
    // Serial.print("\n");


    int indexedSection = 0;
     if (section[0] == 0 &&
        section[1] == 0) {
            indexedSection = 0;
     }
     else if (section[0] == 1 &&
        section[1] == 0) {
       indexedSection = 1;

     }
     else if (section[0] == 0 &&
        section[1] == 1) {
       indexedSection = 3;

     }
     else {
       indexedSection = 2;
     }
    //  temp_value = 0.33;
     if (indexedSection == current_section_to_illuminate) {
       temp_value = value;
     }
    //  Serial.print("temp_value=");
    //  Serial.print(temp_value);
    //  Serial.print("\n");
    //  Serial.print("\n");

     colors[arrayIndex] = hsvToRgb(hue, saturation * 255, temp_value * 255);
   }
 }

//     for(uint16_t i = FIRST_LED; i < LED_COUNT; i++)
//   {
//     byte x = (time >> 2) - (i << 3);
// //    colors[i] = hsvToRgb((uint32_t)x * 359 / 256, 255, 255);
// //      colors[i] = rgb_color(4, 0, 255);
//       colors[i] = hsvToRgb(131, 255, 255 * 0.92);
//   }

  // Write the colors to the LED strip.
  ledStrip.write(colors, LED_COUNT);

  delay(100);
}
