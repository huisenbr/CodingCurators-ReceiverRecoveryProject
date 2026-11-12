# LURE (Lake Underwater Release Engine)

FLL innovation project: sensor retrieval for lake acoustic receivers

Updated: Nov 8, 2026. Copied from the Claude project doc "Main" in the Lego project.

## GOAL
Design and prototype a low-cost, timed release that brings acoustic receivers up from the lake bottom. It is a backup to the scientist's existing drag line grapple, not a replacement.

## BACKGROUND
- The team talked to a scientist who tracks sturgeon with acoustic receivers.
- The units sit on the bottom of a lake (not a river) for months. Retrieval is hard because of depth, ice and boat traffic at the surface, and mussel fouling or silt burial.
- Today the scientist recovers units with a drag line grapple.
- Release-type receivers run roughly 14 to 26 months on battery (Innovasea VR2AR datasheet).
- Commercial acoustic releases exist (Innovasea Ascent, VR2AR). Pricing is quote-only. One archived 2021 government quote was $4,630 per unit. The exact model it covered is not confirmed, so treat it as a ballpark.
- Our angle: a much cheaper release.

## OPTIONS CONSIDERED
1. Sugar tablet: cheap and good for hands-on testing, but dissolve time drifts by weeks with temperature, flow and bacteria.
2. Mechanical clockwork: no battery, but drifts by weeks over months and is hard to build. The gear train alone does not set total duration; the regulator does.
3. Electronic timer with a release: near-exact clock and cheap parts, but needs a battery and waterproofing. THIS IS THE PICK. The release part changed (see below).
   - a. Burn wire (nichrome around a nylon line): original plan. PAUSED, because we could not get it to work consistently in water.
   - b. Hold magnet: CURRENT PLAN (decided Nov 1). It holds with no power and lets go when it is powered.
4. Acoustic release: you choose the day, but it must listen constantly, so it needs a battery, and commercial ones are expensive to buy. UPDATE Nov 8: we are now building our own low-cost version, using a piezo disc as the listener (see PIEZO TONE TRIGGER below).
5. Housing for the electronics (added Nov 8). It has to sit at about 40 ft (about 17 psi of water pressure) for 4 to 6 months, stay sealed, and let the magnet and the piezo work through its wall. DECISION (Nov 8): for now we plan on PVC pipe. Not final until it passes the dry run and the through-the-wall tests.
   - a. Nalgene bottle: RULED OUT. Some of the team found articles saying these can start to crush around 40 ft. It is not made for outside pressure, and the lid threads and gasket are the likely leak point. (Add the article links here.)
   - b. Pelican-style case: RULED OUT. These are rated IP67, which is 1 meter for 30 minutes (Pelican 1150 spec). Months at 40 ft is far outside that. A big flat lid also carries a lot of total force (rough estimate: about 1,000 lb on a 9 by 7 inch lid at 17 psi). They also have a pressure equalizing valve, which is the wrong feature for a unit that has to stay sealed.
   - c. PVC pipe (Schedule 40) with one cemented cap and one O-ring end: CURRENT PLAN (for now).
     - The pipe wall is very strong against crushing compared to 17 psi. A manufacturer collapse table for Schedule 40 PVC lists 327 psi for 2 inch, 279 psi for 3 inch, 169 psi for 4 inch and 84 psi for 6 inch (the table is for clear PVC; white should be in the same ballpark). Bigger pipe collapses at lower pressure.
     - The pressure rating printed on pipe is for internal pressure only. There is no matching external rating, so use the collapse table and test.
     - The weak spot is the end seals, not the wall. The open-source Cave Pearl Project used 2 inch Schedule 40 PVC with screw caps for underwater Arduino loggers running about a year.
     - Avoid expanding gripper (test) plugs. A ScubaBoard thread reports they popped out at 10 to 15 psi and can leak through the center bolt. Use a threaded clean-out or flange with an O-ring, and glue the other end permanently.
     - Scuba forum anecdotes (not tested by us): a capped 3 inch Schedule 40 pipe reportedly did not implode at 484 ft, and 2 inch pipe sealed with ABS cement was used at 90 to 130 ft for about a year. One poster warns that plastics are temperature sensitive and cold-water failures can be sudden.
     - Concerns for our design: the magnet's pull has to cross the end cap (thicker than a bottle wall, and holding force drops fast with distance), and the piezo signal has to cross it too. Thin the cap from the outside with a flat recess if needed.
     - Size: the APW 32 mm magnet, Pro Mini, DS3231 and 4 AA holder probably need 3 inch pipe or larger. Check the fit with the real parts.
   - d. OtterBox-style dry box: BACKUP OPTION if PVC does not work out.
     - Ratings are deeper than Pelican: the 3250 series is IP68, immersed up to 90 feet for 30 minutes, and older series are listed to 100 feet.
     - A ScubaBoard commenter used an OtterBox rated to 100 ft as a dive container, nearly neutral with about 2 lb of weight, with no problems reported (recreational dives, so hours, not months). Another forum user said a couple of OtterBoxes they tested were water resistant rather than fully waterproof.
     - Pros: flat thin walls help the magnet and piezo, and the lid is easy to open. Cons: the ratings come from 30 minute tests, one gasket under constant pressure in cold water for months, and a flat lid can flex.
   - e. Big Blue water filter housing: NOT PLANNED, ranked lowest. Thin research, no one found using it underwater.
     - Pros: a threaded sump with a top-seated Buna-N O-ring and wrench, lots of room, and cheap (10 inch from about $52).
     - Cons: the ratings are for internal pressure only (100 psi for the 10 inch, 90 psi for the 20 inch) and we found no external or collapse rating. Rough estimate: a sealed 10 inch housing displaces about 9 liters, which is about 20 lb of lift against about 2 lb for a 3 by 8 inch PVC tube, while the float only pulls 2 to 4 lb. The cap has threaded ports that all need plugging (avoid the pressure relief button and drain versions). The temperature rating is 40 to 100 F and the data sheet warns against freezing, and a lake bottom is near 39 F. Wall thickness for the magnet and piezo is unknown.
   - For any housing:
     - Do a dry run before the electronics go in: sink the sealed empty housing at 40 ft with paper towels inside for at least a week, then check for water and for any wall deformation. Ideally also test cold.
     - A sealed housing full of air floats. Anchor it firmly to the cinder blocks (not zip ties) and check that the anchor still holds.
     - Measure magnet hold force and piezo signal through the actual wall or cap thickness.

