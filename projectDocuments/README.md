# projectDocuments

Documentation for the FLL BIOGLOW sensor retrieval project (backup release for lake acoustic receivers). The firmware lives in `Main.ino` at the repo root.

| Path | What it is |
|---|---|
| `design-doc.md` | The full project design doc (goal, options, chosen design, PVC housing, piezo trigger, build log, tests, safety). Copied from the Claude project doc "Main", Nov 8, 2026. |
| `research/presentation-prep.md` | Resources, papers, Dr. Gerig's info and presentation talking points. |
| `diagrams/system-layout.svg` | At rest vs. after release. |
| `diagrams/electronics-wiring.svg` | DS3231, MOSFET, hold magnet, piezo bias. |
| `diagrams/pvc-housing-section.svg` | PVC housing cross-section. |
| `viewer.html` | Tabbed viewer for the diagrams. Rebuild with `python3 tools/build_viewer.py`. |

Not included yet: the older diagrams (LED and breadboard wiring, burn wire cross-sections, retrieval stack). They are in the original Claude chats.
