/*******************************************************************************
 * POLTERGUST G-00 - Luigi's Mansion 3 Prop
 * Configuración de pines y constantes
 * 
 * Hardware: Arduino UNO (dev) / Nano (final)
 ******************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// PINES - LEDs (FastLED WS2812B)
// ============================================================================
#define PIN_LED_DATA      6    // Pin de datos para la cadena (Anillo -> Tira)
#define PIN_LED_STATUS    13   // LED estado on/off

#define NUM_LEDS_RING     8    // LEDs en el anillo (primero en la cadena)
#define NUM_LEDS_STRIP    30   // LEDs en la tira (segundo en la cadena)
#define NUM_LEDS_TOTAL    38   // Total: 8 + 30

// ============================================================================
// PINES - Audio (DFPlayer Mini)
// ============================================================================
#define PIN_DFPLAYER_RX   10   // Arduino TX -> DFPlayer RX
#define PIN_DFPLAYER_TX   11   // Arduino RX <- DFPlayer TX
#define PIN_BUZZER        9    // Buzzer pasivo (PWM)

// ============================================================================
// PINES - Sensores
// ============================================================================
#define PIN_ULTRA_TRIG    2    // HC-SR04 Trigger
#define PIN_ULTRA_ECHO    3    // HC-SR04 Echo
// MPU6050 usa I2C: SDA=A4, SCL=A5 (hardware)

// ============================================================================
// PINES - Botones (INPUT_PULLUP - activo LOW)
// ============================================================================
#define PIN_BTN_POWER     7    // Encender/Apagar
#define PIN_BTN_VACUUM    8    // Aspirar (mantener)
#define PIN_BTN_STROBULB  12   // Flash Strobulb

// ============================================================================
// AUDIO - Índices de archivos MP3 en SD
// ============================================================================
#define SND_VACUUM        1    // 0001_vacuum.mp3
#define SND_STROBULB      2    // 0002_strobulb.mp3
#define SND_THEME         3    // 0003_theme.mp3
#define SND_CAPTURE       4    // 0004_capture.mp3
#define SND_GHOST_ALERT   5    // 0005_ghost_alert.mp3
#define SND_GHOST_ESCAPE  6    // 0006_ghost_escape.mp3
#define SND_POWER_ON      7    // 0007_power_on.mp3
#define SND_POWER_OFF     8    // 0008_power_off.mp3

// ============================================================================
// GAME - Sistema de fantasmas
// ============================================================================
// Tiempos de spawn (ms)
#define GHOST_SPAWN_MIN   30000   // 30 segundos mínimo
#define GHOST_SPAWN_MAX   120000  // 2 minutos máximo

// Distancia sensor que aumenta probabilidad (cm)
#define GHOST_DETECT_DIST 50

// Tipos de fantasma (dificultad)
enum GhostType {
  GHOST_NONE = 0,
  GHOST_EASY = 1,      // HP 20, 2-3 seg
  GHOST_MEDIUM = 2,    // HP 50, 5-7 seg
  GHOST_HARD = 3,      // HP 100, 10-15 seg
  GHOST_BOSS = 4       // HP 200, 20+ seg
};

// HP por tipo
#define HP_EASY     20
#define HP_MEDIUM   50
#define HP_HARD     100
#define HP_BOSS     200

// Daño por segundo del vacuum
#define VACUUM_DPS  10

// Bonus de daño por Slam
#define SLAM_BONUS  15

// Aceleración mínima para detectar Slam (m/s²)
#define SLAM_THRESHOLD 15.0

// ============================================================================
// EFECTOS - Colores (formato GRB para WS2812B)
// ============================================================================
#define COLOR_GOO_GREEN   0x00FF00
#define COLOR_STROBULB    0xFFFFFF
#define COLOR_GHOST_EASY  0x00FF00
#define COLOR_GHOST_MED   0xFFFF00
#define COLOR_GHOST_HARD  0xFF0000
#define COLOR_GHOST_BOSS  0xFF00FF
#define COLOR_OFF         0x000000

// ============================================================================
// TIEMPOS
// ============================================================================
#define STROBULB_FLASH_MS   1000  // Duración del flash extendida a 1s
#define STUN_DURATION_MS    4000  // Aumentamos stun a 4s por el flash largo

#endif // CONFIG_H
