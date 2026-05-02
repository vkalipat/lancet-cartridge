# Validation data

This directory ships the **set-point profile** for the five-phase
thermal protocol as a CSV. Empirical run data (DS18B20 traces,
reference-probe traces, coloured-water tracing photographs) will be
appended here as it accumulates.

## Inventory

| File                                | Description                                                    |
| ----------------------------------- | -------------------------------------------------------------- |
| `thermal_protocol_set_points.csv`   | Idealised time-vs-temperature profile from the firmware. Five rows, one per phase. |

## CSV schema -- `thermal_protocol_set_points.csv`

| Column            | Type    | Units    | Description                                              |
| ----------------- | ------- | -------- | -------------------------------------------------------- |
| `phase_idx`       | int     | -        | Zero-based phase index (0..4)                            |
| `phase_name`      | string  | -        | Matches the phase name printed by the firmware           |
| `start_time_min`  | int     | min      | Elapsed time at the start of the phase                   |
| `end_time_min`    | int     | min      | Elapsed time at the end of the phase                     |
| `duration_min`    | int     | min      | Phase duration (= end - start)                           |
| `set_point_c`     | float   | C        | Target temperature for the phase                         |
| `description`     | string  | -        | Human-readable purpose of the phase                      |

The profile in the CSV is the same array that is compiled into the
firmware's `PROTOCOL[]` table. If you change one, change the other --
otherwise the documentation and the running firmware will diverge.

## Empirical data conventions

When archiving a run, save the firmware's serial output verbatim as a
CSV with the header:

```
millis,state,phase_idx,phase_name,set_c,measured_c,pwm
```

Use a filename of the form
`run_<YYYYMMDD>_<HHMMSS>_<short-tag>.csv`, where `<short-tag>` is a
brief slug describing the build (e.g. `controller_v1`, `dscalib`). For
example:

```
run_20260502_141500_controller_v1.csv
```

Reference-probe traces logged from an external thermocouple should sit
alongside the firmware CSV with the same timestamp slug and a
`_reference.csv` suffix:

```
run_20260502_141500_controller_v1_reference.csv
```

Coloured-water tracing time-lapses can be archived as MP4 in this
directory or, more practically, linked from a Zenodo deposit since
binary video does not play well with git. A simple
`runs/<timestamp>/` subdirectory containing both CSVs and a README is
the recommended structure for community contributions.
