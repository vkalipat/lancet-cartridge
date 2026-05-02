# LANCET cartridge -- open hardware repository

Companion repository for the HardwareX paper *"An open-source sub-$50 one-pot
cartridge with dual-threshold paraffin-wax valves for sequential RPA-Cas12a
diagnostics at the point of care"* (V. S. Kalipatnapu, Lambert iGEM 2025
Team, [co-authors TBD], 2026).

The LANCET cartridge is a 3D-printed three-compartment polycarbonate tube
with two paraffin-mineral oil wax valves that melt at distinct temperatures
(45 C and 50 C). It is driven by an Arduino-controlled heating pad that
runs a five-phase thermal protocol, releasing reagents in sequence: RPA
amplification, then CRISPR-Cas12a, then a downstream lateral-flow or
fluorescence reporter. Total per-unit reusable hardware cost: USD 48.58.

This repository contains everything needed to build, flash, and operate one.

## Repository layout

```
.
+-- README.md                          (this file)
+-- LICENSE-HARDWARE                   CERN-OHL-W v2 (CAD, schematics, layouts)
+-- LICENSE-FIRMWARE                   MIT License (Arduino sketch)
+-- LICENSE-DOCS                       CC BY 4.0 (Markdown docs and BOM CSV)
+-- CITATION.cff                       Citation metadata (paper + Zenodo DOI)
+-- .zenodo.json                       Zenodo metadata for automatic DOI minting
+-- docs/
|   +-- build_instructions.md          Print -> wire -> assemble (numbered)
|   +-- operation.md                   User-facing run instructions
|   +-- wax_protocol.md                Wax preparation, dispensing, curing
|   +-- validation_protocols.md        Coloured-water + thermal characterization
|   +-- safety.md                      Hot wax, blood handling, electrical
+-- cad/
|   +-- README.md                      What's here, what software opens it
|   +-- tube_body.stl                  PLACEHOLDER -- see CAD README
|   +-- tube_cap.stl                   PLACEHOLDER -- see CAD README
|   +-- tube_assembly.f3d.notes.md     Parametric source description
+-- electronics/
|   +-- schematic.md                   Text-based schematic (KiCad pending)
|   +-- breadboard_layout.md           Step-by-step wiring
|   +-- bom.csv                        Itemized BOM, 9 rows
+-- firmware/
|   +-- heater_firmware.ino            Five-phase Arduino sketch (compilable)
|   +-- README.md                      Flash and dependency instructions
|   +-- platformio.ini                 Optional PlatformIO config
+-- data/
    +-- README.md                      Validation data inventory
    +-- thermal_protocol_set_points.csv  Time-vs-temperature profile
```

## Quick start

```bash
git clone https://github.com/lambert-igem/lancet-cartridge.git
cd lancet-cartridge
```

1. Print the tube. Slice `cad/tube_body.stl` and `cad/tube_cap.stl` at
   0.1 mm layer height, 100 % infill, 260 C nozzle, 105 C bed in clear
   polycarbonate. Anneal at 80 C for 30 min. Wrap interior friction-fit
   surfaces with one layer of PTFE tape. See `docs/build_instructions.md`.
2. Mix the wax. Heat paraffin to 70 C, add mineral oil at 10:7 mass ratio
   (lower-threshold, melts at 45 C) and 10:8 (upper-threshold, melts at
   50 C). Dispense 20-25 microlitres of each into the cartridge with a
   pre-warmed pipette. See `docs/wax_protocol.md`.
3. Wire the controller. Arduino Uno + IRLB8721 N-MOSFET (gate on D9 with a
   10 kohm pull-down) + DS18B20 on D7 (4.7 kohm pull-up) + start button on
   D2. See `electronics/schematic.md` and `electronics/breadboard_layout.md`.
4. Flash the firmware. Open `firmware/heater_firmware.ino` in the Arduino
   IDE (>= 2.0), install OneWire and DallasTemperature from the Library
   Manager, select Arduino Uno, and click Upload. See
   `firmware/README.md`.
5. Run an assay. Load reagents and dehydrated pellets, cap the tube, place
   it on the heating pad, and press the start button. The five-phase
   protocol runs to completion in ~127 min. See `docs/operation.md`.

## Hardware overview

| Subsystem        | Summary                                                         |
| ---------------- | --------------------------------------------------------------- |
| One-pot tube     | 3D-printed polycarbonate, three compartments, ~200 microlitres each, 0.1 mm friction-fit, PTFE-tape sealing surfaces. |
| Wax valves       | 10:7 paraffin-mineral oil (45 C melt) and 10:8 (50 C melt), 20-25 microlitre disks. |
| Thermal control  | Arduino Uno + SparkFun heating pad + IRLB8721 MOSFET + DS18B20 probe. PWM closed-loop, ~10 V supply, ~6 min to 37 C, +/- 1.5 C surface fidelity. |
| Protocol         | 37 C / 70 min -> 46 C / 1 min -> 37 C / 25 min -> 51 C / 1 min -> 37 C / 30 min (~127 min total). |
| Cost             | USD 48.58 per unit (reusable controller); ~USD 1.20 per cartridge in consumables. |

## Licensing

This repository uses three permissive open licenses, applied per directory:

| Material                                                | License       | File                |
| ------------------------------------------------------- | ------------- | ------------------- |
| Hardware design files (`cad/`, `electronics/*.md`)      | CERN-OHL-W v2 | `LICENSE-HARDWARE`  |
| Firmware (`firmware/heater_firmware.ino`)               | MIT           | `LICENSE-FIRMWARE`  |
| Documentation (`docs/`, `*.md`, `bom.csv`, set-point CSV) | CC BY 4.0   | `LICENSE-DOCS`      |

Each source file carries an SPDX identifier where syntactically possible.

## Status of the design files

The firmware is complete, compilable, and matches the protocol described in
the paper. The hardware design files in `cad/` and the schematic in
`electronics/` are placeholders pending the binary CAD/KiCad release;
the parametric description in `cad/tube_assembly.f3d.notes.md` and the text
schematic in `electronics/schematic.md` capture all dimensions, tolerances,
and net connections needed to reproduce the build today. The binary files
will be added in a tagged release that triggers a Zenodo archive.

## Citation

If you use this hardware, firmware, or documentation, please cite both the
paper and the archived release. Machine-readable metadata is in
`CITATION.cff`:

> Kalipatnapu, V. S., Lambert iGEM 2025 Team, et al. (2026). An open-source
> sub-$50 one-pot cartridge with dual-threshold paraffin-wax valves for
> sequential RPA-Cas12a diagnostics at the point of care. *HardwareX*.
> doi:10.0000/PLACEHOLDER. Repository: https://github.com/lambert-igem/lancet-cartridge.
> Archive: https://doi.org/10.5281/zenodo.PLACEHOLDER.

## Contact

- Corresponding author: V. S. Kalipatnapu (lambertigem621@gmail.com)
- Team: Lambert iGEM 2025, Lambert High School, Suwanee, Georgia, USA
- Issues and pull requests: https://github.com/lambert-igem/lancet-cartridge

## Acknowledgements

This work was supported by Lambert High School and the Lambert iGEM
Sponsorship Program. We thank the Bhamla Lab (M. Saad Bhamla, Georgia
Institute of Technology) for the Evapinator blowdown evaporator and
dehydration protocol, the Global Lyme Alliance for clinical guidance, and
the iGEM Foundation.
