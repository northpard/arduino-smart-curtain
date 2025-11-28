#include <Stepper.h>

const int stepsPerRevolution = 2048;  // 28BYJ-48 기준
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

int sensorPin = A0;
int threshold = 500;

// 커튼 완전 개폐용 스텝 수 (여러 바퀴)
// 예: 5바퀴 -> 5 * stepsPerRevolution
long curtainSteps = 5L * stepsPerRevolution;

bool isOpen = false;

void setup() {
  Serial.begin(9600);
  myStepper.setSpeed(10);  // RPM
}

void loop() {
  int value = analogRead(sensorPin);
  Serial.println(value);

  if (value > threshold && !isOpen) {
    openCurtain();
    isOpen = true;
  }

  if (value < threshold && isOpen) {
    closeCurtain();
    isOpen = false;
  }

  delay(300);
}

void openCurtain() {
  myStepper.step(curtainSteps);
}

void closeCurtain() {
  myStepper.step(-curtainSteps);
}
