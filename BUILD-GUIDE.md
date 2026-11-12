# LURE build guide (sealed build)

LURE (Lake Underwater Release Engine) is a low-cost backup release for acoustic receivers on a lake bottom. It sleeps for months, wakes on a set date, listens for one coded tone, and when it hears the tone it powers a hold magnet that lets go of a marker float. The float carries a haul rope to the surface and someone in a boat hauls the receiver up.

This guide walks through building one, testing it on a bench, and getting it ready to deploy. It was written by an FLL team (Coding Curators) and is a work in progress. Steps we have not tested yet are marked **UNTESTED** or **TBD** so you know what to check yourself.

> **LURE is a backup, not a replacement.** Keep using your normal retrieval method (for example a drag line grapple). If the battery or board dies, the magnet never lets go.

## How it works

1. You set a wake date and power the unit on. This is "arming."
2. Everything sleeps. A DS3231 clock keeps time and the Arduino draws almost nothing.
3. On the wake date the clock wakes the Arduino.
4. The Arduino listens through a piezo disc for a 1000 Hz tone (the tone is adjustable). It only counts the tone if it is loud enough and pure enough, 8 windows in a row (about 0.2 seconds).
5. When it hears the tone, it switches a MOSFET on for 3 seconds. That powers the hold magnet, which lets go of the float.
6. The float rises. The unit goes back to sleep.

The design doc ([design-doc.md](design-doc.md)) has the reasoning, what we tried before (a burn wire), and the open questions.

## Status: what is and is not proven

| Part | Status |
|---|---|
| Sleep, wake on clock alarm, Goertzel tone detection (Arduino Nano, desk) | Works on the bench |
| Piezo bias circuit, strong signal when the disc touches a speaker | Works on the bench |
| Piezo hearing a tone through air | Does not work (expected, it is a contact sensor) |
| Piezo hearing a tone through water or through the PVC end cap | UNTESTED |
| APW 32 mm hold magnet release on the battery pack | UNTESTED (magnet voltage and release current TBD) |
| PVC housing at 40 ft for a week | UNTESTED |
| Pro Mini (3.3V) version of the wiring and idle current | UNTESTED |
| Playing the tone through the lab's existing transducer, range, and tone coding | TBD |

## Parts

Search terms only, prices not checked. Buy a spare of the small parts.

| Part | Notes |
|---|---|
| Arduino Pro Mini, 3.3V / 8 MHz, ATmega328P | The sealed build uses this. Needs an FTDI USB-to-serial adapter (3.3V) to upload. An Arduino Nano works for bench testing. |
| DS3231 real-time clock module | Use the coin cell type its listing recommends. |
| IRLZ44N logic-level MOSFET | Not an IRF520 module. |
| APW 32 mm hold magnet (electro-permanent) | Holds with no power, lets go when powered. About 40 lb holding, about 1.2 A listed. **Check the rated voltage on the listing before wiring.** |
| Steel plate for the magnet to grab | Ours is 1 mm 304 stainless sheet for bucket tests. Match the plate to the magnet's listing. |
| Flyback diode (1N4007 or similar) | Goes across the magnet. |
| 35 mm piezo disc | A pack of 10 is cheap. |
| Resistors: two 100k, one 220 ohm, one 10k | |
| 0.1 uF (104) ceramic capacitor | |
| 4 AA battery holder + lithium AA cells | Lithium holds up better in cold water than alkaline. |
| 3 to 5 A fuse and holder | In line with the battery. |
| 3 inch Schedule 40 PVC pipe, one cap to cement on, one scrap cap for tests | See the housing step. |
| Threaded clean-out adapter with plug and O-ring | The opening end. Do not use an expanding gripper plug. |
| PVC primer and cement | |
| Marker float (about 1 to 2 L, bright color) | |
| 2 mm cord for the float tether | |
| Braided Dyneema or polyester haul rope, 4 to 6 mm, 500 lb or more | Length = depth x 1.25 + 10 ft. |
| Rope canister (PVC tube or food container) | See [design-doc.md](design-doc.md), ROPE STORAGE. |
| Wire, heat shrink, epoxy, breadboard, jumpers | |

Tools: multimeter, kitchen or luggage scale (for holding force), soldering iron, phone with a tone generator app, plastic tub or bucket, zip bag.