## CHOSEN DESIGN (updated Nov 1: hold magnet replaces burn wire)
Battery, DS3231 real-time clock, Arduino, logic-level MOSFET, and a hold magnet that keeps the float latched with no power until the tone arrives after the wake date.
1. Arm with a push button (reed switch and magnet in the sealed version) and set the alarm date.
2. Sleep for months while the clock keeps time. The magnet needs no power while it holds, so the battery only runs the clock.
3. On the set date the clock wakes the board, which listens for the coded tone from the boat. When it hears the tone, the MOSFET powers the hold magnet, which lets go of the float.
4. The float rises and takes the haul rope to the surface. The board then switches itself off so it cannot drain the battery.

Prototype on an Arduino Nano. The sealed version moves to a 3.3V Pro Mini with lithium AA cells.
Why the change: the burn wire would not part the line reliably in water. A magnet has no heating step, so cold water should not matter the same way.
Nov 8 add-on: a piezo tone listener can also trigger the MOSFET. See the next sections.

## PVC HOUSING PLAN (for now, decided Nov 8)
- Start with 3 inch Schedule 40 PVC, a cemented cap on one end, and a threaded clean-out with an O-ring (not a gripper plug) on the other. Fall back to 4 inch if the parts do not fit. 2 inch is probably too small for the 4 AA holder and the 32 mm magnet.
- Where things go: the magnet and piezo sit against the inside of the cemented end cap, so the signal and the pull only cross one wall. The electronics and batteries sit behind them, and the O-ring end opens for arming and battery changes.
- Steps before we trust it:
  1. Fit check with the real parts (APW 32 mm, Pro Mini or Nano, DS3231, 4 AA holder).
  2. Measure magnet hold force through a scrap of the real cap, dry. Thin the cap with a flat recess from the outside if the force drops too much.
  3. Piezo test: glue the disc flat to the inside of a scrap cap and repeat the tub test. Compare to the bare disc.
  4. Dry run: sealed empty pipe with paper towels, 40 ft, at least a week. Check for water and deformation.
  5. Buoyancy and tie-down: weigh the lift of the sealed pipe, and plan a rope or strap to the cinder blocks.
  6. Cold test if possible, since forum posts warn that plastics can fail suddenly in cold water.
