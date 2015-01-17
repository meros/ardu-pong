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

  for (int x = 0; x < CHARS_WIDTH; x++) {
    for (int y = 0; y < CHARS_HEIGHT; y++) {
      lcd.setCursor(x, y);
      lcd.write((byte)' ');
    }
  }

  lcd.createChar(0, buf[0]);
  lcd.createChar(1, buf[1]);
  lcd.createChar(2, buf[2]);
  lcd.createChar(3, buf[3]);

  for (int charX = 0; charX < 2; charX++) {
    for (int charY = 0; charY < 2; charY++) {
      if (baseCharX + charX < CHARS_WIDTH && baseCharY + charY < CHARS_HEIGHT) {
        lcd.setCursor(baseCharX + charX, baseCharY + charY);
        lcd.write((byte)(charX + 2 * charY));
      }
    }
  }
}

void printPaddles(int x, int y) {
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

  for (int px = 0; px < 2; px++) {
    for (int py = 0; py < 4; py++) {
      if (((py == 0) + (px == 2) + (py == 3)) == 2) {
        continue;
      }

      int currCharY = (y + py) / (CHAR_HEIGHT + SPACE_HEIGHT);

      int subX = (x + px) % (CHAR_WIDTH + SPACE_WIDTH);
      int subY = (y + py) % (CHAR_HEIGHT + SPACE_HEIGHT);

      int charIndex = currCharY;

      if (subX < CHAR_WIDTH && subY < CHAR_HEIGHT) {
        buf[charIndex][subY] |= power(CHAR_WIDTH - 1 - subX);
      }
    }
  }

  lcd.createChar(4, buf[0]);
  lcd.createChar(5, buf[1]);
  lcd.createChar(6, buf[2]);
  lcd.createChar(7, buf[3]);

  for (int charY = 0; charY < 2; charY++) {
    lcd.setCursor(0, charY);
    lcd.write((byte)(charY + 4));
  }
}

uint32_t last = 0;

void setup() {
  last = millis();

  Serial.begin(9600);           // set up Serial library at 9600 bps

  // initialize LCD and set up the number of columns and rows:
  lcd.begin(16, 2);
}

float ballX = 0;
float ballY = 0;

float ballDX = 0.005;
float ballDY = 0.01;

void loop() {
  int buttonVoltage = analogRead(0);

  bool up = false;
  bool down = false;
  bool left = false;
  bool right = false;

  if (abs(buttonVoltage - 99) < 10) {
    up = true;
  }

  if (abs(buttonVoltage - 255) < 10) {
    down = true;
  }

  if (abs(buttonVoltage - 0) < 10) {
    right = true;
  }

  if (abs(buttonVoltage - 410) < 10) {
    left = true;
  }

  uint32_t now = millis();
  uint32_t diff = (now - last);
  last = now;

  ballDX -= left ? diff * 0.00001 : 0;
  ballDX += right ? diff * 0.00001 : 0;
  ballDY -= up ? diff * 0.00001 : 0;
  ballDY += down ? diff * 0.00001 : 0;

  float distX = ballDX * diff;
  float distY = ballDY * diff;

  if ((ballX + distX) + 4 > GRID_WIDTH || (ballX + distX) < 0) {
    ballDX = -ballDX;
    ballX -= distX;
  } else {
    ballX += distX;
  }

  if ((ballY + distY) + 4 > GRID_HEIGHT || (ballY + distY) < 0) {
    ballDY = -ballDY;
    ballY -= distY;
  } else {
    ballY += distY;
  }

  printBallChars(ballX, ballY);

 //rintPaddles(1, 5);

  delay(50);
}
