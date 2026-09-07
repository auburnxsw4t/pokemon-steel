# Pokémon Steel — Opening Prototype v0.0.1

## Goal
Build the first playable vertical slice of Pokémon Steel in pokeemerald-expansion: the player finishes school, walks home with Kyle, watches Logan demonstrate catching, chooses a starter, battles Kyle, receives a Silk Scarf from Madison, and gains access to the local woods and the road toward Alumina Village.

## Region
- Region: Ferrane
- Starting community: Homestead Ridge, outside Alumina Village
- Inspiration: rural/suburban American Southeast

## Player
- Male default name: Jayson
- Female default name: Evelyn
- Player can rename during the school dismissal roll call.
- Male design direction: modernized Gen III protagonist; white-and-black ball cap; loose/baggy casual clothes.

## Family
### Logan — father
- Bald, beard
- Jeans and flannel with rolled sleeves
- Former accomplished trainer
- Battle philosophy: pressure, momentum, physical offense, broad coverage
- Familiar partner used for the catching demonstration: Scizor

### Madison — mother
- Blonde
- Athletic clothing
- Former accomplished trainer
- Favors Water Pokémon; each major team should include a Water-type ace with coverage around it
- Battle philosophy: control, switching, status, and denying the opponent's plan
- Gives the player a Silk Scarf before departure

### Kyle — twin brother / primary rival
- Always male
- Identical twin if player is male; clearly related sibling if player is female
- Nine minutes older and constantly calls the player his "younger sibling"
- Arrogant, highly competitive, irritating, but deeply loyal to family
- Chooses the starter whose eventual secondary typing counters the player's choice

## Starter Trio
All begin pure Normal type and evolve at levels 16 and 36.

### Posskit -> Scavoul -> Feignar
- Final typing: Normal / Fighting
- Role: resilient physical comeback attacker
- Final BST target: 530
- Feignar target stats: HP 95 / Atk 120 / Def 90 / SpA 50 / SpD 85 / Spe 90
- Signature ability: Playing Opossum
  - Once per battle, if it would be knocked out by a damaging attack while above 1 HP, it survives at 1 HP and sharply raises Speed.
- Signature move working name: Snapback
  - Fighting / Physical / 70 BP / 100 Acc
  - Doubles in power if the user has been damaged by an opponent since its previous move.
- Personality: nocturnal, playful, food-motivated, mischievous; sleeps hanging by its tail after eating and is exceptionally difficult to wake.

### Sheldo -> Platerra -> Terradon
- Final typing: Normal / Rock
- Role: accelerating defensive bruiser / momentum tank
- Final BST target: 530
- Terradon target stats: HP 105 / Atk 110 / Def 125 / SpA 45 / SpD 90 / Spe 55
- Signature ability working concept: Rolling Start
  - Damaging Rock-type moves raise Speed by one stage.
- Signature move working name: Rolling Ridge
  - Rock / Physical / 60 BP / 100 Acc
  - Consecutive uses increase power by 20 up to 120; resets if another move is used or the user switches.
- Personality: digs for food and builds burrows; increasingly territorial as it evolves; final form is large, tough, rowdy, and built to move earth.

### Mimbri -> Harmonyl -> Cadenwing
- Final typing: Normal / Flying
- Role: fast special attacker / disruption
- Final BST target: 530
- Cadenwing target stats: HP 75 / Atk 70 / Def 70 / SpA 120 / SpD 90 / Spe 105
- Ability concept: Keen Ear
  - Boosts sound-based attacks and prevents Accuracy reduction.
- Signature move working name: Birdsong
  - Normal / Status / 100 Acc / +1 priority
  - Random useful song effect such as sleep, paralysis, confusion, or Sp. Atk reduction; balance to be tested.
- Personality: beautiful songbird; songs wake Pokémon in the morning and settle them in the evening; uses song and mimicry to control battles.

## Starter Triangle
- Fighting beats Rock
- Rock beats Flying
- Flying beats Fighting

Kyle always chooses the starter whose final secondary typing beats the player's.

## Opening Story Sequence
1. Last day at Alumina School.
2. Teacher performs dismissal roll call; player chooses name/identity here.
3. Player follows Kyle home while Kyle boasts about the League challenge and being nine minutes older.
4. At home, Logan explains that two starter Pokémon are ready but he still needs to catch the third.
5. Player and Kyle accompany Logan to the creek/woods.
6. Catching tutorial:
   - Logan uses Scizor.
   - Demonstrates False Swipe / weakening a target.
   - Explains status improves catch odds.
   - Throws Poké Ball and catches the third starter.
