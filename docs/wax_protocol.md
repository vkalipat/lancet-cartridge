# Wax-valve preparation, dispensing, and curing protocol

This protocol covers the two paraffin-mineral oil formulations that form
the LANCET cartridge's dual-threshold valves. The lower-threshold valve
(10:7 mass ratio, melts at 45 C) sits between the bottom (RPA) and middle
(Cas12a) compartments. The upper-threshold valve (10:8, melts at 50 C)
sits between the middle and top (reporter) compartments. Each valve is
dispensed as a 20-25 microlitre disk.

## Materials

- Paraffin wax (laboratory grade, melt point > 56 C). Bulk paraffin is
  fine; this is the same kind sold for histology embedding.
- Light mineral oil (USP-grade or molecular-biology grade).
- Hot plate or temperature-controlled water bath capable of holding 70 C.
- Analytical balance with 0.01 g resolution for the mass ratio.
- 200 microlitre micropipette with disposable polypropylene tips.
- Heat block, thermocycler block, or small oven that can hold a tip-warming
  rack at 45 C and 50 C.
- Loaded LANCET cartridge body (3D-printed polycarbonate tube; see
  `build_instructions.md`).
- Borosilicate vial or glass beaker for melting (do not use polystyrene).

## Mass ratios

| Valve              | Paraffin (g) | Mineral oil (g) | Ratio | Melt point |
| ------------------ | ------------ | --------------- | ----- | ---------- |
| Lower threshold    | 1.00         | 0.70            | 10:7  | 45 C       |
| Upper threshold    | 1.00         | 0.80            | 10:8  | 50 C       |

A 10:6 mix was tested and rejected: the solid-phase mass fraction exceeded
the threshold for clean actuation. Do not substitute.

Make at least 0.5-1.0 g of each formulation -- you will use only ~25 mg per
valve, but the residue clings to the vial walls and pipette tips, and you
want a homogeneous melt to draw from. Mixed wax can be re-melted and
re-used multiple times.

## Procedure

1. **Heat paraffin to 70 C.** Weigh paraffin into the glass vial, place
   on the hot plate, and bring to 70 C. Wait for the wax to fully
   liquefy and clear (no cloudy domains).
2. **Add mineral oil at the target mass ratio** (10:7 for the
   lower-threshold valve, 10:8 for the upper-threshold valve). Stir gently
   for at least 60 s with a glass rod or stir bar until the mixture is
   homogeneous and free of striations. Do not let the temperature drop
   below 65 C while you stir, or the paraffin will start to seed solid
   nuclei and the mix will be inhomogeneous.
3. **Pre-warm a pipette tip** in a heat block at the matching melt
   temperature: 45 C for the 10:7 formulation, 50 C for the 10:8
   formulation. A standard 200 microlitre tip warms in under 60 s. Do
   not over-warm -- a tip held at 70 C will let the wax flow out through
   the orifice before you press the plunger.
4. **Aspirate 20-25 microlitres** of the molten wax into the warmed tip.
   Work quickly: you have ~10 s before the wax in a 45-50 C tip starts
   to cloud.
5. **Dispense into the cartridge.** With the cartridge body upright and
   the lower compartment already loaded with its dehydrated reagent
   pellet (see `operation.md`), eject the wax in a single smooth push so
   it forms a clean disk on top of the pellet. Avoid dispensing against
   the tube wall.
6. **Cool to room temperature for at least 5 min** before adding the next
   reagent layer. The disk should look opaque and feel firm to a gentle
   probe with a clean pipette tip.
7. **Repeat steps 3-6** for the upper-threshold valve once the middle
   compartment is loaded.

## Quality checks

- The cured disk should be a uniform opaque white. Translucent or oily
  domains indicate undermixed wax; remelt the formulation and re-stir.
- Tilt the assembled cartridge to ~90 degrees: the wax disk should not
  shear off the polycarbonate wall. If it does, the friction-fit
  surface was contaminated -- clean with isopropanol, refresh the PTFE
  wrap, and re-cast.
- The lower-threshold valve must hold at 37-39 C for at least 70 min
  (the RPA incubation). Validate this with the coloured-water tracing
  protocol in `validation_protocols.md` for any new wax lot.

## Storage

Cast cartridges can be stored at 4 C in a sealed bag with desiccant.
Empirical room-temperature shelf life beyond ~3 months has not been
measured. Do not store above 30 C: even brief excursions to 40 C will
soften the lower-threshold valve.

## Disposal

Cooled wax/oil residue and used pipette tips are non-hazardous and can be
discarded as ordinary lab waste, unless they have been in contact with
clinical samples (see `safety.md`).
