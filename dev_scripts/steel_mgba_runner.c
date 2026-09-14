// Headless mGBA test driver. Uses a fresh, in-memory cartridge save.
// Build against mGBA with the same defines as its core library.
#include <mgba/core/core.h>
#include <mgba/core/log.h>
#include <mgba/gba/core.h>
#include <mgba-util/vfs.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
static void quiet(struct mLogger *l, int category, enum mLogLevel level, const char *fmt, va_list args) {}
int main(int argc, char **argv)
{
    if (argc != 2) return 2;
    struct mLogger logger = {.log = quiet};
    mLogSetDefaultLogger(&logger);
    struct mCore *core = GBACoreCreate();
    if (!core || !core->init(core)) return 3;
    mCoreInitConfig(core, "steel-tests");
    mCoreConfigSetDefaultValue(&core->config, "idleOptimization", "remove");
    mCoreLoadConfig(core);
    mColor *pixels = calloc(240 * 160, sizeof(mColor));
    core->setVideoBuffer(core, pixels, 240);
    if (!core->loadROM(core, VFileOpen(argv[1], O_RDONLY))) return 4;
    if (!core->loadSave(core, VFileMemChunk(NULL, 128 * 1024))) return 11;
    core->reset(core);
    char line[1024], path[900];
    unsigned int a, b, c;
    puts("ready"); fflush(stdout);
    while (fgets(line, sizeof(line), stdin))
    {
        if (sscanf(line, "frame %u %x", &a, &b) == 2)
        {
            core->setKeys(core, b);
            for (unsigned int i=0; i<a; i++) core->runFrame(core);
            puts("ok");
        }
        else if (sscanf(line, "read %x %u", &a, &b) == 2)
        {
            for (unsigned int i=0; i<b; i++) printf("%02x", core->busRead8(core, a+i));
            puts("");
        }
        else if (sscanf(line, "write %x %x %u", &a, &b, &c) == 3)
        {
            if (c==4) core->busWrite32(core,a,b);
            else if (c==2) core->busWrite16(core,a,b);
            else core->busWrite8(core,a,b);
            puts("ok");
        }
        else if (sscanf(line, "shot %899s", path) == 1)
        {
            FILE *f=fopen(path,"wb");
            if (!f) return 5;
            fprintf(f,"P6\n240 160\n255\n");
            for(int i=0;i<240*160;i++) {fputc(pixels[i]&255,f);fputc((pixels[i]>>8)&255,f);fputc((pixels[i]>>16)&255,f);}
            fclose(f); puts("ok");
        }
        else if (sscanf(line, "save %899s", path) == 1)
        {
            size_t size=core->stateSize(core);void *buf=malloc(size);
            core->saveState(core,buf);FILE *f=fopen(path,"wb");fwrite(buf,1,size,f);fclose(f);free(buf);puts("ok");
        }
        else if (sscanf(line, "load %899s", path) == 1)
        {
            size_t size=core->stateSize(core);void *buf=malloc(size);FILE *f=fopen(path,"rb");
            if(!f || fread(buf,1,size,f)!=size)return 6;
            fclose(f);core->loadState(core,buf);free(buf);puts("ok");
        }
        else if (sscanf(line, "cartsave %899s", path) == 1)
        {
            void *sram = NULL;
            size_t size = core->savedataClone(core, &sram);
            FILE *f = fopen(path, "wb");
            size_t written = f ? fwrite(sram, 1, size, f) : 0;
            if (f) fclose(f);
            free(sram);
            if (written != size) printf("error %zu %zu\n", size, written);
            else puts("ok");
        }
        else if (sscanf(line, "cartload %899s", path) == 1)
        {
            FILE *f = fopen(path, "rb");
            if (!f) return 8;
            fseek(f, 0, SEEK_END); size_t size = ftell(f); rewind(f);
            void *sram = malloc(size);
            if (fread(sram, 1, size, f) != size) return 9;
            fclose(f);
            if (!core->savedataRestore(core, sram, size, false)) return 10;
            free(sram); puts("ok");
        }
        else if (sscanf(line,"reg %899s",path)==1) {a=0;core->readRegister(core,path,&a);printf("%08x\n",a);}
        else if (!strncmp(line,"reset",5)) {core->reset(core);puts("ok");}
        else if (!strncmp(line,"quit",4)) break;
        else puts("unknown");
        fflush(stdout);
    }
    mCoreConfigDeinit(&core->config);core->deinit(core);free(pixels);return 0;
}
