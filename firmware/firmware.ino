#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// --- PIN DEFINITIONS ---
#define ONE_WIRE_BUS 2    // DS18B20 temperature sensor data pin (D2)
#define GAS_PIN A0        // MQ gas sensor analog output (A0)
#define RED_LED 5         // RGB LED red pin (D5)
#define GREEN_LED 6       // RGB LED green pin (D6)
#define BUZZER 7          // Buzzer (D7)

// --- SETTINGS ---
#define GAS_THRESHOLD 400 // Gas alarm threshold (between 0-1023, fine-tune as needed)

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C LCD address (if nothing displays, try 0x3F)

void setup() {
  // Set pins as input or output
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(GAS_PIN, INPUT);
  
  // Initialize communication
  sensors.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {
  int gasVal = analogRead(GAS_PIN);
  
  if (gasVal > GAS_THRESHOLD) {
    // ==========================================
    // --- GAS DETECTED EMERGENCY ---
    // ==========================================
    
    // 1. LED: Red on, green off
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    
    // 2. LCD: Flash warning message
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("GAZ ESZLELVE!");
    
    // 3. Buzzer: Sound ON
    digitalWrite(BUZZER, HIGH);
    delay(300);
    
    // Flashing: backlight off, buzzer off
    lcd.noBacklight();
    digitalWrite(BUZZER, LOW);
    delay(300);
    lcd.backlight();
    
  } else {
    // ==========================================
    // --- NORMAL OPERATION ---
    // ==========================================
    
    // 1. LED: Green on, red off
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    
    // 2. Buzzer: Ensure off
    digitalWrite(BUZZER, LOW);
    
    // 3. Request temperature
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0);
    
    // 4. Normal LCD display mode
    lcd.setCursor(0, 0);
    lcd.print("Homerseklet:    "); // Spaces to clear trailing characters
    lcd.setCursor(0, 1);
    lcd.print(tempC, 1);
    lcd.print(" C  Gaz: ");
    lcd.print(gasVal);
    lcd.print("   ");
    
    delay(500); // Small delay between updates
  }
}