# LURE (Lake Underwater Release Engine): Presentation Prep

Last updated: Nov 5, 2026. The final prototype is still in progress, so everything below is "where we are now," not the finished design.

Source: rebuilt from the "Research resources and presentation talking points" chat (https://claude.ai/chat/5f6ce9a9-23e7-452b-bdc0-39f331a713ff), with all the later price and Dr. Gerig edits applied.

---

## Part 1: Resources and Papers We Researched

Items marked **(verify)** came from our research chats without a saved link or full citation. Look them up and get the exact citation before they go on a slide or in the notebook.

### Commercial products (what exists today)

| Resource | What it is | Why it matters to us |
|---|---|---|
| [Innovasea acoustic release solutions](https://www.innovasea.com/fish-tracking/acoustic-release-solutions/) | Overview of the Ascent Acoustic Release and the ARC canister | The ARC floats gear up on a rope tied to the anchor. That is the same idea as our marker float and haul rope. |
| [Innovasea VR2AR datasheet](https://innovasea.com/wp-content/uploads/2021/06/Innovasea-Fish-Tracking-vr2ar-data-sheet-0621.pdf) | Receiver with a built-in acoustic release | Battery life roughly 14 months, and the long-life VR2AR-X up to about 26 months. Sets how long our timer has to wait. |
| [Innovasea VR2Tx datasheet](https://www.innovasea.com/wp-content/uploads/2026/02/Innovasea-ft-vr2tx.pdf) | Receiver with a built-in transmitter | A boat can check if a unit is alive, but it does not solve getting it back. |
| [Innovasea NexTrak R1 overview](https://rsaqua.co.uk/wp-content/uploads/2023/10/Innovasea-NexTrak-R1-Receiver.pdf) and [launch announcement](https://www.innovasea.com/announcements/innovasea-introduces-nextrak-acoustic-telemetry-system/) | Newer receiver generation | Shows what current units look like. Innovasea reports about 39% more range than VR2. |
| VR4-UWM (see [Innovasea receivers page](https://www.innovasea.com/fish-tracking/receivers/)) | Receiver that sends data to a surface unit | Some teams solve this from the data side, not the retrieval side. Good contrast for our approach. |
| [VR100-300 deck box (RS Aqua)](https://rsaqua.co.uk/product/vr100-300-receiver-and-deckbox/) | Boat-side unit that sends the release command | Shows what the commercial acoustic release needs on the boat. |
| [RS Aqua VR2 listing](https://rsaqua.co.uk/product/fish-tracking-receivers-69-khz/) | Plain-English overview of the VR2 family | Easy background reading. |

**Price reference (from Dr. Gerig):** the receiver **with the release is about $6,000**, and the **non-release model is about $2,000**. So the release feature adds roughly $4,000. Innovasea pricing is quote-only, so Dr. Gerig's numbers are our best source. (An archived 2021 government quote we found earlier showed $4,630 per unit, but we could not confirm the model, so use Dr. Gerig's figures.)

### Papers, patents, and related designs

| Resource | What it is | Why it matters to us |
|---|---|---|
| "Underwater Magnetic Release System," Jackson State University **(verify)** | Bluetooth-controlled electropermanent magnet release, about $85.44 in parts | Closest related design to our hold magnet. Shows a cheap magnet release is realistic. |
| Crook et al., mooring retrieval pod **(verify)** | Academic work on retrieving moorings | Benchmark for our float and haul approach. |
| WHOI pop-up float recovery system **(verify)** | Woods Hole system that sends a float to the surface to recover gear | Same marker float concept at a research-lab level. |
| Several magnetic release patents **(verify)** | Patents on magnet-based release latches | Shows the idea is established, and helps us explain what is different about ours. |
| APW Company | US source for permanent (latching) electromagnets, about $28.60 to $46.20 depending on size and holding force | Possible magnet supplier. Our actual magnet cost so far is about $23. |

### People and organizations we reached out to or identified

- **The scientist** who tracks sturgeon with acoustic receivers. Original source of the problem.
- **Dr. Brandon Gerig**, Assistant Professor, School of Freshwater Sciences, UW-Milwaukee. We met him at Harbor Fest and emailed with him in October. He gave us the unit prices, the deployment depth, and the recovery rate (see below).
  - His research: freshwater fish and the food webs that support them, including Lake Michigan. His UWM profile and publication list: [uwm.edu/freshwater/people/gerig-brandon](https://uwm.edu/freshwater/people/gerig-brandon/)
  - Note: his profile does not list acoustic telemetry or sturgeon papers, and no lab website is mentioned. So the receiver work we talked about is his field work, not something we found a paper on. Describe it as "what Dr. Gerig told us," not as a publication.
  - Recent publications on his profile (titles only, no direct article links on the page): "Testing food web theory in a large lake: The role of body size in habitat coupling in Lake Michigan" (Ecology, 2024); "Stable isotopes and movement of Walleye change following ecological shifts driven by dreissenid mussels" (North American Journal of Fisheries Management, 2022); "Temporal variation in the niche partitioning of Lake Michigan salmonines as it relates to alewife abundance and size structure" (Canadian Journal of Fisheries and Aquatic Sciences, 2022).
- **Elizabeth (Liz) Sutton**, Outreach Manager, UWM School of Freshwater Sciences. Set up our contact with the faculty and invited us to Harbor Fest on Sept 27.
- **Harvey Bootsma**, Professor and Senior Associate Dean, UWM School of Freshwater Sciences. Forwarded our first email to the right faculty.
- **UW-Madison Center for Limnology** (including Trout Lake Station, which handles gear deployment and retrieval).
- **Wisconsin DNR Winnebago sturgeon acoustic telemetry program**: runs the same kind of receiver retrieval work.

### What Dr. Gerig told us (Oct 26 email)

- He liked the idea of a release mechanism that is separate from the receiver.
- His team already uses **acoustic release receivers**. A second battery unscrews the receiver from a lug so a submerged buoy carries it up. They are **$6,000 each**, and he said he wishes they had more.
- Their buoys have **10 lb of buoyancy** and sink easily with an **80 lb weight** attached, but still carry the receiver up.
- Their receivers go to about **40 ft max**. Other groups go to over **200 ft**.
- They try to recover all receivers **every year**, with about a **5% loss rate**.
- Sensors are in **Lake Michigan**.

### Physics and engineering topics we looked into

- **Radio in water:** Bluetooth and RFID only work for a few centimeters underwater, so we cannot talk to a deployed unit that way. Acoustic is the standard. This is why our timer is set at the surface before deployment.
- **Fish tag frequency:** sturgeon tags use about 69 kHz, so any acoustic signal of ours should stay well away from that.
- **Lifting electromagnets vs. permanent (electro-permanent) magnets:** lifting magnets need power the whole time they hold, so they do not work for months. Permanent magnets hold with no power and let go on a short pulse.
- **Batteries in cold water:** alkaline batteries struggled under multi-amp loads in our tests, and cold lowers output further.
- **Environmental impact:** lost units, line entanglement, anchor placement on spawning gravel and plants, and invasive species like zebra mussels.

---

## Part 2: Talking Points on Innovation

### The problem

- Scientists leave acoustic receivers on the lake bottom for months to track sturgeon. Dr. Gerig's team sets them at about 40 ft in Lake Michigan and tries to recover all of them each year.
- About 5% are lost each year, so on a fleet of 100 receivers that is about 5 lost units. At the $2,000 non-release price that is real money, plus the data and the gear left in the lake.
- Getting them back is hard: depth, ice and boat traffic, and mussel fouling or silt burial.
- Today the scientist uses a drag line grapple, and a few acoustic release receivers when they can afford them.
- Commercial acoustic releases exist, but the release model costs about $6,000 (the non-release model is about $2,000, per Dr. Gerig) and needs a battery that listens all the time.

### Our angle

- A **much cheaper backup release**, not a replacement for what the scientist already uses.
- Our rough cost estimate is about **$137 to $159 per unit** (depends on rope material). That is an estimate, not a final number.
- We want other teams and scientists to be able to build one, so we plan to share the design.

### How we got here: what we tried

1. **Sugar tablet.** Cheap and fun to test, but dissolve time drifts by weeks with temperature, flow, and bacteria. Not reliable enough.
2. **Mechanical clockwork.** No battery, but it drifts by weeks over months and is hard to build. Our first gear idea did not work on paper, and we learned that the regulator, not the gear train, sets the timing.
3. **Acoustic release.** You pick the day, but it has to listen constantly, which drains a battery, and it is expensive. We also learned we would need to avoid the 69 kHz fish tag band.
4. **Electronic timer.** A real-time clock is nearly exact and the parts are cheap. This is our pick. The tradeoff is it needs a battery and waterproofing.

### Then we iterated on the release itself

- **Burn wire (first version).** An Arduino, a DS3231 clock, and a MOSFET heat nichrome wire to melt a nylon line.
  - We built it step by step on a breadboard, using an LED first so we could test safely before adding heat.
  - It worked at first, but connecting the wire straight to batteries wore them down after a few runs. We learned alkaline cells do not like big current bursts.
  - We could not get it to cut the line **consistently in water**.
- **Hold magnet (current plan, decided Nov 1).** A magnet that holds with no power and lets go when it gets a short pulse.
  - No heating step, so cold water should matter less.
  - The battery only has to run the clock while it waits.
  - The burn wire is on the shelf, not scrapped. We may revisit it if we figure out why results were inconsistent.

### Other design decisions and what we learned

- **Marker float, haul by hand.** We first thought the float would lift everything. The math said no: two cinder blocks are about 35 lb underwater, which would need a 16+ liter float. So the float just marks the spot and brings the haul rope up, and a person in a boat hauls the rest.
- **Two lines, two jobs.** A light tether for the float and a strong haul rope (braided, 4 to 6 mm, 500 lb+ rated).
- **Rope length formula:** depth x 1.25 + 10 ft. For a 40 ft deep site that is 60 ft.
- **Rope storage.** Keeping the rope tangle free for 4 to 6 months is its own challenge. We are starting with rope flaked into a canister and will run a tangle test.
- **Housing.** A Nalgene bottle with the magnet mounted inside, so there are no holes through the wall to leak. (As of Nov 8 this changed: Nalgene was ruled out, and the plan is now PVC pipe. See design-doc.md.)
- **Finding the float.** Reflective tape plus a flashing LED, because it is cheap and practical.
- **Why no underwater communication.** The timer is set at the surface, so we skip the problem of radio not working in water.

### Safety and environment

- Adults handle all powered tests, and strong magnets can pinch badly and affect electronics.
- Testing happens in buckets and tanks, not the lake.
- We thought about lost units, line entanglement, anchor placement away from spawning gravel and plants, and invasive species. Real deployment only under the scientist's permits.

### Honest challenges (this is a strength, judges like it)

- The first design failed in cold water, and we changed it based on our own testing.
- If the battery or board dies, the magnet never releases. The scientist's grapple stays as the fallback, and we plan a land test pulse before deploying.
- Silt, rust, and mussels could weaken the magnet's hold or stop it from letting go.
- Final prototype is **still in progress**.

### What we are testing next

- Magnet hold force, dry and in cold water, clean and fouled
- Release current and pulse length, and whether the battery can deliver it after sitting in the cold
- Long hold test: magnet latched in a bucket of cold water for weeks
- Tangle test for rope storage (20 pulls per design)
- Haul test: can one person realistically haul the blocks, and how do 4 mm and 6 mm rope feel in gloves

### Possible closing lines

- "We started with a problem a scientist told us about, and we kept changing our design when our tests told us something new."
- "Our goal is a backup release for roughly $140 to $160, compared to about $4,000 extra for a commercial release, that any team can build."

---

## Part 3: Gaps To Fill Before the Presentation

- Get real citations and links for the Jackson State paper, Crook et al., WHOI, and the patents.
- Confirm with Dr. Gerig the exact model names behind the $6,000 (release) and $2,000 (non-release) prices.
- Already answered by Dr. Gerig: Lake Michigan, about 40 ft max, recovered yearly, about 5% lost. We still need to ask: which receiver model, how it is anchored besides the 80 lb weight, and whether one person could hand-haul that weight from 40 ft.
- Compare our float to theirs. Our planning number was a 1 to 2 L float (about 2 to 4 lb of pull). Dr. Gerig's team uses 10 lb buoyancy buoys with an 80 lb weight. Check whether our float is big enough to bring the haul rope up and be seen, and whether a bigger float makes sense.
- Our 60 ft of rope (40 ft deep x 1.25 + 10 ft) now matches his 40 ft depth. Good to say in the presentation.
- Send Dr. Gerig a thank you with an update on the hold magnet.
- Replace estimates (float size, pull force, cost) with measured numbers as tests finish.
- Update the team deck and build plan doc, which still show the burn wire design.
