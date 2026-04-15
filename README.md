# nu87dk-ble-wifi-attendance

> **BLE + WiFi Attendance System using NU87 DK (RTL8720DF)**
> NU87 DK (RTL8720DF) 를 활용한 BLE + WiFi 출석 시스템

📝 [Blog Post (Naver)](#)

---

## Overview | 개요

| EN | KR |
|---|---|
| An attendance system that detects students' BLE beacon signals and reflects attendance status on a real-time web dashboard via WiFi. | 학생 폰의 BLE 비콘 신호를 감지해 WiFi로 웹 대시보드에 출석 현황을 실시간 반영하는 시스템 |
| No dedicated app required — uses nRF Connect's built-in Advertiser feature. | 별도 앱 개발 없이 nRF Connect 앱의 Advertiser 기능만으로 동작 |
| NU87 DK supports dual-band WiFi (2.4/5GHz), enabling BLE and WiFi to operate on separate bands — eliminating interference that affects ESP32. | NU87 DK의 듀얼밴드 WiFi로 BLE(2.4GHz)와 WiFi(5GHz) 대역을 분리해 간섭 없이 동시 동작 |

---

## Why NU87 DK? | 왜 NU87 DK인가?

| Feature | NU87 DK (RTL8720DF) | ESP32 |
|---|---|---|
| WiFi Band | **2.4GHz + 5GHz** | 2.4GHz only |
| BLE | 5.0 | 4.2 |
| BLE + WiFi interference | ✅ No interference (separate bands) | ⚠️ Possible (shared 2.4GHz) |
| Arduino IDE support | ✅ | ✅ |

---

## How It Works | 동작 방식

```
[Student Phone]                [NU87 DK]               [Browser]
nRF Connect Advertiser  -BLE-> BLE Scanner   -WiFi 5GHz-> Web Dashboard
(UUID broadcasting)            (UUID matching)          (Attendance status)
```

1. Each student sets a unique UUID in nRF Connect Advertiser
   학생마다 nRF Connect Advertiser에서 고유 UUID 설정
2. NU87 DK continuously scans for BLE beacons
   NU87 DK가 주변 BLE 비콘을 지속적으로 스캔
3. When a registered UUID is detected (RSSI > -70dBm), attendance is marked
   등록된 UUID가 감지되면 (RSSI > -70dBm) 출석 처리
4. Attendance status is served via built-in WiFi web server
   출석 현황은 보드 내장 WiFi 웹서버로 실시간 제공

---

## Requirements | 필요 사항

### Hardware | 하드웨어
- NU87 DK (RTL8720DF)
- USB-C cable

### Software | 소프트웨어
- Arduino IDE
- Realtek Ameba Arduino SDK

  Add this URL to Board Manager:
  ```
  https://github.com/ambiot/ambd_arduino/raw/master/Arduino_package/package_realtek.com_amebad_index.json
  ```
  Select board: **AMB25/AMB26 (RTL8720DF)**

### Mobile App | 모바일 앱
- [nRF Connect for Mobile](https://www.nordicsemi.com/Products/Development-tools/nrf-connect-for-mobile) (iOS / Android, Free)

---

## Setup | 설정

### 1. Student UUID Registration | 학생 UUID 등록

Edit `studentUUIDs` and `studentNames` in the sketch:

```cpp
const char* studentUUIDs[] = {
    "12345678-1234-1234-1234-000000000001",  // Student-01
    "12345678-1234-1234-1234-000000000002",  // Student-02
    "12345678-1234-1234-1234-000000000003",  // Student-03
};
const char* studentNames[] = {"Student-01", "Student-02", "Student-03"};
```

### 2. WiFi Credentials | WiFi 설정

```cpp
char ssid[] = "YOUR_SSID";
char pass[] = "YOUR_PASSWORD";
```

### 3. nRF Connect Advertiser Setup | nRF Connect 설정

| Step | EN | KR |
|---|---|---|
| 1 | Open nRF Connect → Peripheral tab | nRF Connect → Peripheral 탭 |
| 2 | Tap "Add Advertiser" | "Add Advertiser" 탭 |
| 3 | Set Name (e.g. Student-01) | 이름 설정 (예: Student-01) |
| 4 | Add Service → enter UUID | 서비스 추가 → UUID 입력 |
| 5 | Start advertising | 광고 시작 |