- If the pipe fails the dry run or kills the magnet or piezo signal, switch to the OtterBox-style box and repeat the same tests.

## PIEZO TONE TRIGGER (CURRENT PLAN, decided Nov 8)
Idea: a piezo disc listens for one coded tone. When it hears the tone, the board fires the same MOSFET output that drives the release. This is the low-cost version of how commercial acoustic releases work.

How the desk sketch works now (Arduino Nano):
1. DS3231 alarm wakes the board from sleep after a set time (30 s for testing).
2. The board samples the piezo on A0 in 200-sample windows at 8 kHz.
3. A Goertzel filter checks how much of the energy is at the target tone (1000 Hz for testing).
4. A window counts as a hit only if it is loud enough (rms above MIN_RMS, 3.0) and pure enough (ratio above MIN_RATIO, 0.5). It needs 8 hits in a row (about 0.2 s of tone).
5. On a detection it drives the MOSFET pin (D9) high for 3 s, then goes back to sleep.

Rms is how much the reading wiggles around its average (loudness). Ratio is the share of that energy that sits at the target tone (purity). Both have to pass, so quiet noise, claps and voices should not trigger it.

Piezo circuit (desk version):
- Two 100k resistors in series from 5V to GND. Their midpoint is the bias point, about 2.5V. This lets the Nano read the piezo's positive and negative swings.
- The midpoint goes to A0.
- The midpoint also goes through a 0.1 uF (104) ceramic capacitor to the piezo's ceramic face.
- The piezo's brass side goes to GND.
- Check with a multimeter: A0 should read about 2.5V before trusting any readings.
- The DS3231 and MOSFET wiring did not change.

Results so far (Nov 5):
- Quiet room: rms about 0.5 (just electrical noise).
- Piezo in contact with a speaker: ratios very high, so the bias circuit and detector code work.
- Piezo across a table from the speaker: ratio only about 0.07 to 0.11. A bare disc is a contact sensor and barely picks up sound through air. This is expected, not a code problem.
- Do not lower the thresholds to chase this. That moves the cutoff toward the noise floor and invites false triggers.

Design notes and constraints:
- Piezo discs work best in the audio range, about 1 to 10 kHz, and usually have a resonance peak around 3 to 5 kHz. We plan to sweep the tone and pick the strongest frequency. Above 4 kHz the ADC sampling must be sped up (prescaler 64, about 16 kHz sampling).
- Sound from a speaker in the air mostly bounces off the water surface. The boat side plays the tone through an underwater transducer. The scientists already have one, so we do not need to buy a speaker. We still need to check what frequencies it can play and at what range.
- The disc should be glued flat to the inside of the housing wall (the flat end cap of the PVC pipe, not the curved side) so the wall carries the vibration, which also means no wires through the wall. Expect some signal loss through the wall (a guess of 10 to 20 dB, not measured). Test the bare disc in water and the disc mounted inside a scrap PVC cap.
- If the signal is too weak, add a small op-amp stage (about 10 to 20x gain) between the piezo and A0.
- We would avoid the 69 kHz band used by fish tags. At 1 to 4 kHz this is not an issue.
- The 100k bias divider draws about 25 uA all the time. In the sealed build, power it from a spare pin and turn it on only while listening.
- Idle goal: under about 100 microamps average, otherwise the battery will not last months.

## PIEZO OPEN DESIGN QUESTIONS (not decided yet)
- DECIDED (Nov 12): the timer only saves power. Everything sleeps until the set date, then wakes and listens for the tone. The tone from the boat is the only thing that fires the release. The timer never releases the float on its own. Still open: how long it listens each time it wakes, and whether it listens in set windows (for example 10 minutes a day) or all the time after the wake date. Fail-stuck risk matters here, because a unit that never hears the tone never releases.
- Listening window length and schedule, and what it costs in battery.
- Which tone and how to code it (single tone, or a short sequence) so boat noise cannot trigger it.
- How the boat plays the tone underwater, and at what range. Target depth is about 40 ft (the 100 ft case was also discussed).
- Does the disc work when mounted inside the PVC end cap, and how much signal is lost through the wall?
- Is an amplifier stage needed?
- False triggers from boat motors, waves, ice and fish.

