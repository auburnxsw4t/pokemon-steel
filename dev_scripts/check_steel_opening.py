#!/usr/bin/env python3
"""Check Steel map contracts and execute the actual actor-visibility C special."""
from pathlib import Path
from collections import deque
import csv, io, json, re, struct, subprocess, tempfile, zipfile
ROOT = Path(__file__).resolve().parents[1]

def check_actors():
    source = (ROOT / 'src/pokemon_steel.c').read_text()
    start = source.index('void SteelSyncOpeningActors(void)')
    end = source.index('{', start)
    depth = 1
    i = end + 1
    while depth:
        depth += (source[i] == '{') - (source[i] == '}')
        i += 1
    function = source[start:i]
    flags = re.findall(r'^#define (FLAG_HIDE_STEEL_\w+) (0x[0-9A-Fa-f]+)', (ROOT/'include/constants/flags.h').read_text(), re.M)
    definitions = '\n'.join('#define %s %s' % f for f in flags)
    expected = {0: 'SCHOOL', 1: 'VILLAGE', 2: 'HOME', 3: 'HOME', 4: 'RIDGE', 5: 'VILLAGE', 6: 'HOME', 7: 'RIDGE', 8: 'WOODS', 9: 'WOODS', 10: 'RIDGE', 11: 'HOME', 12: 'HOME', 13: 'HOME', 14: 'HOME', 15: 'REGISTRATION', 16: 'WOODS'}
    checks = '\n'.join('stage=%d; SteelSyncOpeningActors(); assert(!hidden[FLAG_HIDE_STEEL_KYLE_%s]); assert(visible()==1);' % (stage, actor) for stage, actor in expected.items())
    c = ('#include <stdint.h>\n#include <assert.h>\n#include "constants/pokemon_steel.h"\ntypedef uint16_t u16;\n'
         + definitions + '\n#define VAR_STEEL_OPENING 0\n#define VAR_STEEL_REGISTRATION 1\nstatic u16 stage, registration=STEEL_REGISTRATION_ACTIVE; static unsigned char hidden[4096];\n'
         + 'u16 VarGet(u16 id) { return id == VAR_STEEL_OPENING ? stage : registration; }\nvoid VarSet(u16 id, u16 value) { if (id == VAR_STEEL_REGISTRATION) registration=value; }\nvoid FlagSet(u16 f) { hidden[f]=1; }\nvoid FlagClear(u16 f) { hidden[f]=0; }\n'
         + function + '\nint visible(void) { return ' + '+'.join('!hidden[%s]' % f for f,_ in flags if 'KYLE' in f) + '; }\n'
         + 'int main(void) { for(int pass=0;pass<4;pass++) { registration=STEEL_REGISTRATION_ACTIVE; ' + checks + ' } return 0; }\n')
    with tempfile.TemporaryDirectory(prefix='steel-check-') as temp:
        path = Path(temp)/'actors.c'; path.write_text(c)
        exe = Path(temp)/'actors'
        subprocess.run(['gcc','-I',str(ROOT/'include'),str(path),'-o',str(exe)],check=True)
        subprocess.run([str(exe)],check=True)
    print('PASS: actor flags, stage transitions, and repeated map re-entry synchronization')

