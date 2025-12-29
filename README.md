# df64-control-board — Quick Tutorial

## Overview

This repository folder contains a PlatformIO-based ESP32 project (df64). This README explains how to build, flash, and debug the firmware as well as brief notes on wiring and the code structure.

![Wemos D1 mini pinout](assets/WeMos-D1-Mini-Pinout.png)

## Prerequisites

- Install PlatformIO (via VS Code extension or `pip install platformio`).
- An ESP32 board and USB cable.
- Optional: `esptool.py` for manual flashing, serial terminal such as `picocom`, `miniterm`, or PlatformIO's serial monitor.

## Project layout

- `platformio.ini` — build configuration for the project.
- `include/` — headers and configuration files.
- `src/main.cpp` — application entrypoint and main firmware code.

## Build and upload

From the `df64-control-board` directory you can use PlatformIO CLI or VS Code PlatformIO controls.

Using PlatformIO CLI:

```bash
cd df64-control-board
pio run            # build
pio run -t upload  # build + upload to board
```

If you need to erase flash before uploading:

```bash
pio run -t erase_flash
pio run -t upload
```

## Serial monitor

Open a serial monitor to view logs and interactive output. With PlatformIO:

```bash
pio device monitor
```

Default baud rate is commonly 115200; check `src/main.cpp` for any `Serial.begin()` value.

## Wiring / Hardware notes

- Connect the board's USB to your computer.
- If external peripherals (displays, sensors) are used, consult the pin definitions in `include/` or `src/main.cpp`.
- Push button connects to D5 (GPIO14) with internal pullup, active low (ground to active).
- Relay module connects to D6 (GPIO12), recommend 220V/10VAC.
- Entire system powered by 5VDC 1A adapter.

## Working:

- Relay off on startup.
- Press and hold push button (>500ms): relay on until released.
- Press button: relay on; press again to off. Auto off after 60s without any press to protect motor.

## Code overview (`src/main.cpp`)

- `setup()` typically configures peripherals, pins, and serial logging.
- `loop()` runs continuously and contains the main application logic.
- Look for configuration macros or constants in `include/` to change pins, display orientation, or other hardware settings.

Open [src/main.cpp](src/main.cpp) for the implementation details and comments.

## Configuration tips

- To change board settings, edit `platformio.ini` (board, upload speed, monitor port).
- To change pinouts or hardware options, edit files under `include/`.

## Troubleshooting

- Upload fails: ensure correct board selected in `platformio.ini`, press boot/EN if needed.
- Serial output missing: confirm correct serial port and baud rate.
- Build errors: check PlatformIO dependencies and installed frameworks (ESP-IDF or Arduino-ESP32).

## Contributing

If you make improvements, open a PR or create issues describing the change.

---

If you'd like, I can expand this README with wiring diagrams, annotated code snippets from `src/main.cpp`, or PlatformIO configuration examples tailored to your board. Which would you like next?



