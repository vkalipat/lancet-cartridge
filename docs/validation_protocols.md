# Validation protocols

Two protocols verify that a built cartridge behaves as the paper claims:
**coloured-water tracing** confirms selective wax-valve actuation, and
**thermal characterization** confirms that the controller hits and holds
each set point. Run both before using a new wax lot, a new print, or a
new controller wiring.

## Protocol 1 -- Coloured-water tracing of sequential valve actuation

### Purpose

Verify that the lower-threshold (10:7, 45 C) valve opens at the 46 C
phase and that the upper-threshold (10:8, 50 C) valve opens only at the
51 C phase, with both valves remaining solid throughout the long 37 C
incubations.

### Materials

- Three-compartment cartridge assembled as in `build_instructions.md`
  but loaded with food-coloured water instead of biological reagents:
  - Bottom compartment: 50 microlitres of yellow food-colour solution
    (water + food colouring, ~1 % v/v).
  - Middle compartment: 50 microlitres of blue food-colour solution.
  - Top compartment: 50 microlitres of clear water.
- Cast wax valves between compartments per `wax_protocol.md`.
- Built and flashed thermal controller.
- Smartphone camera for time-lapse imaging at 1 frame per 30 s.

### Procedure

1. Cap the cartridge and seal the loading port. Place on the heating
   pad with the camera positioned to capture the full tube length.
2. Start the time-lapse and press the firmware start button.
3. Phase 0 (37 C / 70 min): both valves should remain intact; dyes
   stay in their compartments.
4. Phase 1 (46 C / 1 min): the yellow dye should rise into the middle
   compartment within 60 s of reaching 46 C. The blue dye should
   remain confined.
5. Phase 2 (37 C / 25 min): the mixed yellow + blue solution in the
   middle compartment should appear green; the upper-threshold valve
   should remain intact.
6. Phase 3 (51 C / 1 min): the green solution should rise into the top
   compartment within 60 s of reaching 51 C and combine with the
   clear water there.
7. Phase 4 (37 C / 30 min): all three compartments share a common
   green volume.
8. Stop the time-lapse and review the recording.

### Pass criteria

- The 10:7 valve does not leak during phase 0 (verified by absence of
  yellow dye in the middle compartment).
- The 10:8 valve does not leak during phases 0-2 (no blue or green
  in the top compartment).
- Each valve fully opens within 1 min of reaching its target
  temperature.

A representative time-lapse is reproduced in Figure 2 of the paper.

## Protocol 2 -- Thermal characterization

### Purpose

Confirm that the controller reaches and holds each phase set point and
quantify deviation from the on-board DS18B20 reading.

### Materials

- Built and flashed thermal controller.
- An empty cartridge body (no wax, no liquids).
- A second temperature probe for ground truth. We used a calibrated
  K-type thermocouple with a Fluke 87V multimeter in temperature mode;
  any NIST-traceable probe with +/- 0.5 C uncertainty is sufficient.
- A computer running the Arduino IDE serial monitor (or `pio device
  monitor`) capable of writing the CSV log to a file.

### Procedure

1. Tape the reference probe to the outside of the empty cartridge body
   at the same height as the on-board DS18B20.
2. Place the cartridge on the heating pad. Power up the controller and
   start logging the serial output to a CSV file:
   ```bash
   pio device monitor -b 115200 | tee thermal_run_$(date +%Y%m%d_%H%M%S).csv
   ```
3. Press the start button. Note the time at which the reference probe
   stabilises within +/- 0.3 C of the 37 C set point.
4. Let the protocol run to completion. The reference probe should
   track each set point and resolve the brief excursions to 46 C and
   51 C without overshoot beyond 2 C.
5. After the run, plot the on-board reading (`measured_c` in the CSV)
   against the reference probe over time.

### Pass criteria

The paper reports the following targets, against which a new build
should be compared:

- Time to reach the 37 C RPA set point from ambient: ~6 min.
- Total elapsed protocol time: ~127 min.
- Surface-temperature agreement between the on-board DS18B20 and the
  external reference probe: within +/- 1.5 C across all five phases.

If the on-board reading and the reference probe disagree by more than
2 C, check that:

- The DS18B20 is in firm thermal contact with the cartridge wall (not
  hanging in air).
- The 4.7 kohm pull-up on D7 is present.
- The supply voltage is in the 3-24 V range and stable under load.

### Reporting

Archive the raw CSV alongside any plots in the `data/` directory of a
forked repository. Pull requests with reference-probe traces from
independent builds are welcomed.
