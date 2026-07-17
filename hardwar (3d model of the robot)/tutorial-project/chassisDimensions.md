# Chassis engineering update

## Added in revision

1. Added editable hardware-fit parameters in `parameters.kcl` for motor reserves, battery reserve/access opening, driver reserves, M3 nut-relief diameter/depth, and corner-bracket geometry.
2. Added `removableCornerBracket.kcl` with four printable removable L-brackets. Brackets include M3 clearance holes and cylindrical nut-relief counterbores for captured hardware access.
3. Added four bracket instances to `main.kcl` for removable front/rear and side panel support.

## Hardware-fit tuning parameters

- Motor reserve: 46 x 36 mm
- Motor hole spacing: 32 x 24 mm
- Battery reserve: 70 x 72 mm
- Battery access opening: 42 x 52 mm
- Driver reserve: 36 x 22 mm
- M3 clearance: 3.4 mm
- Nut-relief diameter/depth: 6.6 / 2.4 mm

These are editable envelope values, not modeled electronic or motor geometry. Tune them against the selected purchased hardware before fabrication.

## Validation

- `main.kcl` mock execution: passed
- Corner-bracket constraint check: passed
- Lint: passed
- Individual printable parts remain separate; no glue-dependent assembly was introduced.
