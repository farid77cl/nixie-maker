/*******************************************************************************
 * POLTERGUST G-00 - Módulo de Sensores
 * HC-SR04 Ultrasónico + MPU6050 Acelerómetro
 ******************************************************************************/

#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include "config.h"

// Dirección I2C del MPU6050
#define MPU6050_ADDR 0x68

// Variables de aceleración
float accelX, accelY, accelZ;
float accelMagnitude;

// ============================================================================
// Inicialización
// ============================================================================
void sensors_init() {
  // HC-SR04
  pinMode(PIN_ULTRA_TRIG, OUTPUT);
  pinMode(PIN_ULTRA_ECHO, INPUT);
  digitalWrite(PIN_ULTRA_TRIG, LOW);
  
  // MPU6050
  Wire.begin();
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B);  // PWR_MGMT_1
  Wire.write(0x00);  // Wake up
  Wire.endTransmission(true);
  
  // Configurar rango acelerómetro ±8g
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x1C);  // ACCEL_CONFIG
  Wire.write(0x10);  // ±8g
  Wire.endTransmission(true);
  
  Serial.println(F("Sensores OK!"));
}

// ============================================================================
// HC-SR04 - Distancia en cm
// ============================================================================
long ultrasonic_read() {
  // Trigger
  digitalWrite(PIN_ULTRA_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_ULTRA_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_ULTRA_TRIG, LOW);
  
  // Echo
  long duration = pulseIn(PIN_ULTRA_ECHO, HIGH, 30000);  // Timeout 30ms
  
  // Calcular distancia (velocidad sonido 343 m/s)
  long distance = duration * 0.034 / 2;
  
  return distance;
}

bool ultrasonic_detect_object() {
  long dist = ultrasonic_read();
  return (dist > 0 && dist < GHOST_DETECT_DIST);
}

// ============================================================================
// MPU6050 - Acelerómetro
// ============================================================================
void mpu_read() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x3B);  // ACCEL_XOUT_H
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)MPU6050_ADDR, (uint8_t)6, (uint8_t)true);
  
  int16_t rawX = Wire.read() << 8 | Wire.read();
  int16_t rawY = Wire.read() << 8 | Wire.read();
  int16_t rawZ = Wire.read() << 8 | Wire.read();
  
  // Convertir a g (±8g = 4096 LSB/g)
  accelX = rawX / 4096.0;
  accelY = rawY / 4096.0;
  accelZ = rawZ / 4096.0;
  
  // Magnitud total (para detectar sacudidas)
  accelMagnitude = sqrt(accelX*accelX + accelY*accelY + accelZ*accelZ);
}

bool mpu_detect_slam() {
  mpu_read();
  // Detectar si la magnitud supera el umbral (restando gravedad ~1g)
  float delta = abs(accelMagnitude - 1.0) * 9.8;  // Convertir a m/s²
  return (delta > SLAM_THRESHOLD);
}

float mpu_get_magnitude() {
  return accelMagnitude;
}

#endif // SENSORS_H
