# JotaJoti2026LX9S — ScoutLink Relay

**A two-ESP32 JOTA-JOTI 2026 scouting communications project**

ScoutLink Relay turns two ESP32 boards into a small local Scout communication camp:

- **Base Station (ESP32 #1):** creates the local Wi-Fi camp network, hosts the dashboard, tracks the field node, and issues Scout Challenges.
- **Field Node (ESP32 #2):** joins the base station, reports button activity, receives challenges, and acknowledges them.
- **Browser simulation:** demonstrates the whole experience without hardware.
- **Visual assets:** wiring and system diagrams are included in `assets/`.

The project is designed as a JOTA-JOTI-style activity around communication, teamwork, digital skills, discovery, challenges and sustainability. The Base Station itself serves the HTML dashboard, so Scouts connect with a phone browser instead of adding an LCD or other display. The minimum build uses ESP32 boards, a breadboard, one push button, one LED and a resistor.

## What it does

### Base Station
1. Starts a Wi-Fi access point named `JOTA-JOTI-2026`.
2. Serves a ScoutLink dashboard at `http://192.168.4.1/`.
3. Accepts heartbeat/status reports from the field node.
4. Sends a rotating Scout Challenge.
5. Displays node status, activity counter and latest event.

### Field Node
1. Connects to the Base Station Wi-Fi.
2. Sends a heartbeat every 5 seconds.
3. Sends an event when the Scout button is pressed.
4. Polls for the current challenge.
5. Shows activity on its LED.

### Example Scout Challenges
- **Radio Scout:** send a short Morse message using the button.
- **Green Scout:** identify one way your group can reduce energy use.
- **World Scout:** invent a greeting for another country.
- **Digital Scout:** explain one rule for staying safe online.
- **Team Scout:** complete a challenge together and log the result.

## Hardware

Minimum:
- 2 × ESP32 development boards
- 2 × USB cables
- 1 × push button
- 1 × LED + 220–330 Ω resistor (optional if your board has a usable onboard LED)
- jumper wires + breadboard

Optional:
- SSD1306 I²C OLED
- BME280 temperature/humidity sensor
- light sensor
- buzzer
- battery pack

### Field-node pins

| Part | GPIO |
|---|---:|
| Scout button | 4 to GND |
| Status LED | 2 |
| Optional I²C SDA | 21 |
| Optional I²C SCL | 22 |

GPIO 4 is used for the button because it avoids the common boot-strap concern of GPIO 0 on many ESP32 DevKit boards.

## Project layout

```text
JotaJoti2026LX9S/
├── README.md
├── LICENSE
├── platformio.ini
├── firmware/
│   ├── controller/main.cpp
│   └── field_node/main.cpp
├── simulation/index.html
└── assets/
    ├── system-overview.svg
    ├── wiring.svg
    └── PHOTO_GUIDE.md
```

## Quick start

### Browser simulation

No ESP32 required.

1. Open `simulation/index.html` in a browser.
2. Press **Scout Button** to simulate field-node activity.
3. Press **New Challenge** to rotate the JOTA-JOTI challenge.
4. Use **Send Message** to simulate communication.

### Two real ESP32s

Install VS Code + PlatformIO.

1. Open this repository.
2. Upload the `controller` environment to ESP32 #1.
3. Connect a phone/laptop to Wi-Fi `JOTA-JOTI-2026` using password `ScoutLink2026`.
4. Open `http://192.168.4.1/`.
5. Upload the `field_node` environment to ESP32 #2.
6. The field node connects automatically.
7. Press its button and watch the dashboard.

## Scout walkthrough — 30–45 minutes

### 1. Discover — 5 min
Ask: **How can two Scouts communicate without a normal Wi-Fi router?**

Show that ESP32 #1 becomes the camp's access point.

### 2. Build — 10 min
Wire the button and LED on the field node.

Explain:
- input = button
- output = LED
- network = Wi-Fi
- protocol = HTTP
- data = JSON

### 3. Connect — 5 min
Power the base station first, then the field node.

Look for:

`FIELD NODE: ONLINE`

### 4. Challenge — 10 min
Give the team a challenge, for example:

> **Green Scout:** find one practical way your Scout group can reduce electricity use at camp.

Press the button to acknowledge the activity.

### 5. JOTA-JOTI extension — 10 min
Pair groups. One group runs the Base Station and another runs the Field Node.

Exchange:
- a Scout greeting
- a Morse message
- a sustainability idea
- a fun fact about the group

### 6. Reflection — 5 min
Discuss:
- What information should never be sent over an open network?
- Why is a local network useful?
- How could this become a worldwide Scout activity?
- How could the project use less energy?

## Protocol

Field event:

`POST /api/event`

```json
{"node":"FIELD-01","event":"SCOUT_BUTTON","value":1}
```

Heartbeat:

`GET /api/heartbeat`

Challenge:

`GET /api/challenge`

Dashboard state:

`GET /api/state`

The controller exposes a small local HTTP API and does not require a cloud account.

## Safety and privacy

This is an educational local network.

- Do not collect real names, phone numbers, passwords or private messages.
- Do not connect the demo to an untrusted network unless you understand the consequences.
- Change the default Wi-Fi password before real deployment.
- This project does not transmit on amateur-radio frequencies.
- Any actual amateur-radio activity must use appropriate licensed equipment and operating procedures.

## JOTA-JOTI theme

Use this as a hands-on activity about **connection, discovery, challenges, sustainability, digital skills and international friendship**. It is an independent educational project, not an official JOTA-JOTI service.

Official event information: https://jotajoti.info/

## License

MIT — see `LICENSE`.
