#define BLYNK_TEMPLATE_ID "TMPL3jShAc33I"
#define BLYNK_TEMPLATE_NAME "home automation"
#define BLYNK_AUTH_TOKEN "3svgjSz2_4FrRHggSijRMJsqW-WQL_T7"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <BlynkSimpleEsp8266.h>

// WiFi
char ssid[] = "sanathkumar";
char pass[] = "";
unsigned lastMotionTime = 0;
//bool motionDetected = true;
// Pins
#define RELAY1 14
#define RELAY2 12
#define PIR_PIN 10

ESP8266WebServer server(80);

// Web activity timer
unsigned long lastWebAccess = 0;
bool webActive = false;

// ------------------ WEB SERVER ------------------

void handleRoot() {
  webActive = true;
  lastWebAccess = millis();

  String page = R"====(
  <!DOCTYPE html>
  <html>
  <head>
    <title>ESP8266 Control</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <script>
      setInterval(function(){
        fetch('/ping');   // send heartbeat
      }, 2000);
    </script>
    
    <style>
      body {
        font-family: Arial;
        text-align: center;
        background-color: #0f172a;
        color: white;
      }
      h1 {
        margin-top: 20px;
      }
      .btn {
        display: block;
        width: 200px;
        margin: 15px auto;
        padding: 15px;
        font-size: 18px;
        border: none;
        border-radius: 10px;
        cursor: pointer;
      }
      .on {
        background-color: #22c55e;
        color: white;
      }
      .off {
        background-color: #ef4444;
        color: white;
      }
    </style>
  </head>

  <body>
    <h1>ESP8266 Device Control</h1>

    <h3>Device 1</h3>
    <button class="btn on" onclick="location.href='/on1'">ON</button>
    <button class="btn off" onclick="location.href='/off1'">OFF</button>

    <h3>Device 2</h3>
    <button class="btn on" onclick="location.href='/on2'">ON</button>
    <button class="btn off" onclick="location.href='/off2'">OFF</button>

  </body>
  </html>
  )====";

  server.send(200, "text/html", page);
}
void handleOn1() {
  webActive = true;
  lastWebAccess = millis();
  digitalWrite(RELAY1, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleOff1() {
  webActive = true;
  lastWebAccess = millis();
  digitalWrite(RELAY1, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleOn2() {
  webActive = true;
  lastWebAccess = millis();
  digitalWrite(RELAY2, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleOff2() {
  webActive = true;
  lastWebAccess = millis();
  digitalWrite(RELAY2, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}
void handlePing() {
  webActive = true;
  lastWebAccess = millis();
  server.sendHeader("Location", "/");
  server.send(303);
}
// ------------------ BLYNK ------------------

BLYNK_WRITE(V0) {
  if (!webActive)
    digitalWrite(RELAY1, param.asInt());
}

BLYNK_WRITE(V1) {
  if (!webActive)
    digitalWrite(RELAY2, param.asInt());
}

// ------------------ SETUP ------------------

void setup() {
  Serial.begin(9600);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(PIR_PIN, INPUT);
  
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.println(WiFi.localIP());

  // Web routes
  server.on("/", handleRoot);
  server.on("/on1", handleOn1);
  server.on("/off1", handleOff1);
  server.on("/on2", handleOn2);
  server.on("/off2", handleOff2);
  server.on("/ping", handlePing);
  
  server.begin();

  // Blynk setup
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

// ------------------ LOOP ------------------

void loop() {

  server.handleClient();

  // If browser sending heartbeat → web active
  if (millis() - lastWebAccess < 4000) {
    webActive = true;

    if (Blynk.connected()) {
      Blynk.disconnect();
    }

  } else {
    webActive = false;

    if (!Blynk.connected()) {
      Blynk.connect();
    }

    Blynk.run();
  }

  if (digitalRead(PIR_PIN) == HIGH){
      lastMotionTime = millis();
       
    }

  // ---------- NO MOTION ----------
  if (millis() - lastMotionTime > 30000) {
   
    digitalWrite(RELAY1, LOW);
    digitalWrite(RELAY2, LOW);  
  }
}
