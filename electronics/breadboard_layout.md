# Breadboard layout

Step-by-step solderless build of the LANCET thermal controller using a
single 170-tie-point breadboard. Refer to `schematic.md` for the
canonical net list. Total time: ~20 min for an experienced builder.

The breadboard rows are numbered 1-17 (top to bottom) and the columns
are A-E (left half) and F-J (right half), with two power rails on each
side labelled `+` and `-`. The build below uses only the left half plus
the rails.

## Step 1 -- Power rails

1. Run a jumper from the Arduino `+5V` pin to the breadboard's left-side
   `+` rail.
2. Run a jumper from the Arduino `GND` pin to the breadboard's left-side
   `-` rail. Add a second jumper between the two `-` rails so both
   ground rails are tied.
3. Connect the benchtop supply ground to the same `-` rail. **The
   common ground is mandatory** -- without it the MOSFET will not
   switch cleanly.
4. Bring the benchtop supply positive lead to a free row near the
   bottom of the board (we use row 15, column A) for later connection
   to the heating pad. Do not energise the supply yet.

## Step 2 -- MOSFET (IRLB8721)

1. Insert the IRLB8721 with the metal tab facing away from you so that
   the printed label is readable. From left to right the pins are
   gate, drain, source.
2. Place the gate pin in row 5 column A, drain in row 6 column A,
   source in row 7 column A.
3. Run a jumper from row 7 column E to the `-` rail (MOSFET source to
   ground).

## Step 3 -- Gate pull-down (R1, 10 kohm)

1. Insert R1 between row 5 column B (gate) and the `-` rail.
2. Tug both leads to confirm a firm seat. This resistor is
   safety-critical -- it is the reason the heater is off whenever the
   Arduino is unpowered.

## Step 4 -- Gate drive

1. Run a jumper from Arduino pin D9 to row 5 column C. This carries the
   PWM gate drive `HTR_PWM`.

## Step 5 -- Heating-pad wiring

1. Connect the heating-pad negative lead to row 6 column E (MOSFET
   drain).
2. Connect the heating-pad positive lead to row 15 column A (benchtop
   supply +).
3. Verify that the heating pad is not in contact with any conductive
   surface other than its two leads.

## Step 6 -- DS18B20 probe

1. Run a jumper from the breadboard's `+` rail to row 10 column A
   (DS18B20 +5 V).
2. Run a jumper from the `-` rail to row 11 column A (DS18B20 ground).
3. Run a jumper from Arduino pin D7 to row 12 column A (DS18B20 data).
4. Insert R2 (4.7 kohm) between row 12 column B (data) and the `+`
   rail. This is the 1-Wire pull-up.
5. Strip the DS18B20 leads if not pre-tinned and insert: red into row
   10, black into row 11, yellow into row 12. Use ferrules or short
   lengths of solid-core wire to extend if your tinned leads are too
   short to stay seated.

## Step 7 -- Start button

1. Insert a 6 mm tactile pushbutton spanning the centre channel between
   rows 14 and 15 in columns C and F (or any equivalent symmetric
   position). The button bridges the two pins on the left and right
   when pressed.
2. Run a jumper from Arduino pin D2 to row 14 column B.
3. Run a jumper from row 15 column B to the `-` rail.
4. The firmware enables `INPUT_PULLUP` on D2, so no external pull-up is
   required.

## Step 8 -- Final checks before powering up

1. Visual inspection: confirm that no jumper is shorting the `+` and
   `-` rails. A continuity meter on the rails should read open with the
   Arduino unplugged.
2. Confirm R1 (10 kohm) is between the MOSFET gate row and the `-`
   rail (gate-to-ground, not gate-to-+5V).
3. Confirm R2 (4.7 kohm) is between the DS18B20 data row and the `+`
   rail (data-to-+5V, not data-to-ground).
4. Confirm the IRLB8721 is oriented with the metal tab away from you;
   reversing it puts the body diode in the wrong direction and will
   short the heater supply through the MOSFET when powered.

## Step 9 -- First power-up

1. Connect the Arduino to your computer over USB. The Arduino's
   built-in 5 V regulator powers the breadboard's `+` rail.
2. Open the serial monitor at 115200 baud. You should see the
   firmware boot banner followed by 1 Hz CSV log lines with `IDLE` in
   the state column and a sensible room-temperature reading in the
   `measured_c` column (typically 22-26 C).
3. **Only after the serial log looks correct**, energise the benchtop
   supply at 10 V.
4. Press the start button. The state column should switch to
   `RUNNING`, the heater PWM column should rise, and the DS18B20
   reading should climb toward 37 C over the next ~6 min.

If anything else happens, kill the benchtop supply first, then unplug
the USB. Diagnose with the supply off.
