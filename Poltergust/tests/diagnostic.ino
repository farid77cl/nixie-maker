/*******************************************************************************
 * DIAGNÓSTICO POLTERGUST G-00
 * Verifica cada componente paso a paso
 * 
 * Instrucciones:
 * 1. Sube este código
 * 2. Abre Serial Monitor (115200 baud)
 * 3. Sigue las instrucciones en pantalla
 ******************************************************************************/

#include <FastLED.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// === CONFIGURACIÓN DE PINES ===
#define PIN_LED_DATA    6
#define NUM_LEDS_TOTAL  38   // 8 anillo + 30 tira
#define PIN_DFPLAYER_RX 11
#define PIN_DFPLAYER_TX 10
#define PIN_BUZZER      9
#define PIN_BTN_POWER   7
#define PIN_BTN_VACUUM  8
#define PIN_BTN_STROBE  12
#define PIN_LED_STATUS  13

// === OBJETOS ===
CRGB leds[NUM_LEDS_TOTAL];
SoftwareSerial dfSerial(PIN_DFPLAYER_RX, PIN_DFPLAYER_TX);
DFRobotDFPlayerMini dfPlayer;

// === VARIABLES ===
int testStep = 0;
bool waitingInput = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println(F(""));
  Serial.println(F("╔══════════════════════════════════════════════╗"));
  Serial.println(F("║   DIAGNÓSTICO POLTERGUST G-00  🦇👻          ║"));
  Serial.println(F("║   Verificación componente por componente     ║"));
  Serial.println(F("╚══════════════════════════════════════════════╝"));
  Serial.println(F(""));
  
  // Configurar pines
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_BTN_POWER, INPUT_PULLUP);
  pinMode(PIN_BTN_VACUUM, INPUT_PULLUP);
  pinMode(PIN_BTN_STROBE, INPUT_PULLUP);
  pinMode(PIN_LED_STATUS, OUTPUT);
  
  // Inicializar LEDs
  FastLED.addLeds<WS2812B, PIN_LED_DATA, GRB>(leds, NUM_LEDS_TOTAL);
  FastLED.setBrightness(100);
  
  Serial.println(F("Escribe 'S' + Enter para iniciar diagnóstico..."));
  Serial.println(F(""));
}

void loop() {
  // Esperar comando serial
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'S' || c == 's' || c == '\n' || c == '\r') {
      if (waitingInput || testStep == 0) {
        testStep++;
        waitingInput = false;
        runTest(testStep);
      }
    }
  }
}

void runTest(int step) {
  switch(step) {
    case 1: testLEDStatus(); break;
    case 2: testAnillo(); break;
    case 3: testTira(); break;
    case 4: testBuzzer(); break;
    case 5: testDFPlayer(); break;
    case 6: testBotonPower(); break;
    case 7: testBotonVacuum(); break;
    case 8: testBotonStrobulb(); break;
    case 9: testComplete(); break;
    default: 
      Serial.println(F("Diagnóstico finalizado."));
      testStep = 0;
      break;
  }
}

// ============================================================================
// TEST 1: LED de Estado (D13)
// ============================================================================
void testLEDStatus() {
  Serial.println(F("═══════════════════════════════════════════════"));
  Serial.println(F("TEST 1: LED DE ESTADO (D13)"));
  Serial.println(F("═══════════════════════════════════════════════"));
  Serial.println(F("Conexión: D13 → [220Ω] → LED(+), LED(-) → GND"));
  Serial.println(F(""));
  Serial.println(F("El LED D13 debería parpadear 5 veces..."));
  
  for (int i = 0; i < 5; i++) {
    digitalWrite(PIN_LED_STATUS, HIGH);
    delay(300);
    digitalWrite(PIN_LED_STATUS, LOW);
    delay(300);
  }
  
  Serial.println(F(""));
  Serial.println(F("¿Viste el LED parpadear? [S para continuar]"));
  waitingInput = true;
}

