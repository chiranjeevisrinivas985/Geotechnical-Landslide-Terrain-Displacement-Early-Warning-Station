# 🏔️ Early Landslide & Geological Shift Warning System

An Arduino-based early warning prototype designed to monitor **soil moisture and water-level conditions** and provide immediate local and remote alerts when environmental conditions reach predefined risk thresholds.

The system combines sensor monitoring, LED status indication, an audible buzzer, and **GSM-based SMS notifications** to provide an early warning mechanism for potentially hazardous ground conditions.

---

## 🚀 Features

* 🌱 **Soil Moisture Monitoring**

  * Continuously measures soil moisture using an analog soil moisture sensor.
  * Converts the sensor reading into a percentage.
  * Generates an alert when soil moisture reaches the configured threshold.

* 💧 **Water Level Monitoring**

  * Monitors water-level conditions using an analog sensor.
  * Converts the analog reading into a percentage.
  * Generates an alert when the water level becomes high.

* ⚠️ **Combined Risk Detection**

  * Calculates a combined percentage from soil moisture and water level.
  * Generates a high-priority landslide alert when the combined value reaches the configured threshold.

* 🔴 **Visual Warning**

  * Red LED indicates an alert condition.
  * Green LED indicates normal operation.

* 🔊 **Audible Warning**

  * Buzzer activates during high-risk water-level and combined-risk conditions.
  * Soil moisture alerts use the red LED without activating the buzzer.

* 📱 **GSM SMS Notification**

  * Sends SMS alerts when predefined thresholds are reached.
  * Uses flags to prevent repeatedly sending the same SMS every second.

* 📊 **Serial Monitoring**

  * Displays soil moisture, water level, combined percentage, and system status through the Serial Monitor.

---

## 🧰 Hardware Requirements

| Component             |    Quantity |
| --------------------- | ----------: |
| Arduino Uno           |           1 |
| GSM Module            |           1 |
| Soil Moisture Sensor  |           1 |
| Water Level Sensor    |           1 |
| Buzzer                |           1 |
| Red LED               |           1 |
| Green LED             |           1 |
| Resistor for LEDs     |           2 |
| Jumper Wires          | As required |
| Suitable Power Supply |           1 |

---

## 🔌 Pin Connections

The following connections correspond directly to the uploaded Arduino code.

| Component            | Pin / Terminal | Arduino Uno                       |
| -------------------- | -------------- | --------------------------------- |
| Soil Moisture Sensor | Analog Output  | **A0**                            |
| Water Level Sensor   | Analog Output  | **A1**                            |
| Buzzer               | + / Signal     | **D10**                           |
| Red LED              | Anode (+)      | **D6**                            |
| Green LED            | Anode (+)      | **D5**                            |
| GSM Module           | TX             | **D9**                            |
| GSM Module           | RX             | **D8**                            |
| LEDs                 | Cathode (−)    | GND                               |
| Buzzer               | −              | GND                               |
| Sensors              | VCC            | According to sensor specification |
| Sensors              | GND            | GND                               |

---

## 📡 GSM Connection

The program uses:

```cpp
SoftwareSerial mySerial(9, 8);
```

Therefore:

```text
Arduino D9  ← GSM TX
Arduino D8  → GSM RX
Arduino GND → GSM GND
```

The GSM module is configured to communicate at:

```cpp
mySerial.begin(38400);
```

The phone number must be configured in:

```cpp
mySerial.println("AT+CMGS=\"+91********\"");
```

Replace the placeholder number with the intended recipient's phone number.

> **Important:** GSM modules such as SIM800L/SIM900 can require a dedicated power supply capable of handling their current peaks. Avoid powering a high-current GSM module directly from the Arduino's 5V pin unless the module specifically supports it and the supply is adequate.

---

## 🌱 Soil Moisture Monitoring

The soil moisture sensor is connected to:

```cpp
const int soilMoisturePin = A0;
```

The analog sensor value is converted into a percentage using:

```cpp
int soilMoisturePercent = map(soilValue, 1023, 0, 0, 100);
```

The configured threshold is:

```cpp
const int soilThreshold = 81;
```

When:

```text
Soil Moisture ≥ 81%
```

the system generates a soil moisture warning.

The system:

```text
Soil Moisture ≥ 81%
        │
        ▼
     Red LED ON
     Green LED OFF
     Buzzer OFF
        │
        ▼
    GSM SMS Alert
```