## BUILD LOG: WHAT WE TRIED AND WHY WE MOVED ON
Kept for the engineering notebook and the judges. Dates are approximate.

### A. Release mechanism

1. Burn wire (nichrome around nylon fishing line), mid Oct to Nov 1. Paused.
   Setup: Arduino Nano, DS3231, logic-level MOSFET, 30 AWG nichrome 60, 4 AA pack. We built it step by step on a breadboard and used an LED as a safe stand-in before adding heat.
   What happened:
   - First tests used about 1 inch of nichrome on alligator clips. That is too little resistance for 4 AA (high current, risk of blowing fuses or burning unevenly). Fixes discussed: a longer wire or thinner wire (34 to 36 AWG) and ring terminals for repeatable contact.
   - It worked at first, but wired straight to alkaline AAs the batteries wore down after a few runs. Alkaline cells sag under big current bursts, and cold makes it worse.
   - We could not get it to part the line consistently in water. Cold water pulls heat away from the wire, and burn time in water is unpredictable. Other teams report the same kind of problem (wet wire leaking current, a release that worked shallow but failed deeper).
   - Worry about the line lasting 6 months underwater (creep, water absorption, abrasion, knots). We looked at fluorocarbon or braided polyester instead of nylon, and at Vectran, which needs more heat so we skipped it.
   Ideas discussed but not built: hinged mechanical latch so the line only carries a small preload; a sealed air pocket around the wire; retry logic; lithium AAs; a supercapacitor pulse (two 2.7V 10F caps in series); a second redundant wire.
   Prior art found: WHOI pop-up float recovery (Dunn and Gordon, 1989, two burn wires on spring cam latches), Scripps L-Cheapo seismometers, Global Ocean Design burnwire release, SensorDrop.
   Why we moved on: results in water were inconsistent and we did not find the root cause. The wire and parts stay on the shelf in case we revisit it.

2. Commercial 150 lb electromagnetic door lock. Considered, not used.
   It holds only while powered, so it would drain the battery fast and let go when power dies, which is the opposite of what we need. It is also hard to waterproof.

3. Release options compared (Oct 21): plain burn wire (baseline), hook magnet plus burn wire, electro-permanent magnet (PEM), small latching solenoid, linear actuator used as a sliding pin.
   APW PEMs considered (20 mm about $28.60, 25 mm about $38 at 27 lb and 0.55 A, 32 mm about $48 at 40 lb and 1.2 A), plus a few $1.09 latching solenoids and 1 mm 304 stainless sheet for bucket tests. Nov 1 note: actual magnet cost came in at $23.
   Nov 8: ordered the APW 32 mm (listing: about 40 lb holding, 1.2 A). Hoping it arrives in time for testing.
   The linear actuator was set aside because of the moving seal problem.

4. Hold magnet (Nov 1). Current plan. No heating step, so cold water should matter less, and it needs no power while holding. Main new risk is that it only releases when powered, so a dead battery means it never lets go (see HOLD MAGNET open questions).

### B. Trigger and sensing

1. Timer only (original plan). Acoustic release was listed as an option but rejected as the main plan because it must listen constantly and commercial ones are expensive.

2. HC-SR04 ultrasonic module (about Oct 30 to Nov 1). Not used for listening.
   It is an air distance sensor: it sends a 40 kHz ping and times its own echo, so it cannot receive and decode a separate signal. Its cans are tuned for air and are not waterproof, so it would be a poor sender and receiver in water. We wired it on the desk as a hand-wave trigger just to test the chain of sensor, Arduino, MOSFET and load.

3. KY-038 sound sensor module from the Elegoo kit (Nov 1). Stand-in until the piezo arrived.
   Steps: a double-clap trigger first, then the tone detector (Goertzel) at 1000 Hz.
   Results: quiet room rms about 0.5; a clap read 300; with the phone playing the tone a little way away, rms stayed at 0.5 and ratio at 0.01; with the speaker pressed against the mic, rms got close to 10. We lowered MIN_RMS from 10 to 3.
   Limits: the analog output is weak and drops off quickly with distance, so we had to hold the phone against the mic. It is made for loud bursts, not clean audio, and it works in air only. The tone detector code itself worked, which is why the same sketch carried over to the piezo.

