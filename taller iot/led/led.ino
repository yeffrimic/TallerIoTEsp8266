#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// --- CONFIGURAR ---
const char* WIFI_SSID   = "TU_WIFI";
const char* WIFI_PASS   = "TU_PASS";
const char* MQTT_SERVER = "test.mosquitto.org"; // Broker público
const char* MQTT_TOPIC  = "taller/prueba";
// ------------------

WiFiClient espClient;
PubSubClient client(espClient);

// LED integrado (NodeMCU / ESP8266)
const int LED_PIN = LED_BUILTIN;

void callback(char* topic, byte* payload, unsigned int length) {
  String mensaje;
  for (unsigned int i = 0; i < length; i++) {
    mensaje += (char)payload[i];
  }
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(mensaje);

  if (mensaje == "1") {
    digitalWrite(LED_PIN, LOW);  // en ESP8266 el LED se enciende con LOW
    Serial.println("LED ENCENDIDO");
  } else if (mensaje == "0") {
    digitalWrite(LED_PIN, HIGH); // se apaga con HIGH
    Serial.println("LED APAGADO");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // apagado por defecto

  // Conectar WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");

  // Configuración MQTT
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);

  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("conectado!");
      client.subscribe(MQTT_TOPIC);
      Serial.println("Suscrito a: " + String(MQTT_TOPIC));
    } else {
      Serial.print(" fallo, rc=");
      Serial.print(client.state());
      Serial.println(" reintento en 2s");
      delay(2000);
    }
  }
}
