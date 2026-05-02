# Parametric source description -- `tube_assembly.f3d`

The `tube_assembly.f3d` file referenced in the paper's design-files
table (Section 4) is the editable Autodesk Fusion 360 parametric source
for the cartridge. The binary `.f3d` will ship in the `v0.1.0` release
tagged for Zenodo archiving. This Markdown file is the authoritative
geometry record until then.

If you need to reproduce the build today, you can either reconstruct the
parametric model from the parameters below or print the placeholder
`tube_body.stl` and `tube_cap.stl` once they are exported.

## File-level metadata

- License: CERN-OHL-W v2 (see `../LICENSE-HARDWARE`).
- Authoring software: Autodesk Fusion 360 (Personal Use license is
  sufficient).
- Native unit: millimetres.
- Lineage: derived from the open-source 1.5 mL laboratory tube model by
  Bobby Dyer (Thingiverse), modified to a three-compartment vertical
  stack with a press-fit cap.

## Components

The assembly contains two components:

1. **`tube_body`** -- the three-compartment polycarbonate tube.
2. **`tube_cap`** -- the press-fit cap with a 1.5 mm reagent-loading
   port.

Both components inherit from the same set of named parameters listed
below, so changing one parameter (e.g. `compartment_volume_uL`)
propagates to all derived dimensions.

## Named parameters

| Name                         | Default  | Notes                                  |
| ---------------------------- | -------- | -------------------------------------- |
| `outer_diameter`             | 12.0 mm  | OD of the body and cap                 |
| `wall_thickness`             | 1.5 mm   | Polycarbonate wall                     |
| `compartment_volume_uL`      | 200      | Used to compute compartment height     |
| `compartment_count`          | 3        | Bottom (RPA), middle (Cas12a), top (reporter) |
| `friction_fit_tolerance`     | 0.1 mm   | Diametric clearance at each seam       |
| `ptfe_wrap_thickness`        | 0.075 mm | One layer of PTFE thread tape          |
| `cap_height`                 | 6.0 mm   | Press-fit cap                          |
| `cap_loading_port_diameter`  | 1.5 mm   | Through-hole sealed with adhesive      |
| `total_assembled_height`     | ~30 mm   | Computed                               |

## Sketches

Three driving sketches:

1. **`sk_profile`** -- a single 2D revolve profile that defines the
   body's outer wall, the three compartment seats, and the
   friction-fit ridges at each seam. Each compartment is sized so
   that `pi * (outer_diameter/2 - wall_thickness)^2 *
   compartment_height = compartment_volume_uL`.
2. **`sk_seam`** -- a 2D detail at each compartment seam showing the
   0.1 mm radial clearance plus a 1 mm-tall ridge for the PTFE wrap.
3. **`sk_cap`** -- the cap profile with the centered loading-port
   bore.

## Features

- `revolve_body`: revolves `sk_profile` about the central axis to form
  the tube body.
- `extrude_seams`: extrudes the friction-fit ridges from `sk_seam`
  along the central axis at three positions.
- `revolve_cap`: revolves `sk_cap` to form the cap.
- `bore_loading_port`: cuts the 1.5 mm through-hole in the cap centre.
- `chamfer_lead_ins`: 0.5 mm x 30 deg chamfers at every press-fit
  lead-in to ease assembly without compromising the seal.

## Export procedure

To regenerate `tube_body.stl` and `tube_cap.stl` from this file:

1. Open `tube_assembly.f3d` in Fusion 360.
2. To export the body: in the browser, right-click the `tube_body`
   component and choose **Save As Mesh**. Use refinement **High** and
   units **mm**. Save as `tube_body.stl`.
3. To export the cap: repeat for the `tube_cap` component, saving as
   `tube_cap.stl`.
4. Both files should be ASCII STL for diffability under git, or binary
   STL for smaller archive size. Either format is accepted by every
   modern slicer.

## Variants

Two variants are anticipated for the v0.2 release:

- **`tube_assembly_optical.f3d`** -- the cap face is polished/clearcoat
  finished for higher optical transparency to support quantitative
  fluorescence reading.
- **`tube_assembly_4-compartment.f3d`** -- adds a fourth compartment for
  multiplexed assays, sized so the existing controller and wax
  formulations are reusable.

Both are out-of-scope for the present paper.
