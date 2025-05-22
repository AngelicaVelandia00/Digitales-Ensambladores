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
int velocidadMaxima = 150;  // velocidad máxima (reducida)
int velocidadCurva = 100;   // velocidad reducida para curvas
int velocidadBusqueda = 80; // velocidad para buscar línea

void setup() {
  // Configuración pines motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Configuración pines sensores
  pinMode(SENSOR_I, INPUT);
  pinMode(SENSOR_D, INPUT);

  // PWM ESP32
  ledcSetup(0, 1000, 8);     
  ledcAttachPin(ENA, 0);    

  ledcSetup(1, 1000, 8);     
  ledcAttachPin(ENB, 1);    

  Serial.begin(115200);
}

void loop() {
  // Leer sensores
  int sensorIzq = digitalRead(SENSOR_I);
  int sensorDer = digitalRead(SENSOR_D);

  // Monitor Serial
  Serial.print("Izq: ");
  Serial.print(sensorIzq);
  Serial.print("  |  Der: ");
  Serial.println(sensorDer);

  // Lógica de navegación
  if(sensorIzq == 1 && sensorDer == 1){
    avanzarRecto();
  } else if(sensorIzq == 0 && sensorDer == 1){
    curvaDerecha();
  } else if(sensorIzq == 1 && sensorDer == 0){
    curvaIzquierda();
  } else {
    buscarLinea(); // si pierde la línea
  }
}

// ---------------------- Funciones de Movimiento ----------------------

void avanzarRecto() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); // Motor derecho adelante
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); // Motor izquierdo adelante
  ledcWrite(0, velocidadMaxima);
  ledcWrite(1, velocidadMaxima);
}

void curvaDerecha() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); // Motor derecho adelante
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); // Motor izquierdo adelante
  ledcWrite(0, velocidadCurva);    // Motor derecho baja velocidad
  ledcWrite(1, velocidadMaxima);   // Motor izquierdo normal
}

void curvaIzquierda() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); // Motor derecho adelante
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); // Motor izquierdo adelante
  ledcWrite(0, velocidadMaxima);    // Motor derecho normal
  ledcWrite(1, velocidadCurva);     // Motor izquierdo baja velocidad
}

void buscarLinea() {
  // Avanzar lentamente en curva buscando la línea
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); // Motor derecho adelante
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); // Motor izquierdo adelante
  ledcWrite(0, velocidadBusqueda); // Menor velocidad
  ledcWrite(1, velocidadMaxima);   // Motor izquierdo normal
}
