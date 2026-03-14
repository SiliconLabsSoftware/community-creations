# AegisRover
> Bluetooth-controlled microcontroller car + mobile controller app  

[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)]()
[![Issues](https://img.shields.io/github/issues/USERNAME/REPO_NAME)]()
[![Last Commit](https://img.shields.io/github/last-commit/USERNAME/REPO_NAME)]()

## Project summary
AegisRover is a Bluetooth-controlled car built with Silicon Labs microcontroller boards and a mobile app to control speed, direction and some extra features (lights/horn/auto-mode). This repo contains firmware, mobile app source, wiring diagrams, docs and a demo video.

## Features
- Drive control: forward, backward, left, right, stop  
- Speed control (PWM)  
- Mobile app UI with joystick + buttons  

## Hardware
- Silicon Labs board 
- Motor driver
- DC motors ×4 
- Battery pack   
- Chassis + wheels  
- Blutooth

## Software stack
- Firmware: Silicon Labs Simplicity Studio SDK  
- Language: C (firmware)  
- Mobile App: Android (Kotlin)  
- Build tools: Android Studio 

## Repository structure
```
.
├── firmware/ # MCU firmware source (C)
│   ├── README.md
│   ├── board/
│   └── src/
├── mobile-app/
│   └── README.md
├── docs/
│   ├── wiring.md
│   ├── demo_video_link.md
│   └── report.pdf
├── hardware/
│   └── PCB_schematics_or_photos/
├── .gitignore
├── LICENSE
└── README.md
```