// ============================================================================
// TEST 2: Anillo LED (primeros 8 LEDs)
// ============================================================================
void testAnillo() {
  Serial.println(F("═══════════════════════════════════════════════"));
  Serial.println(F("TEST 2: ANILLO LED (8 LEDs) - Strobulb"));
  Serial.println(F("═══════════════════════════════════════════════"));
  Serial.println(F("Conexión: D6 → Anillo DIN, 5V, GND"));
  Serial.println(F(""));
  Serial.println(F("El anillo debería mostrar: BLANCO → ROJO → VERDE → AZUL..."));
  
  // Blanco (Strobulb)
  for(int i = 0; i < 8; i++) leds[i] = CRGB::White;
  for(int i = 8; i < NUM_LEDS_TOTAL; i++) leds[i] = CRGB::Black;
  FastLED.show();
  delay(1000);
  
  // Rojo
  for(int i = 0; i < 8; i++) leds[i] = CRGB::Red;
  FastLED.show();
  delay(1000);
  
  // Verde
  for(int i = 0; i < 8; i++) leds[i] = CRGB::Green;
  FastLED.show();
  delay(1000);
  
  // Azul
  for(int i = 0; i < 8; i++) leds[i] = CRGB::Blue;
  FastLED.show();
  delay(1000);
  
  // Apagar
  for(int i = 0; i < 8; i++) leds[i] = CRGB::Black;
  FastLED.show();
  
  Serial.println(F(""));
  Serial.println(F("¿El ANILLO mostró los 4 colores? [S para continuar]"));
  waitingInput = true;
}

// ============================================================================
// TEST 3: Tira LED (LEDs 9-38)
// ============================================================================
void testTira() {
  Serial.println(F("═══════════════════════════════════════════════"));
  Serial.println(F("TEST 3: TIRA LED (30 LEDs) - Tanque Goo"));
  Serial.println(F("═══════════════════════════════════════════════"));
  Serial.println(F("Conexión: Anillo DOUT → Tira DIN (en cadena)"));
  Serial.println(F(""));
  Serial.println(F("La tira debería mostrar: VERDE → efecto cascada..."));
  
  // Verde completo
  for(int i = 8; i < NUM_LEDS_TOTAL; i++) leds[i] = CRGB::Green;
  FastLED.show();
  delay(1500);
  
  // Efecto cascada
  for(int i = 8; i < NUM_LEDS_TOTAL; i++) leds[i] = CRGB::Black;
  FastLED.show();
  
  for(int i = 8; i < NUM_LEDS_TOTAL; i++) {
    leds[i] = CRGB::Green;
    FastLED.show();
    delay(50);
  }
  
  delay(500);
  
  // Apagar
  for(int i = 8; i < NUM_LEDS_TOTAL; i++) leds[i] = CRGB::Black;
  FastLED.show();
  
  Serial.println(F(""));
  Serial.println(F("¿La TIRA mostró verde con efecto cascada? [S para continuar]"));
  waitingInput = true;
}

// ============================================================================
// TEST 4: Buzzer
// ============================================================================
void testBuzzer() {
  Serial.println(F("═══════════════════════════════════════════════"));
  Serial.println(F("TEST 4: BUZZER (D9)"));
  Serial.println(F("═══════════════════════════════════════════════"));
  Serial.println(F("Conexión: D9 → Buzzer(+), Buzzer(-) → GND"));
  Serial.println(F(""));
  Serial.println(F("Deberías escuchar 3 tonos diferentes..."));
  
  // Tono grave
  tone(PIN_BUZZER, 500, 300);
  delay(400);
  
  // Tono medio
  tone(PIN_BUZZER, 1000, 300);
  delay(400);
  
  // Tono agudo
  tone(PIN_BUZZER, 2000, 300);
  delay(400);
  
  noTone(PIN_BUZZER);
  
  Serial.println(F(""));
  Serial.println(F("¿Escuchaste 3 tonos (grave, medio, agudo)? [S para continuar]"));
  waitingInput = true;
}

// ============================================================================
// TEST 5: DFPlayer Mini
// ============================================================================
void testDFPlayer() {
  Serial.println(F("═══════════════════════════════════════════════"));
  Serial.println(F("TEST 5: DFPLAYER MINI"));
  Serial.println(F("═══════════════════════════════════════════════"));
  Serial.println(F("Conexión: D10→[1K]→RX, D11→TX, 5V, GND, SPK1/2→Altavoz"));
  Serial.println(F("Asegura: SD con archivo 0001.mp3"));
  Serial.println(F(""));
  Serial.println(F("Iniciando comunicación..."));
  
  dfSerial.begin(9600);
  delay(500);
  
  if (!dfPlayer.begin(dfSerial)) {
    Serial.println(F(""));
    Serial.println(F("❌ ERROR: DFPlayer NO detectado!"));
    Serial.println(F("   Revisa:"));
    Serial.println(F("   - Conexiones (RX con resistor 1K)"));
    Serial.println(F("   - SD insertada correctamente"));
    Serial.println(F("   - Archivo 0001.mp3 en la SD"));
    Serial.println(F(""));
  } else {
    Serial.println(F("✅ DFPlayer detectado!"));
    Serial.println(F("Reproduciendo track 1..."));
    dfPlayer.volume(25);
    dfPlayer.play(1);
    delay(3000);
    dfPlayer.stop();
    Serial.println(F(""));
  }
  
  Serial.println(F("¿Escuchaste audio? (o viste el error) [S para continuar]"));
  waitingInput = true;
}

