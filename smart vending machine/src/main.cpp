#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

// Wokwi specific credentials
const char *ssid = "Wokwi-GUEST";
const char *password = "";
const char *server_ip = "192.168.1.167";

WebSocketsClient webSocket;
const int MOTOR_PIN = 18;

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.println("[WS] Disconnected!");
    break;
  case WStype_CONNECTED:
    Serial.println("[WS] Connected to server!");
    break;
  case WStype_TEXT:
    StaticJsonDocument<200> doc;
    deserializeJson(doc, payload);

    const char *action = doc["action"];
    if (action && strcmp(action, "dispense") == 0)
    {
      Serial.println("Payment Confirmed! Dispensing...");
      digitalWrite(MOTOR_PIN, HIGH);
      delay(2000);
      digitalWrite(MOTOR_PIN, LOW);
    }
    break;
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(MOTOR_PIN, OUTPUT);

  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  // Connect to Python WebSocket Server
  // Ensure your Python server is running and listening on 0.0.0.0:8000
  webSocket.begin(server_ip, 8000, "/ws/vending01");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

void loop()
{
  webSocket.loop();
}