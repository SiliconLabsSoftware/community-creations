# pothole-detection-gps-iot
IoT-based smart pothole detection system using accelerometer &amp; GPS that auto-reports road damage on a live web dashboard. Built for Silicon Labs Centre of Innovation in IoT Challenge.

🛣️ PotholeAI — Real-Time Pothole Detection & Road Intelligence System
> *"Every broken road tells a story of neglect. This project makes sure that story is heard — automatically, precisely, and in real time."*
![Silicon Labs COI](https://img.shields.io/badge/Silicon%20Labs-COI%20IoT%20Challenge-blue?style=for-the-badge)
![Flask](https://img.shields.io/badge/Flask-2.3-black?style=for-the-badge&logo=flask)
![OpenCV](https://img.shields.io/badge/OpenCV-4.8-green?style=for-the-badge&logo=opencv)
![YOLOv8](https://img.shields.io/badge/YOLOv8-INT8%20TFLite-purple?style=for-the-badge)
![ESP32-CAM](https://img.shields.io/badge/ESP32--CAM-Hardware-orange?style=for-the-badge)
---
📌 Project Overview
India's road network — the second largest in the world — loses an estimated ₹8,100 crore annually to pothole-related accidents, vehicle damage, and emergency repairs. Traditional detection methods depend on manual inspection: a process that is slow, inconsistent, expensive, and ultimately reactive. By the time a pothole is reported, filed, surveyed, and repaired, lives and money have already been lost.
PotholeAI is a low-cost, autonomous IoT system that transforms an ordinary ESP32-CAM module into an intelligent road-inspection device. Mounted on a vehicle, it continuously captures road imagery, transmits frames over Wi-Fi to an edge server running a YOLOv8 deep learning model, and — in real time — detects potholes, calculates the exact volume of material needed for repair, tags each detection with a geographic location, and plots it on a live administrative map. No human required.
This is not a proof-of-concept sketch. It is a working prototype with functioning hardware and a fully deployed web platform, built end-to-end by a student team as a submission for the Silicon Labs Centre of Innovation IoT Challenge.
---
❓ Problem Statement
Aspect	Current Reality
Detection method	Manual patrolling and citizen complaints
Response time	Days to weeks after damage appears
Coverage	Incomplete; biased toward high-traffic areas
Material estimation	Done by eye; prone to over/under-ordering
Geographic logging	Rarely done; paper-based or absent
Cost	High — every surveyor-hour is a liability
PotholeAI addresses every row of that table simultaneously.
---
🎯 Objectives
Deploy an ESP32-CAM as a mobile, wireless road-scanning sensor
Run YOLOv8 INT8 inference on an edge server for real-time pothole classification
Dynamically compute repair material quantities (cement, sand, aggregate) scaled to actual pothole dimensions
Automatically resolve geographic coordinates from the device's Wi-Fi/IP signal — no GPS hardware required
Present all detections on a live, filterable map dashboard accessible to road-maintenance administrators
Secure the administrative interface with role-based authentication while keeping the device endpoint completely open (so the ESP32-CAM never needs a login)
---
🏗️ System Architecture
```
┌─────────────────────────────────────────────────────────────────────┐
│                        PotholeAI — System Flow                       │
└─────────────────────────────────────────────────────────────────────┘

  ┌──────────────┐        JPEG over Wi-Fi          ┌──────────────────┐
  │  ESP32-CAM   │  ──── POST /detect ──────────►  │   Flask Server   │
  │  (on vehicle)│  (no auth needed — public)      │   (Python 3.x)   │
  └──────────────┘                                 └────────┬─────────┘
                                                            │
                          ┌─────────────────────────────────┤
                          │                                 │
                   ┌──────▼──────┐                 ┌────────▼────────┐
                   │  YOLOv8     │                 │  IP Geolocation │
                   │  INT8       │                 │  (ip-api.com)   │
                   │  TFLite     │                 │  Wi-Fi → lat/lng│
                   │  Inference  │                 └────────┬────────┘
                   └──────┬──────┘                         │
                          │ Detection Result                │ Coordinates
                          └─────────────┬───────────────────┘
                                        │
                               ┌────────▼────────┐
                               │    SQLite DB     │
                               │  potholes.db     │
                               │  (auto-updated)  │
                               └────────┬────────┘
                                        │
                    ┌───────────────────┼────────────────────┐
                    │                   │                    │
           ┌────────▼──────┐  ┌────────▼───────┐  ┌────────▼──────┐
           │   Dashboard   │  │   Live Map     │  │  REST API     │
           │   (Admin)     │  │  (Leaflet.js)  │  │  JSON output  │
           │   Charts /    │  │  GPS Pins +    │  │  /api/*       │
           │   Stats       │  │  Popup details │  │  endpoints    │
           └───────────────┘  └────────────────┘  └───────────────┘
```
Component Breakdown
Layer	Technology	Role
Edge Sensor	ESP32-CAM (OV2640)	Captures JPEG frames, POSTs over Wi-Fi
Inference Engine	YOLOv8 INT8 TFLite	Single-class pothole detector (160×160)
Fallback Engine	OpenCV (image processing)	Active when TFLite model absent
Backend	Python / Flask 2.3	REST API, session auth, DB writes
Database	SQLite	Lightweight, zero-config persistence
Geolocation	ip-api.com (free tier)	Wi-Fi/IP → city-level lat/lng
Frontend	HTML5 / Tailwind CSS / JS	Admin dashboard, dark-themed UI
Map	Leaflet.js + OpenStreetMap	Interactive pothole map with pins
---
⚙️ Working Methodology
Step 1 — Image Capture
The ESP32-CAM, mounted on a vehicle dashboard or frame, captures a JPEG frame and immediately transmits it via HTTP POST to the server's `/detect` endpoint. This endpoint requires no authentication — by design — so the embedded device never encounters a login redirect.
Step 2 — Deep Learning Inference
The server's detection core attempts to load a YOLOv8 INT8-quantised TFLite model (`best_int8.tflite`, input: `[1, 160, 160, 3]`). If present, bounding boxes are extracted from the model's `[1, 5, N]` output tensor and filtered with Non-Maximum Suppression. If the model file is absent, an OpenCV-based image-processing pipeline activates as a graceful fallback, ensuring the system never crashes during demonstration.
Step 3 — Material Volume Estimation
Rather than assuming a fixed depth for every pothole regardless of size — a naive approach that wildly over-estimates material for micro-cracks — PotholeAI applies a tiered depth model calibrated to observed pothole morphology:
Pothole Area	Assumed Depth	Severity
< 0.05 m²	2 cm	Surface crack
0.05 – 0.20 m²	4 cm	Minor pothole
0.20 – 0.60 m²	6 cm	Moderate pothole
≥ 0.60 m²	9 cm	Severe pothole
Material quantities are then derived from the computed volume:
```
Volume (m³) = Area (m²) × Depth (m)

Cement    = Volume × 350 kg/m³
Sand      = Volume × 700 kg/m³
Aggregate = Volume × 1050 kg/m³
```
Step 4 — Geolocation (Wi-Fi Based)
On receiving each image, the server reads the client's IP address. If it falls within a private subnet (192.168.x.x / 10.x.x.x), the server's own public IP is resolved via `api.ipify.org` and forwarded to `ip-api.com` for city-level geocoding. The resulting latitude and longitude are stored alongside the detection record — with no GPS hardware required on the device.
Step 5 — Database & Dashboard
Every detection is persisted in SQLite with confidence score, pothole count, area, material estimates, coordinates, location source, and timestamp. The admin dashboard surfaces aggregate statistics (total detections, average confidence, cumulative material requirements) and the map view plots each geo-tagged detection as a severity-coloured pin, with a rich popup on click.
---
🔌 Circuit Design / Hardware Implementation
Components Used
Component	Specification	Purpose
ESP32-CAM	AI-Thinker, OV2640 sensor	Image capture + Wi-Fi transmission
FTDI Programmer	CP2102 / CH340	ESP32-CAM flashing (no USB on module)
Server / Laptop	Any x86 or ARM machine	Inference + web dashboard
Power Supply	5V USB or Li-Po with regulator	Powering the ESP32-CAM in field
Wi-Fi Router / Hotspot	2.4 GHz	Network bridge between cam and server
ESP32-CAM Wiring (for flashing via FTDI)
```
ESP32-CAM Pin    →   FTDI Programmer Pin
─────────────────────────────────────────
5V               →   VCC (5V)
GND              →   GND
U0R (RX)         →   TX
U0T (TX)         →   RX
IO0              →   GND  ← Only during flashing; remove after
```
> ⚠️ IO0 must be connected to GND **only** during firmware upload. Remove it before normal operation.
Communication Protocol
```
ESP32-CAM  →  HTTP POST  →  /detect  →  Flask Server
           multipart/form-data
           field: "image" (JPEG bytes)
           field: "lat"   (optional float)
           field: "lng"   (optional float)
```
No library changes are needed for geolocation — it is resolved entirely server-side from the IP of the incoming connection.
---
🖥️ Software Platform
Project Structure
```
PotholeAI/
├── app.py                    ← Entry point; registers all blueprints
├── requirements.txt          ← Python dependencies
├── best_int8.tflite          ← YOLOv8 model (place here to activate)
├── potholes.db               ← SQLite database (auto-created on first run)
├── detected_images/          ← Annotated JPEG archive (auto-created)
│
├── config/
│   └── settings.py           ← All tunable parameters in one place
│
├── core/
│   ├── detector.py           ← YOLOv8 TFLite + OpenCV inference pipeline
│   ├── database.py           ← SQLite helpers (init, save, query)
│   ├── auth.py               ← login_required decorator
│   └── geo.py                ← Wi-Fi / IP geolocation module
│
├── routes/
│   ├── api_routes.py         ← /detect (public) and /api/* (protected)
│   ├── auth_routes.py        ← /login  /logout
│   ├── dashboard_routes.py   ← /  (admin overview)
│   ├── map_routes.py         ← /map
│   └── about_routes.py       ← /about
│
├── templates/                ← Jinja2 HTML (dark-theme, responsive)
└── static/                   ← CSS + JavaScript assets
```
API Reference
Method	Endpoint	Auth	Description
`POST`	`/detect`	❌ Public	ESP-CAM image upload — no login ever needed
`GET`	`/`	✅ Admin	Dashboard with live statistics
`GET`	`/map`	✅ Admin	Interactive Leaflet map
`GET`	`/about`	❌ Public	Project information page
`POST`	`/api/detect`	✅ Admin	Manual test detection from browser
`GET`	`/api/detections`	✅ Admin	All detection records (JSON)
`GET`	`/api/stats`	✅ Admin	Summary statistics (JSON)
`GET`	`/api/map-points`	✅ Admin	GPS-tagged detections (JSON)
`GET`	`/api/status`	✅ Admin	Active inference engine info
Detection Response (JSON)
```json
{
  "pothole_detected": true,
  "confidence": 0.84,
  "pothole_count": 2,
  "engine": "tflite",
  "timestamp": "2024-11-01T14:32:17",
  "materials": {
    "area_m2": 0.31,
    "depth_cm": 6.0,
    "volume_m3": 0.0186,
    "cement_kg": 6.51,
    "sand_kg": 13.02,
    "aggregate_kg": 19.53
  },
  "location": {
    "source": "wifi_ip",
    "lat": 27.1767,
    "lng": 78.0081,
    "city": "Agra",
    "region": "Uttar Pradesh",
    "country": "India",
    "ip": "103.x.x.x"
  }
}
```
---
🚀 Quick Start
```bash
# Clone / unzip the project
cd PotholeAI

# Install dependencies
pip install flask opencv-python-headless numpy

# Optional: activate YOLOv8 AI mode
# Place best_int8.tflite in the project root, then:
pip install tflite-runtime   # Raspberry Pi / ARM
# OR
pip install tensorflow       # Windows / Linux desktop

# Run the server
python app.py
```
Open http://localhost:5000 → Login: `Vinayak` / `vinayak@123`
ESP32-CAM endpoint (no login needed):
```
POST http://<server-ip>:5000/detect
```
---
📊 Current Progress Status
Component	Status
ESP32-CAM firmware (Wi-Fi + HTTP POST)	✅ Complete & tested
Flask server with public `/detect` endpoint	✅ Complete & running
YOLOv8 INT8 TFLite inference pipeline	✅ Complete
OpenCV fallback engine	✅ Complete
Dynamic material volume estimation	✅ Complete
Wi-Fi / IP geolocation (no GPS hardware)	✅ Complete
SQLite persistence layer	✅ Complete
Admin dashboard with authentication	✅ Complete
Interactive Leaflet map	✅ Complete
Vehicle-mounted field test	✅ Prototype tested
Full hardware deployment (production)	🔄 In progress
---
🔭 Future Scope
Near-term (3–6 months)
Integrate a GPS module (u-blox NEO-6M) directly on the ESP32 for precise coordinate logging
Add severity classification: surface crack → minor → moderate → severe → critical
Build a municipal reporting API so detections automatically raise work orders
Medium-term (6–12 months)
Deploy the server on a Raspberry Pi 4 mounted in the vehicle for fully offline, edge-only inference
Add multi-class detection: speed bumps, road markings, drainage grates
Enable OTA (Over-The-Air) firmware updates for the ESP32-CAM fleet
Long-term vision
City-scale fleet deployment: one server, many cameras across all municipal vehicles
Historical trend analysis: track which roads deteriorate fastest and predict repair cycles
Integration with government road-maintenance portals (PMGSY / Smart Cities Mission data)
Mobile app for citizen reporting that feeds the same backend
---
🧑‍💻 Team & Acknowledgements
Developer: Vinayak
Institution: [Your College Name]
Domain: Internet of Things · Computer Vision · Web Systems
We extend our gratitude to Dr. Anjan Kumar for bringing the Silicon Labs Centre of Innovation IoT Challenge to our attention, and to the Silicon Labs team for creating a platform where student prototypes receive the recognition — and the career opportunities — they deserve.
---
📎 Submission Checklist (Silicon Labs COI)
[x] Project Overview & Problem Statement
[x] Block Diagram / System Architecture
[x] Working Methodology & Project Flow
[x] Circuit Design & Hardware Implementation
[x] Current Progress Status
[x] Future Scope
[x] Working software prototype (server + dashboard)
[x] Working hardware prototype (ESP32-CAM)
[x] Repository structure and documentation
---
📄 License
This project is submitted under the MIT License.
Free to use, study, adapt, and build upon — with attribution.
---
🔗 Resources & References
Silicon Labs COI Campaign Template
Silicon Labs Community Forum
ip-api.com Geolocation Docs
YOLOv8 by Ultralytics
ESP32-CAM Datasheet — AI-Thinker
Leaflet.js Map Library
---
<div align="center">
Built with curiosity. Tested on broken roads. Submitted with conviction.
PotholeAI — because every pothole is a data point, and every data point is a step toward safer roads.
</div>
