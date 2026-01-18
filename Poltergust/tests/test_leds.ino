/*******************************************************************************
 * TEST LED - Prueba básica de tira WS2812B
 * 
 * Conexiones:
 *   Arduino D6  → DIN (data)
 *   Arduino 5V  → VCC
 *   Arduino GND → GND
 ******************************************************************************/

#include <FastLED.h>

#define PIN_LED     6      // Pin de datos
#define NUM_LEDS    30     // Cantidad de LEDs en tu tira

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  Serial.println("Test LED iniciando...");
  
  FastLED.addLeds<WS2812B, PIN_LED, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(100);  // Brillo moderado (0-255)
  
  Serial.println("LEDs listos!");
}

void loop() {
  // =========================================
  // TEST 1: Verde (como el tanque de Goo)
  // =========================================
  Serial.println("Verde...");
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Green;
  }
  FastLED.show();
  delay(2000);
  
  // =========================================
  // TEST 2: Rojo
  // =========================================
  Serial.println("Rojo...");
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
  }
  FastLED.show();
  delay(2000);
  
  // =========================================
  // TEST 3: Azul
  // =========================================
  Serial.println("Azul...");
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Blue;
  }
  FastLED.show();
  delay(2000);
  
  // =========================================
  // TEST 4: Blanco (como Strobulb)
  // =========================================
  Serial.println("Blanco...");
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;
  }
  FastLED.show();
  delay(2000);
  
  // =========================================
  // TEST 5: Efecto arcoiris
  // =========================================
  Serial.println("Rainbow...");
  for (int j = 0; j < 256; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV((i * 256 / NUM_LEDS + j) % 256, 255, 255);
    }
    FastLED.show();
    delay(20);
  }
  
  // =========================================
  // TEST 6: Apagar
  // =========================================
  Serial.println("Off...");
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  delay(1000);
  
  Serial.println("--- Reiniciando ciclo ---");
}
