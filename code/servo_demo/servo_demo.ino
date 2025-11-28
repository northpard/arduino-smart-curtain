#include <Servo.h>

Servo servo;

int sensorPin = A0;    // LDR 연결 핀
int threshold = 500;   // 밝기 기준값 (실측 후 수정)

int openPos  = 90;     // 커튼 열린 상태
int closePos = 0;      // 커튼 닫힌 상태

bool isOpen = false;

void setup() {
  Serial.begin(9600);
  servo.attach(9);
  servo.write(closePos);
}

void loop() {
  int value = analogRead(sensorPin);
  Serial.println(value);

  if (value > threshold && !isOpen) {
    servo.write(openPos);
    isOpen = true;
    delay(500);
  }

  if (value < threshold && isOpen) {
    servo.write(closePos);
    isOpen = false;
    delay(500);
  }

  delay(100);
}
