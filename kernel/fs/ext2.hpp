#include <inttypes.h>
#include <dev/sd.hpp>

#define EXT2_SIGNATURE 0xEF53

typedef struct Ext2Superblock {
	uint32_t inodesTotal;
	uint32_t blocksTotal;
	uint32_t reservedSu; // These are the blocks that are reserved for the superuser
	uint32_t unallocatedBlocks;
	uint32_t unallocatedInodes;
	uint32_t superblock;
	uint32_t blockSize;
	uint32_t fragmentSize;
	uint32_t blocksPerGroup;
	uint32_t fragmentsPerGroup;
	uint32_t inodesPerGroup;
	uint32_t lastMount;
	uint32_t lastWritten;
	uint16_t mountedSinceCheck;
	uint16_t mountsUntilCheck;
	uint16_t signature; // For ext2 this must be 0xEF53
	uint16_t state;
	uint16_t errorAction;
	uint16_t minorVersion;
	uint32_t lastCheck;
	uint32_t checkInterval;
	uint32_t osId;
	uint32_t majorVersion;
	uint16_t userUseReserved;
	uint16_t groupUseReserved;

	/* Extended Superblock */
	uint32_t firstNonReservedInode;
	uint16_t inodeSize;
	uint16_t blockGroup;
	uint32_t optionalFeatures;
	uint32_t requiredFeatures;
	uint32_t readOnlyFeatures;
	uint8_t fsId[16];
	uint8_t volumeName[16];
	uint8_t lastMountPath[64];
} __attribute__((packed)) Ext2Superblock;

typedef struct Ext2BgDescriptor {
	uint32_t blockUsageMapBlock;
	uint32_t inodeUsageMapBlock;
	uint32_t inodeTableStart;
	uint16_t unallocatedBlocksInGroup;
	uint16_t unallocatedInodesInGroup;
	uint16_t directoriesInGroup;
	uint8_t reserved[14];
} __attribute__((packed)) Ext2BgDescriptor;

enum ext2_inode_type {
	EXT2_INODE_TYPE_FIFO = 0x1000,
	EXT2_INODE_TYPE_CHARDEV = 0x2000,
	EXT2_INODE_TYPE_DIRECTORY = 0x4000,
	EXT2_INODE_TYPE_BLOCKDEV = 0x6000,
	EXT2_INODE_TYPE_REGULAR = 0x8000,
	EXT2_INODE_TYPE_SYMBOLIC_LINK = 0xA000,
	EXT2_INODE_TYPE_SOCKET = 0xC000
};

typedef struct Ext2Inode {
	uint16_t type;
	uint16_t userId;
	uint32_t sizeLow;
	uint32_t lastAccess;
	uint32_t creationTime;
	uint32_t lastModification;
	uint32_t deletionTime;
	uint16_t groupId;
	uint16_t hardLinksCount;
	uint32_t diskSectorsCount;
	uint32_t flags;
	uint32_t osSpecific;
	uint32_t directBlockPtr[12];
	uint32_t indirectSinglyBlockPtr;
	uint32_t indirectDoublyBlockPtr;
	uint32_t indirectTriplyBlockPtr;
	uint32_t generationNumber;
	uint32_t extendedAttribute;
	uint32_t sizeHigh;
	uint32_t fragmentBlock;
	uint8_t reserved[12];
} __attribute__((packed)) Ext2Inode;

typedef struct Ext2DirEntry {
	uint32_t inode;
	uint16_t size;
	uint8_t nameLength;
	uint8_t type;
	uint8_t nameReserved;
} __attribute__((packed)) Ext2DirEntry;

class Ext2Fs {
    private:
        StorageDevice *dev;

        uint32_t startSector;
        uint32_t blockSize;
        uint32_t inodesPerBlock;
        uint32_t blockGroupsTotal;
        uint16_t inodeSize;

        Ext2BgDescriptor *bgdt;
        Ext2Superblock *sb;
        uint8_t *rootBuf;

        uint32_t blockToSector(uint32_t block);
        void readBlock(uint32_t block, uint8_t *buf);
        void readSinglyLinked(uint32_t blockPtr, uint8_t *buf);
        void readDoublyLinked(uint32_t blockPtr, uint8_t *buf);
        void readInode(uint32_t inode, Ext2Inode *buf);
        uint32_t findInode(char* path);

    public:
        Ext2Fs(StorageDevice *dev, uint32_t startLba);

        int read(char *path, uint8_t *buf);
};