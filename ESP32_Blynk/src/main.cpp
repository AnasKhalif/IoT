#define BLYNK_PRINT Serial

#define BLYNK_AUTH_TOKEN "i_Oc56uPegSKtWK1FJq9MieDOkSTZc4B"
#define BLYNK_TEMPLATE_ID "TMPL6gFVQI_7e"
#define BLYNK_TEMPLATE_NAME "AnasTemplate"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHTesp.h>

// WiFi Credentials
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Wokwi-GUEST"; 
char pass[] = ""; 

// Pin Konfigurasi
#define DHT_PIN 15  // Pin untuk DHT22 (SDA -> D15)
#define LED_PIN 26  // Pin untuk LED (D26)

DHTesp dht;
BlynkTimer timer;

void sendSensor() {
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Gagal membaca data DHT22!");
    return;
  }

  // Kirim data ke Blynk
  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);

  // Print ke Serial Monitor
  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.print("°C  -  Kelembaban: ");
  Serial.print(humidity);
  Serial.println("%");
}

// Fungsi untuk mengontrol LED dari Blynk
BLYNK_WRITE(V2) {
  int ledState = param.asInt(); // Baca nilai dari Blynk
  digitalWrite(LED_PIN, ledState);
  Serial.println(ledState ? "LED ON" : "LED OFF");
}

void setup() {
  Serial.begin(115200);
  
  Blynk.begin(auth, ssid, pass);
  dht.setup(DHT_PIN, DHTesp::DHT22);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  timer.setInterval(2000L, sendSensor); // Kirim data setiap 2 detik
}

void loop() {
  Blynk.run();
  timer.run();
}
