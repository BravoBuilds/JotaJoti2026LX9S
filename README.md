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

## Hardware — deliberately simple

This project is **ESP32 + breadboard only**. There is no LCD, OLED, sensor pack, buzzer or other extra hardware.

You need:
- 2 × ESP32 development boards
- 2 × USB cables
- 1 × solderless breadboard
- 1 × push button
- 1 × red LED
- 1 × 220–330 Ω resistor
- jumper wires
- a phone with Wi-Fi and a web browser

The **phone is the display**. ESP32 #1 creates the Wi-Fi network and serves the HTML interface directly.

### Field-node wiring

| Component | Connection |
|---|---|
| Push button | GPIO 4 → button → GND |
| LED anode | GPIO 2 → 220–330 Ω resistor → LED |
| LED cathode | GND |
| ESP32 power | USB |

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
    ├── ui-and-build-mockup.svg
    └── PHOTO_GUIDE.md
```

## What the finished project should look like

The intended setup is shown by the visual mockup in `assets/ui-and-build-mockup.svg`.

**Physical side:** two ESP32 boards, a breadboard, a push button, one LED and a resistor.

**Phone side:** a dark JOTA-JOTI-themed HTML dashboard with Base Station status, Field Node status, Scout challenge, event counter, latest activity, challenge completion and communication log.

The HTML is served locally by the Base Station ESP32. A phone joins the ESP32 Wi-Fi and opens the page in a normal browser.

## Phone connection — the main demo

1. Power **ESP32 #1 Base Station**.
2. On the phone, open Wi-Fi settings.
3. Join **`JOTA-JOTI-2026`**.
4. Enter the demo password **`ScoutLink2026`**.
5. Open **`http://192.168.4.1`**.
6. The ScoutLink dashboard appears.
7. Power **ESP32 #2 Field Node**.
8. Press the breadboard button.
9. Watch the phone dashboard show the field-node event.

The project does **not** need internet access for this demonstration.

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

## Suggested generated-photo / presentation set

For a project presentation, make or generate these five visuals using the exact build above:

1. **Hardware hero:** two ESP32 DevKit boards on a breadboard with the red LED and button visible.
2. **Phone dashboard:** a phone connected to `JOTA-JOTI-2026` showing the ScoutLink challenge screen.
3. **Connected setup:** both ESP32s powered and the phone dashboard showing **FIELD NODE: ONLINE**.
4. **Challenge moment:** Scouts using the physical button while the phone displays the active challenge.
5. **Result:** the dashboard showing completed challenges and the activity log.

Keep every generated or photographed hardware visual consistent with the actual project: **ESP32 + breadboard + button + LED + resistor + phone only**.

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
