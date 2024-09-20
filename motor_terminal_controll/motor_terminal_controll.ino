#include <Encoder.h>

// 엔코더 정의
Encoder encoderA(18, 31);
Encoder encoderB(19, 38);
Encoder encoderC(3, 49);
Encoder encoderD(2, 55);

// 모터 제어 핀 설정
const int motorA_DirectionPin1 = 34;
const int motorA_DirectionPin2 = 35;
const int motorA_PWM_Pin = 12;

const int motorB_DirectionPin1 = 37;
const int motorB_DirectionPin2 = 36;
const int motorB_PWM_Pin = 8;

const int motorC_DirectionPin1 = 43;
const int motorC_DirectionPin2 = 42;
const int motorC_PWM_Pin = 6;

const int motorD_DirectionPin1 = 58;
const int motorD_DirectionPin2 = 59;
const int motorD_PWM_Pin = 5;

// 모터 PWM 값 저장
int motorAValue = 0;
int motorBValue = 0;
int motorCValue = 0;
int motorDValue = 0;

void setup() {
  Serial.begin(9600);  
  Serial3.begin(9600); 

  // 모터 핀을 출력으로 설정
  pinMode(motorA_DirectionPin1, OUTPUT);
  pinMode(motorA_DirectionPin2, OUTPUT);
  pinMode(motorA_PWM_Pin, OUTPUT);
  
  pinMode(motorB_DirectionPin1, OUTPUT);
  pinMode(motorB_DirectionPin2, OUTPUT);
  pinMode(motorB_PWM_Pin, OUTPUT);

  pinMode(motorC_DirectionPin1, OUTPUT);
  pinMode(motorC_DirectionPin2, OUTPUT);
  pinMode(motorC_PWM_Pin, OUTPUT);

  pinMode(motorD_DirectionPin1, OUTPUT);
  pinMode(motorD_DirectionPin2, OUTPUT);
  pinMode(motorD_PWM_Pin, OUTPUT);
}

void loop() {
  // 블루투스 모듈로부터 명령어 읽기
  if (Serial3.available() > 0) {
    String command = Serial3.readStringUntil('\n'); // 한 줄 읽기
    handleCommand(command);
  }

  // 시리얼 0번으로부터 명령어 읽기
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n'); // 한 줄 읽기
    handleCommand(command);
  }

  delay(100);
}

void handleCommand(String command) {
  Serial.print("받은 명령: ");
  Serial.println(command); // 받은 명령어 시리얼 모니터에 출력

  // 각 모터의 새로운 PWM 값을 설정
  updateMotorValue(command, 'a', motorAValue);
  updateMotorValue(command, 'b', motorBValue);
  updateMotorValue(command, 'c', motorCValue);
  updateMotorValue(command, 'd', motorDValue);

  // 모터의 방향 및 PWM 값 설정
  setMotorDirectionAndPWM('a', motorAValue);
  setMotorDirectionAndPWM('b', motorBValue);
  setMotorDirectionAndPWM('c', motorCValue);
  setMotorDirectionAndPWM('d', motorDValue);

  // 출력값을 시리얼 0번으로 출력
  Serial.print("모터 출력: a:");
  Serial.print(motorAValue);
  Serial.print(" b:");
  Serial.print(motorBValue);
  Serial.print(" c:");
  Serial.print(motorCValue);
  Serial.print(" d:");
  Serial.println(motorDValue);
}

void updateMotorValue(String command, char motor, int &motorValue) {
  int value = getMotorValue(command, motor);
  if (value != -1) {
    motorValue = value;  // 값이 존재할 경우에만 업데이트
  }
}

void setMotorDirectionAndPWM(char motor, int pwmValue) {
  int direction = (pwmValue >= 0) ? 1 : 0;  // PWM 값이 양수면 정방향, 음수면 역방향
  int absPWMValue = abs(pwmValue);  // PWM 값은 절대값으로 설정
  
  if (motor == 'a') {
    digitalWrite(motorA_DirectionPin1, direction);
    digitalWrite(motorA_DirectionPin2, !direction);
    analogWrite(motorA_PWM_Pin, absPWMValue);
  } else if (motor == 'b') {
    digitalWrite(motorB_DirectionPin1, direction);
    digitalWrite(motorB_DirectionPin2, !direction);
    analogWrite(motorB_PWM_Pin, absPWMValue);
  } else if (motor == 'c') {
    // 모터 C의 방향 반전
    digitalWrite(motorC_DirectionPin1, !direction);
    digitalWrite(motorC_DirectionPin2, direction);
    analogWrite(motorC_PWM_Pin, absPWMValue);
  } else if (motor == 'd') {
    // 모터 D의 방향 반전
    digitalWrite(motorD_DirectionPin1, !direction);
    digitalWrite(motorD_DirectionPin2, direction);
    analogWrite(motorD_PWM_Pin, absPWMValue);
  }
}

int getMotorValue(String command, char motor) {
  int index = command.indexOf(motor);
  if (index != -1) {
    int colonIndex = command.indexOf(':', index);
    if (colonIndex != -1) {
      int endIndex = command.indexOf(' ', colonIndex);
      if (endIndex == -1) endIndex = command.length();
      return command.substring(colonIndex + 1, endIndex).toInt();
    }
  }
  return -1;  // 값을 찾지 못했을 때는 -1 반환
}