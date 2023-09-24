#include <cpu/gdt.hpp>
#include <inttypes.h>

typedef struct GdtDescriptor {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) GdtDescriptor;

typedef struct GdtEntry {
    uint16_t limitLower;
    uint16_t baseLower;
    uint8_t baseMiddle;
    uint8_t accessRights;
    uint8_t flagsLimitHigher;
    uint8_t baseHigher;
} __attribute__((packed)) GdtEntry;

GdtDescriptor gdtDescriptor;
GdtEntry gdt[3];

extern "C" {
    extern void gdt_install();
}

static void gdtSetEntry(unsigned int pos, uint32_t base, uint32_t limit, uint8_t accessRights, uint8_t flags)
{
    gdt[pos].baseLower = base & 0xffff;
    gdt[pos].baseMiddle = (base >> 16) & 0xff;
    gdt[pos].baseHigher = (base >> 24) & 0xffff;

    gdt[pos].limitLower = limit & 0xffff;
    gdt[pos].flagsLimitHigher = ((flags << 4) & 0xf0) | ((limit >> 16) & 0x0f);

    gdt[pos].accessRights = accessRights;
}

void gdtSetup()
{
    gdtDescriptor.limit = 3 * sizeof(GdtEntry) - 1;
    gdtDescriptor.base = (uint32_t) (uintptr_t) gdt;

    gdtSetEntry(0, 0, 0, 0, 0);
    gdtSetEntry(1, 0, 0xfffff, 0x9a, 0xcf);
    gdtSetEntry(2, 0, 0xfffff, 0x92, 0xcf);

    gdt_install();
}