#include <ESP32Servo.h>  // ESP32용 Servo 라이브러리

Servo servo1;

const int motorPin = 2;  // ESP32 GPIO2에 연결
int angle1 = 0;          // 초기 각도 (0°)

void setup() {
  Serial.begin(115200);
  delay(100);

  // 50Hz(20ms) 서보 표준 주기 설정
  servo1.setPeriodHertz(50);
  // attach(pin, minPulse, maxPulse)
  servo1.attach(motorPin, 500, 2500);

  // 초기 위치
  servo1.write(angle1);

  Serial.println("Single Servo Control");
  Serial.println("  w: +30°   s: -30°");
  Serial.println("  0~180 각도 입력 시 해당 각도로 이동");
  Serial.println("Enter w/s or angle (0-180):");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() == 0) {
      // 빈 입력 무시
      return;
    }

    // ‘w’ 또는 ‘s’ 명령 처리
    if (input.equalsIgnoreCase("w")) {
      moveByDelta(+30);
    }
    else if (input.equalsIgnoreCase("s")) {
      moveByDelta(-30);
    }
    // 숫자 입력 처리
    else {
      int target = input.toInt();
      if (target < 0 || target > 180) {
        Serial.println("잘못된 각도입니다. 0~180 사이를 입력하세요.");
      } else {
        Serial.print("Moving to ");
        Serial.print(target);
        Serial.println("°");
        moveToAngle(target);
      }
    }

    Serial.println("Enter w/s or angle (0-180):");
  }
}

// 현재 각도에서 delta만큼 증감
void moveByDelta(int delta) {
  int target = constrain(angle1 + delta, 0, 180);
  moveToAngle(target);
}

// 현재 각도에서 target 각도까지 단계별로 이동
void moveToAngle(int target) {
  if (angle1 < target) {
    for (; angle1 < target; angle1++) {
      servo1.write(angle1);
      delay(10);
    }
  } else {
    for (; angle1 > target; angle1--) {
      servo1.write(angle1);
      delay(10);
    }
  }
  Serial.print("Current angle: ");
  Serial.println(angle1);
}
