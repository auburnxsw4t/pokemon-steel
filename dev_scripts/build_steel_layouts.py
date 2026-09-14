#!/usr/bin/env python3
"""Build the authored Steel layouts from existing Emerald metatile stamps.
No original map or tileset is changed. Re-run after editing these coordinates.
"""
from pathlib import Path
import csv, io, json, struct, zipfile
ROOT=Path(__file__).resolve().parents[1]
PATH=ROOT/'data/layouts/layouts.json'
DATA=json.loads(PATH.read_text())
LAYOUTS={l['name']:l for l in DATA['layouts']}
def blocks(name):
    l=LAYOUTS[name+'_Layout'];raw=(ROOT/l['blockdata_filepath']).read_bytes()
    return l['width'],struct.unpack('<'+'H'*(len(raw)//2),raw)
def write_layout(name,ident,w,h,tiles,source='PetalburgCity'):
    old=LAYOUTS[source+'_Layout'].copy()
    folder=ROOT/'data/layouts'/name;folder.mkdir(exist_ok=True)
    (folder/'map.bin').write_bytes(struct.pack('<'+'H'*len(tiles),*tiles))
    (folder/'border.bin').write_bytes((ROOT/LAYOUTS[source+'_Layout']['border_filepath']).read_bytes())
    old.update(id='LAYOUT_'+ident,name=name+'_Layout',width=w,height=h,
        blockdata_filepath='data/layouts/'+name+'/map.bin',border_filepath='data/layouts/'+name+'/border.bin')
    DATA['layouts'][:]=[l for l in DATA['layouts'] if l['id']!=old['id']]
    DATA['layouts'].append(old)
def remove_layout(ident):
    DATA['layouts'][:]=[l for l in DATA['layouts'] if l['id']!='LAYOUT_'+ident]
class Map:
    def __init__(self,w,h):
        self.w,self.h=w,h;self.tiles=[0x3001]*(w*h)
    def fill(self,x,y,w,h,mid,blocked=False,elevation=3):
        for yy in range(y,y+h):
            for xx in range(x,x+w):self.tiles[yy*self.w+xx]=mid|(0x400 if blocked else 0)|(elevation<<12)
    def stamp(self,name,sx,sy,w,h,x,y):
        sw,src=blocks(name)
        for yy in range(h):
            for xx in range(w):self.tiles[(y+yy)*self.w+x+xx]=src[(sy+yy)*sw+sx+xx]
    def trees(self,x,y,w,h):
        sw,src=blocks('PetalburgCity')
        for yy in range(y,y+h):
            for xx in range(x,x+w):self.tiles[yy*self.w+xx]=src[(yy%2)*sw+xx%2]
    def border(self):
        self.trees(0,0,self.w,2);self.trees(0,self.h-2,self.w,2)
        self.trees(0,2,2,self.h-4);self.trees(self.w-2,2,2,self.h-4)
    def path(self,x,y,w,h):self.fill(x,y,w,h,0x121)
    def tall_grass(self,x,y,w,h):self.fill(x,y,w,h,0x00D)
    def house(self,x,y):self.stamp('PetalburgCity',5,2,5,5,x,y)
    def water(self,x,y,w,h):
        # Emerald pond banks and water; elevation 1, impassable on foot.
        mids=[[0xB0,0xB1,0xB2],[0xB8,0xA1,0xBA],[0xC8,0xC9,0xCA]]
        for yy in range(h):
            for xx in range(w):
                by=0 if yy==0 else 2 if yy==h-1 else 1
                bx=0 if xx==0 else 2 if xx==w-1 else 1
                self.fill(x+xx,y+yy,1,1,mids[by][bx],True,1)
def semantic_grid(filename):
    archive=ROOT/'docs/pokemon_steel/reference/chapter1/Pokemon_Steel_Chapter1_Map_Construction_V2.zip'
    with zipfile.ZipFile(archive) as z:
        rows=list(csv.reader(io.TextIOWrapper(z.open(filename),encoding='utf-8-sig')))
    grid=[row[1:] for row in rows[1:]]
    assert grid and all(len(row)==len(grid[0]) for row in grid), filename
    return grid
def semantic_map(grid):
    h,w=len(grid),len(grid[0])
    result=Map(w,h)
    result.trees(0,0,w,h)
    for y,row in enumerate(grid):
        for x,code in enumerate(row):
            if code in ('P','E','B'):
                result.path(x,y,1,1)
            elif code=='G':
                result.tall_grass(x,y,1,1)
            elif code=='C':
                result.fill(x,y,1,1,0x001)
            elif code=='W':
                result.fill(x,y,1,1,0x0A1,True,1)
            elif code=='R':
                # Existing tan earth is the temporary red-clay vocabulary.
                result.fill(x,y,1,1,0x121,True)
            elif code=='L':
                # Keep the authored shortcut lanes traversable until their final
                # one-way ledge metatiles are selected in Porymap.
                result.path(x,y,1,1)
            elif code=='M':
                # The old mine is a landmark, not a Chapter 1 entrance.
                result.fill(x,y,1,1,0x016,True)
            elif code=='X':
                result.tiles[y*w+x] |= 0x400
            else:
                raise ValueError((x,y,code))
    return result
v=Map(40,32);v.border()
v.water(3,3,9,8)
v.path(0,15,40,3);v.path(31,8,3,17)
# Keep the central path clear of the lower service buildings, then route
# the south exit along the east side of the civic block.
v.path(15,18,3,9);v.path(26,18,3,14);v.path(22,18,3,9)
v.house(30,4) # school door (32,7)
v.house(15,2);v.house(22,2) # north residential feel
v.stamp('PetalburgCity',24,10,4,4,15,21) # Mart door (16,23)
v.stamp('PetalburgCity',19,14,4,4,22,21) # Center door (23,23)
v.stamp('PetalburgCity',12,4,6,6,30,21) # Registration door (33,25)
v.stamp('PetalburgCity',12,4,6,6,6,21) # chapel placeholder door (9,25)
v.path(14,6,12,1);v.path(14,13,12,1);v.path(14,7,1,6);v.path(25,7,1,6)
v.water(18,8,5,5);v.fill(20,10,1,1,0x002,True) # stone fountain basin, existing rock/water
for x,y in [(15,8),(15,11),(24,8),(24,11),(28,5),(28,7)]:v.fill(x,y,1,1,4)
write_layout('Steel_AluminaVillage','STEEL_ALUMINA_VILLAGE',v.w,v.h,v.tiles)
r=Map(36,28);r.border();r.water(2,2,32,5)
r.path(0,15,36,3);r.path(7,12,3,12);r.path(24,12,3,16)
for x,y in [(6,8),(23,8),(6,19),(23,19)]:r.house(x,y)
# Eastern garden: soil beds separated by grass, with existing shrub metatiles.
for yy in [9,11]:
 r.fill(29,yy,4,1,0x121)
 for xx in [29,31]:r.fill(xx,yy,1,1,0x16,True)
# The south-west path is the Homestead trailhead into Southwoods V2. Keep the
# eastern garden path internal so it cannot conflict with that connection.
r.path(5,12,5,16)
r.trees(24,24,3,4)
write_layout('Steel_HomesteadRidge','STEEL_HOMESTEAD_RIDGE',r.w,r.h,r.tiles)
w=Map(28,24);w.border();w.water(2,2,24,5)
w.trees(2,7,8,15);w.trees(20,7,6,6);w.trees(20,19,6,3)
w.path(10,15,18,3);w.fill(10,10,10,9,1)
w.tall_grass(15,10,5,5)
for x,y in [(10,8),(12,8),(18,8),(11,19),(17,19),(21,13)]:w.fill(x,y,1,1,0x16,True)
for x,y in [(11,10),(18,17),(12,18)]:w.fill(x,y,1,1,4)
write_layout('Steel_CatchingWoods','STEEL_CATCHING_WOODS',w.w,w.h,w.tiles)
# Southwoods and Route 1 use the approved V2 semantic grids directly. The
# mapping is intentionally conservative: traversal/collision comes first and
# custom red-clay, bridge, mine, and ledge art can replace the placeholders.
s=semantic_map(semantic_grid('southwoods_grid_v2.csv'))
write_layout('Steel_SouthWoods','STEEL_SOUTH_WOODS',s.w,s.h,s.tiles)
# A quiet connector separates the dense woods from the long open route.
trail=Map(36,12);trail.trees(0,0,trail.w,trail.h)
trail.path(16,0,5,trail.h)
trail.tall_grass(14,4,2,4);trail.tall_grass(21,4,2,4)
write_layout('Steel_SouthTrail','STEEL_SOUTH_TRAIL',trail.w,trail.h,trail.tiles)
t=semantic_map(semantic_grid('route1_grid_v2.csv'))
remove_layout('STEEL_ROUTE1_STUB')
write_layout('Steel_Route1','STEEL_ROUTE1',t.w,t.h,t.tiles)
# Independent interior layouts, preserving Emerald's useful furnishings.
for name,ident,source in [('Steel_AluminaSchool','STEEL_ALUMINA_SCHOOL','RustboroCity_PokemonSchool'),('Steel_FamilyHome','STEEL_FAMILY_HOME','LittlerootTown_BrendansHouse_1F')]:
 l=LAYOUTS[source+'_Layout'];sw,src=blocks(source)
 write_layout(name,ident,sw,l['height'],src,source)
# Keep the downstairs actor positions and warp coordinates stable, but give the
# second floor three compact bedrooms with existing bedroom wall blocks.
l=LAYOUTS['LittlerootTown_BrendansHouse_2F_Layout'];sw,src=blocks('LittlerootTown_BrendansHouse_2F')
up=list(src)
for y in range(3,7):
    up[y*sw+2]=0x204
    up[y*sw+5]=0x204
write_layout('Steel_FamilyHome_2F','STEEL_FAMILY_HOME_2F',sw,l['height'],up,'LittlerootTown_BrendansHouse_2F')
PATH.write_text(json.dumps(DATA,indent=2)+'\n')
print('Authored independent Steel layouts and structural stubs.')
