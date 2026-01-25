/*******************************************************************************
 * POLTERGUST G-00 - Módulo de LEDs
 * Control de tira WS2812B (Tanque Goo) y anillo (Strobulb)
 ******************************************************************************/

#ifndef LEDS_H
#define LEDS_H

#include <FastLED.h>
#include "config.h"

// Array global de LEDs (en serie)
extern CRGB leds[NUM_LEDS_TOTAL];

// ============================================================================
// Inicialización
// ============================================================================
void leds_init() {
  // Inicializar toda la cadena en el pin definido
  FastLED.addLeds<WS2812B, PIN_LED_DATA, GRB>(leds, NUM_LEDS_TOTAL);
  FastLED.setBrightness(150);
  
  pinMode(PIN_LED_STATUS, OUTPUT);
  digitalWrite(PIN_LED_STATUS, LOW);
}

// ============================================================================
// Tanque de Goo - Efecto verde burbujeante
// ============================================================================
void leds_goo_idle() {
  // Efecto de "respiración" verde
  static uint8_t brightness = 0;
  static int8_t direction = 1;
  
  brightness += direction * 2;
  if (brightness >= 200 || brightness <= 50) {
    direction = -direction;
  }
  
  for (int i = NUM_LEDS_RING; i < NUM_LEDS_TOTAL; i++) {
    leds[i] = CRGB(0, brightness, 0);
  }
  FastLED.show();
}

void leds_goo_bubble() {
  // Efecto de burbuja random
  static unsigned long lastBubble = 0;
  if (millis() - lastBubble > 200) {
    int bubblePos = NUM_LEDS_RING + random(NUM_LEDS_STRIP);
    leds[bubblePos] = CRGB(100, 255, 100);
    lastBubble = millis();
  }
  FastLED.show();
}

// ============================================================================
// Strobulb - Flash blanco
// ============================================================================
void leds_strobulb_flash() {
  // Flash máximo brillo (solo anillo: 0-7)
  for (int i = 0; i < NUM_LEDS_RING; i++) {
    leds[i] = CRGB::White;
  }
  FastLED.setBrightness(255);
  FastLED.show();
  delay(STROBULB_FLASH_MS);
  
  // Apagar anillo
  for (int i = 0; i < NUM_LEDS_RING; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.setBrightness(150);
  FastLED.show();
}

// ============================================================================
// Efectos de Fantasma
// ============================================================================
void leds_ghost_alert(uint8_t ghostType) {
  CRGB color;
  switch (ghostType) {
    case GHOST_EASY:   color = CRGB::Green;   break;
    case GHOST_MEDIUM: color = CRGB::Yellow;  break;
    case GHOST_HARD:   color = CRGB::Red;     break;
    case GHOST_BOSS:   color = CRGB::Magenta; break;
    default:           color = CRGB::Green;   break;
  }
  
  // Parpadeo de alerta (en la tira)
  for (int flash = 0; flash < 5; flash++) {
    for (int i = NUM_LEDS_RING; i < NUM_LEDS_TOTAL; i++) {
      leds[i] = color;
    }
    FastLED.show();
    delay(100);
    
    for (int i = NUM_LEDS_RING; i < NUM_LEDS_TOTAL; i++) {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
    delay(100);
  }
}

void leds_ghost_hp(int currentHP, int maxHP, uint8_t ghostType) {
  // Mostrar HP restante como barra de progreso en la tira
  int ledsOn = map(currentHP, 0, maxHP, 0, NUM_LEDS_STRIP);
  
  CRGB color;
  switch (ghostType) {
    case GHOST_EASY:   color = CRGB::Green;   break;
    case GHOST_MEDIUM: color = CRGB::Yellow;  break;
    case GHOST_HARD:   color = CRGB::Red;     break;
    case GHOST_BOSS:   color = CRGB::Magenta; break;
    default:           color = CRGB::Green;   break;
  }
  
  for (int i = 0; i < NUM_LEDS_STRIP; i++) {
    if (i < ledsOn) {
      leds[NUM_LEDS_RING + i] = color;
    } else {
      leds[NUM_LEDS_RING + i] = CRGB::Black;
    }
  }
  FastLED.show();
}

void leds_capture_success() {
  // Efecto de captura exitosa - rainbow celebración
  for (int j = 0; j < 3; j++) {
    for (int i = NUM_LEDS_RING; i < NUM_LEDS_TOTAL; i++) {
      leds[i] = CHSV(((i - NUM_LEDS_RING) * 256 / NUM_LEDS_STRIP + j * 50) % 256, 255, 255);
    }
    FastLED.show();
    delay(100);
  }
  
  // Volver a verde
  for (int i = NUM_LEDS_RING; i < NUM_LEDS_TOTAL; i++) {
    leds[i] = CRGB(0, 100, 0);
  }
  FastLED.show();
}

void leds_ghost_escape() {
  // Efecto de escape - fade rápido a negro
  for (int b = 150; b >= 0; b -= 10) {
    FastLED.setBrightness(b);
    FastLED.show();
    delay(30);
  }
  FastLED.setBrightness(150);
}

// ============================================================================
// Control de estado
// ============================================================================
void leds_status(bool on) {
  digitalWrite(PIN_LED_STATUS, on ? HIGH : LOW);
}

void leds_all_off() {
  fill_solid(leds, NUM_LEDS_TOTAL, CRGB::Black);
  FastLED.show();
  leds_status(false);
}

#endif // LEDS_H
