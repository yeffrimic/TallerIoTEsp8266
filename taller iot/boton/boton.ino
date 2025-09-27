#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define Cliente yeffrimic
// --- CONFIGURAR ---
const char* WIFI_SSID  = "TU_WIFI";
const char* WIFI_PASS  = "TU_PASS";
const char* MQTT_SERVER = "test.mosquitto.org"; // puedes usar este broker público
const char* MQTT_TOPIC  = "taller/boton";
// ------------------

WiFiClient espClient;
PubSubClient client(espClient);

const int BUTTON_PIN = D5;  // Botón entre pin y GND (con INPUT_PULLUP)
int lastState = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Conectar WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");

  // Conectar MQTT
  client.setServer(MQTT_SERVER, 1883);
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int reading = digitalRead(BUTTON_PIN);

  // Detectar cuando se PRESIONA el botón (de HIGH a LOW)
  if (lastState == HIGH && reading == LOW) {
    client.publish(MQTT_TOPIC, "pressed");
    Serial.println("Botón presionado -> enviado MQTT");
  }

  lastState = reading;
}

// Función de reconexión MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    if (client.connect(Cliente)) {
      Serial.println("conectado!");
    } else {
      Serial.print(" fallo, rc=");
      Serial.print(client.state());
      Serial.println(" reintento en 2s");
      delay(2000);
    }
  }
}
