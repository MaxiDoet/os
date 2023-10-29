#include <fs/ext2.hpp>
#include <mem/heap.hpp>
#include <lib/string.hpp>
#include <debug.hpp>

uint32_t Ext2Fs::blockToSector(uint32_t block)
{
    uint32_t sector = (block * blockSize) / 512;

    return sector;
}

void Ext2Fs::readBlock(uint32_t block, uint8_t *buf)
{
    if (block > sb->blocksTotal) {
        debugPrint("EXT2 | Attempted to read invalid block! (%d)\n", block);
    }

    dev->read(startSector + blockToSector(block), blockSize / 512, buf);
}

void Ext2Fs::readSinglyLinked(uint32_t blockPtr, uint8_t *buf)
{
    uint16_t entriesMax = blockSize / sizeof(uint32_t);

    uint8_t *blockListBuf = (uint8_t *) heapAlloc(blockSize);
    readBlock(blockPtr, blockListBuf);
    uint32_t *block = (uint32_t *) blockListBuf;

    for (uint32_t i=0; i < entriesMax; i++) {
		if (block[i] == 0) {
			// EOF
			break;
		}

		uint8_t *blockBuf = (uint8_t *) heapAlloc(blockSize);
		readBlock(block[i], blockBuf);

		memcpy(buf + (i * blockSize), blockBuf, blockSize);
		heapFree(blockBuf);
	}
}

void Ext2Fs::readDoublyLinked(uint32_t blockPtr, uint8_t *buf)
{
    uint16_t entriesMax = blockSize / sizeof(uint32_t);

    uint8_t *blockListBuf = (uint8_t *) heapAlloc(blockSize);
    readBlock(blockPtr, blockListBuf);
	uint32_t *block = (uint32_t *) blockListBuf;

	for (uint32_t i=0; i < entriesMax; i++) {
		if (block[i] == 0) {
			// EOF
			break;
		}

		readSinglyLinked(block[i], buf + i * (blockSize * blockSize / 4));
	}

	heapFree(blockListBuf);
}

void Ext2Fs::readInode(uint32_t inode, Ext2Inode *buf)
{
	if (inode > sb->inodesTotal) {
		debugPrint("EXT2 | Attempted to read invalid inode!\n", inode);
		return;
	}

	uint32_t bg = (inode - 1) / sb->inodesPerGroup;
	uint32_t bgIndex = (inode - 1) % sb->inodesPerGroup;
	uint32_t block = (bgIndex * inodeSize) / blockSize;
	uint32_t blockIndex = bgIndex % inodesPerBlock;

	Ext2BgDescriptor *bgDescriptor = &bgdt[bg];

	uint8_t *blockBuf = (uint8_t *) heapAlloc(blockSize);
	readBlock(bgDescriptor->inodeTableStart + block, blockBuf);

	memcpy(buf, &blockBuf[blockIndex * inodeSize], sizeof(Ext2Inode));

	heapFree(blockBuf);
}

uint32_t Ext2Fs::findInode(char* path)
{
	uint32_t result = 0;
	uint32_t current = strsplit(path, '/');
	char* filename = (char *) path + 1; // Remove slash
	Ext2DirEntry *dir;
	Ext2Inode *inodeBuf = (Ext2Inode *) heapAlloc(sizeof(Ext2Inode));

	if (current == 1) {
		// Root is always inode 2
	    readInode(2, inodeBuf);

		for (uint32_t i=0; i < 12; i++) {
			uint32_t block = inodeBuf->directBlockPtr[i];
			readBlock(block, rootBuf);
			dir = (Ext2DirEntry *) rootBuf;

			debugPrint("block: %d\n", block);

			while(dir->inode != 0) {
				char* name = (char *) heapAlloc(dir->nameLength + 1);
				memcpy(name, &dir->nameReserved, dir->nameLength);
				name[dir->nameLength] = '\0';

				if (strcmp(name, filename) == 0) {
					result = dir->inode;

					break;
				}
				
				heapFree(name);

				dir = (Ext2DirEntry *) ((uint32_t) dir + dir->size);
			}
		}
	}

	heapFree(inodeBuf);

	return result;
}

Ext2Fs::Ext2Fs(StorageDevice *dev, uint32_t startLba) {
    this->dev = dev;

	uint8_t *sbBuf = (uint8_t *) heapAlloc(1024);
    dev->read(startLba + 2, 2, sbBuf);
	
	Ext2Superblock *sb = (Ext2Superblock *) sbBuf;

	startSector = startLba;
	this->sb = sb;

	inodeSize = 128;

	if (sb->majorVersion >= 1) {
		debugPrint("EXT2 | Using extended superblock\n");

		inodeSize = sb->inodeSize;
	}

	blockSize = (1024 << sb->blockSize);
	inodesPerBlock = blockSize / inodeSize;
	blockGroupsTotal = sb->blocksTotal / sb->blocksPerGroup + 1;
 
	debugPrint("EXT2 | Block size: %d Inode size: %d\n", blockSize, inodeSize);

	/* Read block group descriptor table */
	uint8_t sectorCount = (sizeof(Ext2BgDescriptor) * blockGroupsTotal) / 512 + 1;
	bgdt = (Ext2BgDescriptor *) heapAlloc(sectorCount * 512);
	
	//dev->read(startLba + blockToSector(2), sectorCount, (uint8_t *) bgdt);
	readBlock(1, (uint8_t *) bgdt);

	// Init buffers
    rootBuf = (uint8_t *) heapAlloc(blockSize);
}

int Ext2Fs::read(char *path, uint8_t *buf)
{
    uint32_t inode = findInode(path);
    if (inode == 0) return -1;

	Ext2Inode *inodeBuf = (Ext2Inode *) heapAlloc(sizeof(Ext2Inode));
	readInode(inode, inodeBuf);

    for (uint32_t i=0; i < 12; i++) {
		uint32_t block = inodeBuf->directBlockPtr[i];
		if (block == 0) {
			// EOF
			return 0;
		}

		uint8_t *blockBuf = (uint8_t *) heapAlloc(blockSize);
		readBlock(block, blockBuf);
		memcpy(buf + (i * blockSize), blockBuf, blockSize);
		heapFree(blockBuf);
	}

	if (inodeBuf->indirectSinglyBlockPtr != 0) {
		readSinglyLinked(inodeBuf->indirectSinglyBlockPtr, buf + (12 * blockSize));
	}

	if (inodeBuf->indirectDoublyBlockPtr != 0) {
		readDoublyLinked(inodeBuf->indirectDoublyBlockPtr, buf + (12 * blockSize) + (blockSize * blockSize / 4));
	}

	heapFree(inodeBuf);

	return 1;
}