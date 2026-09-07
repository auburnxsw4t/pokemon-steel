# Pokémon Steel — Approved Visual Reference Pack

This folder is a visual-design aid for implementation. It is **not** the gameplay-data authority.
When image text conflicts with `docs/pokemon_steel/opening_prototype.md` or later approved design docs, the written docs win.

## Maps

### `maps/ferrane_region_concept.jpg`
High-level Ferrane mood/geography concept only. Useful for overall regional direction. City names, route numbers, gym placements, and labels are not all final.

### `maps/homestead_ridge_sketch.jpg`
User's original rough layout. Treat the basic four-house neighborhood, central road, north water/creek, and east garden/farm idea as the key layout intent.

### `maps/homestead_ridge_concept.jpg`
Visual refinement reference, not a literal map import.
**Important superseded detail:** the image labels a separate “Twin's House.” That is wrong. Kyle is the player's twin and lives in the same family house as the player. Repurpose the lower-right house for another neighbor/family. Preserve the overall neighborhood feel rather than the incorrect label.

### `maps/alumina_village_sketch.jpg`
User's original Alumina layout. Key intent: lake/water northwest, school east/northeast, central green/landmark, Mart and Pokémon Center lower-central, League Registration Center southeast/east, homes north, west connection toward Homestead Ridge.

### `maps/alumina_village_concept.jpg`
Visual refinement reference. Use for atmosphere and relative placement, but adapt to Gen III tile/map constraints. Do not treat every label or decorative detail as locked.

## Starter Lines

These sheets are **approved visual identity references**, but the embedded text may contain old working mechanics/names. Use the written design docs for stats, abilities, moves, and final naming.

### `starters/possKit_line.jpg`
Approved visual direction for Posskit → Scavoul → Feignar.
Core visual identity: opossum line, increasingly fierce and muscular, powerful pink tail retained across stages, final form clearly a resilient physical bruiser.

### `starters/sheldo_line.jpg`
Approved **first** Sheldo line visual. This is the canon visual direction; ignore later alternate/background-removal variants.
Core visual identity: Sheldo → Platerra → Terradon, armored burrowing animal, increasingly rocky/heavy, final form built to move earth and roll through obstacles.

### `starters/mimbri_line.jpg`
Approved revised visual direction for Mimbri → Harmonyl → Cadenwing.
Core visual identity: cute songbird → elegant middle stage → striking, argument-worthy final starter with a more imposing avian silhouette.

## Implementation Guidance

- These are concept references, not ROM-ready assets.
- Do not directly import these large images as battle sprites.
- Final Gen III Pokémon assets will require clean front/back sprites, party icons, limited palettes, and appropriate GBA dimensions.
- For map work, preserve player flow and story geography first; final custom metatiles can come later.
- Kyle should never exist simultaneously on multiple maps. His visible object event must be driven by opening-story state/flags.
