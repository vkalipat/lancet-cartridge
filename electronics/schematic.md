# Thermal controller schematic (text)

This file is the authoritative wiring reference for the LANCET thermal
controller until the KiCad source files (`heater_schematic.sch` and
`heater_pcb.kicad_pcb` referenced in the paper's design-files table) are
released in the `v0.1.0` tagged release. The text-based schematic below
captures every net and component value needed to reproduce the build.

## Block diagram

```
                     +-----------------+
                     |  Arduino Uno R3 |
                     |                 |
            +---+--->| D2  start_btn   |
            |   |    | D7  ds18b20_data|<--- DS18B20 yellow
            |   |    | D9  heater_pwm  |---> MOSFET gate
            |   |    | +5V             |---> DS18B20 red, pull-ups
            |   |    | GND             |---> common ground
            |   |    +-----------------+
            |   |
            |   |    +----------------+        +---------------+
            |   |    | IRLB8721       |        | Heating pad   |
            |   +----| Gate           |        |  (5 V, 5x10cm)|
            |        | Drain         <---------|  -            |
            |        | Source        +-->GND   |  +            |
            |        +----------------+        +---|           |
            |                                      |   |       |
            |                                      |   v       |
            |                                      |  (3-24 V) |
            |                                      |  benchtop |
            |   start button (NO, momentary)        \  supply  /
            |   between D2 and GND                   +---------+
            |
            | (D2 has firmware-enabled INPUT_PULLUP)
            v
           GND
```

## Net list

| Net               | From                       | To                            | Notes                                          |
| ----------------- | -------------------------- | ----------------------------- | ---------------------------------------------- |
| `+5V`             | Arduino +5 V               | DS18B20 Vdd, R_pullup high    | 5 V logic rail                                 |
| `GND`             | Arduino GND                | DS18B20 GND, MOSFET source, pad supply GND, R_pulldown low, button low | Common ground -- mandatory                     |
| `DS_DATA`         | Arduino D7                 | DS18B20 data, R_pullup low    | 1-Wire bus                                     |
| `HTR_PWM`         | Arduino D9                 | MOSFET gate, R_pulldown high  | PWM-modulated gate drive                       |
| `START_BTN`       | Arduino D2                 | Pushbutton high               | Active-low; firmware uses `INPUT_PULLUP`       |
| `BTN_GND`         | Pushbutton low             | GND                           |                                                |
| `HTR_DRAIN`       | MOSFET drain               | Heating pad negative          | Switched leg                                   |
| `HTR_SUPPLY+`     | Benchtop supply +          | Heating pad positive          | 3-24 V DC, ~10 V typical                       |
| `HTR_SUPPLY-`     | Benchtop supply ground     | GND                           | Tie to common ground                           |

## Component values

| Reference | Component                                            | Value / Part      |
| --------- | ---------------------------------------------------- | ----------------- |
| U1        | Microcontroller board                                | Arduino Uno R3    |
| Q1        | N-channel MOSFET, logic-level, 60 V / 30 A, TO-220   | IRLB8721          |
| H1        | Resistive heating pad, 5 V, 5 cm x 10 cm             | SparkFun COM-11288 |
| TS1       | DS18B20 waterproof 1-Wire temperature probe          | Adafruit 381      |
| R1        | Gate pull-down (D9 to GND)                           | 10 kohm, 1/4 W    |
| R2        | DS18B20 data pull-up (D7 to +5 V)                    | 4.7 kohm, 1/4 W   |
| SW1       | Tactile pushbutton, 6 mm, momentary, normally open   | Generic           |
| PSU       | Benchtop DC supply, 3-24 V, >= 2 A                   | Lab supply        |

## Why these values

- **R1 (10 kohm pull-down on the MOSFET gate)** ensures the gate is held
  at 0 V whenever the Arduino is unpowered, in reset, or in
  high-impedance state. Without it, the MOSFET can latch into a partial
  conduction state and heat the pad uncontrollably. This is a
  safety-critical part.
- **R2 (4.7 kohm pull-up on the DS18B20 data line)** is the standard
  1-Wire pull-up for parasite or external power configurations and is
  the value Maxim/Analog Devices and Adafruit recommend for cable runs
  under ~1 m.
- **IRLB8721** is a logic-level MOSFET fully on at Vgs = 5 V, has very
  low Rds(on) at Arduino-driven gate voltages, and is a TO-220 part
  that handles the heater current with no heatsink.

## Layout notes (for the eventual `heater_pcb.kicad_pcb`)

A PCB build is documented as optional in the paper. The reference build
is breadboard-only (see `breadboard_layout.md`) -- a 170-tie-point
solderless breadboard accommodates every part above with room for a few
labelling jumpers.

When the KiCad PCB is committed it will use 2 oz copper on the heater
trace, a 0.1 in pitch screw terminal for the heating-pad leads, a TVS
diode across the gate-source for ESD survival, and silkscreen labels
matching the net names in the table above.
