/*
 * BASURERO INTELIGENTE v2
 * =======================
 * ESP8266 NodeMCU Amica + HC-SR04 + Servo + ISD1820
 * 
 * Funcionamiento:
 * - Detecta < 30cm → Abre tapa (y la mantiene abierta)
 * - Detecta > 30cm → Cierra tapa + Reproduce mensaje
 * 
 * Conexiones:
 * -----------
 * HC-SR04:
 *   - VCC  → VIN (5V)
 *   - GND  → GND
 *   - TRIG → D1 (GPIO5)
 *   - ECHO → D2 (GPIO4) con divisor de voltaje 1k/2k
 * 
 * Servo:
 *   - VCC (rojo)     → 5V externo
 *   - GND (marrón)   → GND común
 *   - Signal (naranja) → D5 (GPIO14)
 * 
 * ISD1820:
 *   - VCC → 3.3V
 *   - GND → GND
 *   - P-E → D6 (GPIO12)
 * 
 * Canal: Planius - De la pantalla a la realidad
 */

#include <Servo.h>

// ============== PINES ==============
#define TRIG_PIN    D1  // GPIO5
#define ECHO_PIN    D2  // GPIO4
#define SERVO_PIN   D5  // GPIO14
#define ISD_PIN     D6  // GPIO12

// ============== CONFIGURACIÓN ==============
#define DISTANCIA_ACTIVACION  30    // cm para detectar presencia
#define ANGULO_CERRADO        50     // posición tapa cerrada
#define ANGULO_ABIERTO        140    // posición tapa abierta

// ============== VARIABLES ==============
Servo servoTapa;

bool tapaAbierta = false;

// ============== SETUP ==============
void setup() {
  Serial.begin(115200);
  Serial.println("\n=============================");
  Serial.println("🗑️ BASURERO INTELIGENTE v2");
  Serial.println("=============================");
  Serial.println("Inicializando...");
  
  // Configurar pines
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ISD_PIN, OUTPUT);
  digitalWrite(ISD_PIN, LOW);
  
  // Iniciar servo en posición cerrada
  servoTapa.attach(SERVO_PIN);
  servoTapa.write(ANGULO_CERRADO);
  delay(500);
  
  Serial.println("✓ Sistema listo!");
  Serial.print("✓ Distancia de activación: ");
  Serial.print(DISTANCIA_ACTIVACION);
  Serial.println(" cm");
  Serial.println("=============================\n");
}

// ============== LOOP ==============
void loop() {
  float distancia = medirDistancia();
  
  // Debug en serial
  if (distancia > 0) {
    Serial.print("Distancia: ");
    Serial.print(distancia, 1);
    Serial.print(" cm | Tapa: ");
    Serial.println(tapaAbierta ? "ABIERTA" : "CERRADA");
  }
  
  // Presencia detectada (< 30cm) → Abrir tapa
  if (distancia > 0 && distancia < DISTANCIA_ACTIVACION && !tapaAbierta) {
    abrirTapa();
  }
  
  // Sin presencia (> 30cm) y tapa abierta → Cerrar + Mensaje
  if (distancia >= DISTANCIA_ACTIVACION && tapaAbierta) {
    reproducirMensaje();
    delay(500);  // Pequeña pausa antes de cerrar
    cerrarTapa();
  }
  
  delay(100);  // Pausa entre lecturas
}

// ============== FUNCIONES ==============

/**
 * Mide la distancia usando el sensor HC-SR04
 * @return distancia en cm, o -1 si no hay lectura
 */
float medirDistancia() {
  // Pulso de trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Leer echo con timeout de 30ms (~5 metros máx)
  long duracion = pulseIn(ECHO_PIN, HIGH, 30000);
  
  if (duracion == 0) {
    return -1;  // Sin lectura válida
  }
  
  // Calcular distancia (velocidad del sonido ~343 m/s a 20°C)
  float distancia = duracion * 0.0343 / 2.0;
  
  return distancia;
}

/**
 * Abre la tapa del basurero con movimiento suave
 */
void abrirTapa() {
  Serial.println("\n>>> ABRIENDO TAPA <<<");
  
  // Movimiento suave de cerrado a abierto
  for (int pos = ANGULO_CERRADO; pos <= ANGULO_ABIERTO; pos += 2) {
    servoTapa.write(pos);
    delay(15);
  }
  
  tapaAbierta = true;
  Serial.println("Tapa abierta - Esperando que te alejes...\n");
}

/**
 * Cierra la tapa del basurero con movimiento suave
 */
void cerrarTapa() {
  Serial.println("\n>>> CERRANDO TAPA <<<");
  
  // Movimiento suave de abierto a cerrado
  for (int pos = ANGULO_ABIERTO; pos >= ANGULO_CERRADO; pos -= 2) {
    servoTapa.write(pos);
    delay(15);
  }
  
  tapaAbierta = false;
  Serial.println("Tapa cerrada - Listo para detectar\n");
}

/**
 * Reproduce el mensaje grabado en el ISD1820
 */
void reproducirMensaje() {
  Serial.println("\n>>> REPRODUCIENDO MENSAJE <<<");
  
  // Pulso para activar reproducción (P-E = Play Edge-triggered)
  digitalWrite(ISD_PIN, HIGH);
  delay(100);
  digitalWrite(ISD_PIN, LOW);
}