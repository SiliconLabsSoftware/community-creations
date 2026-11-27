# Silicon Labs Community Creations

Welcome to the **Silicon Labs Community Creations** repository!
This space is dedicated to showcasing, celebrating, and sharing amazing projects built using **Silicon Labs (SiLabs)** technologies.
Whether you're tinkering with wireless protocols, building embedded applications, or experimenting with IoT, we want to see what you've made!

Add the `silicon-labs` topic to your project’s GitHub repository to help more people discover what you’ve created!

## Table of Content

- [What's This About?](#whats-this-about)
- [Who Can Contribute?](#who-can-contribute)
- [What Kind of Projects Can I Submit?](#what-kind-of-projects-can-i-submit)
- [How to Contribute](#how-to-contribute)
- [Project List](#project-list)
  - [Bluetooth / Bluetooth Mesh](#bluetooth--bluetooth-mesh)
  - [CircuitPython](#circuitpython)
  - [Energy Harvesting](#energy-harvesting)
  - [Hardware Prototypes](#hardware-prototypes)
  - [IoT Utilities (Tools)](#iot-utilities-tools)
  - [Matter](#matter)
  - [Middleware](#middleware)
  - [Platform](#platform)
  - [Proprietary](#proprietary)
  - [Training](#training)
  - [Wi-Fi](#wi-fi)
  - [Wi-Sun](#wi-sun)
  - [Z-Wave](#z-wave)
  - [Zepyhr](#zepyhr)
  - [Zigbee](#zigbee)
- [Highlighted Projects](#highlighted-projects)
- [Join the Community](#join-the-community)
- [Reporting Bugs/Issues and Posting Questions and Comments](#reporting-bugsissues-and-posting-questions-and-comments)
- [Disclaimer](#disclaimer)
- [License](#license)





## What's This About?

At Silicon Labs, we believe in the power of community-driven innovation. This repository is a hub for developers, makers, and hobbyists to:

- Share their projects built with SiLabs hardware or software
- Explore what others are building in the community
- Inspire and get inspired
- Learn and collaborate

## Who Can Contribute?

Anyone! Whether you're a seasoned engineer, student, or hobbyist, if you've created something cool using Silicon Labs products, we'd love to feature it here.

## What Kind of Projects Can I Submit?

You can share any completed, working project that involves Silicon Labs products or tools, including (but not limited to):

- Wireless applications (Zigbee, Bluetooth, Wi-Fi, Thread, Matter etc.)
- Embedded software for SiLabs MCUs
- IoT sensors and smart devices
- Demos, prototypes
- Open source libraries or tools that extend SiLabs platforms

## How to Contribute

This repository accepts **link submissions** (not project source code). To share your project:

1. **Choose a category** that best fits your project (Bluetooth, Zigbee, Wi-Fi, Matter, etc.).
2. **Edit the category listing table** and append a new row at the bottom with:
   - Your project GitHub repository link
   - A 1-2 sentence description
   - Your GitHub profile link
   - (please do not reorder or edit existing rows; it helps avoid merge conflicts)
3. **Submit a pull request** with a brief explanation of your project and the GitHub URL.

For detailed contribution guidelines, see [CONTRIBUTING.md](.github/CONTRIBUTING.md).


## Project List

Below is a list of application projects that have been contributed by the community.

To avoid merge conflicts when multiple PRs are created around the same time, please append your new row at the bottom of the table.

Example row (copy/paste and edit):

```markdown
| [my-project](https://github.com/<org-or-user>/<repo>) | One or two sentences describing what it does and which Silicon Labs technology it uses. | [my-handle](https://github.com/<your-handle>) |
```
### Bluetooth / Bluetooth Mesh

| Project (GitHub) | Description | Author/Contributor |
|:--|:--|:--|
| [Transformer Fault Detector](https://github.com/User8085/Transformer-Fault-Detector) | TinyML-based acoustic anomaly detection for power transformer fault prediction using Silicon Labs EFR32xG24 and Bluetooth LE. The system performs on-device inference and transmits BLE alerts when transformer faults are detected. | [User8085](https://github.com/User8085) |
| [Presentation & Media Controller](https://github.com/23ECE046/Silicon-Labs-Project.git) | Presentation & Media Controller Gesture Based Smart Controller for Presentations and Media "Speak the word, wave the wrist — the slide steps forward; rotate your arm and the room obeys."|[indhum](https://github.com/23ECE046) |
| [BLE-Based Edge Predictive Maintenance & Auto Fault Ticket Generator](https://github.com/GokulSiddarth3108/DigitalTwinMonitor) | Edge predictive-maintenance system for the Silicon Labs SiWG917 that processes vibration and motor-current telemetry locally using edge inference and CMSIS-NN to detect equipment anomalies. | [GokulSiddarth3108](https://github.com/GokulSiddarth3108) |

### CircuitPython
| Project (GitHub) | Description | Author/Contributor |
|:--|:--|:--|
|   |   |   |

### Energy Harvesting
| Project (GitHub) | Description | Author/Contributor |
|:--|:--|:--|
|   |   |   |


### Hardware Prototypes
| Project (GitHub) | Description | Author/Contributor |
|:--|:--|:--|
| [FPGA-Based CNN Hardware Accelerator for Heart Murmur Detection](https://github.com/ronakmahlawat/https://github.com/ronakmahlawat/hardware_accelerator_for_murmur_detection_in_pcg_signals) | Hardware prototype implementing a trained CNN model on an FPGA for fast, portable, low-power heart murmur detection from phonocardiogram (PCG) signals to enable scalable cardiac screening. | [ronakmahlawat](https://github.com/ronakmahlawat) |

### IoT Utilities (Tools)

| Project (GitHub) | Description | Author/Contributor |
|:--|:--|:--|
|[AI-Based Bird Behavior Monitoring using SiWx917](https://github.com/SahilBhusan/Silicon_labs)|AI-powered bird classification and behavior monitoring system built using the Silicon Labs SiWx917 Wi-Fi, combining edge intelligence, wireless sensor networking, and cloud analytics for real-time wildlife monitoring.|[SahilBhusan](https://github.com/SahilBhusan)|
|[siwx917-dv-harness](https://github.com/m0hiith/siwx917-dv-harness) | Unattended design-validation harness for SiWx917 kits: flashes RPS firmware via Simplicity Commander, resets, and repeats the cycle N times, reporting every result as mean ± standard deviation against configurable pass/fail limits with timestamped raw data and a run manifest. Power measurement requires an AEM-equipped setup (BRD4338A on BRD4002A) or an external meter — the DK2605A reports `AEM Supported: False`. | [m0hiith](https://github.com/m0hiith)|

### Matter

| Project (GitHub) | Description | Author/Contributor |
|:--|:--|:--|
|[ClimateSphere](https://github.com/shubhi-garg27/ClimateSphere)|ClimateSphere creates personalized comfort zones by adapting lighting, airflow, and environmental conditions to the people who actually occupy a space, improving comfort while reducing energy waste.|[Shubhi Garg](https://github.com/shubhi-garg27)|

### Middleware
| Project (GitHub) | Description | Author/Contributor |
|:--|:--|:--|
|   |   |   |

### Platform
| Project (GitHub) | Description | Author/Contributor |
|:--|:--|:--|
| [High-Pressure Pipe Acoustic Leak Detection System](https://github.com/GokulSiddarth3108/leak-detector) | An edge AI acoustic leak detection system for continuous high-pressure pipe monitoring using Silicon Labs SiWx917 hardware, MEMS microphone input, DSP, and TinyML inference. | [GokulSiddarth3108](https://github.com/GokulSiddarth3108) |
| [SiWx917 External LED Blink](https://github.com/ab8642/siwx917-led-blink) | Blinks an external LED on SiWx917 (BRD2605A) using the Si91x GPIO driver and sleep timer via Simplicity Studio 5. | [ab8642](https://github.com/ab8642) |
| [SiWx917 IR Sensor](https://github.com/ab8642/siwx917_IRsensor) | IR sensor interfacing on SiWx917 (BRD2605A) using the Si91x GPIO driver for object detection with serial output via Simplicity Studio 5. | [ab8642](https://github.com/ab8642) |

### Proprietary
| Project (GitHub) | Description | Author/Contributor |
|:--|:--|:--|
|   |   |   |

### Training
| Project (GitHub) | Description | Author/Contributor |
|:--|:--|:--|
|   |   |   |

### Wi-Fi / Wi-Fi Coex (Wi-FI + BLE)
| Project (GitHub) | Description | Author/Contributor |
|:--|:--|:--|
| [real-time-gesture-recognition-siwx917](https://github.com/Sree-Charan05/real-time-gesture-recognition-siwx917) | Real-time gesture recognition using the onboard IMU of the Silicon Labs SiWx917 SoC with RTOS-based sensor processing. | [Sree-Charan05](https://github.com/Sree-Charan05) |
| [SiWx917 HC-SR04 Ultrasonic Distance](https://github.com/sp678-crypto/siwx917-hcsr04-ultrasonic) | HC-SR04 ultrasonic distance measurement on SiWx917 (BRD2605A) using a custom GPIO echo-timing driver and sleep timer. Arduino generates the trigger pulse. | [sp678-crypto](https://github.com/sp678-crypto) |
| [Smart City IoT Platform](https://github.com/Somnathsuraj007/Smart_City) | Smart City IoT platform built with Silicon Labs SiWx917, featuring environmental monitoring, smart parking, and noise pollution monitoring with MQTT-based telemetry and a unified dashboard. | [Somnathsuraj007](https://github.com/Somnathsuraj007) |
| [Engine Monitoring System](https://github.com/Karthikeyang61/Engine-Monitoring-System) | An IoT-based smart engine health monitoring system using the SiWx917 board to monitor temperature, vibration, noise, and exhaust gas levels in real time. The system detects abnormal engine conditions, provides alerts, and displays engine health data on a web dashboard and onboard display for preventive maintenance. | [Karthikeyang61](https://github.com/Karthikeyang61) |
| [SolarOps](https://github.com/Pankaj-Singh-Rawat/SolarOps) | An enterprise-grade smart grid optimization system designed for Silicon Labs wireless hardware targets. It integrates a local-autonomy edge state machine, a containerized multi-service backend with automated Docker health checks, and a predictive AI engine forecasting day-ahead solar yield. | [@Pankaj-Singh-Rawat](https://github.com/Pankaj-Singh-Rawat) |
| [IoT Smart Helmet](https://github.com/Arjxn404/IoT-Smart-Helmet_ECE_KPRIET) | An IoT-enabled smart helmet built with the Silicon Labs SiWx917 platform for industrial worker safety. It provides Wi-Fi connectivity and enables hazard monitoring, emergency alerts, and real-time safety status reporting. | [Arjxn404](https://github.com/Arjxn404) |
| [siliconflow-ci](https://github.com/m0hiith/siliconflow-ci) | Build, test and release-gating service for embedded firmware: GitHub Actions builds C11 firmware for Cortex-M4 and SiWx917 (BRD4338A) targets, and a FastAPI backend verifies signed run reports and artifact hashes and checks coverage, cppcheck and flash-size gates before a binary is release-eligible. Includes optional hardware-in-the-loop flashing of a SiWx917 board. | [m0hiith](https://github.com/m0hiith) |
| [MiniFace for SiWG917 BRD2605A](https://github.com/TharunBabu-05/Silicon_labs_SiWG917_BRD2605A) | An interactive VCOM-native desktop interface for the Silicon Labs SiWG917 BRD2605A. It provides on-demand Wi-Fi configuration, live board telemetry, and audio diagnostics while conserving RAM when tools are closed. | [TharunBabu-05](https://github.com/TharunBabu-05) |
| [Voice Controlled Relay Light](https://github.com/vichukuttan4000/WiVox_ECE_KPRIET/tree/main/aiml_soc_voice_control_light_siwg917_baremetal1) | A voice-controlled home automation application built using the Silicon Labs SiWG917 platform, featuring offline voice recognition with TensorFlow Lite Micro and GPIO-based relay control for appliance automation. | [vichukuttan4000](https://github.com/vichukuttan4000) |
| [Voice Controlled RGB Lighting System](https://github.com/vichukuttan4000/WiVox_ECE_KPRIET/tree/main/aiml_soc_voice_control_light_siwg917_baremetal2) | A voice-controlled RGB lighting application using the Silicon Labs SiWG917 platform, onboard microphone, TensorFlow Lite Micro, and RGB LED control for embedded Edge AI demonstration. | [vichukuttan4000](https://github.com/vichukuttan4000) |
| [Ambient Light Monitoring System](https://github.com/vichukuttan4000/WiVox_ECE_KPRIET/tree/main/sl_si91x_veml6035) | An ambient light monitoring application built on the Silicon Labs SiWG917 platform using the onboard VEML6035 sensor and I2C interface for real-time lux measurement and brightness classification. | [vichukuttan4000](https://github.com/vichukuttan4000) |
| [Si917 Edge-AI Predictive Maintenance](https://github.com/Saisriram53/si917-predictive-maintenance) | Real-time Edge-AI vibration and environmental anomaly detection with predictive maintenance dashboard using Silicon Labs Si917 SoC and WiSeConnect SDK. | [Saisriram53](https://github.com/Saisriram53) |
| [Classroom Environmental Monitoring using SiWG917](https://github.com/AdityaKolluru05/Classroom-monitoring_using_SiWG917-Development-Board) | IoT-based classroom monitoring system using the SiWG917 to track temperature, humidity, and noise, publishing data to ThingSpeak via MQTT with automated email alerts. | [Aditya Kolluru](https://github.com/AdityaKolluru05) |
| [SiWG917 Digital Twin](https://github.com/gnaneshchandragupta17-cyber/siwg917-digital-twin) | Industrial machine monitoring and digital twin system using the Silicon Labs SiWG917 Wi-Fi SoC, with sensor acquisition, AI-based health prediction, remaining useful life prediction, backend services, and a web dashboard. | [gnaneshchandragupta17-cyber](https://github.com/gnaneshchandragupta17-cyber) |
| [twindock](https://github.com/m0hiith/twindock) | Browser-based 3D digital twin of a warehouse and factory, driven live over Wi-Fi/MQTT by a SiWx917 (BRD2605A) dev kit running custom WiSeConnect C firmware that reports status and buttons and takes LED commands. Everything except the board is a clearly labelled simulation. | [m0hiith](https://github.com/m0hiith) |
| [SiWG917-SafeHaven](https://github.com/muralikrishnapalisetti/SiWG917-SafeHaven) | A safety-monitoring prototype built with the Silicon Labs SiWG917/SiWx917 that streams microphone audio over Wi-Fi UDP to a host PC for local Vosk speech recognition. A configured emergency phrase can trigger event validation and automatic webcam video evidence capture on the host. | [muralikrishnapalisetti](https://github.com/muralikrishnapalisetti) |
| [BLU-PATH-Embedded-Web-Based-Indoor-Navigation-System_ECE_KPRIET](https://github.com/mjjerlin-gif/BLU-PATH-Embedded-Web-Based-Indoor-Navigation-System_ECE_KPRIET) | Embedded web-based indoor navigation system on the Silicon Labs SiWx917 (BRD2605A) that runs a local Wi-Fi access point and web server, so visitors can find campus facilities via QR code without internet access or a mobile app. | [mjjerlin-gif](https://github.com/mjjerlin-gif) |
|[Bus Stop Alert System for Visually Impaired using SiWG917](https://github.com/sajithathangaraj-lan/VoiceBus_ECE_KPRIET)|VoiceBus is a smart bus information system for visually impaired passengers using the Silicon Labs SiWG917 Wi-Fi 6 MCU. The board broadcasts bus route details via Wi-Fi, while a mobile application retrieves and announces the information using Text-to-Speech.|[sajithathangaraj-lan](https://github.com/sajithathangaraj-lan)|
| [IoT-Enabled-Greenhouse-Environmental-Monitoring-System-SiWG917-ECE-KPRIET](https://github.com/Ragav2107/IoT-Enabled-Greenhouse-Environmental-Monitoring-System-SiWG917-ECE-KPRIET) | IoT-based greenhouse environmental monitoring system that monitors temperature, humidity, and light conditions and provides the sensor data through a web interface using the Silicon Labs SiWG917 Wi-Fi platform. | [Ragavappranesh](https://github.com/Ragav2107) |
| [Smart-IoT-Based-Heatwave-Protection-System](https://github.com/Devilwelcometohell/Smart-IoT-Based-Heatwave-Protection-System) | HeatShield-IoT is a smart wearable system that combines real-time health monitoring with automatic cooling to protect users from heat stress and heatstroke during extreme heatwave conditions. The system uses a Smart Health Monitoring Band and a Smart Neck Cooling Gadget that work together through IoT connectivity to monitor vital signs, detect heat-related risks, and provide proactive cooling and safety alerts.| [Devang Shukla](https://github.com/Devilwelcometohell) |

### Wi-Sun

| Project (GitHub) | Description | Author/Contributor |
|:--|:--|:--|
| [creip-wisun-mesh](https://github.com/shubhi-garg27/creip-wisun-mesh) | CREIP is an AI-powered wireless mesh networking system built using Silicon Labs devices to improve communication reliability in dynamic RF environments. | [shubhi-garg27](https://github.com/shubhi-garg27) |

### Z-Wave
| Project (GitHub) | Description | Author/Contributor |
|:--|:--|:--|
|   |   |   |

### Zepyhr
| Project (GitHub) | Description | Author/Contributor |
|:--|:--|:--|
|   |   |   |

### Zigbee
| Project (GitHub) | Description | Author/Contributor |
|:--|:--|:--|
|   |   |   |

## Highlighted Projects

We regularly highlight standout community creations! If your project gets featured, we'll share it on our website, newsletters, and social channels (with your permission).

## Join the Community

- [Silicon Labs Community Forum](https://www.silabs.com/community)
- [Developer Documentation](https://docs.silabs.com/)
- [Follow us on LinkedIn](https://www.linkedin.com/company/siliconlabs)


## Reporting Bugs/Issues and Posting Questions and Comments

To report bugs, please create a new "Issue" in the "Issues" section of this repo. Please reference the board, project, and source files associated with the bug, and reference line numbers. If you are proposing a fix, also include information on the proposed fix. Since these examples are provided as-is, there is no guarantee that these examples will be updated to fix these issues.

Questions and comments related to these examples should be made by creating a new "Issue" in the "Issues" section of this repo.

## Disclaimer

This repository contains *Community Creations* listings, links, and related documentation contributed by the community. Unless explicitly stated otherwise, all content is considered to be of **EXPERIMENTAL QUALITY** and is provided **as-is**, without any warranty of functionality, completeness, or reliability.

Silicon Labs reviews submitted pull requests solely for **compliance with the contribution guide**, but **does not perform additional testing**. Consequently, Silicon Labs makes no guarantees regarding the correctness, quality, or suitability of any listed project for a specific application.

Use of these projects is entirely at your own risk. They are **not recommended** for deployment in production environments.

While community contributions are welcomed and appreciated, Silicon Labs offers **no commitment** to maintain, support, or update any part of this repository. Silicon Labs may, at its sole discretion, modify or remove community projects without notice.

Silicon Labs lists URLs to third-party Github repos in this Community Creations repo solely “as is” and without any sponsorship or endorsement and without any obligations, support or warranties, express or implied, from Silicon Labs.  

Any access, execution, or other use of code or other information or materials available at these URLs is solely at the risk of the user.


## License

Projects submitted for listing must include a license file in the linked GitHub repository.

This repository is licensed under the terms specified in the [LICENSE.md](./LICENSE.md) file. Please review it for details regarding usage rights and limitations.

Let’s build the future together. One project at a time.
**– The Silicon Labs Team**
