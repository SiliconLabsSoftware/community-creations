# IoT Enabled Greenhouse Environmental Monitoring System with Integrated Wireless Gateway using Silicon Labs SiWG917_ECE_KPRIET

## Silicon Labs Centre of Innovation in IoT Project

### Team Information

**Institution:** KPR Institute of Engineering and Technology, Coimbatore, Tamil Nadu, India

**Department:** Electronics and Communication Engineering

**Program:** Silicon Labs Centre of Innovation (COI) in IoT

---

# Abstract

Greenhouse farming requires continuous monitoring of environmental conditions to ensure healthy plant growth and maximize crop yield. Traditional monitoring systems often depend on cloud platforms, internet connectivity, and expensive infrastructure.

This project presents an IoT Enabled Greenhouse Environmental Monitoring System using the Silicon Labs SiWG917 Wireless MCU. The system continuously monitors critical environmental parameters including temperature, humidity, and ambient light intensity using the onboard sensors available in the SiWG917 development kit.

The SiWG917 processes the collected sensor data locally and operates as a standalone Wi-Fi Access Point while hosting an embedded HTTP web server. Users can connect directly to the device through Wi-Fi and access a real-time environmental monitoring dashboard using any standard web browser without requiring cloud services, mobile applications, databases, or external gateways.

The system further analyzes environmental conditions and provides intelligent recommendations to improve greenhouse quality. Historical sensor readings can also be downloaded in CSV format for analysis and record keeping.

The proposed solution offers a low-cost, lightweight, portable, and easy-to-deploy smart greenhouse monitoring platform suitable for agriculture, research, educational, and IoT applications.

---

# Problem Statement

Greenhouse environments require continuous monitoring to maintain optimal growing conditions. Existing solutions often face challenges such as:

* Dependence on internet connectivity
* Cloud subscription costs
* Complex deployment and maintenance
* Limited portability
* High implementation costs

There is a need for a lightweight standalone monitoring solution capable of providing real-time environmental insights and recommendations without relying on cloud infrastructure.

---

# Project Objectives

The objectives of this project are:

* Monitor greenhouse temperature
* Monitor relative humidity
* Monitor ambient light intensity
* Analyze environmental conditions locally
* Generate environment status reports
* Provide recommendations for environmental improvement
* Configure Silicon Labs SiWG917 as a Wi-Fi Access Point
* Host a lightweight embedded web server
* Display environmental information through a browser dashboard
* Enable CSV data download for analysis
* Eliminate dependency on cloud services
* Minimize power consumption and memory usage

---

# System Overview

The Silicon Labs SiWG917 serves as the central controller of the system.

The device performs four major functions:

1. Environmental Data Acquisition
2. Data Processing and Analysis
3. Recommendation Generation
4. Wireless Gateway and Web Server Operation

The collected environmental data is displayed on a local web dashboard accessible through Wi-Fi.

---

# Key Features

* Real-time environmental monitoring
* Temperature sensing
* Humidity sensing
* Ambient light sensing
* Environmental quality analysis
* Automated recommendations
* Local Wi-Fi Access Point mode
* Embedded HTTP web server
* Browser-based dashboard
* CSV report download
* Lightweight firmware architecture
* No cloud dependency
* No MQTT dependency
* No database requirement
* Low-cost implementation
* Easy deployment

---

# Hardware Components

| Component                    | Purpose                              |
| ---------------------------- | ------------------------------------ |
| Silicon Labs SiWG917 Dev Kit | Main Controller and Wireless Gateway |
| Onboard Temperature Sensor   | Temperature Monitoring               |
| Onboard Humidity Sensor      | Humidity Monitoring                  |
| Onboard Light Sensor         | Ambient Light Monitoring             |
| USB Power Supply             | System Power                         |

---


# System Architecture

```text
+---------------------------+
| Temperature Sensor        |
+-------------+-------------+
              |
+-------------v-------------+
| Humidity Sensor           |
+-------------+-------------+
              |
+-------------v-------------+
| Light Sensor              |
+-------------+-------------+
              |
              v

+---------------------------+
|   Silicon Labs SiWG917    |
|                           |
| - Data Acquisition        |
| - Data Analysis           |
| - Recommendation Engine   |
| - Wi-Fi Access Point      |
| - HTTP Web Server         |
| - CSV Data Logger         |
+-------------+-------------+
              |
              | Wi-Fi AP
              |
      +-------+-------+
      |               |
+-----v-----+ +-------v----+
| Smartphone| |   Laptop   |
+-----------+ +------------+
              |
              v
      Web Dashboard
```

