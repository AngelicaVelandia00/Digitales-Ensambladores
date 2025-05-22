#include <WiFi.h>
#include <WebServer.h>


// === CREDENCIALES WiFi ===
const char* ssid = "Panchita";
const char* password = "A1%P@nch1t@#2025";

WebServer server(80);

// === PINES PUENTE H ===
const int IN1 = 21, IN2 = 22, IN3 = 27, IN4 = 26;
const int ENA = 25, ENB = 23;

// Pines de sensores (no usados en este control)
const int FL_PIN = 34;
const int FR_PIN = 35;
const int BL_PIN = 32;
const int BR_PIN = 33;
const int TRIG_PIN = 18;
const int ECHO_PIN = 19;

void setup() {
  Serial.begin(115200);

  // Puente H como salida
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  // Activar motores (PWM o HIGH fijo)
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);

  // Conectar a WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado a WiFi. IP: " + WiFi.localIP().toString());

  // Servidor Web
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", getHTML());
  });

  server.on("/comando", HTTP_GET, []() {
    String accion = server.arg("accion");
    moverCarrito(accion);
    server.send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}

void moverCarrito(String accion) {
  if (accion == "adelante") {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  } else if (accion == "atras") {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  } else if (accion == "izquierda") {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  } else if (accion == "derecha") {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  } else if (accion == "stop") {
    digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  }
}

String getHTML() {
  return R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Control Carrito</title>
      <script>
        document.addEventListener('keydown', function(event) {
          let accion = "";
          if (event.key === "ArrowUp") accion = "adelante";
          else if (event.key === "ArrowDown") accion = "atras";
          else if (event.key === "ArrowLeft") accion = "izquierda";
          else if (event.key === "ArrowRight") accion = "derecha";
          if (accion !== "") fetch("/comando?accion=" + accion);
        });
        document.addEventListener('keyup', function(event) {
          fetch("/comando?accion=stop");
        });
      </script>
    </head>
    <body>
      <h1>Usa las flechas del teclado para mover el carrito</h1>
    </body>
    </html>
  )rawliteral";
}