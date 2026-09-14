#!/usr/bin/env python3
"""Run a fresh ROM through Steel's opening using a headless mGBA core.
Usage: python3 dev_scripts/test_steel_opening_emulator.py /path/to/steel-mgba-runner
The runner never opens the user's .sav file.
"""
from pathlib import Path
import subprocess,sys,struct,json,tempfile,re
ROOT=Path(__file__).resolve().parents[1]
class Emulator:
    def __init__(self,runner):
        self.symbols={parts[2]:int(parts[0],16) for line in subprocess.check_output(['arm-none-eabi-nm',str(ROOT/'pokeemerald.elf')],text=True).splitlines() if len(parts:=line.split())==3}
        self.p=subprocess.Popen([runner,str(ROOT/'pokeemerald.gba')],stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.DEVNULL,text=True,bufsize=1)
        assert self.p.stdout.readline().strip()=='ready'
        with tempfile.TemporaryDirectory(prefix='steel-offsets-') as tmp:
            c=Path(tmp)/'offsets.c'; asm=Path(tmp)/'offsets.s'
            c.write_text('''#include "global.h"
#include "battle.h"
#include "constants/items.h"
#include "constants/species.h"
const unsigned offsets[]={
__builtin_offsetof(struct SaveBlock1,vars),
__builtin_offsetof(struct SaveBlock1,flags),
__builtin_offsetof(struct SaveBlock1,playerPartyCount),
__builtin_offsetof(struct SaveBlock1,playerParty),
__builtin_offsetof(struct SaveBlock1,bag.items),
__builtin_offsetof(struct SaveBlock1,bag.keyItems),
__builtin_offsetof(struct SaveBlock2,encryptionKey),
__builtin_offsetof(struct Pokemon,box.secure),
__builtin_offsetof(struct Pokemon,hp),
__builtin_offsetof(struct Pokemon,maxHP),
sizeof(struct Pokemon), NUM_SUBSTRUCT_BYTES,
__builtin_offsetof(struct BattlePokemon,species),
__builtin_offsetof(struct BattlePokemon,hp),
__builtin_offsetof(struct BattlePokemon,maxHP),
sizeof(struct BattlePokemon), sizeof(struct ItemSlot),
__builtin_offsetof(struct ItemSlot,itemId),
__builtin_offsetof(struct ItemSlot,quantity),
BAG_ITEMS_COUNT, BAG_KEYITEMS_COUNT, SPECIES_ZIGZAGOON, SPECIES_SANDSHREW,
SPECIES_TAILLOW, ITEM_SILK_SCARF};
''')
            subprocess.run(['arm-none-eabi-gcc','-mabi=apcs-gnu','-mthumb','-mcpu=arm7tdmi','-std=gnu17','-DMODERN=1','-DEMERALD=1','-iquote',str(ROOT/'include'),'-S',str(c),'-o',str(asm)],check=True)
            values=list(map(int,re.findall(r'\.word\s+(\d+)',asm.read_text())))
            (self.vars_offset,self.flags_offset,self.party_count_offset,self.party_offset,
             self.bag_items_offset,self.bag_key_items_offset,self.encryption_key_offset,self.mon_secure_offset,
             self.mon_hp_offset,self.mon_max_hp_offset,self.mon_size,self.substruct_size,
             self.battle_species_offset,self.battle_hp_offset,self.battle_max_hp_offset,
             self.battle_mon_size,self.item_slot_size,self.item_id_offset,
             self.item_quantity_offset,self.bag_items_count,self.bag_key_items_count,self.species_zigzagoon,
             self.species_sandshrew,self.species_taillow,self.item_silk_scarf)=values
        self.frames=0
        self.history=[]
    def cmd(self,s):
        self.p.stdin.write(s+'\n');self.p.stdin.flush()
        result=self.p.stdout.readline().strip()
        if not result:raise RuntimeError('mGBA runner exited: '+str(self.p.poll()))
        return result
    def read(self,a,n):return bytes.fromhex(self.cmd(f'read {a:x} {n}'))
    def write(self,a,value,size):return self.cmd(f'write {a:x} {value:x} {size}')
    def u32(self,a):return int.from_bytes(self.read(a,4),'little')
    def sym(self,n):return self.symbols[n]
    def step(self,n=1,keys=0):self.cmd(f'frame {n} {keys:x}');self.frames+=n
    def tap(self,key):self.step(1,key);self.step(20)
    def sb1(self):return self.u32(self.sym('gSaveBlock1Ptr'))
    def stage(self):
        p=self.sb1()
        return int.from_bytes(self.read(p+self.vars_offset+2*0xf7,2),'little') if 0x2000000<=p<0x2040000 else None
    def var(self,index):return int.from_bytes(self.read(self.sb1()+self.vars_offset+2*index,2),'little')
    def flag(self,index):return bool(self.read(self.sb1()+self.flags_offset+index//8,1)[0]&(1<<(index%8)))
    def party_count(self):return self.read(self.sym('gPartiesCount'),1)[0]
    def party_hp(self):return int.from_bytes(self.read(self.sym('gParties')+self.mon_hp_offset,2),'little')
    def party_max_hp(self):return int.from_bytes(self.read(self.sym('gParties')+self.mon_max_hp_offset,2),'little')
    def decode_mon_species(self,raw):
        personality,ot_id=struct.unpack_from('<II',raw)
        secure=bytearray(raw[self.mon_secure_offset:self.mon_secure_offset+4*self.substruct_size])
        key=personality^ot_id
        for offset in range(0,len(secure),4):
            struct.pack_into('<I',secure,offset,struct.unpack_from('<I',secure,offset)[0]^key)
        sub0_offsets=(0,0,0,0,0,0,1,1,2,3,2,3,1,1,2,3,2,3,1,1,2,3,2,3)
        return struct.unpack_from('<H',secure,self.substruct_size*sub0_offsets[personality%24])[0]&0x7ff
    def trainer_party_species(self,trainer):
        address=self.sym('gParties')+trainer*6*self.mon_size
        return self.decode_mon_species(self.read(address,self.mon_size))
    def party_species(self):return self.trainer_party_species(0)
    def item_quantity(self,item):
        key=int.from_bytes(self.read(self.u32(self.sym('gSaveBlock2Ptr'))+self.encryption_key_offset,2),'little')
        for offset,count in ((self.bag_items_offset,self.bag_items_count),
                             (self.bag_key_items_offset,self.bag_key_items_count)):
            raw=self.read(self.sb1()+offset,self.item_slot_size*count)
            for i in range(count):
                base=i*self.item_slot_size
                item_id=int.from_bytes(raw[base+self.item_id_offset:base+self.item_id_offset+2],'little')
                if item_id==item:
                    quantity=int.from_bytes(raw[base+self.item_quantity_offset:base+self.item_quantity_offset+2],'little')
                    return quantity^key
        return 0
    def battle_species(self,battler):
        address=self.sym('gBattleMons')+battler*self.battle_mon_size+self.battle_species_offset
        return int.from_bytes(self.read(address,2),'little')
    def set_battle_hp(self,battler,hp):
        self.write(self.sym('gBattleMons')+battler*self.battle_mon_size+self.battle_hp_offset,hp,2)
    def location(self):return tuple(self.read(self.sb1()+4,2))
    def player(self):
        raw=self.read(self.sym('gObjectEvents'),36*16)
        for i in range(16):
            o=raw[i*36:(i+1)*36]
            if int.from_bytes(o[:4],'little')&0x10001==0x10001:
                x,y=struct.unpack_from('<hh',o,16);return x-7,y-7
        return None
    def object_position(self,local_id):
        raw=self.read(self.sym('gObjectEvents'),36*16)
        for i in range(16):
            o=raw[i*36:(i+1)*36]
            if int.from_bytes(o[:4],'little')&1 and o[8]==local_id:
                x,y=struct.unpack_from('<hh',o,16)
                return x-7,y-7
        return None
    def advance(self,predicate,limit=16000):
        for _ in range(limit//20):
            if predicate():return
            self.step(2,1);self.step(18)
            stage=self.stage()
            if not self.history or self.history[-1]!=stage:
                self.history.append(stage);print('stage',stage,'map',self.location(),'player',self.player(),flush=True)
        self.cmd('shot /tmp/steel-stuck.ppm')
        self.cmd('save /tmp/steel-stuck.state')
        raise AssertionError(('opening did not progress',self.stage(),self.location(),self.player(),hex(self.u32(self.sym('sGlobalScriptContext')+8))))
    def walk(self,key,frames=16):self.step(frames,key);self.step(2)
    def goto(self,x,y):
        for target,axis in [(x,0),(y,1)]:
            for _ in range(100):
                pos=self.player()
                if pos[axis]==target:break
                key=(16 if target>pos[axis] else 32) if axis==0 else (128 if target>pos[axis] else 64)
                self.walk(key,8)
            else:raise AssertionError(('blocked walk',self.location(),self.player(),x,y))
    def assert_kyle(self,where):
        flags=((0x20,'SCHOOL'),(0x21,'VILLAGE'),(0x22,'RIDGE'),
               (0x23,'HOME'),(0x24,'WOODS'),(0x2d,'REGISTRATION'))
        active=[name for flag,name in flags if not self.flag(flag)]
        assert active==[where],(active,where,self.stage())
    def close(self):
        self.cmd('shot /tmp/steel-last.ppm')
        self.p.stdin.write('quit\n');self.p.stdin.flush();self.p.wait()

def main():
    e=Emulator(sys.argv[1]);e.step(400);e.tap(8)
    try:
        e.advance(lambda:e.stage()==12)
        e.step(100)
        print('PASS: new game, school roll call, Alumina escort, ridge escort, catching tutorial, return home',e.history)
        e.cmd('save /tmp/steel-home.state')
        e.assert_kyle('HOME')
        e.walk(128,32);e.step(120)
        assert e.location()[1]==1,('home exit',e.location(),e.player())
        e.assert_kyle('HOME')
        e.goto(8,16);e.goto(30,16);e.goto(30,19);e.goto(35,19);e.walk(16,40);e.step(90)
        assert e.location()[1]==4,('village entry',e.location(),e.player())
        e.goto(3,22);e.goto(12,22);e.goto(12,19);e.goto(22,19);e.goto(22,20);e.goto(30,20);e.goto(30,19);e.goto(40,19);e.goto(40,13);e.walk(64,24);e.step(120)
        assert e.location()[1]==0,('school entry',e.location(),e.player())
        e.assert_kyle('HOME')
        e.walk(128,32);e.step(120)
        assert e.location()[1]==4
        e.goto(40,19);e.goto(30,19);e.goto(30,20);e.goto(22,20);e.goto(22,19);e.goto(12,19);e.goto(12,22);e.goto(3,22);e.goto(3,19);e.goto(0,19);e.walk(32,32);e.step(80)
        assert e.location()[1]==1
        e.goto(30,19);e.goto(30,16);e.goto(0,16);e.walk(32,32);e.step(80)
        assert e.location()[1]==1,('ridge west edge remains bounded',e.location(),e.player())
        e.goto(30,16);e.goto(30,19);e.goto(35,19);e.walk(16,32);e.step(80)
        assert e.location()[1]==4,('village re-entry',e.location(),e.player())
        e.goto(3,22);e.goto(12,22);e.goto(12,19);e.goto(22,19);e.goto(22,20);e.goto(30,20);e.goto(30,19);e.goto(46,19);e.walk(16,32);e.step(80)
        assert e.location()[1]==4,('old direct Route 1 edge remains closed',e.location(),e.player())
        e.goto(30,19);e.goto(30,20);e.goto(24,20);e.goto(24,23);e.goto(23,23);e.goto(23,25);e.goto(24,25);e.goto(24,33);e.walk(128,16);e.step(100)
        assert e.location()[1]==6,('Alumina Southwoods trailhead',e.location(),e.player())
        e.walk(64,16);e.step(100)
        assert e.location()[1]==4,('Southwoods return to Alumina',e.location(),e.player())
        assert e.stage()==12
        print('PASS: home/school/Alumina/ridge re-entry and revised Southwoods trailhead; Kyle remains exclusively at home')
    finally:e.close()
if __name__=='__main__':main()
