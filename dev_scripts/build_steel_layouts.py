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
def semantic_grid(filename, archive_name='Pokemon_Steel_Chapter1_Map_Construction_V2.zip'):
    archive=ROOT/'docs/pokemon_steel/reference/chapter1'/archive_name
    with zipfile.ZipFile(archive) as z:
        rows=list(csv.reader(io.TextIOWrapper(z.open(filename),encoding='utf-8-sig')))
    grid=[row[1:] for row in rows[1:]]
    assert grid and all(len(row)==len(grid[0]) for row in grid), filename
    return grid
def semantic_map(grid):
    h,w=len(grid),len(grid[0])
    result=Map(w,h)
    tree_w,tree_src=blocks('PetalburgCity')
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
                # A path-colored blocked tile was unreadable in playtesting.
                # Use a visibly solid shrub/rock placeholder until red-clay
                # scenery receives its final authored edge vocabulary.
                result.fill(x,y,1,1,0x016,True)
            elif code=='L':
                # Keep the authored shortcut lanes traversable until their final
                # one-way ledge metatiles are selected in Porymap.
                result.path(x,y,1,1)
            elif code=='M':
                # The old mine is a landmark, not a Chapter 1 entrance.
                result.fill(x,y,1,1,0x016,True)
            elif code=='X':
                # Only draw a tree quadrant when its complete 2x2 tree exists.
                # Irregular semantic edges use self-contained dense shrubs so
                # no half/chopped trees imply a walkable opening.
                ax,ay=x-x%2,y-y%2
                complete=(ay+1<h and ax+1<w and
                          all(grid[yy][xx]=='X' for yy in range(ay,ay+2) for xx in range(ax,ax+2)))
                if complete:
                    result.tiles[y*w+x]=tree_src[(y%2)*tree_w+x%2]
                else:
                    result.fill(x,y,1,1,0x016,True)
            else:
                raise ValueError((x,y,code))
    return result
# Syl's approved Alumina V2 semantic plan is authoritative for the village.
# Translate its zones into coherent Emerald metatiles, then stamp recognizable
# buildings at the exact approved door cells.  The east Route 1 reservation is
# deliberately scenery-only: Chapter 1 progression goes south through Longleaf.
ag=semantic_grid('alumina_village_tile_grid_v2.csv',
                 'Pokemon_Steel_Alumina_Village_Construction_V2.zip')
v=Map(len(ag[0]),len(ag)); tree_w,tree_src=blocks('PetalburgCity')
for y,row in enumerate(ag):
    for x,code in enumerate(row):
        if code in ('P','D','B','L'): v.path(x,y,1,1)
        elif code in ('G','C'): v.fill(x,y,1,1,0x001)
        elif code=='W': v.fill(x,y,1,1,0x0A1,True,1)
        elif code in ('F','V'): v.fill(x,y,1,1,0x016,True)
        elif code in ('H','R'): v.fill(x,y,1,1,0x001)
        elif code=='T': v.fill(x,y,1,1,0x002,True)
        elif code=='X':
            ax,ay=x-x%2,y-y%2
            complete=(ay+1<len(ag) and ax+1<len(row) and
                      all(ag[yy][xx]=='X' for yy in range(ay,ay+2) for xx in range(ax,ax+2)))
            if complete: v.tiles[y*v.w+x]=tree_src[(y%2)*tree_w+x%2]
            else: v.fill(x,y,1,1,0x016,True)
        else: raise ValueError((x,y,code))
# lake shore, homes, school, chapel and public services
v.water(2,2,13,11)
v.house(10,7)                 # resident A, door (12,10)
v.house(31,7)                 # resident B, door (33,10)
v.house(38,9)                 # school, door (40,12)
v.stamp('PetalburgCity',12,4,6,6,4,16)   # chapel, door (7,20)
v.stamp('PetalburgCity',24,10,4,4,16,28) # Mart, door (17,30)
v.stamp('PetalburgCity',19,14,4,4,27,28) # Center, door (28,30)
v.stamp('PetalburgCity',12,4,6,6,36,25)  # Registration, door (39,29)
# The semantic west road meets the chapel footprint. Route the public path
# visibly around its south wall so Homestead remains directly connected.
v.path(2,18,2,6);v.path(2,22,11,2);v.path(11,18,2,6)
# Registration's south-facing door needs a legible public approach through the
# landscaping band.
v.path(35,31,5,1)
# A compact civic marker anchors the plaza while the approved east-west spine
# remains fully open through rows 18-20.
v.fill(25,17,1,1,0x002,True)
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
# Bend the approved east Alumina road down three cells so it meets Syl's V2
# west gate directly without relying on a fragile offset connection.
r.path(30,15,6,6)
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
for name,ident,source in [('Steel_AluminaSchool','STEEL_ALUMINA_SCHOOL','RustboroCity_PokemonSchool'),
                          ('Steel_LeagueRegistration','STEEL_LEAGUE_REGISTRATION','RustboroCity_DevonCorp_1F')]:
 l=LAYOUTS[source+'_Layout'];sw,src=blocks(source)
 write_layout(name,ident,sw,l['height'],src,source)
# Revised 16x12 downstairs: west living/trophy area, northeast kitchen/trainer
# area, north-center starter counter and a clear south-center foyer.
home=Map(16,12);home.fill(0,0,16,12,0x201)
home.fill(0,0,16,1,0x204,True);home.fill(0,11,16,1,0x204,True)
home.fill(0,0,1,12,0x204,True);home.fill(15,0,1,12,0x204,True)
# Existing Emerald furnishing stamps keep the expanded room visually grounded.
home.stamp('LittlerootTown_BrendansHouse_1F',0,3,7,6,1,4)
home.stamp('LittlerootTown_BrendansHouse_1F',5,3,6,6,9,4)
# Indoor floor only: outdoor path metatiles are invalid in this tileset and
# render as the bright-magenta corruption caught in playtesting.
home.fill(6,4,4,7,0x201)
for x in range(7,10): home.tiles[3*home.w+x]=0x3293
# stair landing in the northeast; exterior door tiles remain open at (8,11)/(9,11)
home.tiles[2*home.w+14]=0x3208
home.tiles[11*home.w+8]=0x0202;home.tiles[11*home.w+9]=0x0203
write_layout('Steel_FamilyHome','STEEL_FAMILY_HOME',home.w,home.h,home.tiles,'LittlerootTown_BrendansHouse_1F')
# Revised 18x12 upstairs: player room west, parents north-center and Kyle east,
# all opening onto a broad southern hall and stair landing.
up=Map(18,12);up.fill(0,0,18,12,0x201)
up.fill(0,0,18,1,0x204,True);up.fill(0,11,18,1,0x204,True)
up.fill(0,0,1,12,0x204,True);up.fill(17,0,1,12,0x204,True)
for x in (6,11):
    up.fill(x,1,1,6,0x204,True)
    up.fill(x,6,1,1,0x201)
up.stamp('LittlerootTown_BrendansHouse_2F',0,2,4,5,1,1)
up.stamp('LittlerootTown_BrendansHouse_2F',4,2,4,5,7,1)
up.stamp('LittlerootTown_BrendansHouse_2F',4,2,4,5,12,1)
up.tiles[10*up.w+9]=0x3208
write_layout('Steel_FamilyHome_2F','STEEL_FAMILY_HOME_2F',up.w,up.h,up.tiles,'LittlerootTown_BrendansHouse_2F')
PATH.write_text(json.dumps(DATA,indent=2)+'\n')
print('Authored independent Steel layouts and structural stubs.')
