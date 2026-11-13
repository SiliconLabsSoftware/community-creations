# Centre of Innovation in IoT PROJECT DESCRIPTION TEMPLATE

## 1. Project Overview
BLU-PATH is an embedded web-based indoor navigation system built on the Silicon Labs BRD2605A (SiWx917) development board. It hosts a local Wi-Fi access point and an embedded web server that helps visitors locate campus facilities such as libraries, departments, and offices. Users connect via a QR code and receive navigation guidance without needing internet access or a mobile application.

## 2. Technical Architecture
The SiWx917 SoC runs in Wi-Fi Access Point mode and hosts a lightweight embedded HTTP web server. Visitors scan a QR code, connect to the local access point, and their browser loads the navigation webpage served directly from the device — no internet or backend server involved.

```mermaid
flowchart LR
    A[Visitor scans QR code] --> B[Connects to SiWx917 Wi-Fi AP]
    B --> C[Browser requests page]
    C --> D[Embedded HTTP server on BRD2605A]
    D --> E[Navigation webpage served: HTML/CSS/JS]
```

## 3. Technologies Used
- Wi-Fi networking (SoC Access Point mode)
- Embedded HTTP web server
- Embedded C
- HTML, CSS, JavaScript
- Simplicity Studio
- WiseConnect SDK

## 4. Hardware components
**Silicon labs hardware**
- BRD2605A development board (SiWx917 Wi-Fi SoC)

**External hardware**
- None

## 7. Software components / Dependencies
**Silicon Labs Dependencies**
- WiseConnect SDK
- Simplicity Studio (version as used in project)

**External software Dependencies**
- None

## 8. Licensing
Apache License 2.0 — see LICENSE file in the project repository. No third-party licenses apply.

## 9. Maintainers / Contacts
| Name           | Role      | Contact Information             |
| -------------- | --------- | ------------------------------- | 
| Jerlin Jeba MJ | Developer | mjjerlin@gmail.com              |
| Ilakkiya V     | Developer | ilakkiya.v.2006@gmail.com       | 
| Kanishka M     | Developer | kanishkamuthukumar16@gmail.com  | 
| Riyash M       | Developer | riyashmoorthy@gmail.com         |          
| Ramprasath V   | Developer | veluramprasath777@gmail.com     | 

**Full project repository:**
https://github.com/mjjerlin-gif/BLU-PATH-Embedded-Web-Based-Indoor-Navigation-System_ECE_KPRIET
