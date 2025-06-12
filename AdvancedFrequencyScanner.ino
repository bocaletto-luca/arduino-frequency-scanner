#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ----- User Settings -----
const uint8_t LCD_ADDR    = 0x27;     // I2C address of 16×2 LCD
const float   TICK_US     = 0.5;      // Timer1 tick period in µs (16 MHz/8 prescaler)
const float   ALPHA       = 0.10;     // EMA smoothing factor (0 < α < 1)

// ----- Globals -----
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);
volatile uint16_t lastCapture = 0;
volatile uint16_t periodTicks = 0;
volatile bool     newData     = false;
float             freqSmooth  = 0.0;

// ----- Input Capture ISR -----
// Triggered on every rising edge at ICP1 (Arduino pin 8)
ISR(TIMER1_CAPT_vect) {
  uint16_t ic = ICR1;                   
  periodTicks  = ic - lastCapture;      // difference between edges
  lastCapture  = ic;                    
  newData      = true;                  
}

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Advanced Scanner");
  delay(1000);
  lcd.clear();

  // Configure ICP1 (Pin 8) as input
  pinMode(8, INPUT);

  // Timer1 setup: Normal mode, noise cancel, rising edge capture, prescaler = 8
  TCCR1A = 0;  
  TCCR1B = _BV(ICNC1) | _BV(ICES1) | _BV(CS11);  
  TIFR1  = _BV(ICF1);               // clear pending flags
  TIMSK1 = _BV(ICIE1);              // enable input capture interrupt
}

void loop() {
  if (newData) {
    noInterrupts();
    uint16_t ticks = periodTicks;
    newData = false;
    interrupts();

    if (ticks > 0) {
      // Compute raw frequency
      float period_us = ticks * TICK_US;
      float freq = 1e6 / period_us;

      // Exponential Moving Average
      freqSmooth = ALPHA * freq + (1 - ALPHA) * freqSmooth;

      // Update LCD
      lcd.setCursor(0, 0);
      lcd.print("Freq: ");
      lcd.print(freqSmooth, 1);
      lcd.print(" Hz  ");
      lcd.setCursor(0, 1);
      lcd.print("Raw:");
      lcd.print(freq, 1);
      lcd.print("Hz   ");
    }
  }
}
