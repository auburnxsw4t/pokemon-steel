#!/usr/bin/env python3
"""Check Steel map contracts and execute the actual actor-visibility C special."""
from pathlib import Path
import json, re, struct, subprocess, tempfile
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
         + definitions + '\n#define VAR_STEEL_OPENING 0\nstatic u16 stage; static unsigned char hidden[4096];\n'
         + 'u16 VarGet(u16 id) { return stage; }\nvoid FlagSet(u16 f) { hidden[f]=1; }\nvoid FlagClear(u16 f) { hidden[f]=0; }\n'
         + function + '\nint visible(void) { return ' + '+'.join('!hidden[%s]' % f for f,_ in flags if 'KYLE' in f) + '; }\n'
         + 'int main(void) { for(int pass=0;pass<4;pass++) { ' + checks + ' } return 0; }\n')
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
                     'Steel_SouthWoods', 'Steel_Route1Stub', 'Steel_FamilyHome_2F'}
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
    assert {'left', 'down', 'right'} <= set(directions('Steel_AluminaVillage'))
    assert {'right', 'down'} <= set(directions('Steel_HomesteadRidge'))
    assert {'up'} <= set(directions('Steel_SouthWoods'))
    assert directions('Steel_Route1Stub') == ['left']
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
    assert maps['Steel_FamilyHome_2F']['layout'] == 'LAYOUT_STEEL_FAMILY_HOME_2F'
    registration_kyle = next(o for o in maps['Steel_AluminaVillage']['object_events']
                             if o.get('local_id') == 'LOCALID_STEEL_VILLAGE_KYLE_REGISTRATION')
    assert (registration_kyle['x'], registration_kyle['y']) == (33, 27)
    assert registration_kyle['flag'] == 'FLAG_HIDE_STEEL_KYLE_REGISTRATION'
    print('PASS: map sizes, topology, tall-grass target, NPC collision tiles, Kyle hide flags, and destination warps')

def check_starter_flow():
    maps = {p.parent.name: json.loads(p.read_text()) for p in (ROOT/'data/maps').glob('Steel_*/map.json')}
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
    assert 'giveitem ITEM_SILK_SCARF' in section('Steel_Home_GiveSilkScarf')
    assert 'setflag FLAG_RECEIVED_STEEL_SILK_SCARF' in section('Steel_Home_GiveSilkScarf')
    assert 'setvar VAR_STEEL_OPENING, STEEL_OPENING_COMPLETE' in section('Steel_Home_KyleDeparts')

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
    check_starter_flow()
