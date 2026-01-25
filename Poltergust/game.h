/*******************************************************************************
 * POLTERGUST G-00 - Sistema de Juego
 * Detección de fantasmas, dificultad, mecánica de captura
 ******************************************************************************/

#ifndef GAME_H
#define GAME_H

#include "config.h"
#include "leds.h"
#include "audio.h"
#include "sensors.h" 

// Estado del juego
struct GameState {
  bool powerOn;
  bool vacuumActive;
  bool ghostPresent;
  bool ghostStunned;
  uint8_t currentGhostType;
  int currentGhostHP;
  int currentGhostMaxHP;
  unsigned long lastSpawnCheck;
  unsigned long stunStartTime;
  unsigned long lastVacuumDamage;
  int ghostsCaptured;
};

GameState game;

// ============================================================================
// Inicialización
// ============================================================================
void game_init() {
  game.powerOn = false;
  game.vacuumActive = false;
  game.ghostPresent = false;
  game.ghostStunned = false;
  game.currentGhostType = GHOST_NONE;
  game.currentGhostHP = 0;
  game.currentGhostMaxHP = 0;
  game.lastSpawnCheck = 0;
  game.stunStartTime = 0;
  game.lastVacuumDamage = 0;
  game.ghostsCaptured = 0;
  
  randomSeed(analogRead(A0));  // Semilla random
}

// ============================================================================
// Spawn de fantasmas (híbrido: random + sensor)
// ============================================================================
uint8_t game_roll_ghost_type() {
  int roll = random(100);
  if (roll < 50) return GHOST_EASY;      // 50%
  if (roll < 80) return GHOST_MEDIUM;    // 30%
  if (roll < 95) return GHOST_HARD;      // 15%
  return GHOST_BOSS;                      // 5%
}

int game_get_ghost_hp(uint8_t type) {
  switch (type) {
    case GHOST_EASY:   return HP_EASY;
    case GHOST_MEDIUM: return HP_MEDIUM;
    case GHOST_HARD:   return HP_HARD;
    case GHOST_BOSS:   return HP_BOSS;
    default:           return HP_EASY;
  }
}

void game_spawn_ghost() {
  game.currentGhostType = game_roll_ghost_type();
  game.currentGhostHP = game_get_ghost_hp(game.currentGhostType);
  game.currentGhostMaxHP = game.currentGhostHP;
  game.ghostPresent = true;
  game.ghostStunned = false;
  
  // Alertas
  leds_ghost_alert(game.currentGhostType);
  buzzer_ghost_alert(game.currentGhostType);
  audio_play(SND_GHOST_ALERT);
  
  Serial.print(F("FANTASMA! Tipo: "));
  Serial.print(game.currentGhostType);
  Serial.print(F(" HP: "));
  Serial.println(game.currentGhostHP);
}

void game_check_spawn() {
  if (game.ghostPresent) return;  // Ya hay fantasma
  
  unsigned long now = millis();
  
  // Check cada segundo
  if (now - game.lastSpawnCheck < 1000) return;
  game.lastSpawnCheck = now;
  
  // Probabilidad base: aumenta con el tiempo
  static unsigned long timeSinceLastGhost = 0;
  timeSinceLastGhost += 1000;
  
  // Probabilidad: 0.5% por segundo, max 10% después de 20 segundos
  // (Sin sensor ultrasónico, usamos probabilidad más alta)
  float baseChance = min(timeSinceLastGhost / 1000.0 * 0.5, 10.0);
  
  // Roll
  if (random(1000) < baseChance * 10) {
    game_spawn_ghost();
    timeSinceLastGhost = 0;
  }
}

// ============================================================================
// Mecánica de combate
// ============================================================================
// ============================================================================
// Mecánica de combate
// ============================================================================
void game_capture_success() {
  game.ghostPresent = false;
  game.ghostStunned = false;
  game.currentGhostType = GHOST_NONE;
  game.ghostsCaptured++;
  
  leds_capture_success();
  buzzer_capture_success();
  audio_play(SND_CAPTURE);
  
  Serial.print(F("CAPTURADO! Total: "));
  Serial.println(game.ghostsCaptured);
}

