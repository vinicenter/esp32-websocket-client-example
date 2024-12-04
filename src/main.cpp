#include <WiFi.h>
#include <ArduinoWebsockets.h>

int LED_BUILTIN = 2;
int BUTTON_BUILTIN = 0;

using namespace websockets;

// wifi credentials
const char* ssid = "Wifi";
const char* password = "Password";

// websocket server
const char* websocket_server_host = "192.168.1.2";
const uint16_t websocket_server_port = 8080;

WebsocketsClient wsClient;

void setup() {
  Serial.begin(115200);

  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  String serverURL = "ws://" + String(websocket_server_host) + ":" + String(websocket_server_port);
  Serial.println("Conectando ao websocket: " + serverURL);
  wsClient.connect(serverURL);

  wsClient.onMessage([](WebsocketsMessage message) {
    Serial.print("Mensagem recebida do servidor: ");
    Serial.println(message.data());

    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
  });

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_BUILTIN, INPUT);
}

void loop() {
  if (wsClient.available()) {
    wsClient.poll();
  }

  if (digitalRead(BUTTON_BUILTIN) == LOW) {
    wsClient.send("Botão pressionado!");

    delay(100);
  }
}
