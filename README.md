# Advanced Arduino Frequency Scanner
#### Author: Bocaletto Luca

This project upgrades the basic pulse‐counting scanner into a **high‐resolution, wide‐range** instrument using Arduino’s Timer1 Input Capture. You’ll get instant period readings, automatic smoothing, dynamic range up to several MHz, and a clean I²C LCD interface.

---

## 1. Key Features
- **Direct Period Measurement** via Timer1 Input Capture on ICP1 (digital pin 8)  
- **Wide Dynamic Range** (from a few Hz up to 2 MHz+ with prescaler options)  
- **Exponential Moving Average** for stable readout  
- **I²C 16×2 LCD Display** for real‐time feedback  
- **Optional Serial Logging** for PC/SD‐card integration  

---

## 2. Components
- Arduino UNO (16 MHz)  
- Schmitt‐trigger comparator (e.g. LM311/LM393) + BNC connector  
- 16×2 I²C LCD (PCF8574 module)  
- Pull-up resistor for comparator output (10 kΩ)  
- Breadboard, jumpers, 5 V supply (USB or external)  
- (Optional) SD-card module + Real‐Time Clock for timestamped logs  

---

## 3. Hardware Wiring

    Signal In ──► [Comparator] ──► TTL Output ──► Arduino Pin 8 (ICP1)
                     │                     
                    VCC (5 V)
                     │                     
                    GND

    Arduino A4 (SDA) ──► LCD SDA
    Arduino A5 (SCL) ──► LCD SCL
    Arduino 5 V ──► LCD VCC
    Arduino GND ──► LCD GND

> **Tip:** Add a small RC filter (10 kΩ + 100 pF) ahead of the comparator to clean jittery inputs.

---

## 4. How It Works
1. **Comparator** shapes any analog or RF source into clean TTL pulses.  
2. **Timer1** runs at F_CPU/8 (2 MHz → 0.5 µs tick) and captures the timer value on each rising edge (ICP1 ISR).  
3. **Period Calculation**: ISR computes Δticks between consecutive edges → period (µs) → frequency (Hz).  
4. **Smoothing**: an exponential moving average (α = 0.1) filters out noise while preserving responsiveness.  
5. **Display**: Shows both raw and smoothed frequency on the LCD.

---

## 5. Calibration & Verification
- **Low‐frequency test**: verify down to <10 Hz by observing stable updates.  
- **High‐frequency test**: feed a 1 MHz signal; confirm correct readout up to ~2 MHz.  
- **Smoothing α**: adjust 0 < α < 1 to trade off noise vs. response time.  
- **Optional**: add an auto‐prescaler selector to switch Timer1 prescalers for extreme ranges.

---

## 6. Next‐Step Extensions
- **Serial/SD Logging**: timestamped CSV output for trend analysis.  
- **Automatic Range Switching**: detect Δticks out of bounds and change prescaler on the fly.  
- **Graphical Touch Display**: show history or simple FFT spectrum.  
- **Bluetooth/Wi-Fi Module**: remote monitoring via smartphone or web dashboard.  
- **Peak-Hold & Min/Max**: capture transient spikes or dips over a user‐set interval.  
