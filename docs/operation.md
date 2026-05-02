# Operation instructions

End-user procedure for running an assay on a built LANCET cartridge.
Assumes you have already built the controller and flashed the firmware
(`build_instructions.md`) and prepared the wax-valve formulations
(`wax_protocol.md`). Total assay time is ~127 min.

## Materials per run

- One assembled LANCET cartridge (3D-printed polycarbonate tube + cap,
  friction-fit, PTFE-tape sealed).
- 20 microlitres dehydrated RPA reagent pellet (RPA basic kit + primers
  for the target locus -- in our case CspZ, see Lambert iGEM 2025).
- 20 microlitres dehydrated Cas12a reagent pellet (LbCas12a + guide RNA +
  ssDNA reporter, dehydrated via the Bhamla Lab Evapinator protocol).
- ~20 microlitres of the lower-threshold (10:7, 45 C) molten wax.
- ~20 microlitres of the upper-threshold (10:8, 50 C) molten wax.
- Detection reagents: lateral-flow strip (Milenia HybriDetect or
  equivalent) **or** fluorescence buffer suitable for the chosen
  reporter dye.
- Patient sample: whole blood, urine, or synovial fluid lysate
  (see `safety.md` for handling).
- Removable adhesive seal for the cap port.

## Loading the cartridge

Work upright; the cartridge has a clear top and bottom orientation.

1. Place a 20 microlitre dehydrated RPA pellet in the bottom compartment.
2. Cast the lower-threshold (45 C) wax valve disk on top of the RPA
   pellet using a pre-warmed pipette tip. Allow the disk to solidify
   for at least 5 min at room temperature before proceeding.
3. Place a 20 microlitre dehydrated Cas12a pellet in the middle
   compartment.
4. Cast the upper-threshold (50 C) wax valve disk on top of the Cas12a
   pellet. Allow at least 5 min to solidify.
5. Load the detection reagents into the top compartment: either a strip
   of HybriDetect lateral-flow membrane folded to fit the compartment,
   or 100 microlitres of fluorescence buffer.
6. Snap the cap into place.
7. Pipette the patient sample (typically 50-100 microlitres) into the
   bottom compartment via the loading port in the cap, then seal the
   port with a removable adhesive sticker.

## Running the protocol

1. Power up the controller and confirm the serial banner. Verify the
   heating pad is dry and the DS18B20 probe is in firm thermal contact
   with the tube exterior (we tape the probe to the tube wall with a
   single wrap of polyimide tape).
2. Place the loaded cartridge on the heating pad with the bottom
   compartment in contact with the pad surface.
3. Press the start button. The firmware logs `# Protocol start` and
   begins phase 0 (RPA, 37 C, 70 min).
4. The five-phase protocol then runs to completion in roughly 127 min,
   without any further user intervention:
   - Phase 0: 37 C / 70 min -- RPA amplification.
   - Phase 1: 46 C / 1 min -- lower-threshold valve melts; RPA
     amplicon descends into the Cas12a compartment.
   - Phase 2: 37 C / 25 min -- Cas12a collateral cleavage on the
     ssDNA reporter.
   - Phase 3: 51 C / 1 min -- upper-threshold valve melts; reaction
     mixes with the detection reagent in the top compartment.
   - Phase 4: 37 C / 30 min -- final reporter incubation.
5. The firmware logs `# Protocol complete` and parks the heater in idle
   when the run finishes. Remove the cartridge and let it cool for ~2
   min before reading.

## Reading the result

- **Fluorescence mode.** Excite at 485 nm and read emission at 535 nm
  through the cap port. A handheld blue-LED transilluminator with an
  amber filter is sufficient for a qualitative readout.
- **Lateral-flow mode.** Peel the adhesive over the cap port and
  withdraw 30 microlitres of the top-compartment fluid with a clean
  pipette tip. Apply to the sample pad of the lateral-flow strip per
  the manufacturer's protocol. Read at 5 min: a visible test line
  indicates a positive result; the control line should always appear.

## Troubleshooting

- **No heating.** Check that the supply is on, that the MOSFET source is
  tied to common ground, and that the firmware is running. Press the
  start button (not the reset button).
- **Heater stuck on.** Cycle the supply. The 10 kohm pull-down on the
  MOSFET gate should drop PWM to zero whenever the Arduino is reset, but
  a missing pull-down or a shorted MOSFET will hold the heater on.
- **Negative result with a known positive sample.** Confirm that both
  wax valves actuated using the coloured-water tracing procedure in
  `validation_protocols.md`. A failed lower valve will keep the RPA
  amplicon out of the Cas12a compartment and produce a false negative.
- **Both wax disks melted before the run.** The cartridge was stored
  above 40 C. Discard and recast.
