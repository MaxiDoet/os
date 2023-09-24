#include <fs/fs.hpp>
#include <fs/gpt.hpp>
#include <fs/ext2.hpp>
#include <mem/heap.hpp>
#include <lib/string.hpp>
#include <debug.hpp>

const uint8_t gptSignature[8] = {0x45, 0x46, 0x49, 0x20, 0x50, 0x41, 0x52, 0x54};

Fs fsList[10];
int fsListIndex = 0;

void fsProbe(StorageDevice *dev)
{
    debugPrint("FS | Probing for filesystems on %s\n", dev->getIdentifier());

    // Read GPT Header
    uint8_t *gptTableHeaderBuf = (uint8_t *) heapAlloc(sizeof(GptTableHeader));
    dev->read(1, 1, gptTableHeaderBuf);
    GptTableHeader *gptTableHeader = (GptTableHeader *) gptTableHeaderBuf;

    // Check GPT signature
    for (int i=0; i < 8; i++) {
		if (gptTableHeader->signature[i] != gptSignature[i]) {
			debugPrint("FS | Invalid gpt signature\n");
			return;
		}
	}

    uint8_t sectorCount = (gptTableHeader->tableEntryCount * gptTableHeader->tableEntrySize) / 512 + 1;
    uint8_t *gptTableBuf = (uint8_t *) heapAlloc(sectorCount * 512);
    dev->read(gptTableHeader->tableLba, 1, gptTableBuf);
	GptTableEntry *gptTable = (GptTableEntry *) gptTableBuf;

    while(gptTable->startLba != 0) {
        Ext2Fs *ext2FsBuf = (Ext2Fs *) heapAlloc(sizeof(Ext2Fs));
        Ext2Fs ext2Fs(dev, gptTable[0].startLba);
        memcpy(ext2FsBuf, &ext2Fs, sizeof(Ext2Fs));

        fsList[fsListIndex].privateData = (void *) ext2FsBuf;
        fsList[fsListIndex++].dev = dev;

        gptTable++;
    }
}

void fsRead(char *path, uint8_t *buf)
{
    /* Later this will be selected through the path */
    Fs *fs = &fsList[0];

    Ext2Fs *ext2Fs = (Ext2Fs *) fs->privateData;

    ext2Fs->read(path, buf);
}