// ============================================================================
// TEST 6: Botón Power
// ============================================================================
void testBotonPower() {
  Serial.println(F("═══════════════════════════════════════════════"));
  Serial.println(F("TEST 6: BOTÓN POWER (D7)"));
  Serial.println(F("═══════════════════════════════════════════════"));
  Serial.println(F("Conexión: D7 → Botón → GND"));
  Serial.println(F(""));
  Serial.println(F("Presiona el botón POWER 3 veces..."));
  
  int presses = 0;
  unsigned long timeout = millis() + 10000;  // 10 segundos
  bool lastState = HIGH;
  
  while(presses < 3 && millis() < timeout) {
    bool state = digitalRead(PIN_BTN_POWER);
    if (state == LOW && lastState == HIGH) {
      presses++;
      Serial.print(F("   ✅ Press "));
      Serial.println(presses);
      delay(200);
    }
    lastState = state;
  }
  
  if (presses >= 3) {
    Serial.println(F("✅ Botón POWER funciona!"));
  } else {
    Serial.println(F("❌ Timeout - botón no detectado"));
  }
  
  Serial.println(F(""));
  Serial.println(F("[S para continuar]"));
  waitingInput = true;
}

// ============================================================================
// TEST 7: Botón Vacuum
// ============================================================================
void testBotonVacuum() {
  Serial.println(F("═══════════════════════════════════════════════"));
  Serial.println(F("TEST 7: BOTÓN VACUUM (D8)"));
  Serial.println(F("═══════════════════════════════════════════════"));
  Serial.println(F("Conexión: D8 → Botón → GND"));
  Serial.println(F(""));
  Serial.println(F("Presiona el botón VACUUM 3 veces..."));
  
  int presses = 0;
  unsigned long timeout = millis() + 10000;
  bool lastState = HIGH;
  
  while(presses < 3 && millis() < timeout) {
    bool state = digitalRead(PIN_BTN_VACUUM);
    if (state == LOW && lastState == HIGH) {
      presses++;
      Serial.print(F("   ✅ Press "));
      Serial.println(presses);
      delay(200);
    }
    lastState = state;
  }
  
  if (presses >= 3) {
    Serial.println(F("✅ Botón VACUUM funciona!"));
  } else {
    Serial.println(F("❌ Timeout - botón no detectado"));
  }
  
  Serial.println(F(""));
  Serial.println(F("[S para continuar]"));
  waitingInput = true;
}

// ============================================================================
// TEST 8: Botón Strobulb
// ============================================================================
void testBotonStrobulb() {
  Serial.println(F("═══════════════════════════════════════════════"));
  Serial.println(F("TEST 8: BOTÓN STROBULB (D12)"));
  Serial.println(F("═══════════════════════════════════════════════"));
  Serial.println(F("Conexión: D12 → Botón → GND"));
  Serial.println(F(""));
  Serial.println(F("Presiona el botón STROBULB 3 veces..."));
  
  int presses = 0;
  unsigned long timeout = millis() + 10000;
  bool lastState = HIGH;
  
  while(presses < 3 && millis() < timeout) {
    bool state = digitalRead(PIN_BTN_STROBE);
    if (state == LOW && lastState == HIGH) {
      presses++;
      Serial.print(F("   ✅ Press "));
      Serial.println(presses);
      delay(200);
    }
    lastState = state;
  }
  
  if (presses >= 3) {
    Serial.println(F("✅ Botón STROBULB funciona!"));
  } else {
    Serial.println(F("❌ Timeout - botón no detectado"));
  }
  
  Serial.println(F(""));
  Serial.println(F("[S para continuar]"));
  waitingInput = true;
}

// ============================================================================
// TEST COMPLETO
// ============================================================================
void testComplete() {
  Serial.println(F(""));
  Serial.println(F("╔══════════════════════════════════════════════╗"));
  Serial.println(F("║   DIAGNÓSTICO COMPLETADO  🦇✨               ║"));
  Serial.println(F("╚══════════════════════════════════════════════╝"));
  Serial.println(F(""));
  Serial.println(F("Si todos los tests pasaron, estás listo para"));
  Serial.println(F("cargar Poltergust.ino y cazar fantasmas! 👻"));
  Serial.println(F(""));
  Serial.println(F("Si algún test falló, revisa las conexiones de"));
  Serial.println(F("ese componente específico."));
  Serial.println(F(""));
  
  testStep = 0;
}