---

# Hardware Block Diagram

```text
Temperature Sensor
        |
Humidity Sensor
        |
Light Sensor
        |
        v

+-------------------+
|   SiWG917 MCU     |
+-------------------+
        |
        |
        +----------------------+
                               |
                         Wi-Fi AP Mode
                               |
            +------------------+------------------+
            |                                     |
         Mobile                               Laptop
            |                                     |
            +----------- Dashboard ---------------+
```

---

# Data Flow Diagram

```text
Temperature
      +
Humidity
      +
Light
      |
      v

Data Acquisition
      |
      v

Data Processing
      |
      v

Environment Analysis
      |
      v

Recommendation Engine
      |
      v

JSON Generation
      |
      v

HTTP Server
      |
      v

Web Dashboard
```

---

# Working Principle

1. The onboard sensors continuously collect environmental data.
2. SiWG917 reads temperature, humidity, and light intensity values.
3. Sensor readings are processed locally.
4. Environmental conditions are analyzed against predefined thresholds.
5. The system determines the current greenhouse status.
6. Recommendations are generated to improve environmental quality.
7. Sensor data is logged periodically.
8. CSV files are generated for historical data storage.
9. SiWG917 operates as a Wi-Fi Access Point.
10. Users connect directly to the device through Wi-Fi.
11. Embedded HTTP server provides dashboard pages.
12. Browser periodically requests updated sensor information.

---

# Wi-Fi Configuration

SSID:
SiWG917

Password:
12345678

Default IP:
192.168.10.11

---

# Dashboard Interface

```text
-------------------------------------------
Greenhouse Monitoring Dashboard
-------------------------------------------

Temperature : 29.5 °C

Humidity    : 62 %

Light Level : 740 Lux

Status      : Optimal

Recommendation:
Maintain current environmental conditions.

Last Update : 10:15:22

[ Download CSV ]

-------------------------------------------
```

---

# Example JSON Response

```json
{
  "temperature": 29.5,
  "humidity": 62,
  "light": 740,
  "status": "Optimal",
  "recommendation": "Maintain current environmental conditions."
}
```

---

# Environmental Analysis Logic

| Parameter   | Condition | Status                          |
| ----------- | --------- | ------------------------------- |
| Temperature | Too High  | Ventilation Required            |
| Temperature | Too Low   | Increase Heating                |
| Humidity    | Too High  | Reduce Moisture                 |
| Humidity    | Too Low   | Increase Irrigation             |
| Light       | Too Low   | Additional Lighting Recommended |
| Light       | Too High  | Provide Shade                   |

---

# Example Recommendations

* Increase greenhouse ventilation.
* Reduce humidity levels.
* Increase irrigation frequency.
* Activate supplemental lighting.
* Deploy shading mechanisms.
* Maintain current environmental conditions.

---

# CSV Data Logging

The system periodically stores environmental data and allows users to download logs in CSV format.

Example:

```csv
Timestamp,Temperature,Humidity,Light,Status
10:00:00,29.1,61,735,Optimal
10:05:00,29.4,62,742,Optimal
10:10:00,29.5,62,740,Optimal
```

---

---

# Results

The system successfully:

* Acquired environmental parameters
* Processed sensor data locally
* Analyzed greenhouse conditions
* Generated improvement recommendations
* Hosted an embedded web server
* Operated as a standalone Wi-Fi Access Point
* Displayed real-time environmental information
* Generated downloadable CSV reports
* Eliminated cloud dependency

---

# Applications

* Smart Greenhouses
* Precision Agriculture
* Indoor Farming
* Hydroponic Systems
* Agricultural Research Centers
* Educational IoT Laboratories
* Environmental Monitoring Systems

---

# Advantages

* Low Cost
* Portable
* Easy Deployment
* Cloud Independent
* Low Power Consumption
* Real-Time Monitoring
* CSV Data Export
* Intelligent Recommendations
* Scalable Architecture
* Beginner Friendly

---

tunities that enabled the successful development of this Greenhouse Environmental Monitoring System.
