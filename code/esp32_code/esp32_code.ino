#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "vivo22";
const char* password = "bunny2002";

// Motor control pins for L298N - MATCHING YOUR ESP32 PINS
const int IN1 = 19;    // D19 - Right motors FORWARD
const int IN2 = 18;    // D18 - Right motors BACKWARD
const int IN3 = 5;     // D5  - Left motors FORWARD
const int IN4 = 4;     // D4  - Left motors BACKWARD
const int ENA = 21;    // D21 - Right motors SPEED (PWM)
const int ENB = 22;    // D22 - Left motors SPEED (PWM)

const int motorSpeed = 200;  // PWM speed (0-255)

WebServer server(80);

void setup() {
  Serial.begin(115200);
  
  // Set motor control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  // Initialize motors stopped
  stopMotors();

  // Connect to Wi-Fi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Server routing
  server.on("/", handleRoot);
  server.on("/forward", handleForward);
  server.on("/backward", handleBackward);
  server.on("/left", handleLeft);
  server.on("/right", handleRight);
  server.on("/stop", handleStop);
  server.begin();
  
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void handleRoot() {
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>4WD Car Control</title>
    <style>
      body { 
        font-family: Arial, sans-serif; 
        text-align: center; 
        margin: 50px; 
        background-color: #f0f0f0;
      }
      h1 { color: #333; }
      .control-container { 
        display: grid; 
        grid-template-columns: repeat(3, 1fr);
        gap: 10px; 
        max-width: 300px; 
        margin: 0 auto;
      }
      button { 
        padding: 20px; 
        font-size: 18px; 
        border: none; 
        border-radius: 10px; 
        cursor: pointer;
        transition: background-color 0.3s;
      }
      .forward { 
        grid-column: 2; 
        background-color: #4CAF50; 
        color: white; 
      }
      .backward { 
        grid-column: 2; 
        background-color: #2196F3; 
        color: white; 
      }
      .left { 
        background-color: #FF9800; 
        color: white; 
      }
      .right { 
        background-color: #FF9800; 
        color: white; 
      }
      .stop { 
        grid-column: 2; 
        background-color: #f44336; 
        color: white; 
        padding: 25px;
      }
      button:hover { opacity: 0.8; }
    </style>
  </head>
  <body>
    <h1>🚗 4WD Car Control</h1>
    <div class="control-container">
      <button class="forward" onclick="controlCar('forward')">↑ FORWARD</button>
      <button class="left" onclick="controlCar('left')">← LEFT</button>
      <button class="stop" onclick="controlCar('stop')">STOP</button>
      <button class="right" onclick="controlCar('right')">RIGHT →</button>
      <button class="backward" onclick="controlCar('backward')">↓ BACKWARD</button>
    </div>
    <script>
      function controlCar(direction) {
        fetch('/' + direction)
          .then(response => console.log(direction + ' command sent'))
          .catch(error => console.error('Error:', error));
      }
    </script>
  </body>
  </html>
  )rawliteral";
  
  server.send(200, "text/html", html);
}

void handleForward() {
  Serial.println("Moving FORWARD");
  // Both sides forward
  digitalWrite(IN1, HIGH);  // Right forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);  // Left forward
  digitalWrite(IN4, LOW);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
  server.send(200, "text/plain", "Forward");
}

void handleBackward() {
  Serial.println("Moving BACKWARD");
  // Both sides backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);  // Right backward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);  // Left backward
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
  server.send(200, "text/plain", "Backward");
}

void handleLeft() {
  Serial.println("Turning LEFT");
  // Right side forward, left side stopped
  digitalWrite(IN1, HIGH);  // Right forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);   // Left stopped
  digitalWrite(IN4, LOW);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, 0);
  server.send(200, "text/plain", "Left");
}

void handleRight() {
  Serial.println("Turning RIGHT");
  // Left side forward, right side stopped
  digitalWrite(IN1, LOW);   // Right stopped
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);  // Left forward
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, motorSpeed);
  server.send(200, "text/plain", "Right");
}

void handleStop() {
  Serial.println("STOPPING");
  stopMotors();
  server.send(200, "text/plain", "Stop");
}