# LANCET heater firmware

Arduino sketch that runs the five-phase thermal protocol described in the
LANCET hardware paper.

## Files

- `heater_firmware.ino` -- the sketch. Released under the MIT License (see
  `../LICENSE-FIRMWARE`).
- `platformio.ini` -- optional PlatformIO project file for IDE-agnostic builds.

## Hardware required

See `../electronics/bom.csv` for the full bill of materials and
`../electronics/schematic.md` for the wiring. In summary:

- Arduino Uno R3
- SparkFun 5 V resistive heating pad (COM-11288)
- DS18B20 waterproof temperature probe on D7 with a 4.7 kohm pull-up to +5 V
- N-channel MOSFET (IRLB8721) with the gate on D9 and a 10 kohm pull-down to GND
- Momentary pushbutton between D2 and GND (the firmware uses `INPUT_PULLUP`)
- Benchtop 3-24 V DC supply for the heater (we use ~10 V)

## Library dependencies

Install both via the Arduino IDE Library Manager (or the equivalents for your
toolchain):

- `OneWire` by Paul Stoffregen (>= 2.3.7)
- `DallasTemperature` by Miles Burton (>= 3.9.0)

## Building and flashing with the Arduino IDE

1. Open `heater_firmware.ino` in the Arduino IDE (>= 2.0).
2. Select **Tools > Board > Arduino Uno**.
3. Select the serial port for your Uno under **Tools > Port**.
4. Verify that the two libraries above are installed.
5. Click **Upload**.

## Building with PlatformIO (optional)

```bash
pio run -t upload
pio device monitor -b 115200
```

The provided `platformio.ini` targets `uno` and pins the same library
versions as above.

## Serial output format

The firmware logs at 1 Hz over the USB serial port at 115200 baud. Each line
is a CSV record:

```
millis,state,phase_idx,phase_name,set_c,measured_c,pwm
```

- `state` is one of `IDLE`, `RUNNING`, `DONE`, `FAULT`.
- `phase_idx` is `0..4` while running and the previous phase index otherwise.
- `phase_name` matches the name in the protocol table inside the sketch.
- `set_c` is the current set point in degrees Celsius (0 when idle).
- `measured_c` is the DS18B20 reading.
- `pwm` is the 0-255 duty cycle written to the MOSFET gate this tick.

You can pipe the output into any logging tool, e.g.:

```bash
pio device monitor -b 115200 | tee data/run_$(date +%Y%m%d_%H%M%S).csv
```

## Safety behavior

If the DS18B20 reading exceeds 60 degrees Celsius the firmware drops the
heater PWM to zero, prints `FAULT:` over serial, and latches in
`STATE_FAULT` until the Arduino is reset. The same cutoff fires if the
DS18B20 returns the disconnected sentinel (-127 degrees Celsius).

## Operating

1. Wire and power up the controller. Verify the Arduino enumerates over USB
   and the firmware prints the boot banner on the serial monitor.
2. Place the loaded cartridge on the heating pad.
3. Press the start button (D2 to GND). The firmware logs
   `# Protocol start` and steps through phases 0..4.
4. After ~127 minutes the firmware logs `# Protocol complete` and parks the
   heater. Press the button again to re-run.

## Modifying the protocol

The five-phase profile is defined as a `constexpr Phase[]` array near the top
of the sketch. Changing a set point or duration is a one-line edit; the
control loop adapts automatically. The same array is mirrored in
`../data/thermal_protocol_set_points.csv` -- update both to keep documentation
and firmware in sync.