SMS message:

```text
ALERT! Soil moisture is high
```

---

## 💧 Water Level Monitoring

The water-level sensor is connected to:

```cpp
const int waterLevelPin = A1;
```

The analog value is converted into a percentage:

```cpp
int waterLevelPercent = map(waterValue, 0, 1023, 0, 100);
```

The configured threshold is:

```cpp
const int waterThreshold = 40;
```

When:

```text
Water Level ≥ 40%
```

the system activates the warning condition.

```text
Water Level ≥ 40%
        │
        ▼
     Red LED ON
     Green LED OFF
     Buzzer ON
        │
        ▼
    GSM SMS Alert
```

SMS message:

```text
ALERT! Water level is high
```

---

## 🏔️ Combined Landslide Detection

The system combines soil moisture and water-level measurements.

The combined value is calculated as:

```cpp
int combinedPercent =
    (soilMoisturePercent + waterLevelPercent) / 2;
```

The combined threshold is:

```cpp
const int combinedThreshold = 50;
```

When:

```text
Combined Percentage ≥ 50%
```

the system enters the combined alert condition.

```text
        Soil Moisture
              │
              ▼
             50%
              │
              ├────────┐
              │        │
              ▼        ▼
        Water Level   Average
              │        │
              └────┬───┘
                   ▼
          Combined Percentage
                   │
                   ▼
              ≥ 50% ?
                   │
                   ▼
          LANDSLIDE ALERT
```

During this condition:

* 🔴 Red LED → ON
* 🟢 Green LED → OFF
* 🔊 Buzzer → ON
* 📱 GSM → SMS Alert

SMS message:

```text
ALERT! Landslide Detected
```

---

## 📊 System Thresholds

| Parameter                | Threshold |
| ------------------------ | --------: |
| Soil Moisture            |   **81%** |
| Water Level              |   **40%** |
| Combined Risk            |   **50%** |
| Sensor Reading           |    0–1023 |
| Percentage Range         |    0–100% |
| Main Loop Delay          |  1 second |
| GSM Baud Rate            |     38400 |
| Serial Monitor Baud Rate |      9600 |

The thresholds can be adjusted depending on sensor calibration and the conditions of the deployment location.

---

## 🚦 Alert Priority

The program checks the conditions in the following order:

```text
             START
               │
               ▼
       Read Soil Sensor
       Read Water Sensor
               │
               ▼
      Calculate Percentages
               │
               ▼
     Calculate Combined %
               │
               ▼
    ┌─────────────────────┐
    │ Combined ≥ 50% ?    │
    └──────────┬──────────┘
               │ YES
               ▼
        LANDSLIDE ALERT
               │
               │ NO
               ▼
    ┌─────────────────────┐
    │ Water ≥ 40% ?       │
    └──────────┬──────────┘
               │ YES
               ▼
       WATER LEVEL ALERT
               │
               │ NO
               ▼
    ┌─────────────────────┐
    │ Soil ≥ 81% ?        │
    └──────────┬──────────┘
               │ YES
               ▼
       SOIL MOISTURE ALERT
               │
               │ NO
               ▼
          NORMAL STATUS
```

Because the conditions are checked using `if`, `else if`, and `else`, the **combined condition has priority over the individual water and soil conditions**.

---

## 📱 SMS Alert System

The GSM module sends three different types of alerts.

| Condition                  | SMS                            |
| -------------------------- | ------------------------------ |
| Combined threshold reached | `ALERT! Landslide Detected`    |
| Water level high           | `ALERT! Water level is high`   |
| Soil moisture high         | `ALERT! Soil moisture is high` |

The program uses Boolean flags:

```cpp
bool combinedSent = false;
bool waterSent = false;
bool soilSent = false;
```

These flags prevent the same alert from being sent repeatedly during every loop iteration.

Once the system returns to normal conditions, the flags are reset and a new alert can be generated if another event occurs.

---

## 💡 LED Status Indication

### 🟢 Normal Condition

```text
Green LED → ON
Red LED   → OFF
Buzzer    → OFF
```

### 🔴 Alert Condition

```text
Green LED → OFF
Red LED   → ON
```

For water-level and combined-risk alerts:

```text
Buzzer → ON
```

For the soil-moisture alert:

```text
Buzzer → OFF
```

---

## 🔊 Buzzer Logic

The buzzer is connected to:

```cpp
const int buzzerPin = 10;
```

It activates when:

