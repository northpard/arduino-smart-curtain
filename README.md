# Arduino Smart Curtain (LDR + Stepper/Servo)

이 프로젝트는 **조도센서(LDR)** 를 이용하여

- **밝을 때 → 커튼 열기**
- **어두울 때(LDR을 손/종이로 가림) → 커튼 닫기**

자동으로 동작하는 **아두이노 기반 스마트 커튼 시스템**입니다.

특히,  
✔ 좌우 한쌍의 커튼을  
✔ 줄(끈)을 스텝모터로 감거나 풀어서  
✔ 부드럽게 열리고 닫히는 구조  
를 시뮬레이션/제작할 수 있도록 구성되어 있습니다.

---

# ✨ 기능 요약

- LDR 조도센서로 밝기 감지  
- 밝기 기준(threshold) 이상 → 커튼 자동 열림  
- 밝기 기준(threshold) 이하 → 커튼 자동 닫힘  
- 서보모터 버전(단순 시뮬레이션) 포함  
- 스텝모터(28BYJ-48) + ULN2003 드라이버 버전 포함  
- 커튼 좌/우 양쪽을 한 번에 제어하는 구조 지원  

---

# 📁 프로젝트 구조

```
arduino-smart-curtain/
├── code/
│   ├── servo_demo/
│   │   └── servo_demo.ino
│   └── stepper_ldr_curtain/
│       └── stepper_ldr_curtain.ino
├── docs/
│   ├── wiring_servo.md
│   └── wiring_stepper_ldr.md
└── hardware/
    └── bill_of_materials.md
```

---

# 🧪 동작 개념

## 1) LDR 조도 기반 자동 제어

- 밝으면 → 값 증가 → 커튼 열림  
- 어두우면 → 값 감소 → 커튼 닫힘  

---

# 🎬 데모 1: Servo 기반 커튼 시뮬레이터

코드 → `code/servo_demo/servo_demo.ino`  
배선 → `docs/wiring_servo.md`

---

# 🧵 데모 2: Stepper 기반 좌우 자동 커튼

코드 → `code/stepper_ldr_curtain/stepper_ldr_curtain.ino`  
배선 → `docs/wiring_stepper_ldr.md`

---

# 🔧 필요한 부품 (BOM)

```
- Arduino UNO 또는 Nano
- LDR + 10kΩ 저항
- 28BYJ-48 스텝모터 + ULN2003 드라이버
- SG90 또는 MG996R 서보모터
- 점퍼 케이블
- 브레드보드
- 끈/와이어 + 릴
- 5V 전원
```

---

# 🚀 확장 기능

- RTC(DS3231) 기반 일정 시간 개폐  
- ESP32 WiFi 제어  
- 리미트 스위치로 끝점 자동정지  
- 진짜 커튼 레일 기반 버전 제작  

---

# 📝 License

MIT License 추천
