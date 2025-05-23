#include <Servo.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Define pins for LDR sensors
#define LDR1 A0
#define LDR2 A1

// Error threshold
#define THRESHOLD 30

int servoPos = 90;  // Start in center

Servo servo;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd_direct(rs, en, d4, d5, d6, d7);

LiquidCrystal_I2C lcd_i2c(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  servo.attach(9);
  servo.write(servoPos);

  lcd_direct.begin(16, 2);
  lcd_direct.setCursor(0, 0);
  lcd_direct.print("LDR Values:");

  lcd_i2c.init();
  lcd_i2c.backlight();
  lcd_i2c.setCursor(0, 0);
  lcd_i2c.print("Solar Tracker");
}

void loop() {
  int ldrLeft = analogRead(LDR1);
  int ldrRight = analogRead(LDR2);

  Serial.print("LDR Left: "); Serial.print(ldrLeft);
  Serial.print("  LDR Right: "); Serial.println(ldrRight);

  lcd_direct.setCursor(0, 1);
  lcd_direct.print("L1:"); lcd_direct.print(ldrLeft);
  lcd_direct.print(" L2:"); lcd_direct.print(ldrRight);
  lcd_direct.print("   ");

  int diff = ldrLeft - ldrRight;

  if (abs(diff) > THRESHOLD) {
    int stepSize = 3;  // Increase for faster motion (e.g., 2-5)

    if (diff > 0 && servoPos > 0) {
      servoPos -= stepSize;
    } else if (diff < 0 && servoPos < 180) {
      servoPos += stepSize;
    }
  }

  servoPos = constrain(servoPos, 0, 180);
  servo.write(servoPos);

  lcd_i2c.setCursor(0, 1);
  lcd_i2c.print("Angle: ");
  lcd_i2c.print(servoPos);
  lcd_i2c.print("   ");

  delay(10);  // Smaller delay = faster tracking
}