7. Return home and player chooses first from Posskit, Sheldo, or Mimbri.
8. Kyle chooses the eventual counter and immediately challenges the player.
9. Rival Battle #1: Kyle, one Lv. 5 starter.
10. Win response: Kyle claims beginner's luck.
11. Loss response: Kyle says it was what he expected.
12. Kyle runs ahead to the Ferrane League registration office.
13. Madison gives the player a Silk Scarf and explains held items.
14. Logan advises optional training in the woods before leaving.
15. Player may train locally, return home for healing, or head toward Alumina Village.

## Parent Home Details
- Framed Gym Badges from multiple canon regions.
- Old photo of Logan and Madison facing each other across a battlefield.
- Both claim contradictory versions of who won their first battle; the game never resolves it.
- Parent battles unlock later and give EXP but no prize money.

## First Playable Map Set
1. Alumina School interior/exterior transition
2. Homestead Ridge neighborhood
3. Family home interior
4. Creek / catching-tutorial woods
5. Local optional training woods
6. Connection toward Alumina Village

## Alumina Village — later part of the same vertical slice
- Pokémon Center
- General Mart
- Ferrane League Registration Center
- School
- Homes
- Central green / fountain
- Lake and fisherman
- Church / chapel with optional world-lore NPC
- NPC classmates also registering for the League
- Football-focused classmate as a possible secondary friendly rival
- Optional recognized-Gym-badges display explaining the eight Gym types
- Hidden items to reward exploration

## Difficulty Philosophy
- Hard through competent teams, coverage, held items, switching, setup, status, weather, and good AI rather than simple level inflation.
- Gym Leaders have "cheater" coverage Pokémon that punish the obvious counter to their primary type.
- Tone remains classic Pokémon adventure: serious threats without gore or grimdark presentation.

## Near-Future Story Locks
- Gym 1: Normal-type Pokémon-football coach; white polo, khakis, whistle, high-and-tight haircut, hard-nosed pressure style.
- Team Tartarus first major incident occurs around town/city #2, where they corner a scientist carrying an efficiency upgrade for a hydroelectric dam.
- Team Tartarus seeks Aurynex, whose white-hot blue fire could melt Ferrane's industrial infrastructure and force civilization backward.

## Implementation Milestone
The first successful ROM milestone should boot in a GBA emulator and make the opening above playable with working maps, sprites, starter species data, catching tutorial, starter selection, Kyle battle, held-item reward, and local wild encounters.


## Stabilization progress (2026-09-07)

The current map/escort milestone implements the classroom, a separate Alumina
Village exterior, a four-house Homestead Ridge, the shared family home, and a
separate creek/woods map. The authored layouts reuse Emerald metatiles; the
school and family house are on different exterior maps. `build_steel_layouts.py`
records the authored terrain and building placements.

School dismissal now locks control and escorts the player through Alumina,
west into Homestead Ridge, and into the family home. Paired movement scripts
have dialogue stops and explicit waits for both actors. Kyle's visibility is
recomputed by `SteelSyncOpeningActors` from the persistent opening variable
before map objects spawn. Each map's Kyle has a dedicated hide flag.

Existing state values remain 0 (roll call), 1 (dismissed), 2 (home arrival),
and 3 (ready for creek). State 4 is the Homestead escort. Additional stages
will be appended for the catching demonstration and subsequent opening flow.

The catching demonstration and placeholder starter handoff are the next
increment. No custom starter species have been added. Service buildings in
Alumina are exterior placeholders, and population/detail work remains.

### Verification

- `make` and `python3 dev_scripts/check_steel_opening.py` pass.
- The actor check compiles and executes the actual C visibility special.
- The headless mGBA driver uses an isolated, in-memory cartridge save. It does
  not open or overwrite `pokeemerald.sav`.
- `python3 dev_scripts/test_steel_opening_emulator.py /path/to/steel-mgba-runner`
  boots a new game, completes the classroom and escorts, then walks back
  through school, Alumina, Homestead Ridge, and the woods. Kyle remains
  exclusively assigned to the family home after these visits.
- `dev_scripts/steel_mgba_runner.c` builds against mGBA's core library with
  that library's compile definitions. The runner used here is mGBA 0.10.5.

Start a new save when testing the new geography; old map coordinates are not
migrated. Final character graphics, village services, and starter species are
still placeholders. The design above remains the story authority.
