# Photo and UI Mockup Guide

The visual direction is intentionally simple:

**ESP32 + breadboard + button + LED + resistor + phone.**

No LCD, OLED, BME280, buzzer or other display/sensor hardware is part of the core build.

## Generated visual direction

The generated project mockup should show:
- two ESP32 DevKit boards;
- a solderless breadboard;
- one red LED;
- one push button;
- jumper wires;
- a phone showing the ScoutLink HTML dashboard;
- JOTA-JOTI 2026 styling;
- no extra electronics.

The phone UI should show a Scout challenge, Field Node status, event counter and activity log.

## Real photo checklist

If you take photos for the repository:

1. **01-hardware.jpg** — both ESP32 boards and the breadboard.
2. **02-wiring.jpg** — close-up of GPIO 4 button and GPIO 2 LED wiring.
3. **03-phone-dashboard.jpg** — phone connected to the ESP32 Wi-Fi and showing the dashboard.
4. **04-connected.jpg** — both ESP32s operating with the phone dashboard.
5. **05-scout-challenge.jpg** — Scouts completing a challenge.
6. **06-result.jpg** — completed challenge and activity log.

Do not expose the Wi-Fi password in a public photo. Do not photograph children unless your group's photography policy allows it.

## Browser simulation

`../simulation/index.html` is the no-hardware version of the same phone workflow. It should look and behave like the real ESP32-hosted page as closely as practical.

Do not copy random internet photos into this repository unless you have permission to redistribute them.
