#include <WiFi.h>
#include <WebServer.h>

// --- PIN DEFINITIONS FOR WROOM 32D ---
// Wheel Motors (L298N)
const int IN1 = 27;
const int IN2 = 26;
const int IN3 = 25;
const int IN4 = 33;

// Roller Motors (Relay)
const int ROLLER_PIN = 4; 

WebServer server(80);

void setup() {
  Serial.begin(115200);

  // Set pins as outputs
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ROLLER_PIN, OUTPUT);

  // Start with everything OFF
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  digitalWrite(ROLLER_PIN, HIGH); // Relays are often "High" for OFF. If this turns it ON, change to LOW.

  // WiFi Setup (CHANGE THIS)
  WiFi.begin("realme GT 7T", "12345677");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected! IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/action", handleAction);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleAction() {
  if (server.hasArg("go")) {
    String cmd = server.arg("go");
    
    if (cmd == "forward") {
      // Both Motors Forward
      digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    } 
    else if (cmd == "left") {
      // Left Stop, Right Forward
      digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    }
    else if (cmd == "right") {
      // Left Forward, Right Stop
      digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
    }
    else if (cmd == "stop") {
      // All Stop
      digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
      digitalWrite(ROLLER_PIN, HIGH); // Roller OFF
    }
    else if (cmd == "scoop") {
      // Roller ON + Move Forward
      digitalWrite(ROLLER_PIN, LOW); // Relay ON
      digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    }
    
    server.send(200, "text/plain", "OK");
  }
}