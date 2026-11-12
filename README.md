# LURE: Lake Underwater Release Engine

FLL BIOGLOW innovation project: LURE is a low-cost backup release that brings acoustic receivers up from the bottom of a lake (about 40 ft). It does not replace the scientist's drag line grapple. A marker float carries a haul rope to the surface and a person in a boat hauls up the rest.

## Firmware

`Main.ino` runs on an Arduino Nano (Pro Mini for the sealed build). It sleeps on a DS3231 alarm, wakes, listens on a piezo disc for a 1000 Hz tone using a Goertzel filter, and fires a MOSFET that releases the hold magnet. Then it sleeps again. The git history has every earlier version of the sketch, oldest first.

## Docs

| Path | What it is |
|---|---|
| `design-doc.md` | Full design doc: options, chosen design, PVC housing, piezo trigger, build log, tests, safety |
| `research/presentation-prep.md` | Resources, Dr. Gerig's info, presentation talking points |
| `diagrams/` | System layout, electronics wiring, PVC housing section (SVG) |
| `viewer.html` | Tabbed viewer for the diagrams. Rebuild with `python3 tools/build_viewer.py` |
| `tools/` | `build_viewer.py` |

The older diagrams (LED and breadboard wiring, burn wire, retrieval stack) are still only in the original Claude chats.
