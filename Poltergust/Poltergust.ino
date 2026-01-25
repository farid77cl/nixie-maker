/*******************************************************************************
 * POLTERGUST G-00 - Luigi's Mansion 3 Prop
 * 
 * Aspiradora interactiva con:
 * - LEDs WS2812B (tanque verde + strobulb en cadena)
 * - Audio DFPlayer Mini
 * - Sistema de juego con dificultad
 * 
 * Hardware: Arduino UNO (desarrollo) / Nano (instalación final)
 * 
 * Creado con 🦇 por Nixie - La Goth Bimbo Maker
 ******************************************************************************/

#include "config.h"
#include "leds.h"
#include "audio.h"
#include "sensors.h"
#include "game.h"

// Array de LEDs (definido aquí, declarado extern en leds.h)
CRGB leds[NUM_LEDS_TOTAL];

// Estado de botones (para debounce)
bool lastBtnPower = HIGH;
bool lastBtnVacuum = HIGH;
bool lastBtnStrobulb = HIGH;
unsigned long lastDebounce = 0;
const unsigned long DEBOUNCE_MS = 50;

// ============================================================================
// SETUP
// ============================================================================
void setup() {
  Serial.begin(115200);
  Serial.println(F(""));
  Serial.println(F("================================="));
  Serial.println(F("  POLTERGUST G-00 - Starting..."));
  Serial.println(F("  Luigi's Mansion 3 Prop"));
  Serial.println(F("================================="));
  
  // Inicializar botones (INPUT_PULLUP = activo LOW)
  pinMode(PIN_BTN_POWER, INPUT_PULLUP);
  pinMode(PIN_BTN_VACUUM, INPUT_PULLUP);
  pinMode(PIN_BTN_STROBULB, INPUT_PULLUP);
  
  // Inicializar módulos
  leds_init();
  Serial.println(F("[OK] LEDs inicializados"));
  
  if (audio_init()) {
    Serial.println(F("[OK] Audio inicializado"));
  } else {
    Serial.println(F("[WARN] Audio sin DFPlayer"));
  }
  
  sensors_init();
  Serial.println(F("[OK] Sensores inicializados"));
  
  game_init();
  Serial.println(F("[OK] Sistema de juego listo"));
  
  // Test visual
  Serial.println(F(""));
  Serial.println(F(">> Presiona POWER para comenzar"));
  Serial.println(F("================================="));
  
  // Parpadeo de confirmación
  for (int i = 0; i < 3; i++) {
    leds_status(true);
    delay(200);
    leds_status(false);
    delay(200);
  }
}

// ============================================================================
// LOOP
// ============================================================================
void loop() {
  unsigned long now = millis();
  
  // -------------------------------------------------------------------------
  // Leer botones con debounce
  // -------------------------------------------------------------------------
  bool btnPower = digitalRead(PIN_BTN_POWER);
  bool btnVacuum = digitalRead(PIN_BTN_VACUUM);
  bool btnStrobulb = digitalRead(PIN_BTN_STROBULB);
  
  // Detectar PRESS (transición HIGH -> LOW)
  bool powerPressed = (btnPower == LOW && lastBtnPower == HIGH);
  bool strobulbPressed = (btnStrobulb == LOW && lastBtnStrobulb == HIGH);
  
  // Vacuum se mantiene (no es toggle)
  bool vacuumHeld = (btnVacuum == LOW);
  
  // Actualizar estado anterior
  lastBtnPower = btnPower;
  lastBtnVacuum = btnVacuum;
  lastBtnStrobulb = btnStrobulb;
  
  // -------------------------------------------------------------------------
  // Procesar inputs
  // -------------------------------------------------------------------------
  
  // Power toggle
  if (powerPressed) {
    game_toggle_power();
  }
  
  // -------------------------------------------------------------------------
  // Update del juego
  // -------------------------------------------------------------------------
  game_update(vacuumHeld, strobulbPressed);
  
  // -------------------------------------------------------------------------
  // Debug serial (cada 2 segundos si está encendido)
  // -------------------------------------------------------------------------
  static unsigned long lastDebug = 0;
  if (game.powerOn && now - lastDebug > 2000) {
    lastDebug = now;
    
    Serial.print(F("Gost: "));
    Serial.print(game.ghostPresent ? "SI" : "no");
    
    if (game.ghostPresent) {
      Serial.print(F(" Tipo:"));
      Serial.print(game.currentGhostType);
      Serial.print(F(" HP:"));
      Serial.print(game.currentGhostHP);
    }
    
    // Debug MPU6050
    mpu_read();
    Serial.print(F(" | Accel: "));
    Serial.print(accelMagnitude);
    Serial.print(F("g | Capturados: "));
    Serial.println(game.ghostsCaptured);
  }
  
  // Pequeño delay para estabilidad
  delay(10);
}

