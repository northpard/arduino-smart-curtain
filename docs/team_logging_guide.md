# IoT 로그 수집 가이드 (팀원용)

이 문서는 팀원들이 공통 RDS(Database)에 접근하고, 각자 아두이노에서 **로그 포맷을 통일**하기 위한 최소 규칙을 정리한 것입니다.

> 중요한 점: 각자의 아두이노에서 **정해진 형식의 로그 문자열만 출력**하면 됩니다. 
> 그 로그를 읽어서 RDS `ioclean.curtain_log` 테이블에 올리는 작업은 중앙에서 담당합니다.

---

## 1. RDS(MySQL) 접속 정보

- **호스트**: `database-northpard.c9ygm4yuimcy.ap-northeast-2.rds.amazonaws.com`
- **포트**: `3306`
- **DB 이름**: `ioclean`
- **유저**: `ioclean_user`
- **비밀번호**: (별도 전달)

### 1.1. 터미널에서 접속하는 방법

```bash
mysql -h database-northpard.c9ygm4yuimcy.ap-northeast-2.rds.amazonaws.com \
      -P 3306 \
      -u ioclean_user \
      -p \
      ioclean
```

프롬프트가 뜨면 비밀번호를 입력하면 됩니다.

접속 후 기본 확인 명령:

```sql
SHOW DATABASES;
USE ioclean;
SHOW TABLES;
```

현재 스마트 커튼 로그는 `curtain_log` 테이블에 저장됩니다.

---

## 2. 로그 테이블 구조 (`curtain_log`)

```sql
CREATE TABLE curtain_log (
    id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
    device_id VARCHAR(50) NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    light_value INT NOT NULL,
    motor_direction TINYINT NOT NULL,
    current_step INT NOT NULL,
    max_steps INT NOT NULL,
    PRIMARY KEY (id),
    INDEX idx_created_at (created_at),
    INDEX idx_device_time (device_id, created_at)
);
```

각 컬럼 의미:

- `device_id`: 장치 ID (예: `curtain-01`, `curtain-02`)
- `created_at`: DB에 기록된 시각
- `light_value`: 센서 값 (예: LDR 조도값 0~1023)
- `motor_direction`: 모터 방향 (`1`: 정방향/열림, `-1`: 역방향/닫힘, `0`: 정지)
- `current_step`: 현재 위치(스텝 단위)
- `max_steps`: 완전 닫힘→완전 열림까지 필요한 총 스텝 수

---

## 3. 팀 공통 로그 포맷 규칙

각자의 아두이노 스케치에서 **시리얼(Serial)** 로 아래와 같은 한 줄 로그를 주기적으로 출력해 주세요.

### 3.1. 포맷 (CSV 한 줄)

```text
device_id,light_value,motor_direction,current_step,max_steps
```

예시:

```text
curtain-01,842,0,6144,6144
```

- `device_id`: 자신의 장치 고유 이름
- `light_value`: 센서 측정값 (정수)
- `motor_direction`: `1`(정방향), `-1`(역방향), `0`(정지)
- `current_step`: 현재 위치(정수)
- `max_steps`: 장치 기준 최대 스텝 값(정수)

### 3.2. 장치 ID 규칙 예시

- 스마트 커튼 1번: `curtain-01`
- 스마트 커튼 2번: `curtain-02`
- (필요 시) 다른 장치: `door-01`, `fan-01` 등 자유롭게, 단 고유하게

---

## 4. 아두이노 측 구현 예시 (개념)

아두이노 코드에서는 대략 아래와 같이 `Serial.println`으로 한 줄씩 출력하면 됩니다.

```cpp
const char* deviceId = "curtain-01";

void loop() {
  int lightValue = analogRead(A0);   // 예: LDR 값
  int motorDirection = 0;            // 1 / -1 / 0 중 하나
  long currentStep = 6144;           // 현재 스텝 위치
  long maxSteps = 6144;              // 최대 스텝 수

  Serial.print(deviceId);
  Serial.print(",");
  Serial.print(lightValue);
  Serial.print(",");
  Serial.print(motorDirection);
  Serial.print(",");
  Serial.print(currentStep);
  Serial.print(",");
  Serial.println(maxSteps);

  delay(3000); // 3초마다 기록 예시
}
```

> 각자 장치에 맞게 `lightValue`, `motorDirection`, `currentStep`, `maxSteps` 계산 로직은 자유롭게 구현하면 됩니다.

---

## 5. DB 업로드 파이프라인 (참고용)

- 팀 공통 파이프라인에서 PC/노트북이 시리얼 포트(`/dev/ttyACM*`, `/dev/ttyUSB*`)를 통해 위 로그를 읽습니다.
- 파이썬 스크립트(`tools/curtain_log_uploader.py`)가
  - 한 줄 로그를 파싱하고
  - `INSERT INTO curtain_log (...) VALUES (...);` 형태로 RDS에 기록합니다.

팀원들은 **아두이노 쪽에서 로그 포맷만 맞춰주면** 되고, 
RDS 업로드 스크립트와 운영은 중앙에서 관리합니다.
