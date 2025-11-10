#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ModbusMaster.h>

// ==========================================
// CONFIGURATION - ⚙️ UPDATE THESE
// ==========================================
const char* WIFI_SSID = "dlink_DWR-116";       // Wi-Fi SSID
const char* WIFI_PASSWORD = "";                 // Wi-Fi Password
const char* FLASK_SERVER_URL = "http://192.168.0.80:5000/api/data";  // Flask API endpoint
const char* API_KEY = "yPowerAnalyzerKey456";   // MUST MATCH your .env file

// RS-485 configuration
#define MAX485_DE_RE 4
#define RXD2 16
#define TXD2 17

const unsigned long SEND_INTERVAL = 3000;  // Send every 3 seconds

// ==========================================
// GLOBALS
// ==========================================
ModbusMaster modbus;
unsigned long lastSend = 0;
int consecutiveErrors = 0;

// ==========================================
// RS-485 CONTROL
// ==========================================
void preTransmission() {
  digitalWrite(MAX485_DE_RE, HIGH);
  delayMicroseconds(100);
}
void postTransmission() {
  delayMicroseconds(100);
  digitalWrite(MAX485_DE_RE, LOW);
}

// ==========================================
// HELPER FUNCTIONS
// ==========================================
float readScaledRegister(uint16_t reg, uint8_t decimals) {
  uint8_t result = modbus.readHoldingRegisters(reg, 1);
  if (result != modbus.ku8MBSuccess) return NAN;
  uint16_t raw = modbus.getResponseBuffer(0);
  return raw * pow(10, decimals - 4);
}

float readRegisterNoScale(uint16_t reg) {
  uint8_t result = modbus.readHoldingRegisters(reg, 1);
  if (result != modbus.ku8MBSuccess) return NAN;
  return modbus.getResponseBuffer(0);
}

bool isValidReading(float value) {
  return !isnan(value) && value >= -999999 && value <= 999999;
}

// ==========================================
// WIFI CONNECTION CHECK
// ==========================================
void ensureWiFiConnected() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.println("\n⚠️ WiFi disconnected! Reconnecting...");
  WiFi.disconnect();
  delay(200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi reconnected!");
    Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\n❌ WiFi reconnection failed!");
  }
}

// ==========================================
// SEND DATA TO FLASK
// ==========================================
bool sendDataToFlask(JsonDocument& doc) {
  ensureWiFiConnected();
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ Cannot send - WiFi not connected!");
    return false;
  }

  HTTPClient http;
  http.setTimeout(10000);  // 10 seconds timeout

  Serial.println("\n╔════════════════════════════════════╗");
  Serial.println("║   Sending to Flask Server          ║");
  Serial.println("╠════════════════════════════════════╣");
  Serial.printf("║ URL: %-30s║\n", FLASK_SERVER_URL);
  Serial.printf("║ API Key: %-26s║\n", API_KEY);
  Serial.println("╚════════════════════════════════════╝");

  if (!http.begin(FLASK_SERVER_URL)) {
    Serial.println("❌ Failed to begin HTTP connection!");
    return false;
  }

  // ✅ Add required headers
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-API-Key", API_KEY);

  String jsonData;
  serializeJson(doc, jsonData);
  Serial.printf("JSON Size: %d bytes\n", jsonData.length());
  Serial.println("JSON Data:");
  Serial.println(jsonData);

  int httpResponseCode = http.POST(jsonData);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.printf("✅ SUCCESS! HTTP Code: %d\n", httpResponseCode);
    Serial.printf("Response: %s\n", response.c_str());
    Serial.println("╚════════════════════════════════════╝\n");
    http.end();
    consecutiveErrors = 0;
    return true;
  } else {
    Serial.printf("❌ HTTP ERROR! Code: %d\n", httpResponseCode);
    Serial.printf("   Error: %s\n", http.errorToString(httpResponseCode).c_str());
    Serial.println("╚════════════════════════════════════╝\n");
    http.end();
    consecutiveErrors++;
    if (consecutiveErrors >= 5) {
      Serial.println("⚠️ Too many errors, forcing WiFi reconnect...");
      consecutiveErrors = 0;
      WiFi.disconnect();
      delay(1000);
    }
    return false;
  }
}

// ==========================================
// SETUP
// ==========================================
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\n╔════════════════════════════════════╗");
  Serial.println("║  Three-Phase Power Analyzer        ║");
  Serial.println("║  ESP32 → Flask Integration         ║");
  Serial.println("╚════════════════════════════════════╝\n");

  // Initialize RS-485
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  pinMode(MAX485_DE_RE, OUTPUT);
  postTransmission();
  
  modbus.begin(1, Serial2);
  modbus.preTransmission(preTransmission);
  modbus.postTransmission(postTransmission);
  Serial.println("✅ Modbus initialized");

  // Connect WiFi
  Serial.printf("Connecting to WiFi: %s\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("✅ WiFi Connected!");
    Serial.printf("ESP32 IP Address: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("Flask Server Target: %s\n", FLASK_SERVER_URL);
  } else {
    Serial.println("❌ WiFi connection failed! Retrying later...");
  }

  Serial.println("\n╔════════════════════════════════════╗");
  Serial.println("║   READY TO SEND DATA               ║");
  Serial.println("╚════════════════════════════════════╝\n");
}