```text
Water Level ≥ 40%
```

or:

```text
Combined Percentage ≥ 50%
```

The buzzer remains active while the corresponding alert condition remains active.

---

## 🖥️ Serial Monitor

The Arduino continuously displays sensor readings.

Example:

```text
Soil: 72% | Water: 35% | Combined: 53%
ALERT: Combined threshold reached!
```

Normal operation:

```text
Soil: 30% | Water: 20% | Combined: 25%
Status: Normal
```

This makes it easier to calibrate the sensors and observe system behavior during testing.

---

## 🛠️ Software Requirements

* Arduino IDE
* Arduino Uno board package
* `SoftwareSerial` library

`SoftwareSerial` is used to communicate with the GSM module through digital pins 8 and 9.

---

## 📥 Installation

### 1. Clone the Repository

```bash
git clone https://github.com/your-username/your-repository.git
```

### 2. Open the Arduino Project

Open:

```text
Land_slide.ino
```

using Arduino IDE.

### 3. Connect the Hardware

Follow the pin connection table provided above.

### 4. Configure the GSM Number

Change:

```cpp
mySerial.println("AT+CMGS=\"+91********\"");
```

to the required destination number.

### 5. Select Arduino Uno

In Arduino IDE:

```text
Board → Arduino Uno
Port  → Your Arduino COM Port
```

### 6. Upload the Program

Upload the program to the Arduino Uno.

### 7. Open Serial Monitor

Set the Serial Monitor to:

```text
9600 baud
```

You can then observe:

* Soil moisture percentage
* Water level percentage
* Combined percentage
* Alert status
* SMS status

---

## ⚠️ Safety and Reliability Considerations

This project is an **educational/prototype early-warning system** and should not be treated as a certified landslide detection or public-safety system.

For real-world deployment:

* Calibrate sensors for the actual soil and environmental conditions.
* Use multiple sensors at different locations and depths.
* Provide a stable and weather-protected power supply.
* Protect electronics from rain, moisture, dust, and corrosion.
* Add sensor-failure detection.
* Consider redundant communication methods.
* Add long-term data logging.
* Validate threshold values using field measurements.
* Use appropriate professional geological and civil-engineering assessment before relying on the system for safety decisions.

---

## 🔮 Future Improvements

Possible upgrades include:

* 📡 IoT/cloud-based monitoring
* 📱 Mobile application
* 📊 Real-time monitoring dashboard
* 🌧️ Rainfall sensor
* 🌡️ Temperature and humidity monitoring
* 📐 Tilt/inclinometer sensor
* 🌍 Soil displacement monitoring
* 📍 GPS location tracking
* 💾 SD-card data logging
* 🔋 Solar-powered operation
* 📶 LoRa-based long-range communication
* 🤖 Machine-learning-based risk classification
* 🔔 Multi-level warning system
* 🛰️ Remote monitoring station

---

## 🧠 System Architecture

```text
       ┌──────────────────────┐
       │     Soil Sensor      │
       └──────────┬───────────┘
                  │
                  ▼
       ┌──────────────────────┐
       │     Arduino UNO      │
       │                      │
       │ Sensor Processing    │
       │ Threshold Detection  │
       │ Alert Management     │
       └───────┬───────┬──────┘
               │       │
       ┌───────┘       └────────┐
       ▼                        ▼
 Water Level Sensor          GSM Module
       │                        │
       │                        ▼
       │                   SMS Alert
       │
       ▼
 ┌─────────────────────────┐
 │      Alert System       │
 │                         │
 │  🔴 Red LED             │
 │  🟢 Green LED           │
 │  🔊 Buzzer              │
 └─────────────────────────┘
```

---

## 📄 Project Purpose

The purpose of this prototype is to demonstrate how **embedded sensing, threshold-based decision making, local alarms, and GSM communication** can be integrated to create an early-warning monitoring system.

The system continuously evaluates soil moisture and water-level conditions and provides different levels of notification depending on the measured environmental conditions.

---

## 👨‍💻 Project Overview

**Early Landslide & Geological Shift Warning System** is an Arduino-based environmental monitoring prototype developed to demonstrate real-time sensing and remote alert capabilities.

By combining **soil moisture monitoring, water-level measurement, GSM communication, LED indicators, and an audible alarm**, the prototype provides a foundation for developing more advanced geological monitoring systems with additional sensors, IoT connectivity, data analytics, and field-based calibration.
