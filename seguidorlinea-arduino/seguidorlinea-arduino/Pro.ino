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
int velocidadMaxima = 150;  // velocidad máxima
int velocidadCurva = 100;   // velocidad reducida para curvas
int velocidadBusqueda = 80; // velocidad para buscar línea

// ---------------------- Umbral de detección -----------------
int umbral = 2000; // Valor de corte (lo ajustamos con pruebas)

void setup() {
  // Configuración pines motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // PWM ESP32
  ledcSetup(0, 1000, 8);     
  ledcAttachPin(ENA, 0);    

  ledcSetup(1, 1000, 8);     
  ledcAttachPin(ENB, 1);    

  // Comunicación Serial
  Serial.begin(115200);
}

void loop() {
  // Leer sensores en modo analógico
  int sensorIzq = analogRead(SENSOR_I);
  int sensorDer = analogRead(SENSOR_D);

  // Imprimir valores en el Monitor Serial
  Serial.print("Izq: ");
  Serial.print(sensorIzq);
  Serial.print("  |  Der: ");
  Serial.println(sensorDer);

  // Interpretar la lectura
  bool detectaNegroIzq = sensorIzq > umbral;  // Mayor que umbral es negro
  bool detectaNegroDer = sensorDer > umbral;

  // Lógica seguidor de línea
  if(detectaNegroIzq && detectaNegroDer){
    avanzarRecto();
  } else if(!detectaNegroIzq && detectaNegroDer){
    curvaDerecha();
  } else if(detectaNegroIzq && !detectaNegroDer){
    curvaIzquierda();
  } else {
    buscarLinea();
  }
}

// ---------------------- Funciones de Movimiento ----------------------

void avanzarRecto() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  ledcWrite(0, velocidadMaxima);
  ledcWrite(1, velocidadMaxima);
}

void curvaDerecha() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  ledcWrite(0, velocidadCurva);    // Motor derecho baja velocidad
  ledcWrite(1, velocidadMaxima);   // Motor izquierdo normal
}

void curvaIzquierda() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  ledcWrite(0, velocidadMaxima);   // Motor derecho normal
  ledcWrite(1, velocidadCurva);    // Motor izquierdo baja velocidad
}

void buscarLinea() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  ledcWrite(0, velocidadBusqueda);
  ledcWrite(1, velocidadMaxima);
}
