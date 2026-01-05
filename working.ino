/************ BLYNK DEFINITIONS ************/
#define BLYNK_TEMPLATE_ID "TMPL3GpwjK6Nj"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "KN6k9uCC9a5dZozwC7higdK8nsDack78"

#define BLYNK_PRINT Serial
/******************************************/

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <AceButton.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

using namespace ace_button;

/* ---------- WIFI ---------- */
char ssid[] = "ESP32_Plant_System";
char pass[] = "12345678";

/* ---------- PINS ---------- */
#define SOIL_PIN     34
#define RELAY_PIN    25
#define BUTTON_PUMP  32
#define BUTTON_MODE  33
#define DHT_PIN      14
#define LED_PUMP     2
#define LED_MODE     15

/* ---------- OLED ---------- */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/* ---------- DHT ---------- */
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);

/* ---------- BUTTONS ---------- */
ButtonConfig pumpCfg, modeCfg;
AceButton pumpBtn(&pumpCfg);
AceButton modeBtn(&modeCfg);

/* ---------- VARIABLES ---------- */
bool autoMode = true;
bool pumpON = false;
bool sensorReady = false;

int soilRaw = 0;
int soilPercent = 0;
int temperature1 = 0;
int humidity1 = 0;

const int DRY = 3050;
const int WET = 1050;

/* ---------- BLYNK VIRTUAL PINS ---------- */
#define VPUMP   V5
#define VMODE   V6
#define VTEMP   V11
#define VHUM    V8
#define VSOIL   V12

/* ---------- BLYNK CONTROLS ---------- */
BLYNK_WRITE(VPUMP) {
  if (!autoMode) {
    pumpON = param.asInt();
    digitalWrite(RELAY_PIN, pumpON ? LOW : HIGH);
    digitalWrite(LED_PUMP, pumpON);
  }
}

BLYNK_WRITE(VMODE) {
  autoMode = param.asInt();
  digitalWrite(LED_MODE, autoMode);
}

/* ---------- BUTTON HANDLERS ---------- */
void pumpButtonHandler(AceButton*, uint8_t eventType, uint8_t) {
  if (eventType == AceButton::kEventReleased && !autoMode) {
    pumpON = !pumpON;
    digitalWrite(RELAY_PIN, pumpON ? LOW : HIGH);
    digitalWrite(LED_PUMP, pumpON);
    Blynk.virtualWrite(VPUMP, pumpON);
  }
}

void modeButtonHandler(AceButton*, uint8_t eventType, uint8_t) {
  if (eventType == AceButton::kEventReleased) {
    autoMode = !autoMode;
    digitalWrite(LED_MODE, autoMode);
    Blynk.virtualWrite(VMODE, autoMode);
  }
}

/* ---------- SENSOR READ ---------- */
void readSensors() {
  soilRaw = analogRead(SOIL_PIN);
  soilPercent = map(soilRaw, DRY, WET, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100);

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (!isnan(t) && !isnan(h)) {
    temperature1 = (int)t;
    humidity1 = (int)h;
    sensorReady = true;
  }

  Blynk.virtualWrite(VSOIL, soilPercent);
  Blynk.virtualWrite(VTEMP, temperature1);
  Blynk.virtualWrite(VHUM, humidity1);
}

/* ---------- AUTO MODE ---------- */
void autoControl() {
  if (!autoMode || !sensorReady) return;

  if (soilPercent < 20) pumpON = true;
  else if (soilPercent > 80) pumpON = false;

  digitalWrite(RELAY_PIN, pumpON ? LOW : HIGH);
  digitalWrite(LED_PUMP, pumpON);
  Blynk.virtualWrite(VPUMP, pumpON);
}

/* ---------- OLED ---------- */
void updateOLED() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);   // 🔴 CRITICAL FIX
  display.setCursor(0,0);

  display.print("Soil: "); display.print(soilPercent); display.println("%");
  display.print("Temp: "); display.print(temperature1); display.println(" C");
  display.print("Hum : "); display.print(humidity1); display.println(" %");
  display.print("Mode: "); display.println(autoMode ? "AUTO" : "MANUAL");

  display.display();
}

/* ---------- TIMER ---------- */
BlynkTimer timer;

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PUMP, OUTPUT);
  pinMode(LED_MODE, OUTPUT);
  pinMode(BUTTON_PUMP, INPUT_PULLUP);
  pinMode(BUTTON_MODE, INPUT_PULLUP);

  digitalWrite(RELAY_PIN, HIGH);   // motor OFF
  digitalWrite(LED_PUMP, LOW);
  digitalWrite(LED_MODE, HIGH);

  dht.begin();

  Wire.begin(21, 22);
  delay(100);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("OLED READY");
  display.display();
  delay(1000);

  pumpCfg.setEventHandler(pumpButtonHandler);
  modeCfg.setEventHandler(modeButtonHandler);
  pumpBtn.init(BUTTON_PUMP);
  modeBtn.init(BUTTON_MODE);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, readSensors);
  timer.setInterval(2000L, autoControl);
  timer.setInterval(3000L, updateOLED);
}

void loop() {
  Blynk.run();
  timer.run();
  pumpBtn.check();
  modeBtn.check();
}
