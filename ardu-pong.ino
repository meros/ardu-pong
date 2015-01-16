// Arduino-Pong

#include <LiquidCrystal.h>

#define CHARS_WIDTH (16)
#define CHARS_HEIGHT (2)

#define CHAR_WIDTH (5)
#define CHAR_HEIGHT (8)

#define SPACE_WIDTH (1)
#define SPACE_HEIGHT (1)

#define GRID_WIDTH (CHARS_WIDTH * (CHAR_WIDTH + SPACE_WIDTH) - SPACE_WIDTH)
#define GRID_HEIGHT (CHARS_HEIGHT * (CHAR_HEIGHT + SPACE_HEIGHT) - SPACE_HEIGHT)

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

byte power(int n) {
  if (n == 0) {
    return 1;
  } else {
    return power(n - 1) * 2;
  }
}

void printBallChars(int x, int y) {
  // Update characters
  int baseCharX = x / (CHAR_WIDTH + SPACE_WIDTH);
  int baseCharY = y / (CHAR_HEIGHT + SPACE_HEIGHT);

  byte buf[4][8] = {
    {
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000
    },
    {
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000
    },
    {
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000
    },
    {
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000
    }
  };

  for (int bx = 0; bx < 4; bx++) {
    for (int by = 0; by < 4; by++) {
      if (((bx == 0) + (by == 0) + (bx == 3) + (by == 3)) == 2) {
        continue;
      }

      int currCharX = (x + bx) / (CHAR_WIDTH + SPACE_WIDTH);
      int currCharY = (y + by) / (CHAR_HEIGHT + SPACE_HEIGHT);

      int subX = (x + bx) % (CHAR_WIDTH + SPACE_WIDTH);
      int subY = (y + by) % (CHAR_HEIGHT + SPACE_HEIGHT);

      int charIndex = (currCharX - baseCharX) + 2 * (currCharY - baseCharY);

      if (subX < CHAR_WIDTH && subY < CHAR_HEIGHT) {
        buf[charIndex][subY] |= power(CHAR_WIDTH - 1 - subX);
      }
    }
  }

  lcd.createChar(0, buf[0]);
  lcd.createChar(1, buf[1]);
  lcd.createChar(2, buf[2]);
  lcd.createChar(3, buf[3]);

  for (int x = 0; x < CHARS_WIDTH; x++) {
    for (int y = 0; y < CHARS_HEIGHT; y++) {
      lcd.setCursor(x, y);
      lcd.write((byte)' ');
    }
  }

  for (int charX = 0; charX < 2; charX++) {
    for (int charY = 0; charY < 2; charY++) {
      if (baseCharX + charX < CHARS_WIDTH && baseCharY + charY < CHARS_HEIGHT) {
        lcd.setCursor(baseCharX + charX, baseCharY + charY);
        lcd.write((byte)(charX + 2 * charY));
      }
    }
  }
}

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps

  // initialize LCD and set up the number of columns and rows:
  lcd.begin(16, 2);
}

int ballX = 0;
int ballY = 0;

int ballDX = 1;
int ballDY = 1;

void loop() {
    printBallChars(ballX, ballY);
    
    ballX += ballDX;
    ballY += ballDY;
    
    if (ballX + 4 > GRID_WIDTH || ballX < 0) {
      ballDX = -ballDX;
      ballX += 2*ballDX;
    }

    if (ballY + 4 > GRID_HEIGHT || ballY < 0) {
      ballDY = -ballDY;
      ballY += 2*ballDY;
    }
    
    delay(100);
}
