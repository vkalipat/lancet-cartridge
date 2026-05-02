# Safety considerations

This protocol involves three distinct hazard classes. Work in a lab with
appropriate PPE and institutional approval, and treat the cartridge as a
biohazard whenever it has contained a clinical sample.

## Hot wax and hot surfaces

- Paraffin-mineral oil mixtures are dispensed at 45-70 C. Wear nitrile
  gloves and safety glasses while pipetting. Molten wax sticks to skin
  and continues to cool there; if splashed, run cold water over the
  affected area immediately rather than peeling.
- The heating pad reaches ~50 C in normal operation but can run hotter
  if the safety cutoff is bypassed (do not bypass it). Treat the pad
  as hot during and for ~5 min after a run. The cartridge cap is
  cool enough to handle.
- Use a dedicated hot plate or heat block. Do not melt wax on a kitchen
  stove or a microwave -- mineral oil flash points are around 200 C
  and uncontrolled heating is a fire risk.
- Keep flammables (ethanol, isopropanol, paper towels) away from the
  hot plate while wax is melting.

## Biological samples (whole blood, urine, synovial fluid)

- Treat all clinical samples as potentially infectious (CDC universal
  precautions). Wear nitrile gloves, lab coat, and safety glasses.
- Work in a Biosafety Level 2 cabinet for any sample containing live
  *Borrelia burgdorferi* (BSL-2 organism) or unknown clinical material.
  The bovine blood + cultured *B. burgdorferi* spike-ins reported in
  the paper were handled at BSL-2.
- The cartridge becomes a sealed, single-use biohazard once a sample
  is loaded. Dispose of used cartridges in a sharps or biohazard
  container per your institutional policy. Do not autoclave a used
  cartridge with reagents inside -- the polycarbonate tolerates
  autoclaving when empty, but heating a sealed tube with liquid will
  pressurise it.
- Pipette tips and lateral-flow strips that have contacted the sample
  go to biohazard waste. Wax disks from the same cartridge should be
  treated as biohazard if the cartridge has been opened post-run.
- Decontaminate the heating pad surface after each run with 70 %
  ethanol or 10 % bleach (rinse with water afterwards to avoid
  corroding the pad's silicone surface).

## Electrical

- The benchtop supply is rated 3-24 V DC, low current. Voltages in
  this range are not normally a shock hazard, but the heater can draw
  several amps -- enough to overheat undersized wires. Use at least
  22 AWG jumper wire on the heater leg.
- Confirm a common ground between the Arduino and the supply before
  power-up. Without it, the MOSFET gate-source voltage is undefined
  and the heater can latch on at full duty cycle.
- The 10 kohm pull-down on the MOSFET gate is a safety-critical part:
  it ensures the heater is off whenever the Arduino is unpowered or
  reset. Verify the pull-down before every run.
- Never run the controller unattended for the first time after a wiring
  change. Stay within line-of-sight for the first complete protocol run.

## 3D printing

- Polycarbonate prints at high nozzle (260 C) and bed (105 C)
  temperatures and produces some volatile emissions. Print in a
  ventilated room or an enclosure with a HEPA + activated-carbon
  filter.
- The 80 C anneal step uses a small oven. Do not use a kitchen oven --
  even after cleaning, the residue from polycarbonate annealing is
  not food-safe.

## Reagents

- LbCas12a, RPA enzyme mixes, and lateral-flow strips are not
  classified as hazardous in the quantities used here, but the
  manufacturer's safety data sheet should still be on file.
- Mineral oil and paraffin wax are non-hazardous lab consumables.
  Discard cooled residue with normal lab waste unless it has contacted
  a clinical sample (see above).

## Emergency

- For a wax burn: cool the area with water for at least 5 minutes
  before peeling.
- For a heater fire: cut the supply at the source first, then use a
  Class B (flammable liquid) extinguisher. Do not use water on an
  energised circuit.
- For a clinical-sample exposure: follow your institution's bloodborne
  pathogen exposure protocol (typically wash, report, document, and
  consult occupational health).