void game_ghost_escape() {
  game.ghostPresent = false;
  game.ghostStunned = false;
  game.currentGhostType = GHOST_NONE;
  
  leds_ghost_escape();
  buzzer_ghost_escape();
  audio_play(SND_GHOST_ESCAPE);
  
  Serial.println(F("Fantasma ESCAPÓ!"));
}

void game_strobulb() {
  if (!game.ghostPresent) return;
  
  leds_strobulb_flash();
  audio_play(SND_STROBULB);
  
  if (!game.ghostStunned) {
    game.ghostStunned = true;
    game.stunStartTime = millis();
    Serial.println(F("Fantasma STUNNED!"));
  }
}

void game_vacuum_damage() {
  if (!game.ghostPresent || !game.ghostStunned) return;
  
  unsigned long now = millis();
  
  // Daño cada 100ms
  if (now - game.lastVacuumDamage < 100) return;
  game.lastVacuumDamage = now;
  
  // Daño base
  int damage = VACUUM_DPS / 10;  // DPS / 10 porque es cada 100ms
  
  // Bonus por Slam (activado con MPU6050)
  if (mpu_detect_slam()) {
    damage += SLAM_BONUS;
    Serial.print(F("SLAM! Accel: "));
    Serial.println(accelMagnitude);
  }
  
  game.currentGhostHP -= damage;
  
  // Actualizar LEDs
  leds_ghost_hp(game.currentGhostHP, game.currentGhostMaxHP, game.currentGhostType);
  
  // Check si capturado
  if (game.currentGhostHP <= 0) {
    game_capture_success();
  }
}

// ============================================================================
// Update principal del juego
// ============================================================================
void game_update(bool vacuumPressed, bool strobulbPressed) {
  if (!game.powerOn) {
    audio_update(false, false);
    return;
  }
  
  // Actualizar música de fondo
  audio_update(true, game.ghostPresent);
  
  // Strobulb
  if (strobulbPressed) {
    game_strobulb();
  }
  
  // Vacuum
  if (vacuumPressed && game.ghostPresent && game.ghostStunned) {
    if (!game.vacuumActive) {
      game.vacuumActive = true;
      audio_play_vacuum();
    }
    game_vacuum_damage();
  } else {
    if (game.vacuumActive) {
      game.vacuumActive = false;
      audio_stop();
    }
  }
  
  // Check stun timeout
  if (game.ghostStunned && millis() - game.stunStartTime > STUN_DURATION_MS) {
    // Fantasmas difíciles escapan si no capturas a tiempo
    if (game.currentGhostType >= GHOST_HARD) {
      game_ghost_escape();
    } else {
      // Los fáciles solo pierden el stun
      game.ghostStunned = false;
      Serial.println(F("Stun terminado!"));
    }
  }
  
  // Check spawn (solo si no hay fantasma activo)
  game_check_spawn();
  
  // Idle animation
  if (!game.ghostPresent) {
    leds_goo_idle();
  }
}

// ============================================================================
// Power control
// ============================================================================
void game_power_on() {
  game.powerOn = true;
  leds_status(true);
  audio_play(SND_POWER_ON);
  
  // Efecto de encendido (tira: 8-37)
  for (int i = NUM_LEDS_RING; i < NUM_LEDS_TOTAL; i++) {
    leds[i] = CRGB(0, 100, 0);
    FastLED.show();
    delay(30);
  }
  
  Serial.println(F("POWER ON"));
}

void game_power_off() {
  game.powerOn = false;
  game.ghostPresent = false;
  game.vacuumActive = false;
  
  audio_play(SND_POWER_OFF);
  delay(500);
  audio_stop();
  leds_all_off();
  
  Serial.println(F("POWER OFF"));
}

void game_toggle_power() {
  if (game.powerOn) {
    game_power_off();
  } else {
    game_power_on();
  }
}

#endif // GAME_H
