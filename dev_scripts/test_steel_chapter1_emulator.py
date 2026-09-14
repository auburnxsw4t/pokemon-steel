#!/usr/bin/env python3
"""Exercise the Rev2 Southwoods -> South Trail -> Route 1 route in mGBA."""
from collections import deque
from pathlib import Path
import csv
import io
import json
import sys
import zipfile

from test_steel_opening_emulator import Emulator
from test_steel_starters_emulator import CHOICES, finish_opening, wait_for_battle


ROOT = Path(__file__).resolve().parents[1]
ARCHIVE = ROOT / 'docs/pokemon_steel/reference/chapter1/Pokemon_Steel_Chapter1_Map_Construction_V2.zip'
KEYS = {(1, 0): 16, (-1, 0): 32, (0, -1): 64, (0, 1): 128}


def load_grid(name):
    with zipfile.ZipFile(ARCHIVE) as archive:
        rows = list(csv.reader(io.TextIOWrapper(archive.open(name), encoding='utf-8-sig')))
    return [row[1:] for row in rows[1:]]


def path_between(grid, source, target, blocked=()):
    passable = {'P', 'G', 'C', 'E', 'B', 'L'}
    blocked = set(blocked) - {source, target}
    queue = deque([source])
    previous = {source: None}
    while queue:
        point = queue.popleft()
        if point == target:
            break
        x, y = point
        for neighbor in ((x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)):
            nx, ny = neighbor
            if not (0 <= ny < len(grid) and 0 <= nx < len(grid[0])):
                continue
            if neighbor in blocked or grid[ny][nx] not in passable or neighbor in previous:
                continue
            previous[neighbor] = point
            queue.append(neighbor)
    assert target in previous, (source, target, 'no emulator path')
    result = []
    point = target
    while previous[point] is not None:
        result.append(point)
        point = previous[point]
    return list(reversed(result))


def walk_path(emulator, path):
    previous = emulator.player()
    for target in path:
        dx, dy = target[0] - previous[0], target[1] - previous[1]
        emulator.walk(KEYS[(dx, dy)], 16)
        assert emulator.player() == target, ('blocked route', previous, target, emulator.player())
        previous = target


def object_tiles(map_name):
    data = json.loads((ROOT / 'data/maps' / map_name / 'map.json').read_text())
    return {(event['x'], event['y']) for event in data['object_events']}


def choose_starter(emulator):
    name, x, _, _, player_attr, kyle_attr, _, _ = CHOICES[0]
    emulator.advance(lambda: emulator.stage() == 12)
    emulator.step(120)
    emulator.goto(6, 4)
    emulator.goto(x, 4)
    emulator.walk(64, 16)
    emulator.tap(1)
    emulator.advance(lambda: emulator.stage() == 13)
    wait_for_battle(emulator, getattr(emulator, player_attr), getattr(emulator, kyle_attr))
    emulator.set_battle_hp(1, 1)
    finish_opening(emulator)
    assert emulator.stage() == 15, name


def set_flag(emulator, flag):
    address = emulator.sb1() + emulator.flags_offset + flag // 8
    value = emulator.read(address, 1)[0] | 1 << (flag % 8)
    emulator.write(address, value, 1)


