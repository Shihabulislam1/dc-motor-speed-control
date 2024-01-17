#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "motor"; // Replace with your WiFi SSID
const char *password = "12345678"; // Replace with your WiFi password
const char *motorControlAPI = "http://api/motorspeedcontrol";
const char *speedUpdateAPI = "http://api/motorspeedcontrolcontrol";
const char *cmdAPI = "http://api/cmd";

const int IN3 = D7;
const int IN4 = D6;
const int ENB = D8;
const int irSensorPin = A0;

int currentSpeed = 0;

void setup() {
  Serial.begin(115200);

  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(irSensorPin, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  updateSpeedBasedOnSensor();
  controlMotor();
  delay(1000); // Adjust the delay as needed
}

void updateSpeedBasedOnSensor() {
  int irSensorValue = analogRead(irSensorPin);
  currentSpeed = map(irSensorValue, 0, 1023, 0, 255);
}

void controlMotor() {
  HTTPClient http;

  // Fetch the current speed from the motor control API
  http.begin(motorControlAPI);
  int httpCode = http.GET();
  if (httpCode == 200) {
    String speedStr = http.getString();
    int motorControlSpeed = speedStr.toInt();
    digitalWrite(ENB, motorControlSpeed)
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    digitalWrite(ENB, motorControlSpeed)
  }
  http.end();

  // Send the updated speed to the motor control API
  String speedPayload = "speed=" + String(currentSpeed);
  http.begin(speedUpdateAPI);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.POST(speedPayload);
  http.end();

  // Receive commands from the cmd API
  http.begin(cmdAPI);
  httpCode = http.GET();
  if (httpCode == 200) {
    String cmd = http.getString();
    if (cmd == "start") {
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    } else if (cmd == "stop") {
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }
  }
  http.end();
}
