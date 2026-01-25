/*******************************************************************************
 * POLTERGUST G-00 - Módulo de Audio
 * Control de DFPlayer Mini + Buzzer
 ******************************************************************************/

#ifndef AUDIO_H
#define AUDIO_H

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include "config.h"

// Instancias
SoftwareSerial dfSerial(PIN_DFPLAYER_TX, PIN_DFPLAYER_RX); // RX, TX
DFRobotDFPlayerMini dfPlayer;

// Estado del Master Audio
enum AudioState {
  STATE_SILENT,
  STATE_THEME,
  STATE_EFFECT
};

AudioState currentAudioState = STATE_SILENT;
unsigned long lastAudioAction = 0;

// ============================================================================
// Inicialización
// ============================================================================
bool audio_init() {
  pinMode(PIN_BUZZER, OUTPUT);
  
  // BIP DE INICIO (Prueba de Hardware)
  tone(PIN_BUZZER, 2000, 100);
  delay(150);
  tone(PIN_BUZZER, 2500, 100);
  
  dfSerial.begin(9600);
  delay(100);
  
  if (!dfPlayer.begin(dfSerial)) {
    Serial.println(F("DFPlayer no detectado!"));
    return false;
  }
  
  dfPlayer.setTimeOut(500);
  dfPlayer.volume(25);  // 0-30
  dfPlayer.EQ(DFPLAYER_EQ_NORMAL);
  
  Serial.println(F("DFPlayer OK!"));
  return true;
}

// ============================================================================
// Master Audio Manager (Update)
// ============================================================================
void audio_update(bool powerOn, bool ghostPresent) {
  if (!powerOn) {
    if (currentAudioState != STATE_SILENT) {
      dfPlayer.stop();
      currentAudioState = STATE_SILENT;
    }
    return;
  }

  // Si no hay efectos prioritarios sonando, poner el Theme de fondo
  if (currentAudioState == STATE_SILENT || (currentAudioState == STATE_THEME && !ghostPresent)) {
      static unsigned long lastThemePlay = 0;
      // Reiniciar theme cada cierto tiempo si se detiene o al iniciar
      if (currentAudioState != STATE_THEME || millis() - lastThemePlay > 60000) { 
        dfPlayer.play(SND_THEME);
        currentAudioState = STATE_THEME;
        lastThemePlay = millis();
        Serial.println(F("Audio: Theme iniciado"));
      }
  }
}

// ============================================================================
// Reproducción de sonidos
// ============================================================================
void audio_play(uint8_t trackNum) {
  dfPlayer.play(trackNum);
  currentAudioState = STATE_EFFECT;
  lastAudioAction = millis();
}

void audio_play_vacuum() {
  dfPlayer.loop(SND_VACUUM);
  currentAudioState = STATE_EFFECT;
  lastAudioAction = millis();
}

void audio_stop() {
  dfPlayer.stop();
  currentAudioState = STATE_SILENT; // Forzará el reinicio del theme en el próximo loop si powerOn
}

void audio_volume(uint8_t vol) {
  dfPlayer.volume(constrain(vol, 0, 30));
}

// ============================================================================
// Buzzer - Alertas
// ============================================================================
void buzzer_beep(int freq, int duration) {
  tone(PIN_BUZZER, freq, duration);
}

void buzzer_ghost_alert(uint8_t ghostType) {
  int beeps = ghostType;
  int freq = 1000 + (ghostType * 200);
  
  for (int i = 0; i < beeps; i++) {
    buzzer_beep(freq, 100);
    delay(150);
  }
}

void buzzer_capture_success() {
  int melody[] = {523, 659, 784, 1047};
  for (int i = 0; i < 4; i++) {
    buzzer_beep(melody[i], 100);
    delay(120);
  }
}

void buzzer_ghost_escape() {
  for (int f = 800; f > 200; f -= 50) {
    buzzer_beep(f, 30);
    delay(40);
  }
  noTone(PIN_BUZZER);
}

void buzzer_off() {
  noTone(PIN_BUZZER);
}

#endif // AUDIO_H
