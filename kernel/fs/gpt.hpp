#include <inttypes.h>

typedef struct GptTableHeader {
    uint8_t signature[8];
    uint8_t revision[4];
    uint32_t size;
    uint32_t headerChecksum;
    uint8_t reserved[4];
    uint64_t headerLba;
    uint64_t backupLba;
    uint64_t startLba;
    uint64_t lastLba;
    uint8_t guid[16];
    uint64_t tableLba;
    uint32_t tableEntryCount;
    uint32_t tableEntrySize;
    uint32_t tableChecksum;
} __attribute__((packed)) GptTableHeader;

typedef struct GptTableEntry {
    uint8_t typeGuid[16];
    uint8_t guid[16];
    uint64_t startLba;
    uint64_t lastLba;
    uint64_t attributes;
    uint8_t name[72];
} __attribute__((packed)) GptTableEntry;