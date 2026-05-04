#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

// --- WiFi Access Point Config ---
const char* AP_SSID     = "CogniCare";
const char* AP_PASSWORD = "cognicare123"; 

// --- Pins ---
const int GSR_PIN  = 34;

// --- OLED ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- MAX30102 ---
MAX30105 particleSensor;
uint32_t irBuffer[100];
uint32_t redBuffer[100];
int32_t  bufferLength  = 100;
int32_t  spo2          = 0;
int8_t   validSPO2     = 0;
int32_t  heartRate     = 0;
int8_t   validHeartRate = 0;

// --- Servers ---
WebServer      httpServer(80);
WebSocketsServer wsServer(81);

// --- Timing ---
unsigned long lastBroadcast = 0;
const unsigned long BROADCAST_INTERVAL = 1000; 

// --- WebSocket event handler ---
void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
  if (type == WStype_CONNECTED) {
    Serial.printf("WS client [%u] connected\n", num);
  } else if (type == WStype_DISCONNECTED) {
    Serial.printf("WS client [%u] disconnected\n", num);
  }
}

void setup() {
  Serial.begin(115200);

  // 1. OLED Init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED Failed");
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("CogniCare");
  display.println("Starting AP...");
  display.display();

  // 2. MAX30102 Init
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    display.println("Sensor Not Found!");
    display.display();
    while (1);
  }
  particleSensor.setup(60, 4, 2, 100, 411, 4096);

  // 3. Start WiFi Access Point (AP Mode)
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASSWORD);
  
  // Predictable IP: 192.168.4.1
  IPAddress localIP(192, 168, 4, 1);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(localIP, gateway, subnet);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("CogniCare Ready!");
  display.print("WiFi: "); display.println(AP_SSID);
  display.print("IP  : 192.168.4.1"); 
  display.display();

  // 4. Server Setup
  httpServer.on("/", HTTP_GET, []() {
    httpServer.send(200, "text/plain", "CogniCare Server Active\nWebSocket: ws://192.168.4.1:81");
  });

  httpServer.begin();
  wsServer.begin();
  wsServer.onEvent(webSocketEvent);
}

void loop() {
  httpServer.handleClient();
  wsServer.loop();

  // --- 1. Collect Samples ---
  for (byte i = 0; i < bufferLength; i++) {
    while (particleSensor.available() == false) particleSensor.check();
    redBuffer[i] = particleSensor.getRed();
    irBuffer[i]  = particleSensor.getIR();
    particleSensor.nextSample();

    // Keep connections alive during the sampling loop
    if (i % 20 == 0) {
      httpServer.handleClient();
      wsServer.loop();
    }
  }

  // --- 2. Calculate Stats ---
  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
  
  // --- 3. Read GSR ---
  int gsrValue = analogRead(GSR_PIN);
  String stressLevel;
  if (gsrValue < 1500) stressLevel = "Relaxed";
  else if (gsrValue < 2550) stressLevel = "Normal";
  else stressLevel = "IN STRESS";

  // --- 4. OLED Display Output ---
  display.clearDisplay();
  display.setTextSize(1);
  
  display.setCursor(0, 0);
  display.print("HEART RATE: ");
  if (validHeartRate) { display.print(heartRate); display.print(" bpm"); }
  else { display.print("--"); }

  display.setCursor(0, 16);
  display.print("SPO2      : ");
  if (validSPO2) { display.print(spo2); display.print(" %"); }
  else { display.print("--"); }

  display.setCursor(0, 32);
  display.print("GSR VALUE : ");
  display.print(gsrValue);

  display.setCursor(0, 50);
  display.print("STATE: ");
  display.print(stressLevel);

  display.display();

  // --- 5. Broadcast Data via WebSocket ---
  StaticJsonDocument<128> doc;
  doc["hr"] = validHeartRate ? (int)heartRate : -1;
  doc["spo2"] = validSPO2 ? (int)spo2 : -1;
  doc["gsr"] = gsrValue;
  doc["stress"] = stressLevel;

  String payload;
  serializeJson(doc, payload);
  wsServer.broadcastTXT(payload);
}