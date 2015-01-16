// Arduino-Pong

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

byte chars[][8] = {{
    0b11000,
    0b11000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000
  }, {
    0b00011,
    0b00011,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000
  }, {
    0b00000,
    0b00000,
    0b00000,
    0b11000,
    0b11000,
    0b00000,
    0b00000,
    0b00000
  }, {
    0b00000,
    0b00000,
    0b00000,
    0b00011,
    0b00011,
    0b00000,
    0b00000,
    0b00000
  }, {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11000,
    0b11000
  }, {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00011,
    0b00011
  },
  // Paddle high
  {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b11111,
    0b11111,
    0b11111
  },
  // Paddle low
  {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b00000,
    0b00000,
    0b00000,
    0b00000
  }
};

// x = 0 or 1
// y = 0, 1 or 2
byte getBallChar(int x, int y) {
  return x % 2 + y * 2;
}

// y = 0 or 1
byte getPaddleChar(int y) {
  return 6+y;
}


void setup() {
  // initialize LCD and set up the number of columns and rows:
  lcd.begin(16, 2);

  // create a new character
  for (int i = 0; i < 8; i++) {
    lcd.createChar(i, chars[i]);
  }
}

int now = 0;

void loop() {
  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 2; y++) {
      lcd.setCursor(x, y);
      // Print a message to the lcd.
      lcd.write(getBallChar(y, x%3));
    }
  }
  
  delay (500);
  
  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 2; y++) {
      lcd.setCursor(x, y);
      // Print a message to the lcd.
      lcd.write(getPaddleChar(x%2));
    }
  }
  
  delay (500);
}
