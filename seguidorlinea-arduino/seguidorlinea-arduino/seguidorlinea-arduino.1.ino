// ---------------------- Pines Motores ----------------------
// Motor Derecho (ENA, IN1, IN2)
#define IN1 26   // IN1 del puente H → motor Derecho 
#define IN2 27   // IN2 del puente H → motor Derecho  
#define ENA 13   // ENA del puente H → PWM motor Derecho 

// Motor Izquierdo (ENB, IN3, IN4)
#define IN3 25   // IN3 del puente H → motor izquierdo 
#define IN4 14   // IN4 del puente H → motor izquierdo 
#define ENB 12   // ENB del puente H → PWM motor izquierdo

// ---------------------- Pines Sensores ---------------------
#define SENSOR_I 32  // Sensor izquierdo
#define SENSOR_D 33  // Sensor derecho

// ---------------------- Velocidades -------------------------
int velocidadIzquierda = 200; // motor izquierdo (0–255)
int velocidadDerecha = 200;   // motor derecho (0–255)

void setup() {
  // Configuración pines motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Configuración pines sensores
  pinMode(SENSOR_I, INPUT);
  pinMode(SENSOR_D, INPUT);

  // Configuración PWM en ESP32
  ledcSetup(0, 1000, 8);     // Canal 0, 1kHz, resolución 8 bits
  ledcAttachPin(ENA, 0);     // Canal 0 → motor derecho

  ledcSetup(1, 1000, 8);     // Canal 1, 1kHz, resolución 8 bits
  ledcAttachPin(ENB, 1);     // Canal 1 → motor izquierdo

  // Inicializar comunicación serial
  Serial.begin(115200);
}

void loop() {
  // Lectura de sensores (no invertida)
  int sensorIzq = digitalRead(SENSOR_I);
  int sensorDer = digitalRead(SENSOR_D);

  // Monitor Serial
  Serial.print("Izq: ");
  Serial.print(sensorIzq);
  Serial.print("  |  Der: ");
  Serial.println(sensorDer);

  // Lógica seguidor de línea
  if(sensorIzq == 1 && sensorDer == 1){
    avanzar();
  } else if(sensorIzq == 0 && sensorDer == 1){
    girarDerecha();
  } else if(sensorIzq == 1 && sensorDer == 0){
    girarIzquierda();
  } else {
    detener();
  }

// ---------------------- Funciones de Movimiento ----------------------

void avanzar() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); // Motor derecho hacia adelante
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); // Motor izquierdo hacia adelante
  ledcWrite(0, velocidadDerecha);    // Canal 0 controla motor derecho
  ledcWrite(1, velocidadIzquierda);  // Canal 1 controla motor izquierdo
}

void girarDerecha() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); // Motor derecho sigue
  digitalWrite(IN3, LOW);  digitalWrite(IN4, LOW); // Motor izquierdo parado
  ledcWrite(0, velocidadDerecha);
  ledcWrite(1, 0);
}

void girarIzquierda() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, LOW); // Motor derecho parado
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); // Motor izquierdo sigue
  ledcWrite(0, 0);
  ledcWrite(1, velocidadIzquierda);
}

void detener() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  ledcWrite(0, 0);
  ledcWrite(1, 0);
}
