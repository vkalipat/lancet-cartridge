# CAD files

3D-printable geometry for the LANCET cartridge. All files in this folder
are released under the CERN Open Hardware Licence v2 -- Weakly Reciprocal
(see `../LICENSE-HARDWARE`).

## Inventory

| File                            | Format     | Software             | Status        |
| ------------------------------- | ---------- | -------------------- | ------------- |
| `tube_body.stl`                 | ASCII STL  | Any slicer           | **Placeholder** -- see below |
| `tube_cap.stl`                  | ASCII STL  | Any slicer           | **Placeholder** -- see below |
| `tube_assembly.f3d.notes.md`    | Markdown   | Description of the parametric source for the binary `.f3d` (Fusion 360) file that ships with tagged releases | Authoritative for now |

## What "placeholder" means

The `.stl` files in this folder are **comment-only stub files** that
describe the part dimensions and tolerances in plain text. They do not
contain printable mesh data. The binary `.f3d` parametric source file
referenced in the paper's design-files table is similarly pending; until
it is added in a tagged release the parametric description in
`tube_assembly.f3d.notes.md` is the authoritative geometry record.

The actual printable meshes will be exported from the Fusion 360 source
and committed in the `v0.1.0` release that triggers the Zenodo archive.

## What software opens each file

- `.stl` (binary or ASCII): any FDM slicer -- PrusaSlicer, Cura,
  Simplify3D, Bambu Studio, OrcaSlicer.
- `.f3d`: Autodesk Fusion 360. The free Personal Use license is
  sufficient. STEP and IGES exports will accompany future binary
  releases for users on Onshape, FreeCAD, SolidWorks, or Inventor.

## Print parameters (paper Section 5.1)

- Material: clear polycarbonate filament.
- Layer height: 0.1 mm.
- Infill: 100 %.
- Nozzle temperature: 260 C.
- Bed temperature: 105 C.
- Bed adhesion: Magigoo PC or equivalent polycarbonate adhesive.
- Anneal: 80 C for 30 min after printing.
- PTFE wrap: one layer (~0.075 mm) on each interior friction-fit
  surface.

Friction-fit tolerance is 0.1 mm. Each compartment holds approximately
200 microlitres.
