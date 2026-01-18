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

// ============================================================================
// Inicialización
// ============================================================================
bool audio_init() {
  pinMode(PIN_BUZZER, OUTPUT);
  noTone(PIN_BUZZER);
  
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
// Reproducción de sonidos
// ============================================================================
void audio_play(uint8_t trackNum) {
  dfPlayer.play(trackNum);
}

void audio_play_vacuum() {
  dfPlayer.loop(SND_VACUUM);  // Loop mientras aspira
}

void audio_stop() {
  dfPlayer.stop();
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
  // Beeps según dificultad
  int beeps = ghostType;  // 1-4 beeps
  int freq = 1000 + (ghostType * 200);
  
  for (int i = 0; i < beeps; i++) {
    buzzer_beep(freq, 100);
    delay(150);
  }
}

void buzzer_capture_success() {
  // Melodía de éxito
  int melody[] = {523, 659, 784, 1047};  // C5, E5, G5, C6
  for (int i = 0; i < 4; i++) {
    buzzer_beep(melody[i], 100);
    delay(120);
  }
}

void buzzer_ghost_escape() {
  // Sonido de escape triste
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
