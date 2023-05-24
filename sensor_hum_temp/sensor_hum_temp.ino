#include "DHT.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <HTTPClient.h>

#define DHTPIN 4
#define FC28PIN 2
#define DHTTYPE DHT11

const char *ssid = "WIFI_NETWORK"; // isi dengan SSID wifi yg dipake
const char *password = "PASSWORD"; // isi dengan passwordnya wifi

const char *host = " "; // isi dengan alamat web server 

int sensor_analog;
float m, t;

DHT dht(DHTPIN, DHTTYPE);

// Read soil temperature (in celcius) from DHT11 sensor
float readTemp(){
  t = dht.readTemperature();
  Serial.print("Soil Temperature = ");
  Serial.print(t);
  Serial.println(" C");
  return t;
}

// Read soil moisture (in percentage) from FC28 resistive soil moisture sensor
float readMoist() {
  sensor_analog = analogRead(FC28PIN);
  m = (100 - ((sensor_analog/4095.00) * 100));
  Serial.print("Soil Moisture = ");
  Serial.print(m);
  Serial.println(" %");
  return m;
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  dht.begin();
  WiFi.mode(WIFI_OFF); //prevents reconnection issue too long to connect
  delay(1000);
  WiFi.mode(WIFI_STA); // hides the viewing of ESP as wifi hostspot

  WiFi.begin(ssid, password);
  Serial.println("");

  Serial.print("Connecting..");
  // waiting for connection
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //if connection is successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); //IP address assigned to ESP32
}

void loop() {
  HTTPClient http; //declare object of class HTTPClient

  //prepare data
  float temp_temperature, temp_moisture;
  String temperature, moisture, condition, postData;
  temp_temperature = readTemp();
  temp_moisture = readMoist();
  //checking soil condition based on temperature and moisture value
  if(temp_temperature >= 11 && temp_temperature <= 30 && temp_moisture >= 20 && temp_moisture <= 60) {
    condition = "Good";
  } else if (temp_temperature >= 11 && temp_temperature <= 30 && temp_moisture <= 20 && temp_moisture >= 60) {
    condition = "Average";
  } else if (temp_temperature <= 11 && temp_temperature >= 30 && temp_moisture >= 20 && temp_moisture <= 60) {
    condition = "Average";
  } else if (temp_temperature <= 11 && temp_temperature >= 30 && temp_moisture <= 20 && temp_moisture >= 60) {
    condition = "Bad";
  }
  Serial.println("Soil Condition = " + condition);
  temperature = String(temp_temperature);
  moisture = String(temp_moisture);

  //prepare request
  postData = "temperature=" + temperature + "&moisture=" + moisture + "&condition=" + condition;
  http.begin(host);
  //let server know that there's data
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(postData); 
  String payload = http.getString();

  Serial.println(httpCode);
  Serial.println(payload);
  http.end();
  delay(5000);
}