## Step 1: Set up the Arduino

1. Install the Arduino IDE: https://www.arduino.cc/en/software
2. Install the clock library. In the IDE open Tools, Manage Libraries, search for **RTClib** by Adafruit, and install it. (Source: https://github.com/adafruit/RTClib)
3. Pick the board. Tools, Board, **Arduino Pro or Pro Mini**. Tools, Processor, **ATmega328P (3.3V, 8 MHz)**. For a bench Nano, pick Arduino Nano and the 16 MHz processor instead.
4. Connect the FTDI adapter to the Pro Mini (SparkFun has a clear walkthrough for the Pro Mini 3.3V: https://learn.sparkfun.com/tutorials/using-the-arduino-pro-mini-33v). Make sure the adapter is set to 3.3V. Plugging 5V into a 3.3V board can damage it.
5. Open `Main.ino` from this repo and upload it. Open Tools, Serial Monitor at 9600 baud. You should see "Now", "Wakes" and "Sleeping...".

## Step 2: Wire it on a breadboard first

Build the whole circuit on a breadboard and test it before anything goes in a pipe. The diagrams are in `diagrams/` (open `viewer.html`). Pin by pin:

| From | To |
|---|---|
| DS3231 VCC | Arduino VCC (3.3V) |
| DS3231 GND | Arduino GND |
| DS3231 SDA | Arduino A4 |
| DS3231 SCL | Arduino A5 |
| DS3231 SQW (alarm) | Arduino D2 |
| Arduino D9 | 220 ohm resistor, then MOSFET gate |
| MOSFET gate | 10k resistor to GND |
| MOSFET source | GND |
| MOSFET drain | magnet, negative lead |
| Magnet, positive lead | battery positive (through the fuse) |
| Flyback diode | across the magnet, stripe (cathode) toward battery positive |
| Battery positive (through fuse) | Arduino RAW |
| Battery negative | GND (all grounds joined) |

**Piezo listener:**

1. Two 100k resistors in series from VCC to GND. The middle point is the bias point.
2. Middle point to Arduino **A0**.
3. Middle point through the 0.1 uF capacitor to the piezo's **ceramic face** (the white or yellow side).
4. Piezo's **brass side** to GND.
5. With a multimeter, check A0 reads about **half of VCC** at rest (about 1.65 V on 3.3V, 2.5 V on a 5V Nano). Do not go further until it does.

Notes:

- The magnet is fed from the battery, not from the Arduino. The Arduino only switches it through the MOSFET.
- 4 AA lithium cells are about 6 V. Confirm the magnet's rated voltage against that. If it is rated for 12 V or 24 V it may not release on 6 V. **TBD until the magnet is measured.**
- The sketch does not switch the piezo bias on and off. The divider draws about 16 uA on 3.3V all the time. Powering it from a spare pin only while listening is a planned improvement and **is not in the code yet**.
- The IRLZ44N is specified for logic-level drive, but we have not tested it at 3.3 V gate voltage with the real magnet. **UNTESTED.** Measure the current through the magnet when you first fire it.

## Step 3: Edit the settings

At the top of `Main.ino` is a block between `===== EDIT THESE =====` markers:

| Setting | Default | What it does |
|---|---|---|
| `SLEEP_SEC` | 30 | Seconds to sleep before listening. Use 30 for bench tests. For deployment see Step 6. |
| `TARGET_HZ` | 1000 | The tone to listen for. |
| `MIN_RMS` | 3.0 | Ignores near-silence. Raising it makes the unit less sensitive. |
| `MIN_RATIO` | 0.5 | How pure the tone must be (0 to 1). Raise to 0.6 or 0.7 if noise triggers it. |
| `HITS_NEEDED` | 8 | Windows in a row (about 0.2 s). |
| `ON_MS` | 3000 | How long the magnet is powered, in milliseconds. Shorten once you know the minimum that works. |

Do not lower `MIN_RMS` or `MIN_RATIO` to chase a weak signal. That moves the cutoff toward the noise and invites false triggers. Improve the signal instead (see the troubleshooting table).

Two things about the current sketch to know:

- After the unit wakes, it listens **until it hears the tone, with no time limit**. A Pro Mini that is awake and sampling uses far more current than one that is asleep, so a unit that wakes and never hears the tone will run its batteries down. A listening window with a timeout and a return to sleep is **TBD** (see the design doc, PIEZO OPEN DESIGN QUESTIONS).
- After the magnet fires, the sketch goes back to sleeping for `SLEEP_SEC` again and repeats the cycle. For a one-shot release you may want it to sleep forever after firing. **Not changed yet.**

## Step 4: Bench test everything dry

Do these in order. Keep the magnet's wires disconnected for the first two tests, and use an LED with a resistor in place of the magnet if you want to see the output.

1. **Sleep and wake.** Set `SLEEP_SEC` to 30, upload, and watch the Serial Monitor. It should print "Sleeping...", wait 30 seconds, then print "Awake." and "Listening for tone."
2. **Quiet room.** While listening, the monitor prints `rms` and `ratio` each window. In a quiet room, rms should be about 0.5 and ratio low.
3. **Tone test.** Press the piezo flat against a phone speaker playing a 1000 Hz tone. rms and ratio should jump. After 8 good windows in a row you should see "TONE DETECTED" and the output go high for 3 seconds. A bare disc held across a table will barely pick anything up. That is normal.
4. **False trigger test.** Clap, talk, tap the table, play 600 Hz and 1500 Hz tones. It must never print "TONE DETECTED."
5. **Magnet test.** Connect the magnet and plate with the fuse in line. Use a multimeter to read the current when it releases. Measure the real holding force with a scale and note the gap you used. Test the shortest `ON_MS` that reliably lets go.
6. **Cold and long hold test (recommended).** Leave the magnet latched with a weight in a bucket of cold water for weeks and check it still holds and still releases.

## Step 5: Tune the tone for the piezo

Piezo discs have a resonance peak, often around 3 to 5 kHz. Before picking your final tone:

1. Glue the disc flat to the **inside** of a scrap PVC end cap and set it in a tub of water.
2. Play tones from about 500 Hz to 5 kHz from a phone speaker in a zip bag in the water, and note where `rms` peaks.
3. Pick a final tone near the peak and set `TARGET_HZ`.

Above 4 kHz the sketch's 8 kHz sampling is too slow. You would need to speed up the ADC sampling (about 16 kHz) and change `SAMPLE_RATE` and `SAMPLE_US` to match. **That change is not made in the sketch yet.**

## Step 6: Arm it

The sketch starts its timer when it powers up: it sleeps for `SLEEP_SEC` seconds from boot. So arming is:

1. Decide how many days until the wake date. Compute `SLEEP_SEC = days x 86,400`. Example: 120 days = 10,368,000. Use a number with `UL` after it if the compiler complains, like `10368000UL`.
2. Edit `SLEEP_SEC`, upload, and watch the Serial Monitor to confirm the "Wakes:" date is the one you meant.
3. Power the unit from the battery pack (not USB), and check you still see "Sleeping..." on a last USB look if you can.
4. Seal it up (Step 7).

If you reset the board, the clock starts over from that moment. Re-arm any time the unit is reset or the batteries are changed. The DS3231's own coin cell keeps the time if the main battery drops out, but the countdown in the sketch restarts at boot. A wake date that survives resets is **TBD**.

Plan for idle current under about 100 microamps average, or the batteries will not last months. The Pro Mini's power LED and onboard regulator use more than the sleeping chip does, and the DS3231 module has its own LED. Remove the LEDs and measure the sleep current with a multimeter in series with the battery. **UNTESTED, measure before trusting it.**

## Step 7: Housing (PVC, plan for now)

This is the plan and it is **not trusted until the dry run passes.** Full details are in the design doc under PVC HOUSING PLAN.

1. Use 3 inch Schedule 40 PVC (4 inch if the parts do not fit). Check the fit with the real parts first: magnet, Pro Mini, DS3231, 4 AA holder.
2. Cement a cap on one end with primer and cement. Use gloves and ventilation. The other end gets the threaded clean-out with an O-ring, so you can arm it and change batteries.
3. Glue the piezo flat to the **inside** of the cemented end cap. Mount the magnet against the inside of the same cap, so the pull and the sound each cross only one wall. Put the steel plate on the float side, outside the cap.
4. Measure the magnet's hold force through a scrap piece of the real cap. If the force drops too much, thin the cap from the outside with a flat recess.
5. Repeat the piezo test with the disc glued inside the scrap cap, and compare it to the bare disc.
6. **Dry run:** seal the empty housing with paper towels inside and sink it at 40 ft for at least a week, on a rope. Check for water and for any wall deformation. Keep people clear when you raise and open it. Ideally also test cold.
7. A sealed housing full of air floats. Tie it to the anchor with a rope or strap, not zip ties.

If the pipe fails the dry run or kills the magnet or piezo signal, the backup is an OtterBox-style dry box. Repeat the same tests.

## Step 8: Float, rope and anchor

- The float only marks the spot and carries the haul rope to the surface. It does not lift the anchor. A person hauls the rest by hand.
- Use two lines: a light tether (2 mm cord) from the float to the magnet latch, and a strong haul rope (braided, 4 to 6 mm, 500 lb or more) that carries the full pull.
- Rope length is depth x 1.25 + 10 ft. For 40 ft that is 60 ft.
- Store the rope flaked in a canister with the float as the lid. Load the receiver end first (bottom) and the float end last (top). Run a tangle test before deploying.

## Step 9: Deploy and retrieve

1. Do a land test pulse before every deployment: check the battery and that the magnet fires and lets go.
2. Check the wake date on the Serial Monitor, seal the housing, tie it to the anchor, and deploy.
3. On or after the wake date, play the tone through the transducer your team already has on the boat. The unit releases and the float comes up. **The range and tone coding are TBD, and we have not yet checked that the transducer can play our tone.** Sound from a speaker in the air mostly bounces off the water, so it needs to be an underwater transducer. Check what frequencies and signal it can send.
4. Follow the float to the surface and hand-haul the rope.
5. If the float never appears, use your normal retrieval method.

## Troubleshooting

| Problem | Try this |
|---|---|
| "RTC not found" in the monitor | Check SDA to A4 and SCL to A5. Check clock power and ground. |
| Never wakes up | Check SQW goes to D2. Make sure the alarm wiring is intact and the coin cell is in. |
| A0 does not read about half of VCC | Check the two 100k resistors and the connection to the middle point. |
| rms always about 0.5 | Normal in a quiet room. Check that the piezo is touching the source. A bare disc does not hear through air. |
| Signal too weak through the wall or in water | Glue the disc flat against the cap, find the resonance peak, and if still weak add a small op-amp stage (about 10 to 20x gain) between the piezo and A0. |
| Random "TONE DETECTED" | Raise `MIN_RATIO` to 0.6 or 0.7 or `MIN_RMS`. Re-run the false trigger test. |
| Output fires but the magnet does not let go | Check the magnet's rated voltage against the battery. Check battery sag under load, and cold. Check the flyback diode direction. |
| Magnet will not hold the float | Check the gap between plate and magnet and the thickness of the cap. Hold force drops fast with distance. |
| Batteries die early | Measure sleep current. Remove LEDs. Check the unit is not stuck awake listening. |

## Safety

- Adults handle all powered tests.
- Strong magnets can pinch fingers badly and can interfere with phones, cards and medical devices such as pacemakers. Slide magnets apart instead of pulling them.
- Keep the Arduino and piezo electronics dry during tub tests. Only the sealed disc or a bagged speaker goes in the water.
- Always use a fuse on the battery pack. Batteries can overheat on a short.
- A housing that fails under pressure can implode. Run the dry run on a rope and keep people clear while raising and opening it.
- PVC primer and cement need ventilation and gloves.

## Links

- Arduino IDE: https://www.arduino.cc/en/software
- RTClib (Adafruit): https://github.com/adafruit/RTClib
- SparkFun, Using the Arduino Pro Mini 3.3V: https://learn.sparkfun.com/tutorials/using-the-arduino-pro-mini-33v
- Low power Arduino sleep notes (Nick Gammon): https://www.gammon.com.au/power
- Cave Pearl Project, PVC housings for underwater Arduino loggers: https://formufit.com/blogs/news/115424708-water-tight-housing
- Schedule 40 PVC collapse pressure table: https://www.professionalplastics.com/professionalplastics/content/downloads/ClearPVC-Schedule40.pdf
- More links and sources: [design-doc.md](design-doc.md), REFERENCES
