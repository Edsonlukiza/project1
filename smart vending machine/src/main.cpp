#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

const char *ssid = "YOUR_WIFI_NAME";
const char *password = "YOUR_WIFI_PASSWORD";
const char *server_ip = "192.168.1.100"; // Your Python Server IP

WebSocketsClient webSocket;
const int MOTOR_PIN = 18; // Pin connected to Relay/Driver

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  if (type == WStype_TEXT)
  {
    StaticJsonDocument<200> doc;
    deserializeJson(doc, payload);

    const char *action = doc["action"];
    if (strcmp(action, "dispense") == 0)
    {
      Serial.println("Payment Confirmed! Dispensing...");
      digitalWrite(MOTOR_PIN, HIGH);
      delay(2000); // Turn motor for 2 seconds
      digitalWrite(MOTOR_PIN, LOW);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(MOTOR_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
    delay(500);

  // Connect to Python WebSocket Server on port 8000
  webSocket.begin(server_ip, 8000, "/ws/vending01");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

void loop()
{
  webSocket.loop();
}