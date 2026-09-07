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
    expected = {0: 'SCHOOL', 1: 'VILLAGE', 2: 'HOME', 3: 'HOME', 4: 'RIDGE', 5: 'VILLAGE', 6: 'HOME', 7: 'RIDGE', 8: 'WOODS', 9: 'WOODS', 10: 'RIDGE', 11: 'HOME', 12: 'HOME', 13: 'HOME', 14: 'HOME', 15: 'VILLAGE', 16: 'WOODS'}
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
    print('PASS: map sizes, NPC collision tiles, Kyle hide flags, and destination warps')

if __name__ == '__main__':
    check_actors()
    check_maps()
