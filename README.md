# LANCET cartridge -- open-source programmable thermal-sequencing platform

Companion repository for the HardwareX paper *"An Open-Source
Thermal-Sequencing Cartridge Platform for Sequential Multi-Step Point-of-Care
Biochemistry"* (V. S. Kalipatnapu, Lambert iGEM 2025 Team, [co-authors TBD],
2026).

## What this is

The LANCET cartridge is an open-source programmable thermal-sequencing
cartridge platform: a single closed 3D-printed tube in which tunable
paraffin-mineral oil wax valves, set at programmable melt thresholds,
release reagents in turn under a software-defined thermal protocol
delivered by an Arduino-controlled heating pad. The substrate runs
arbitrary multi-step biochemistry inside one tube. Total per-unit
reusable hardware cost: USD 48.58. Per-cartridge consumable cost: ~USD
1.20 (excluding biochemical reagents).

The platform is in principle chemistry-agnostic. Any multi-step
biochemistry whose phases can be sequenced by temperature thresholds
(CRISPR-Cas12a / Cas13, LAMP and LAMP-CRISPR multiplex panels,
sample-preparation cascades, enzymatic readouts) can in principle adopt
the same substrate. To date, only one chemistry has been demonstrated:
the RPA -> CRISPR-Cas12a -> fluorescence / lateral-flow workflow used in
the Lambert iGEM 2025 Lyme-diagnostic application below.

This repository contains everything needed to build, flash, and operate
the platform.

## Demonstrated application: Lyme RPA-Cas12a diagnostic

The platform was first exercised with a CspZ-targeting RPA-Cas12a
workflow for early-window *Borrelia burgdorferi* (Lyme) detection. In
that demonstration, a five-phase thermal protocol releases reagents in
sequence: RPA amplification at 37 C, the lower-threshold (45 C) wax
valve melts to release Cas12a, Cas12a recognition at 37 C, the
upper-threshold (50 C) wax valve melts to release the reporter, and a
final 30-min reporter incubation produces fluorescence or lateral-flow
signal. We recovered both readouts from a single closed reaction and
validated qualitative integration across early-window (2 dpi) and
chronic (250 dpi) bovine-blood samples spiked with cultured *B.
burgdorferi*. Limit of detection, analytical sensitivity, and clinical
specificity for the demonstrated application are next-step work.

The same substrate adapts to a different chemistry by editing the
firmware set-point table and (if the new chemistry uses different
incubation temperatures) re-tuning the paraffin-mineral oil mass ratios.
Re-targeting does not require any change to the printed tube, the
electronics, or the licensing.

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
                                         (RPA-Cas12a demonstration)
```

## Quick start

```bash
git clone https://github.com/vkalipat/lancet-cartridge.git
cd lancet-cartridge
```

1. Print the tube. Slice `cad/tube_body.stl` and `cad/tube_cap.stl` at
   0.1 mm layer height, 100 % infill, 260 C nozzle, 105 C bed in clear
   polycarbonate. Anneal at 80 C for 30 min. Wrap interior friction-fit
   surfaces with one layer of PTFE tape. See `docs/build_instructions.md`.
2. Mix the wax. For the RPA-Cas12a demonstration, heat paraffin to 70 C
   and add mineral oil at a 10:7 mass ratio (lower-threshold, melts at
   45 C) and 10:8 (upper-threshold, melts at 50 C). Dispense 20-25
   microlitres of each into the cartridge with a pre-warmed pipette.
   Other chemistries: re-tune the mass ratio to match the incubation
   temperatures of the new protocol; the formulation principle is in
   `docs/wax_protocol.md`.
3. Wire the controller. Arduino Uno + IRLB8721 N-MOSFET (gate on D9 with a
   10 kohm pull-down) + DS18B20 on D7 (4.7 kohm pull-up) + start button on
   D2. See `electronics/schematic.md` and `electronics/breadboard_layout.md`.
4. Flash the firmware. Open `firmware/heater_firmware.ino` in the Arduino
   IDE (>= 2.0), install OneWire and DallasTemperature from the Library
   Manager, select Arduino Uno, and click Upload. Edit the set-point
   table to match the chemistry you are running. See `firmware/README.md`.
5. Run an assay. Load reagents and (if required) dehydrated pellets, cap
   the tube, place it on the heating pad, and press the start button.
   The five-phase RPA-Cas12a protocol completes in ~127 min. See
   `docs/operation.md`.

## Hardware overview

| Subsystem        | Summary                                                         |
| ---------------- | --------------------------------------------------------------- |
| One-pot tube     | 3D-printed polycarbonate, three compartments, ~200 microlitres each, 0.1 mm friction-fit, PTFE-tape sealing surfaces. Geometry is chemistry-agnostic. |
| Wax valves       | Tunable paraffin-mineral oil disks, 20-25 microlitres each. Demonstrated set: 10:7 (45 C melt) and 10:8 (50 C melt). Formulation principle generalizes to other thresholds. |
| Thermal control  | Arduino Uno + SparkFun heating pad + IRLB8721 MOSFET + DS18B20 probe. PWM closed-loop, ~10 V supply, ~6 min to 37 C, +/- 1.5 C surface fidelity. Software-defined set-point table. |
| Demonstrated protocol | (RPA-Cas12a) 37 C / 70 min -> 46 C / 1 min -> 37 C / 25 min -> 51 C / 1 min -> 37 C / 30 min (~127 min total). |
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

The firmware is complete, compilable, and matches the demonstration
protocol described in the paper. The hardware design files in `cad/` and
the schematic in `electronics/` are placeholders pending the binary
CAD/KiCad release; the parametric description in
`cad/tube_assembly.f3d.notes.md` and the text schematic in
`electronics/schematic.md` capture all dimensions, tolerances, and net
connections needed to reproduce the build today. The binary files will
be added in a tagged release that triggers a Zenodo archive.

## Citation

If you use this hardware, firmware, or documentation, please cite both the
paper and the archived release. Machine-readable metadata is in
`CITATION.cff`:

> Gulati, A., Kalipatnapu, V. S., & Karthik, A. (2026). An open-source
> thermal-sequencing cartridge platform for sequential multi-step
> point-of-care biochemistry. *HardwareX*. doi:10.0000/PLACEHOLDER (in prep).
> Repository: https://github.com/vkalipat/lancet-cartridge.
> Archive: https://doi.org/10.5281/zenodo.20101160 (concept DOI; latest version v0.1.1 = 10.5281/zenodo.20101161).

## Contact

- Corresponding author: V. S. Kalipatnapu (lambertigem621@gmail.com)
- Team: Lambert iGEM 2025, Lambert High School, Suwanee, Georgia, USA
- Issues and pull requests: https://github.com/vkalipat/lancet-cartridge

## Acknowledgements

This work was supported by Lambert High School and the Lambert iGEM
Sponsorship Program. We thank the Bhamla Lab (M. Saad Bhamla, Georgia
Institute of Technology) for the Evapinator blowdown evaporator and
dehydration protocol used in the Lyme demonstration, the Global Lyme
Alliance for clinical guidance on the demonstrated application, and the
iGEM Foundation.
