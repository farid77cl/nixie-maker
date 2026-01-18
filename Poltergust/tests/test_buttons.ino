/*******************************************************************************
 * TEST BUTTONS - Prueba de botones
 * 
 * Conexiones (INPUT_PULLUP):
 *   Power    -> Pin 7  + GND
 *   Vacuum   -> Pin 8  + GND
 *   Strobulb -> Pin 12 + GND
 ******************************************************************************/

#define PIN_POWER     7
#define PIN_VACUUM    8
#define PIN_STROBULB  12

void setup() {
  Serial.begin(115200);
  Serial.println("=== TEST BOTONES ===");
  
  pinMode(PIN_POWER, INPUT_PULLUP);
  pinMode(PIN_VACUUM, INPUT_PULLUP);
  pinMode(PIN_STROBULB, INPUT_PULLUP);
}

void loop() {
  // Leer estados (LOW = presionado)
  bool pwr = digitalRead(PIN_POWER) == LOW;
  bool vac = digitalRead(PIN_VACUUM) == LOW;
  bool str = digitalRead(PIN_STROBULB) == LOW;
  
  if (pwr) Serial.println("BOTON POWER: Presionado! 🔴");
  if (vac) Serial.println("BOTON VACUUM: Aspirando! 🔵");
  if (str) Serial.println("BOTON STROBULB: Flash! 🟢");
  
  if (pwr || vac || str) delay(200); // Pequeño delay para no saturar
}
