#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// ESP32 I2C 핀 정의
#define SDA_PIN    21
#define SCL_PIN    22

// PCA9685 16채널 서보 드라이버
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN   150
#define SERVOMAX   600
#define SERVO_FREQ 50

/* ------------- LOWER ------------- */

// [] 그룹에 묶어서 제어하는 다리 수
// Front Leg -->FLEG
const uint8_t FLEG_SIZE = 1;

// A: Front Left Leg (힙, 무릎, 발목) 서브 서보 -> Ankle // 메인 서보 --> Knee // Roll 축 서보 --> Hip
const uint8_t HIP_A[FLEG_SIZE] = {2};
const uint8_t KNEE_A[FLEG_SIZE] = {1};
const uint8_t ANKLE_A[FLEG_SIZE] = {0};

// B: Back Left Leg (힙, 무릎, 발목)
const uint8_t HIP_B[FLEG_SIZE] = {6};
const uint8_t KNEE_B[FLEG_SIZE] = {5};
const uint8_t ANKLE_B[FLEG_SIZE] = {4};

/* ------------- UPPER ------------- */

// [] 그룹에 묶어서 제어하는 다리 수
// Back Leg --> BLEG
const uint8_t BLEG_SIZE = 1;

// C: Front Right Leg (힙, 무릎, 발목)
const uint8_t HIP_C[BLEG_SIZE] = {10};
const uint8_t KNEE_C[BLEG_SIZE] = {9};
const uint8_t ANKLE_C[BLEG_SIZE] = {8};

// D: Back Right Leg (힙, 무릎, 발목)
const uint8_t HIP_D[BLEG_SIZE] = {14};
const uint8_t KNEE_D[BLEG_SIZE] = {13};
const uint8_t ANKLE_D[BLEG_SIZE] = {12};


// PWM 서보 각도 설정
void setAngle(uint8_t ch, float angle) {
  angle = constrain(angle, 0, 180);
  uint16_t pulse = map((int)angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(ch, 0, pulse);
}

// ============= FRONT ============= 
void setGroupA(float hip, float knee, float ankle) {    // Left
  setAngle(HIP_A[0], hip);
  setAngle(KNEE_A[0], knee);
  setAngle(ANKLE_A[0], ankle);
}

void setGroupB(float hip, float knee, float ankle) {    // Right
  setAngle(HIP_B[0], hip);
  setAngle(KNEE_B[0], knee);
  setAngle(ANKLE_B[0], ankle);
}

// ============= BACk ============= 
void setGroupC(float hip, float knee, float ankle) {    // Legt
  setAngle(HIP_C[0], hip);
  setAngle(KNEE_C[0], knee);
  setAngle(ANKLE_C[0], ankle);
}
void setGroupD(float hip, float knee, float ankle) {    // Right
  setAngle(HIP_D[0], hip);
  setAngle(KNEE_D[0], knee);
  setAngle(ANKLE_D[0], ankle);
}


void setup() {
  Serial.begin(115200);
  delay(100);
  Wire.begin(SDA_PIN, SCL_PIN);
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);

  Serial.println("QV2 Control Ready");

  // 초기 포즈: 다리 모두 중립(90°)
  setGroupA(50, 70, 80);
  setGroupB(87, 70, 70);
  setGroupC(75, 80, 75);
  setGroupD(77, 85, 70);
}

void loop() {
  if (!Serial.available()) return;

  char cmd = Serial.read();

  switch (cmd) {

    /* ------------- LOWER ------------- */

    case 'q':
    case 'Q': // stand up
      setGroupA(50, 70, 80);
      setGroupB(87, 70, 70);
      setGroupC(75, 80, 75);
      setGroupD(77, 85, 70);
      Serial.println("Q: ");
      break;

    case 'a':
    case 'A': // lean foward
      setGroupA(50, 100, 125);
      setGroupB(87, 70, 70);
      setGroupC(75, 50, 30);
      setGroupD(77, 85, 70);
      Serial.println("A: ");
      break;

    case 's':
    case 'S': // lean backward
      setGroupA(50, 70, 80);
      setGroupB(87, 100, 115);
      setGroupC(75, 80, 75);
      setGroupD(77, 55, 25);
      Serial.println("S: ");
      break;

    case 'd':
    case 'D': // lean left
      setGroupA(50, 100, 125);
      setGroupB(87, 100, 115);
      setGroupC(75, 80, 75);
      setGroupD(77, 85, 70);
      Serial.println("D: ");
      break;

    case 'f':
    case 'F': // lean right
      setGroupA(50, 70, 80);
      setGroupB(87, 70, 70);
      setGroupC(75, 50, 30);
      setGroupD(77, 55, 25);
      Serial.println("F: ");
      break;

    case 'p':
    case 'P': // stand up (for walk)
      setGroupA(50, 50, 40);
      setGroupB(87, 50, 30);
      setGroupC(75, 100, 115);
      setGroupD(77, 105, 110);
      Serial.println("P: ");
      break;

    case 'w':
    case 'W': // cycle walk
      setGroupA(50, 50, 40);
      setGroupB(87, 50, 30);
      setGroupC(75, 100, 115);
      setGroupD(77, 105, 110);
      delay(1000);

      setGroupA(50, 80, 100);
      delay(30);
      setGroupD(77, 75, 50);
      delay(140);
      setGroupD(77, 105, 110);
      setGroupA(50, 50, 40);
      
      delay(200);
      setGroupC(75, 70, 55);
      delay(5);
      setGroupB(87, 80, 90);
      
      
      delay(150);

      
      setGroupC(75, 100, 115);
      delay(5);
      setGroupB(87, 50, 30);

      Serial.println("W: ");
      delay(200);
      break;
    
    case 'e':
    case 'E': // cycle walk foward
      setGroupA(50, 50, 40);
      setGroupB(87, 50, 30);
      setGroupC(75, 100, 115);
      setGroupD(77, 105, 110);
      delay(250);

      setGroupA(50, 80, 100);
      delay(20);
      setGroupD(77, 75, 50);
      delay(150);

      setGroupA(50, 45, 40);
      setGroupD(77, 110, 110);
      delay(200);

      setGroupC(75, 70, 55);
      delay(10);
      setGroupB(87, 80, 90);
      
      
      delay(150);

      
      setGroupC(75, 105, 115);
      delay(5);
      setGroupB(87, 45, 30);

      Serial.println("W: ");
      delay(200);
      break;

    default:
      // 기타 입력 무시
      break;
  }
}