// ==========================================
// MAIN LOOP
// ==========================================
void loop() {
  // Send data at intervals
  if (millis() - lastSend < SEND_INTERVAL) {
    delay(10);
    return;
  }
  lastSend = millis();

  Serial.println("═══════════════════════════════════");
  Serial.println("Reading Modbus registers...");

  // Read decimal points (device-specific)
  uint8_t dpt = 4, dct = 4;
  if (modbus.readHoldingRegisters(0x0023, 1) == modbus.ku8MBSuccess) {
    uint16_t dp = modbus.getResponseBuffer(0);
    dpt = (dp >> 8) & 0xFF;
    dct = dp & 0xFF;
  }

  // Read phase & line voltages
  float ua = readScaledRegister(0x0025, dpt);
  float ub = readScaledRegister(0x0026, dpt);
  float uc = readScaledRegister(0x0027, dpt);
  float uab = readScaledRegister(0x0028, dpt);
  float ubc = readScaledRegister(0x0029, dpt);
  float uca = readScaledRegister(0x002A, dpt);

  // Read currents
  float ia = readScaledRegister(0x002B, dct);
  float ib = readScaledRegister(0x002C, dct);
  float ic = readScaledRegister(0x002D, dct);

  // Power & frequency
  float pf = NAN, hz = NAN, activePower = NAN, reactivePower = NAN, apparentPower = NAN;
  if (modbus.readHoldingRegisters(0x0039, 1) == modbus.ku8MBSuccess) pf = modbus.getResponseBuffer(0) / 100.0;
  if (modbus.readHoldingRegisters(0x003E, 1) == modbus.ku8MBSuccess) hz = modbus.getResponseBuffer(0) / 100.0;
  if (modbus.readHoldingRegisters(0x0031, 1) == modbus.ku8MBSuccess) activePower = modbus.getResponseBuffer(0) * 10;
  if (modbus.readHoldingRegisters(0x0035, 1) == modbus.ku8MBSuccess) reactivePower = modbus.getResponseBuffer(0) * 10;
  if (modbus.readHoldingRegisters(0x003D, 1) == modbus.ku8MBSuccess) apparentPower = modbus.getResponseBuffer(0);

  // THD
  float thdU_A = readRegisterNoScale(0x0400) / 100.0;
  float thdU_B = readRegisterNoScale(0x0401) / 100.0;
  float thdU_C = readRegisterNoScale(0x0402) / 100.0;
  float thdI_A = readRegisterNoScale(0x0403) / 100.0;
  float thdI_B = readRegisterNoScale(0x0404) / 100.0;
  float thdI_C = readRegisterNoScale(0x0405) / 100.0;

  // Averages
  float thdU_avg = (isValidReading(thdU_A) + isValidReading(thdU_B) + isValidReading(thdU_C)) ? 
                    (thdU_A + thdU_B + thdU_C) / 3.0 : NAN;
  float thdI_avg = (isValidReading(thdI_A) + isValidReading(thdI_B) + isValidReading(thdI_C)) ? 
                    (thdI_A + thdI_B + thdI_C) / 3.0 : NAN;

  // Print to serial
  Serial.println("=== THREE PHASE READINGS ===");
  Serial.printf("Phase Voltages: A=%.1fV B=%.1fV C=%.1fV\n", ua, ub, uc);
  Serial.printf("Line Voltages: AB=%.1fV BC=%.1fV CA=%.1fV\n", uab, ubc, uca);
  Serial.printf("Phase Currents: A=%.3fA B=%.3fA C=%.3fA\n", ia, ib, ic);
  Serial.printf("Power Factor: %.3f, Frequency: %.2fHz\n", pf, hz);
  Serial.printf("Active Power: %.1fW, Apparent Power: %.1fVA\n", activePower, apparentPower);
  Serial.println("============================");

  // Create JSON
  StaticJsonDocument<1024> doc;
  doc["voltage_a"] = ua;
  doc["voltage_b"] = ub;
  doc["voltage_c"] = uc;
  doc["voltage_ab"] = uab;
  doc["voltage_bc"] = ubc;
  doc["voltage_ca"] = uca;
  doc["current_a"] = ia;
  doc["current_b"] = ib;
  doc["current_c"] = ic;
  doc["power_factor"] = pf;
  doc["frequency"] = hz;
  doc["active_power"] = activePower;
  doc["reactive_power"] = reactivePower;
  doc["apparent_power"] = apparentPower;
  doc["voltage_thd_avg"] = thdU_avg;
  doc["current_thd_avg"] = thdI_avg;

  // Send to Flask
  bool success = sendDataToFlask(doc);

  if (success) {
    Serial.println("✅ Data sent and saved successfully!\n");
  } else {
    Serial.println("❌ Failed to send data. Will retry on next cycle.\n");
  }
}
