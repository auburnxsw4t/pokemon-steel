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
            c.write_text('#include "global.h"\nconst unsigned offsets[]={__builtin_offsetof(struct SaveBlock1,vars),__builtin_offsetof(struct SaveBlock1,flags)};\n')
            subprocess.run(['arm-none-eabi-gcc','-mabi=apcs-gnu','-mthumb','-mcpu=arm7tdmi','-std=gnu17','-DMODERN=1','-DEMERALD=1','-iquote',str(ROOT/'include'),'-S',str(c),'-o',str(asm)],check=True)
            self.vars_offset,self.flags_offset=map(int,re.findall(r'\.word\s+(\d+)',asm.read_text()))
        self.frames=0
        self.history=[]
    def cmd(self,s):
        self.p.stdin.write(s+'\n');self.p.stdin.flush()
        result=self.p.stdout.readline().strip()
        if not result:raise RuntimeError('mGBA runner exited: '+str(self.p.poll()))
        return result
    def read(self,a,n):return bytes.fromhex(self.cmd(f'read {a:x} {n}'))
    def u32(self,a):return int.from_bytes(self.read(a,4),'little')
    def sym(self,n):return self.symbols[n]
    def step(self,n=1,keys=0):self.cmd(f'frame {n} {keys:x}');self.frames+=n
    def tap(self,key):self.step(1,key);self.step(20)
    def sb1(self):return self.u32(self.sym('gSaveBlock1Ptr'))
    def stage(self):
        p=self.sb1()
        return int.from_bytes(self.read(p+self.vars_offset+2*0xf7,2),'little') if 0x2000000<=p<0x2040000 else None
    def location(self):return tuple(self.read(self.sb1()+4,2))
    def player(self):
        raw=self.read(self.sym('gObjectEvents'),36*16)
        for i in range(16):
            o=raw[i*36:(i+1)*36]
            if int.from_bytes(o[:4],'little')&0x10001==0x10001:
                x,y=struct.unpack_from('<hh',o,16);return x-7,y-7
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
        raw=self.read(self.sb1()+self.flags_offset+4,1)[0]
        active=[name for i,name in enumerate(['SCHOOL','VILLAGE','RIDGE','HOME','WOODS']) if not raw&(1<<i)]
        assert active==[where],(active,where,self.stage())
    def close(self):
        self.cmd('shot /tmp/steel-last.ppm')
        self.p.stdin.write('quit\n');self.p.stdin.flush();self.p.wait()

def main():
    e=Emulator(sys.argv[1]);e.step(400);e.tap(8)
    try:
        e.advance(lambda:e.stage()==3)
        e.step(100)
        print('PASS: new game, school roll call, Alumina escort, ridge escort, home arrival',e.history)
        e.cmd('save /tmp/steel-home.state')
        e.assert_kyle('HOME')
        e.walk(128,32);e.step(120)
        assert e.location()[1]==1,('home exit',e.location(),e.player())
        e.assert_kyle('HOME')
        e.goto(8,16);e.goto(35,16);e.walk(16,40);e.step(90)
        assert e.location()[1]==4,('village entry',e.location(),e.player())
        e.goto(32,16);e.goto(32,8);e.walk(64,24);e.step(120)
        assert e.location()[1]==0,('school entry',e.location(),e.player())
        e.assert_kyle('HOME')
        e.walk(128,32);e.step(120)
        assert e.location()[1]==4
        e.goto(32,16);e.goto(0,16);e.walk(32,32);e.step(80)
        assert e.location()[1]==1
        e.goto(0,16);e.walk(32,32);e.step(80)
        assert e.location()[1]==5,('woods entry',e.location(),e.player())
        e.assert_kyle('HOME')
        e.goto(27,16);e.walk(16,32);e.step(80)
        assert e.location()[1]==1
        assert e.stage()==3
        print('PASS: home/school/Alumina/ridge/woods re-entry; Kyle remains exclusively at home')
    finally:e.close()
if __name__=='__main__':main()