def main():
    emulator = Emulator(sys.argv[1])
    emulator.step(400)
    emulator.tap(8)
    try:
        choose_starter(emulator)
        for trainer_id in range(853, 864):
            set_flag(emulator, 0x500 + trainer_id)
        emulator.write(emulator.sym('sWildEncountersDisabled'), 1, 1)

        # Home -> Homestead, then both approved trailheads and their return warps.
        emulator.goto(8, 7)
        emulator.walk(128, 32)
        emulator.step(100)
        assert emulator.location()[1] == 1
        emulator.goto(7, 25)
        emulator.walk(128, 16)
        emulator.step(100)
        assert emulator.location()[1] == 6 and emulator.player() == (6, 3)
        emulator.walk(64, 16)
        emulator.step(100)
        assert emulator.location()[1] == 1
        emulator.goto(7, 16)
        emulator.goto(35, 16)
        emulator.walk(16, 24)
        emulator.step(100)
        assert emulator.location()[1] == 4
        emulator.goto(28, 29)
        emulator.walk(128, 16)
        emulator.step(100)
        assert emulator.location()[1] == 6 and emulator.player() == (29, 3)

        southwoods = load_grid('southwoods_grid_v2.csv')
        walk_path(emulator, path_between(southwoods, emulator.player(), (18, 57), object_tiles('Steel_SouthWoods')))
        emulator.walk(128, 16)
        emulator.step(100)
        assert emulator.location()[1] == 7 and emulator.player() == (18, 2)
        emulator.goto(18, 9)
        emulator.walk(128, 16)
        emulator.step(100)
        assert emulator.location()[1] == 8 and emulator.player() == (18, 3)

        route = load_grid('route1_grid_v2.csv')
        walk_path(emulator, path_between(route, emulator.player(), (18, 70), object_tiles('Steel_Route1')))
        assert emulator.location()[1] == 8 and emulator.player() == (18, 70)

        # Turn encounters back on and exercise real Route 1 grass until a wild
        # party is generated. This confirms the compiled encounter header is live.
        grass = min(((x, y) for y, row in enumerate(route) for x, code in enumerate(row) if code == 'G'),
                    key=lambda point: abs(point[0] - 18) + abs(point[1] - 70))
        walk_path(emulator, path_between(route, emulator.player(), grass, object_tiles('Steel_Route1')))
        emulator.write(emulator.sym('sWildEncountersDisabled'), 0, 1)
        neighbors = [point for point in ((grass[0] + 1, grass[1]), (grass[0] - 1, grass[1]),
                                        (grass[0], grass[1] + 1), (grass[0], grass[1] - 1))
                     if route[point[1]][point[0]] == 'G']
        assert neighbors, ('isolated grass', grass)
        for step in range(400):
            current = emulator.player()
            target = neighbors[0] if current == grass else grass
            emulator.walk(KEYS[(target[0] - current[0], target[1] - current[1])], 16)
            if emulator.read(emulator.sym('gMain') + 0x439, 1)[0] & 2:
                break
        else:
            raise AssertionError('Route 1 grass did not start a wild encounter')
        # Let the wild battle initialize, then force the sole party member to
        # faint. This exercises the game's real whiteout path, including its
        # heal-location lookup, rather than merely inspecting the configured id.
        for _ in range(400):
            if (emulator.u32(emulator.sym('gBattleTypeFlags'))
                    and emulator.battle_species(0) and emulator.battle_species(1)):
                break
            emulator.step(2, 1)
            emulator.step(18)
        else:
            raise AssertionError('wild battle did not initialize')
        emulator.set_battle_hp(0, 0)
        emulator.write(emulator.sym('gParties') + emulator.mon_hp_offset, 0, 2)
        emulator.advance(lambda: emulator.location()[1] == 2 and emulator.player() == (8, 7),
                         limit=50000)
        emulator.step(240)
        assert emulator.stage() == 15, ('story state changed after whiteout', emulator.stage())
        assert emulator.location() == (75, 2), ('whiteout left Steel maps', emulator.location())
        assert emulator.party_hp() == emulator.party_max_hp() > 0, 'whiteout did not heal the party'

        # Use the game's own Save menu, clone the emulated cartridge data, then
        # reset and Continue from it. No developer/player .sav is touched.
        emulator.tap(8)  # Start
        emulator.step(80)
        for _ in range(3):  # POKéMON, BAG, PLAYER, SAVE
            emulator.tap(128)
        emulator.tap(1)
        for _ in range(30):
            emulator.tap(1)
        cartridge = '/tmp/steel-chapter1-after-whiteout.sav'
        emulator.cmd('cartsave ' + cartridge)
        emulator.cmd('cartload ' + cartridge)
        emulator.cmd('reset')
        emulator.step(400)
        emulator.tap(8)
        emulator.step(80)
        emulator.tap(1)  # Continue
        emulator.advance(lambda: emulator.stage() == 15
                         and emulator.location() == (75, 2)
                         and emulator.player() == (8, 7), limit=30000)
        assert emulator.party_hp() == emulator.party_max_hp() > 0
        print('PASS: both forest trailheads, South Trail, Route 1 wild battle, Steel-family-home whiteout, and cartridge save/reload')
    finally:
        emulator.close()


if __name__ == '__main__':
    main()
