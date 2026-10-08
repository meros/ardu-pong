# ardu-pong

'Pong' made for an Arduino. It's not really pong, just a ball bouncing around
on a 16x2 character LCD. The ball moves pixel by pixel across the character
cells, drawn with custom characters. The
shield buttons push the ball up, down, left or right.

Made in January 2015.

## Hardware

- An Arduino board.
- A 16x2 LCD keypad shield (HD44780-compatible): LCD on pins 8, 9, 4, 5, 6,
  7 and the buttons on analog pin A0.

## Run

Open `ardu-pong.ino` in the Arduino IDE and upload it to the board. It only
needs the bundled `LiquidCrystal` library.

Or with `arduino-cli` (put the sketch in a folder named `ardu-pong`, which
the clone already is):

```sh
arduino-cli compile --fqbn arduino:avr:uno .
arduino-cli upload --fqbn arduino:avr:uno -p /dev/ttyACM0 .
```

## Status

Done.

## License

0BSD, see [LICENSE](LICENSE).