4. Piezo disc (Nov 5 to now). Current.
   Why we chose the piezo, mostly cost:
   - A bare 35 mm piezo disc is about $0.90 each in a 10-pack (about $1.10 for a single one at Mouser, under $0.60 at 1,000). Adding the tone listener should add only a few dollars per unit (estimate), against the $4,630 commercial ballpark.
   - It is just a disc, so there is no module board to buy, seal or power, and one can be glued inside the housing.
   - It works in the 1 to 10 kHz audio range, so we can test with a phone.
   - The air-only modules (HC-SR04, KY-038) would have needed to be waterproofed and still have the range and weak-signal problems above.
   Tradeoffs: it needs a bias circuit, the signal is small and may need an amplifier, and it only works well when coupled to the housing or water, not through air.

### C. Housing (Nov 8)
We first planned a Nalgene bottle (magnet and electronics inside, no holes through the wall). Some of the team found articles that these can start to crush around 40 ft, so we looked at pressure-rated options. Pelican-style cases were ruled out on their 1 meter rating. We plan on PVC pipe for now, with the OtterBox-style box as the backup and the Big Blue filter housing not planned. Details and reasons are under OPTIONS CONSIDERED, item 5, and the steps are under PVC HOUSING PLAN.

## DESK-TEST PARTS (search terms only, prices not checked)
- Arduino Nano with headers, plus USB cable
- DS3231 RTC module (use the coin cell type its listing recommends)
- IRLZ44N MOSFET, a few spares (not an IRF520 module)
- Hold magnet that holds with no power and releases when powered: APW 32 mm, ordered Nov 8 (listing: about 40 lb, 1.2 A). Check its rated voltage on the listing, then measure its real release current and holding force when it arrives.
- 4 AA holder with batteries, plus a 3 to 5 A fuse
- Breadboard, jumpers, 220 ohm and 10k resistors, push button
- Multimeter (also needed to measure the magnet's release current)
- Kitchen scale or luggage scale to measure holding force
- Rope storage tests: PVC tube or plastic tub, mesh or cloth bag, about 50 to 100 ft of 4 mm braided Dyneema or polyester line, 2 mm cord for the float tether
- Piezo tone trigger: 35 mm piezo disc (a pack of 10), two 100k resistors, one 0.1 uF (104) ceramic capacitor (the Elegoo kit has them), a plastic tub or bucket, a zip bag for a phone speaker, and a scrap PVC end cap with epoxy for the inside-mount test. Optional: a low-power op-amp for gain.
- PVC housing: a few feet of 3 inch Schedule 40 pipe, two 3 inch caps (one to cement on, one scrap for the through-the-wall tests), a threaded clean-out adapter with plug and O-ring for the opening end, PVC primer and cement, rope and a weight for the 40 ft dry run, paper towels. Try a 4 inch piece too if the 3 inch is tight.
- Later: 3.3V Pro Mini and FTDI adapter, lithium AA cells, epoxy for wire feedthroughs.
- Parts for the paused burn wire approach (keep for later): nichrome 60 wire 30 AWG, nylon fishing line 10 to 20 lb test, ceramic tile or metal tray.

## RETRIEVAL APPROACH (decided Nov 1): MARKER FLOAT, HAUL BY HAND
The float does not lift the cinder block anchor. It only marks the spot and carries the line to the surface. A person in a boat then hauls the whole assembly up by hand, blocks included.
- Lift estimate (planning numbers, not measured): a cinder block is about 30 lb dry and about 17 lb submerged, so two blocks are about 35 lb underwater, plus possible mud suction in silt. Lifting that with a float alone would take a 16+ liter float, so we do not try.
- Two separate lines:
  - Float tether: ties the float to the magnet latch. It only carries the float's own pull (about 2 to 4 lb), so it can be light, such as 2 mm cord or braided line.
  - Haul rope: carries the full pull when the scientist hauls up the blocks. Needs a real load rating (see spec below), not fishing line.
- Float size: just enough to lift the haul rope and stay visible, roughly 1 to 2 L (estimate, to be tested).
- Alternative we set aside: the release cuts the receiver free of the blocks so only the receiver floats up (how commercial release units work). Leaves concrete on the lake bottom. Could come back to this if hauling blocks proves too hard.

## HOLD MAGNET: OPEN DESIGN QUESTIONS (not decided yet)
- Voltage: check the rated voltage on the APW 32 mm listing. A 4 AA pack is about 6V. If the magnet is rated for 12V or 24V it may not release on 6V, and we would need more cells or a boost converter.
- Release power: how much current and how long does the magnet need to let go? The listing says about 1.2 A. Measure it, and test the shortest pulse that works. It should be a short pulse, so battery life should be fine, but we need to confirm the battery can supply that current after months in cold water (cold lowers battery output).
- Holding force: it must hold against the float's pull for months. A rough planning number is a float of 1 to 2 L, which pulls up about 2 to 4 lb (estimate, to be confirmed). The 32 mm is listed at about 40 lb, roughly 10 times that, but rated force drops a lot with any gap, so measure it with a real gap.
- Holding force through the housing wall: the magnet sits inside, so its pull has to cross the PVC end cap. A thick cap could cut the force a lot. Measure it through the actual cap, and thin the cap with an outside recess if needed.
- Fail-stuck behavior: because it only releases when powered, a dead battery or failed board means the float never releases. The scientist's drag line grapple is still the fallback. We should also decide how to check that the battery and board are healthy before deploying (for example a test pulse on land).
- Fouling: silt, rust and mussel growth between the magnet and its steel plate will weaken the hold or could keep it from letting go. Needs a test.
- Waterproofing: magnet, coil and wiring need to be sealed and still release in cold water.
- Release reliability after a long hold: check that the magnet and plate do not stick together after weeks or months, even when powered.
- The burn wire stays on the shelf, not scrapped. We can revisit it if we find out why the results were inconsistent.

## HAUL ROPE SPEC (starting point, planning estimates; check the real rated strength on the product listing)
- Material: braided Dyneema/Spectra, or braided polyester as the budget option. Strong, barely stretches, does not rot or soak up water. Avoid nylon, which stretches and absorbs water.
- Construction: braided (8 or 12 strand), not twisted 3-strand. Twisted rope holds torque and kinks, which causes snarls in storage.
- Diameter: 4 to 6 mm. 2 to 3 mm is strong enough but bites into bare hands under a 35 lb pull. Start with 4 mm and see how the haul test feels.
- Breaking strength: 500 lb or more. Static load is about 35 lb, mud suction might add 2 to 3 times that, and knots and abrasion cost 30 to 50% of strength. Many lines this size are rated well over 1,000 lb.
- Length: depth x 1.25 + 10 ft. This covers waves, a rising lake and a boat drifting while hauling. Example: 30 ft deep is about 50 ft of rope. Actual depth still needed from the scientist.
- Color: bright orange or yellow so it is easy to spot from the boat.
- Storage volume (rough): 100 ft of 4 mm is about 0.4 L of rope, so a canister of roughly 0.75 L. 6 mm needs about double that.
- Weight in water: Dyneema is roughly neutral, so it adds almost nothing for the float to carry. Polyester sinks, but 100 ft of 4 mm is only about 150 g underwater, small next to a 1 to 2 L float.
- Ends and knots: splice an eye in each end if possible, or use a bowline or double fisherman's knot. Dyneema is slippery and slides out of basic knots. Avoid stainless shackles or clips, which can corrode or foul over months. A spliced eye or plastic link is simpler.

## ROPE STORAGE (keeping the haul rope untangled for 4 to 6 months)
Ideas, simplest first:
1. Flaked line in a tube or canister, float as the lid. Load the receiver-end first (bottom) and the float-end last (top) so it pays out without crossing. Cheap, build from PVC or a food container. Try this first.
2. Throw-bag style bag with a drawstring or flap. Light and forgiving, but silt and mussels can clog the opening.
3. Fixed (non-spinning) bobbin, line cross-wound and pulled off the end like a spinning reel. No backlash, but needs careful winding.
4. Rotating spool with drag: most complex, bearings can jam from silt and mussels. Skip unless the others fail.

Notes:
- Floating lines want to drift out of open containers, so keep the container closed or narrow-mouthed.
- Mussels and silt can glue a tight lid shut. Use smooth plastic, loose tolerances, and let the float push the lid off instead of a latch.
- Search terms for existing designs: "rope canister", "ropeless fishing buoy".

## TESTS FOR THE ENGINEERING NOTEBOOK
- Magnet hold force: measure how much pull the magnet holds, dry and in cold water, with a clean gap and with a gap of silt, sand or rust.
- Magnet hold force through the housing: repeat through a piece of the real PVC end cap to see how much force is lost.
- Magnet release: confirm it lets go on command in cold water, with fouled surfaces, and with the float pulling on it. Measure the release current and how long the pulse needs to be.
- Battery check: confirm the batteries can supply the release pulse after sitting in the cold, and estimate sleep-mode battery life for 4 to 6 months.
- Long hold test: leave a magnet latched with a weight in a bucket of cold water for weeks and check that it still holds and still releases.
- Housing dry run: sink the sealed empty PVC housing at 40 ft with paper towels inside for at least a week (cold if possible), then check for water and wall deformation. Repeat for any backup housing we try.
- Housing buoyancy and anchoring: check how much lift the sealed housing has and that the tie-down to the blocks holds.
- Piezo bias check: with a multimeter, confirm A0 reads about 2.5V at rest. Record rms and ratio in a quiet room.
- Piezo tub test: tape the disc flat to the bottom or wall of a plastic tub of water, put a phone speaker in a zip bag in the water (or against the outside wall), play the 1000 Hz tone, and record rms and ratio at several distances.
- Piezo housing test: glue the disc inside a scrap PVC end cap, put it in the tub, and repeat. Compare the signal loss to the bare disc.
- Resonance sweep: play tones from about 500 Hz to 5 kHz and note where rms peaks. Pick the final tone near the peak (needs the faster ADC settings above 4 kHz).
- False trigger test: clap, talk, splash, run a motor, and play wrong tones (600 and 1500 Hz). hits must stay at 0. Raise MIN_RATIO to 0.6 or 0.7 if anything fires it.
- Idle current: measure the average current while sleeping and during a listening window, and estimate battery life.
- Sugar tablets: soak different thicknesses and recipes and time how long each holds.
- Clockwork drift: run timers in a fridge for a few weeks and measure drift.
- Tangle test: for each rope storage design, pull the float out 20 times each straight up, at a 45 degree angle, and after the container has sat in a bucket with a pump running for a week. Count clean releases for each. A stairwell or deep pool works for the pull tests.
- Haul test: weigh the blocks underwater and hand-haul them with the chosen rope to check it is realistic for one person in a boat, and to compare how 4 mm and 6 mm feel in gloved hands.
- Burn wire (paused): earlier plan was to find the power and time that part a 20 lb line with 30 AWG wire in 4 C water, with a rough planning estimate of 4 to 10 W and 1 to 5 s. Not a spec. Revisit only if we return to the burn wire.

## SAFETY
Adults handle all powered tests. Strong magnets can pinch fingers badly and can interfere with electronics and medical devices, so keep them away from phones, cards and anyone with a pacemaker, and slide magnets apart instead of pulling them. Keep the Arduino and piezo electronics dry during the tub tests, with only the sealed disc or a bagged speaker in the water. A housing that fails under pressure can implode, so run the 40 ft dry run with the housing on a rope and keep people clear while it is raised and opened. PVC primer and cement need ventilation and gloves, and adults should handle them. If we go back to the burn wire, adults run every hot test, on a ceramic tile or metal tray, with nothing flammable nearby. The wire can burn skin instantly and stays hot for a few seconds after power cuts off.

## OPEN QUESTIONS FOR THE SCIENTIST
- Which lake, and how deep do the units sit?
- How often are units recovered, and how many come back late or never?
- Which receiver model is used, and how is it anchored? (How is the receiver attached to the cinder blocks today?)
- Are acoustic releases used at all, and why do some units still not come back?
- Could one person in a boat realistically hand-haul the blocks from that depth?
- Can the transducer you already have play our release tone (about 1 to 4 kHz)? What frequencies, signals and range does it support, and how is it controlled?

## STATUS
- Team presentation planned for Oct 9: mechanical vs. electronic options.
- Nov 1: decided on marker float with hand haul, and started exploring rope storage (canister first).
- Nov 1: switched the release from burn wire to a hold magnet (holds with no power, releases when powered) because the burn wire was not working consistently in water.
- Nov 1: added a starting haul rope spec (braided, 4 to 6 mm, 500 lb or more).
- Nov 1: bench tested the HC-SR04 and the KY-038 sound sensor with the tone detector code. The code worked, but the sensor was too weak at any distance.
- Nov 5: swapped in a piezo disc with a bias circuit. Strong signal in contact with a speaker, weak through air.
- Nov 8: piezo tone trigger is now the plan. Added the build log of what we tried and why.
- Nov 8: APW 32 mm hold magnet ordered. Waiting on delivery.
- Nov 8: Nalgene bottle ruled out for the housing (crush risk at 40 ft) and Pelican-style cases ruled out (1 meter rating). For now we plan on PVC pipe. OtterBox-style box is the backup. Not final until the dry run and through-the-wall tests pass.
- Next: order the PVC parts and run the fit check; when the APW 32 mm arrives, check its rated voltage, then measure its release current and holding force, including through a scrap PVC cap; order desk-test parts (including a pack of piezo discs); run the piezo tub and housing tests and the resonance sweep; run the 40 ft dry run; decide how the timer and tone work together; decide how the boat sends the tone; build a first canister and run the tangle test; and ask the scientist the questions above.

## REFERENCES
- Team deck: https://claude.ai/artifact/5BLunfFFvzfiGnLpPrjwGU (still shows the burn wire design)
- Build and test plan doc: https://claude.ai/code/artifact/6865f8ff-a91d-4fc7-b273-fc9f8bd1fcc0 (still shows the burn wire design)
- Arduino sketch and wiring diagram: "Acoustic sensor retrieval system design" chat (written for the burn wire MOSFET, will need a small change for the magnet): https://claude.ai/chat/c2cbab41-ff39-41f6-aeb4-6bc5f1907db6
- Tone detector, sleep and piezo sketches: "Burn wire firing code" chat: https://claude.ai/chat/6b084208-258b-421d-93e5-fdfaf9f9a85d
- Sound release and transducer options (HC-SR04, piezo discs, costs): "Ultrasonic sensor range options" chat: https://claude.ai/chat/2808c2ab-504b-4b66-b4ad-377f40ac7ef6
- Burn wire design and prior art: "Burn wire release mechanisms" chat: https://claude.ai/chat/66f66330-cf1c-4f02-be8b-f085c53e0e66
- Door lock and burn wire power troubleshooting: "Electromagnetic lock and burn wire" chat: https://claude.ai/chat/19117f95-199c-4581-ad85-25fbd5202647
- Release mechanism cost comparison and magnet orders: "Permanent magnet electro release cost" chat: https://claude.ai/chat/6ace801f-5aca-4ea8-a4ad-eb7aa5e049bf
- Per-unit cost estimate and PCB idea: "Burn wire firing project costs" chat: https://claude.ai/chat/2a6916ca-0d35-403f-bcf5-e10bebc322a1
- Housing research (Nov 8): Schedule 40 PVC collapse pressure table https://www.professionalplastics.com/professionalplastics/content/downloads/ClearPVC-Schedule40.pdf ; Cave Pearl PVC housing https://formufit.com/blogs/news/115424708-water-tight-housing ; Pelican 1150 specs https://www.gearx.com/pelican-1150-watertight-case ; 17 psi equals 40 ft of head https://firstsupply.com/Product/OAT33468 ; ScubaBoard 3 inch PVC dry box thread https://scubaboard.com/community/goto/post?id=9641393 ; ScubaBoard Schedule 40 vs 80 thread https://scubaboard.com/community/goto/post?id=7306106 ; OtterBox Drybox 3250 https://digitaltrends.com/outdoors/otterbox-drybox-returns ; Pentek Big Blue spec sheet https://espwaterproducts.com/cdn/shop/files/Pentek_Big_Blue_Housings_Spec_Sheet.pdf
