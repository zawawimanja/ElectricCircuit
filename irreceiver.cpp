// C++ code

// Include the library for the Adafruit LED Backpack (7-segment display)
#include "Adafruit_LEDBackpack.h"

// Include the IRremote library for infrared communication
#include <IRremote.h>

// Global variable to store the button number
int button = 0;

// Create an object for the 7-segment LED display
Adafruit_7segment led_display1 = Adafruit_7segment();

// Variable to store the combined display value
String displayValue = "";

// Maximum length of the display value
const int MAX_DISPLAY_LENGTH = 4;

// Map the IR code to the corresponding remote button.
// The buttons are in this order on the remote:
//    0   1   2
//    4   5   6
//    8   9  10
//   12  13  14
//   16  17  18
//   20  21  22
//   24  25  26
//
// Return -1, if supplied code does not map to a key.
int mapCodeToButton(unsigned long code) {
  // For the remote used in the Tinkercad simulator,
  // the buttons are encoded such that the hex code
  // received is of the format: 0xiivvBF00
  // Where the vv is the button value, and ii is
  // the bit-inverse of vv.
  // For example, the power button is 0xFF00BF000

  // Check for codes from this specific remote
  if ((code & 0x0000FFFF) == 0x0000BF00) {
    // No longer need the lower 16 bits. Shift the code by 16
    code >>= 16;
    // Check that the value and inverse bytes are complementary.
    if (((code >> 8) ^ (code & 0x00FF)) == 0x00FF) {
      return code & 0xFF;
    }
  }
  return -1;
}

// Function to read the infrared signal
int readInfrared() {
  int result = -1; // Initialize the result to -1 (no button pressed)
  // Check if we've received a new code
  if (IrReceiver.decode()) {
    // Get the infrared code
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    // Map it to a specific button on the remote
    result = mapCodeToButton(code);
    // Enable receiving of the next value
    IrReceiver.resume();
  }
  return result; // Return the button number or -1 if no button was pressed
}

// Function to convert button number to letter
char buttonToLetter(int button) {
  switch (button) {
    case 0: return 'A';
    case 1: return 'B';
    case 2: return 'C';
    case 4: return 'D';
    case 5: return 'E';
    case 6: return 'F';
    case 8: return 'G';
    case 9: return 'H';
    case 10: return 'I';
    case 12: return 'J';
    case 13: return 'K';
    case 14: return 'L';
    case 16: return 'M';
    case 17: return 'N';
    case 18: return 'O';
    case 20: return 'P';
    case 21: return 'Q';
    case 22: return 'R';
    case 24: return 'S';
    case 25: return 'T';
    case 26: return 'U';
    default: return '?'; // Unknown character
  }
}

// Setup function (runs once at the beginning)
void setup()
{
  Serial.begin(9600);
  // initializes serial communication with a specific baud rate (9600 in this case
  Serial.println("Hello, Serial Monitor!");

  // Initialize the 7-segment LED display with the I2C address 112
  led_display1.begin(112);
  // Initialize the IR receiver on pin 2
  IrReceiver.begin(2);

  // Display "----" on the 7-segment display during initialization
  led_display1.println("----");
  led_display1.writeDisplay();
}

// Loop function (runs continuously)
void loop()
{
  // Read the infrared signal and get the button number
  button = readInfrared();
  // Check if a valid button was pressed (button number is 0 or greater)
  if (button >= 0) {
    // Convert the button number to a letter
    char letter = buttonToLetter(button);

    // Append the new letter to the display value
    displayValue += letter;

    // Limit the length of the display value to MAX_DISPLAY_LENGTH
    if (displayValue.length() > MAX_DISPLAY_LENGTH) {
      displayValue = displayValue.substring(displayValue.length() - MAX_DISPLAY_LENGTH);
    }

    // Display the combined value on the 7-segment display
    led_display1.println(displayValue);
    led_display1.writeDisplay();

    // Print the button number to the serial monitor
    Serial.print("Button pressed: ");
    Serial.println(letter);
  }
  // Delay a little bit to improve simulation performance
  delay(10);
}