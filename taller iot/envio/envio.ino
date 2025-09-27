#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define cliente "yeffrimic"
// --- CONFIGURAR ---
const char* WIFI_SSID   = "TU_WIFI";
const char* WIFI_PASS   = "TU_PASS";
const char* MQTT_SERVER = "test.mosquitto.org";  // broker público
const char* MQTT_TOPIC  = "taller/esp8266";
// ------------------

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
const long intervalo = 5000; // tiempo entre envíos (5 segundos)

void setup() {
  Serial.begin(115200);

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

  unsigned long ahora = millis();
  if (ahora - lastMsg > intervalo) {
    lastMsg = ahora;

    // Mensaje que se va a enviar
    const char* mensaje = "Hola desde ESP8266";
    client.publish(MQTT_TOPIC, mensaje);
    Serial.println(String("Enviado: ") + mensaje);
  }
}

// Reconexión MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    if (client.connect(cliente)) {
      Serial.println("conectado!");
    } else {
      Serial.print(" fallo, rc=");
      Serial.print(client.state());
      Serial.println(" reintento en 2s");
      delay(2000);
    }
  }
}
