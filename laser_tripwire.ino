#include <MegunoLink.h>
#include <Filter.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char *WIFI_SSID = "<WIFI SSID>";
const char *WIFI_PASS = "<WIFI PASSWORD>";
const int LDR_PIN = A0;
const int HISTORY_LENGTH = 5;
const String base_url = "http://unipi.local/laser-tripped";
float ldr_raw = 0;
float ldr_filtered = 0;
float previous_ldr_filtered = 0;

ExponentialFilter<int> ldr_filter(80, 0);

WiFiClient client;

void setup() {
  Serial.begin(9600);
  connect_to_wifi();
}

void loop() {
  delay(50);
  
  ldr_raw = analogRead(A0);
  ldr_filter.Filter(ldr_raw);
  ldr_filtered = ldr_filter.Current();

  Serial.println(ldr_filtered);

  if (ldr_filtered / previous_ldr_filtered < 0.9) {
    Serial.println("Tripped.");
    send_event();
  }
  
  previous_ldr_filtered = ldr_filtered;
}

void connect_to_wifi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected!");
  Serial.printf("Network: %s\n", WIFI_SSID);
  Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());
}

void send_event()
{
  HTTPClient http;
  http.begin(client, base_url);
  http.GET();
  http.end();
}
