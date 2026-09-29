#define BLYNK_TEMPLATE_ID "TMPL3HWg5MWn_"
#define BLYNK_TEMPLATE_NAME "SMART LIGHT"
#define BLYNK_AUTH_TOKEN "E1nrUPGIvtPnW7cUnC27_CxWvrL_5I7r"


#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Blynk credentials

char ssid[] = "sanath_gpt";
char pass[] = "123456789";

// Pins
#define LDR_PIN 14     // Digital LDR
#define IR_PIN 5
#define RELAY_PIN 12
#define ACS_PIN A0
#define rr 4

unsigned long lastMotionTime = 0;
int set = 0;

BlynkTimer timer;

void setup() {
  Serial.begin(115200);

  pinMode(LDR_PIN, INPUT);
  pinMode(IR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(rr, OUTPUT);
  digitalWrite(rr, LOW);
  digitalWrite(RELAY_PIN, LOW);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, sendData);
}
BLYNK_WRITE(V2){
   set = param.asInt();
  digitalWrite(rr, set);
}
void loop() {
  Blynk.run();
  timer.run();

  bool ldrState = digitalRead(LDR_PIN);  // LOW = DARK
  bool irState = digitalRead(IR_PIN);    // HIGH = MOTION
  
  // Only work in DARK
  if (ldrState == 1) 
  {
     digitalWrite(RELAY_PIN, HIGH);
     if (set == 1){
      digitalWrite(rr, HIGH);
     }
     else {
      digitalWrite(rr, LOW);
     }
    if (irState == 0) {
      lastMotionTime = millis();
                  }

    // If no motion for 30 sec
    if (millis() - lastMotionTime > 10000) {
      digitalWrite(RELAY_PIN, LOW);
    }

  } else {
    // Bright condition → OFF
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(rr, LOW);
  }
}

// Send ACS712 data to Blynk
void sendData() {
  int raw = analogRead(ACS_PIN);

  float voltage = (1023 - raw) * (3.3 / 1023.0);
  Serial.print("voltage: ");
  Serial.println(voltage);
  
  float current = (voltage - 1.40) / 0.90;  // adjust based on module

  Serial.print("Current: ");
  Serial.println(current);
  Blynk.virtualWrite(V0, current);
  Blynk.virtualWrite(V1, voltage);
}
