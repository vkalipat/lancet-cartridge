# Build instructions

Numbered, end-to-end build of the LANCET cartridge: print the tube, cast
the wax valves, wire the controller, flash the firmware. Total bench time
is ~3 hours for a first build (~45 min printing, ~30 min wax prep, ~30 min
controller wiring, ~15 min firmware flash, ~30 min coloured-water
validation), excluding tube anneal and wax cool times.

## A. Print the polycarbonate tube

1. Open `cad/tube_body.stl` and `cad/tube_cap.stl` in your slicer
   (PrusaSlicer, Cura, or equivalent). Use clear polycarbonate filament.
2. Configure the slicer: 0.1 mm layer height, 100 % infill, 260 C
   nozzle, 105 C bed, 0.4 mm nozzle. Disable cooling fans for the first
   three layers.
3. Prepare the build plate with a polycarbonate-compatible adhesive. We
   used Magigoo PC; PVA glue stick on a glass bed also works if applied
   thinly and uniformly.
4. Print both parts in the same job. Expect ~45 min for the body and
   ~15 min for the cap on a typical FDM machine.
5. Anneal the printed parts at 80 C for 30 min in a dedicated oven (do
   not use the same oven as for food). This relieves residual stress
   and tightens the friction-fit tolerance.
6. After cooling, deburr the friction-fit ridges with a fine sanding
   stick (600 grit). The compartment seams should slide together with a
   firm thumb push but not require tools.
7. Wrap each interior friction-fit surface with one layer of PTFE thread
   tape (~0.075 mm thick). Stretch the tape lightly so it conforms to
   the printed thread without bunching.

## B. Cast the wax valves

1. Mix and melt the two paraffin-mineral oil formulations as described in
   `wax_protocol.md`. The lower-threshold valve uses a 10:7 mass ratio
   (45 C melt); the upper-threshold valve uses 10:8 (50 C melt).
2. Set this step aside until you reach the assembly stage in
   `operation.md` -- the wax must be dispensed on top of the loaded
   reagent pellet, not into an empty tube.

## C. Wire the thermal controller

The reference build is a solderless breadboard layout; see
`electronics/breadboard_layout.md` for a step-by-step that mirrors the
photographs in the paper.

1. Place the IRLB8721 N-MOSFET on the breadboard with the metal tab
   facing away from you. From left to right the pins are gate, drain,
   source.
2. Connect the MOSFET source to the breadboard's ground rail.
3. Connect the MOSFET gate to Arduino pin D9 through a series jumper.
   Add a 10 kohm pull-down resistor between gate and ground. The
   pull-down ensures the heater is off whenever the Arduino is reset
   or unpowered.
4. Connect the heating pad's negative lead to the MOSFET drain. Connect
   the pad's positive lead to the positive rail of the benchtop
   3-24 V supply.
5. Tie the supply ground to the Arduino ground (common ground is
   essential -- the MOSFET will not switch cleanly without it).
6. Wire the DS18B20: red to +5 V, black to ground, yellow (data) to
   Arduino pin D7. Add a 4.7 kohm pull-up resistor between the data
   line and +5 V.
7. Wire a momentary pushbutton between Arduino pin D2 and ground. The
   firmware enables `INPUT_PULLUP` so no external resistor is required.
8. Power the Arduino over USB from your build computer for the initial
   flash.

## D. Flash the firmware

1. Open `firmware/heater_firmware.ino` in the Arduino IDE (>= 2.0).
2. Install the OneWire (Paul Stoffregen, >= 2.3.7) and DallasTemperature
   (Miles Burton, >= 3.9.0) libraries via the Library Manager.
3. Select **Tools > Board > Arduino Uno** and the correct serial port.
4. Click **Upload**. The firmware prints a CSV header and a boot banner
   on the serial monitor at 115200 baud.

## E. Verify before the first run

1. With the cartridge empty and the heater pad cold, press the start
   button. The serial log should advance through the five phases. The
   heater PWM column should remain non-zero only when the measured
   temperature is below the current set point.
2. Confirm the safety cutoff: temporarily breathe on the DS18B20 probe
   tip while the firmware is in `STATE_RUNNING` to push the reading
   above the set point; the PWM column should drop to 0 within one tick
   without entering `FAULT`. Resetting the Arduino restores idle.
3. Run the coloured-water tracing protocol in
   `validation_protocols.md` before using the cartridge with biological
   reagents. This is the only way to confirm that the wax valves
   actuate at the right temperatures in your specific build.