def check_maps():
    maps = {p.parent.name: json.loads(p.read_text()) for p in (ROOT/'data/maps').glob('Steel_*/map.json')}
    by_id = {m['id']:m for m in maps.values()}
    layouts = {l['id']:l for l in json.loads((ROOT/'data/layouts/layouts.json').read_text())['layouts']}
    expected_maps = {'Steel_AluminaVillage', 'Steel_HomesteadRidge', 'Steel_CatchingWoods',
                     'Steel_SouthWoods', 'Steel_SouthTrail', 'Steel_Route1', 'Steel_FamilyHome_2F',
                     'Steel_LeagueRegistration', 'Steel_AluminaPokemonCenter_1F', 'Steel_AluminaMart'}
    assert expected_maps <= maps.keys(), expected_maps - maps.keys()
    for name,m in maps.items():
        layout=layouts[m['layout']]
        raw=(ROOT/layout['blockdata_filepath']).read_bytes()
        assert len(raw)==2*layout['width']*layout['height'], name
        for o in m['object_events']:
            x,y=o['x'],o['y']
            assert 0<=x<layout['width'] and 0<=y<layout['height'], (name,o)
            block=struct.unpack_from('<H',raw,2*(y*layout['width']+x))[0]
            assert block & 0xC00 == 0, (name,x,y,'NPC on blocked tile')
            if 'KYLE' in o.get('local_id','') or 'BRENDAN' in o['graphics_id']:
                assert o['flag'].startswith('FLAG_HIDE_STEEL_KYLE_'), (name,'unmanaged Kyle')
        for w in m['warp_events']:
            assert w['dest_map'] in by_id, (name,w)
            assert int(w['dest_warp_id']) < len(by_id[w['dest_map']]['warp_events']), (name,w)
    def directions(name):
        return [c['direction'] for c in maps[name]['connections'] or []]
    assert directions('Steel_AluminaVillage') == ['left']
    assert directions('Steel_HomesteadRidge') == ['right']
    assert maps['Steel_CatchingWoods']['connections'] is None
    assert maps['Steel_SouthWoods']['connections'] is None
    assert maps['Steel_SouthTrail']['connections'] is None
    assert maps['Steel_Route1']['connections'] is None
    assert not any(c['map'] == 'MAP_STEEL_ROUTE1' for c in maps['Steel_AluminaVillage']['connections'])
    expected_sections = {
        'Steel_AluminaSchool': 'MAPSEC_ALUMINA_VILLAGE',
        'Steel_AluminaVillage': 'MAPSEC_ALUMINA_VILLAGE',
        'Steel_LeagueRegistration': 'MAPSEC_ALUMINA_VILLAGE',
        'Steel_AluminaPokemonCenter_1F': 'MAPSEC_ALUMINA_VILLAGE',
        'Steel_AluminaMart': 'MAPSEC_ALUMINA_VILLAGE',
        'Steel_HomesteadRidge': 'MAPSEC_HOMESTEAD_RIDGE',
        'Steel_FamilyHome': 'MAPSEC_HOMESTEAD_RIDGE',
        'Steel_FamilyHome_2F': 'MAPSEC_HOMESTEAD_RIDGE',
        'Steel_CatchingWoods': 'MAPSEC_LONGLEAF_HOLLOW',
        'Steel_SouthWoods': 'MAPSEC_LONGLEAF_HOLLOW',
        'Steel_SouthTrail': 'MAPSEC_SOUTH_TRAIL',
        'Steel_Route1': 'MAPSEC_STEEL_ROUTE_1',
    }
    assert {name: maps[name]['region_map_section'] for name in expected_sections} == expected_sections
    village_triggers = {e['script'] for e in maps['Steel_AluminaVillage']['coord_events']}
    ridge_triggers = {e['script'] for e in maps['Steel_HomesteadRidge']['coord_events']}
    assert 'Steel_Village_ToSouthWoods' in village_triggers
    assert 'Steel_Ridge_ToSouthWoods' in ridge_triggers
    assert {e['x'] for e in maps['Steel_AluminaVillage']['coord_events']
            if e['script'] == 'Steel_Village_ToSouthWoods'} == {22, 23, 24, 25, 26}
    assert {e['x'] for e in maps['Steel_SouthWoods']['coord_events']
            if e['script'] == 'Steel_SouthWoods_ToAlumina'} == {27, 28, 29, 30, 31}
    woods = maps['Steel_CatchingWoods']
    target = next(o for o in woods['object_events'] if o.get('local_id') == 'LOCALID_STEEL_WOODS_TARGET')
    layout = layouts[woods['layout']]
    raw = (ROOT / layout['blockdata_filepath']).read_bytes()
    target_block = struct.unpack_from('<H', raw, 2 * (target['y'] * layout['width'] + target['x']))[0] & 0x3ff
    assert target_block == 0x00D, ('catching target is not in tall grass', hex(target_block))
    south_layout = layouts[maps['Steel_SouthWoods']['layout']]
    south_raw = (ROOT / south_layout['blockdata_filepath']).read_bytes()
    south_blocks = struct.unpack('<' + 'H' * (len(south_raw) // 2), south_raw)
    assert 0x00D in {b & 0x3ff for b in south_blocks}, 'South Woods has no tall grass'
    assert (south_layout['width'], south_layout['height']) == (36, 60)
    route_layout = layouts[maps['Steel_Route1']['layout']]
    assert (route_layout['width'], route_layout['height']) == (36, 72)
    route_raw = (ROOT / route_layout['blockdata_filepath']).read_bytes()
    route_blocks = struct.unpack('<' + 'H' * (len(route_raw) // 2), route_raw)
    assert 0x00D in {b & 0x3ff for b in route_blocks}, 'Route 1 has no tall grass'
    assert len([o for o in maps['Steel_SouthWoods']['object_events'] if o['trainer_type'] == 'TRAINER_TYPE_NORMAL']) == 3
    assert len([o for o in maps['Steel_Route1']['object_events'] if o['trainer_type'] == 'TRAINER_TYPE_NORMAL']) == 5
    assert maps['Steel_FamilyHome_2F']['layout'] == 'LAYOUT_STEEL_FAMILY_HOME_2F'
    village_layout = layouts[maps['Steel_AluminaVillage']['layout']]
    assert (village_layout['width'], village_layout['height']) == (48, 36)
    assert (layouts[maps['Steel_FamilyHome']['layout']]['width'],
            layouts[maps['Steel_FamilyHome']['layout']]['height']) == (16, 12)
    assert (layouts[maps['Steel_FamilyHome_2F']['layout']]['width'],
            layouts[maps['Steel_FamilyHome_2F']['layout']]['height']) == (18, 12)
    registration_kyle = next(o for o in maps['Steel_LeagueRegistration']['object_events']
                             if o.get('local_id') == 'LOCALID_STEEL_REGISTRATION_KYLE')
    assert (registration_kyle['x'], registration_kyle['y']) == (8, 4)
    assert registration_kyle['flag'] == 'FLAG_HIDE_STEEL_KYLE_REGISTRATION'
    print('PASS: map sizes, revised topology, tall grass, required trainers, NPC collision tiles, Kyle flags, and warps')

def check_chapter1_construction():
    archive = ROOT/'docs/pokemon_steel/reference/chapter1/Pokemon_Steel_Chapter1_Map_Construction_V2.zip'
    def grid(name):
        with zipfile.ZipFile(archive) as z:
            rows = list(csv.reader(io.TextIOWrapper(z.open(name), encoding='utf-8-sig')))
        return [row[1:] for row in rows[1:]]
    def shortest(source, target):
        passable = {'P', 'G', 'C', 'E', 'B', 'L'}
        q = deque([(source, 0, int(grid_data[source[1]][source[0]] == 'G'))])
        best = {source: (0, q[0][2])}
        while q:
            (x, y), distance, grass = q.popleft()
            if (x, y) == target:
                return distance, grass
            for nx, ny in ((x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)):
                if not (0 <= ny < len(grid_data) and 0 <= nx < len(grid_data[0])):
                    continue
                if grid_data[ny][nx] not in passable:
                    continue
                candidate = (distance + 1, grass + int(grid_data[ny][nx] == 'G'))
                if (nx, ny) not in best or candidate < best[(nx, ny)]:
                    best[(nx, ny)] = candidate
                    q.append(((nx, ny), *candidate))
        raise AssertionError((source, target, 'disconnected'))
    grid_data = grid('southwoods_grid_v2.csv')
    assert (len(grid_data[0]), len(grid_data)) == (36, 60)
    for source in ((6, 2), (29, 2)):
        distance, grass = shortest(source, (18, 58))
        assert distance >= 100 and grass >= 20, (source, distance, grass)
    grid_data = grid('route1_grid_v2.csv')
    assert (len(grid_data[0]), len(grid_data)) == (36, 72)
    distance, grass = shortest((18, 1), (18, 70))
    assert distance >= 120 and grass >= 20, (distance, grass)
    encounters = json.loads((ROOT/'src/data/wild_encounters.json').read_text())
    maps = {e.get('map'): e for group in encounters['wild_encounter_groups'] for e in group['encounters']}
    for map_id in ('MAP_STEEL_SOUTH_WOODS', 'MAP_STEEL_ROUTE1'):
        assert len(maps[map_id]['land_mons']['mons']) == 12
        assert maps[map_id]['land_mons']['encounter_rate'] > 0
    print('PASS: V2 semantic traversal pressure and Southwoods/Route 1 land encounter tables')

def check_starter_flow():
    maps = {p.parent.name: json.loads(p.read_text()) for p in (ROOT/'data/maps').glob('Steel_*/map.json')}
    layouts = {l['id']:l for l in json.loads((ROOT/'data/layouts/layouts.json').read_text())['layouts']}
    home = maps['Steel_FamilyHome']
    balls = [o for o in home['object_events'] if o['graphics_id'] == 'OBJ_EVENT_GFX_POKE_BALL']
    assert len(balls) == 3, 'the downstairs trainer display must contain exactly three balls'
    expected_balls = {
        'LOCALID_STEEL_HOME_POSSKIT_BALL': ('Steel_Home_PosskitBall', 'FLAG_HIDE_STEEL_STARTER_POSSKIT'),
        'LOCALID_STEEL_HOME_SHELDO_BALL': ('Steel_Home_SheldoBall', 'FLAG_HIDE_STEEL_STARTER_SHELDO'),
        'LOCALID_STEEL_HOME_MIMBRI_BALL': ('Steel_Home_MimbriBall', 'FLAG_HIDE_STEEL_STARTER_MIMBRI'),
    }
    assert {o['local_id'] for o in balls} == set(expected_balls)
    for ball in balls:
        assert (ball['script'], ball['flag']) == expected_balls[ball['local_id']]
    home_layout = layouts[home['layout']]
    home_raw = (ROOT/home_layout['blockdata_filepath']).read_bytes()
    for ball in balls:
        block = struct.unpack_from('<H', home_raw,
                                   2 * (ball['y'] * home_layout['width'] + ball['x']))[0]
        assert block & 0x3ff == 0x293, ('starter ball lacks display table', ball['local_id'], hex(block))
    assert maps['Steel_FamilyHome_2F']['object_events'] == [], 'starter balls must not appear upstairs'

    scripts = (ROOT/'data/maps/Steel_FamilyHome/scripts.inc').read_text()
    def section(label):
        match = re.search(r'^' + re.escape(label) + r'::\n(.*?)(?=^\w[^\n]*::?\n|\Z)', scripts, re.M | re.S)
        assert match, label
        return match.group(1)
    choices = {
        'Posskit': ('STEEL_STARTER_CHOICE_POSSKIT', 'STEEL_STARTER_POSSKIT', 'STEEL_STARTER_CHOICE_MIMBRI',
                    ('POSSKIT', 'MIMBRI')),
        'Sheldo': ('STEEL_STARTER_CHOICE_SHELDO', 'STEEL_STARTER_SHELDO', 'STEEL_STARTER_CHOICE_POSSKIT',
                   ('SHELDO', 'POSSKIT')),
        'Mimbri': ('STEEL_STARTER_CHOICE_MIMBRI', 'STEEL_STARTER_MIMBRI', 'STEEL_STARTER_CHOICE_SHELDO',
                   ('MIMBRI', 'SHELDO')),
    }
    for name, (player, species, kyle, hidden) in choices.items():
        body = section('Steel_Home_Choose' + name)
        assert f'setvar VAR_STEEL_STARTER, {player}' in body
        assert f'setvar VAR_STEEL_KYLE_STARTER, {kyle}' in body
        assert f'givemon {species}, 5' in body
        for mon in hidden:
            assert f'setflag FLAG_HIDE_STEEL_STARTER_{mon.upper()}' in body
            assert f'removeobject LOCALID_STEEL_HOME_{mon.upper()}_BALL' in body
    for mon in ('Posskit', 'Sheldo', 'Mimbri'):
        body = section('Steel_Home_' + mon + 'Ball')
        assert 'MSGBOX_YESNO' in body and f'Steel_Home_Choose{mon}' in body

    battles = {
        'Posskit': 'TRAINER_KYLE_POSSKIT',
        'Sheldo': 'TRAINER_KYLE_SHELDO',
        'Mimbri': 'TRAINER_KYLE_MIMBRI',
    }
    for mon, trainer in battles.items():
        body = section('Steel_Home_BattleKyle' + mon)
        assert f'trainerbattle_earlyrival {trainer}, RIVAL_BATTLE_HEAL_AFTER' in body
        assert 'goto Steel_Home_AfterKyleBattle' in body
    assert 'KYLE: That was luck.' in scripts
    assert "That's about what I expected." in scripts
    assert 'special HealPlayerParty' in section('Steel_Home_AfterKyleBattle')
    assert 'setvar VAR_STEEL_OPENING, STEEL_OPENING_KYLE_LEAVES' in section('Steel_Home_AfterKyleBattle')
    assert 'giveitem ITEM_SILK_SCARF' in section('Steel_Home_MadisonGivesScarf')
    assert 'setflag FLAG_RECEIVED_STEEL_SILK_SCARF' in section('Steel_Home_MadisonGivesScarf')
    assert 'setvar VAR_STEEL_OPENING, STEEL_OPENING_COMPLETE' in section('Steel_Home_KyleFinishesExit')
    assert 'setrespawn HEAL_LOCATION_STEEL_FAMILY_HOME' in section('Steel_Home_KyleFinishesExit')

    parties = (ROOT/'src/data/trainers.party').read_text()
    for trainer, species in [('TRAINER_KYLE_POSSKIT', 'Zigzagoon'),
                             ('TRAINER_KYLE_SHELDO', 'Sandshrew'),
                             ('TRAINER_KYLE_MIMBRI', 'Taillow')]:
        match = re.search(r'^=== ' + trainer + r' ===\n(.*?)(?=^=== |\Z)', parties, re.M | re.S)
        assert match and re.search(r'^' + species + r'\nLevel: 5$', match.group(1), re.M), (trainer, species)
    print('PASS: three starter choices, Kyle counter teams, rival outcome flow, reward, and departure state')

if __name__ == '__main__':
    check_actors()
    check_maps()
    check_chapter1_construction()
    check_starter_flow()
