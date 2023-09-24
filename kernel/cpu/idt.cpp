#include <cpu/idt.hpp>
#include <cpu/cpu.hpp>
#include <cpu/irq.hpp>
#include <drivers/pic.hpp>
#include <debug.hpp>
#include <inttypes.h>

typedef struct IdtEntry {
    uint16_t baseLower;
    uint16_t selector;
    uint8_t unused;
    uint8_t typeAttributes;
    uint16_t baseHigher;
} __attribute__((packed)) IdtEntry;

typedef struct IdtDescriptor {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) IdtDescriptor;


extern "C" {
extern void int_handler_0();
extern void int_handler_1();
extern void int_handler_2();
extern void int_handler_3();
extern void int_handler_4();
extern void int_handler_5();
extern void int_handler_6();
extern void int_handler_7();
extern void int_handler_8();
extern void int_handler_9();
extern void int_handler_10();
extern void int_handler_11();
extern void int_handler_12();
extern void int_handler_13();
extern void int_handler_14();
extern void int_handler_15();
extern void int_handler_16();
extern void int_handler_17();
extern void int_handler_18();
extern void int_handler_19();
extern void int_handler_20();
extern void int_handler_21();
extern void int_handler_22();
extern void int_handler_23();
extern void int_handler_24();
extern void int_handler_25();
extern void int_handler_26();
extern void int_handler_27();
extern void int_handler_28();
extern void int_handler_29();
extern void int_handler_30();
extern void int_handler_31();
extern void int_handler_32();
extern void int_handler_33();
extern void int_handler_34();
extern void int_handler_35();
extern void int_handler_36();
extern void int_handler_37();
extern void int_handler_38();
extern void int_handler_39();
extern void int_handler_40();
extern void int_handler_41();
extern void int_handler_42();
extern void int_handler_43();
extern void int_handler_44();
extern void int_handler_45();
extern void int_handler_46();
extern void int_handler_47();
extern void int_handler_48();
extern void int_handler_49();
extern void int_handler_50();
extern void int_handler_51();
extern void int_handler_52();
extern void int_handler_53();
extern void int_handler_54();
extern void int_handler_55();
extern void int_handler_56();
extern void int_handler_57();
extern void int_handler_58();
extern void int_handler_59();
extern void int_handler_60();
extern void int_handler_61();
extern void int_handler_62();
extern void int_handler_63();
extern void int_handler_64();
extern void int_handler_65();
extern void int_handler_66();
extern void int_handler_67();
extern void int_handler_68();
extern void int_handler_69();
extern void int_handler_70();
extern void int_handler_71();
extern void int_handler_72();
extern void int_handler_73();
extern void int_handler_74();
extern void int_handler_75();
extern void int_handler_76();
extern void int_handler_77();
extern void int_handler_78();
extern void int_handler_79();
extern void int_handler_80();
extern void int_handler_81();
extern void int_handler_82();
extern void int_handler_83();
extern void int_handler_84();
extern void int_handler_85();
extern void int_handler_86();
extern void int_handler_87();
extern void int_handler_88();
extern void int_handler_89();
extern void int_handler_90();
extern void int_handler_91();
extern void int_handler_92();
extern void int_handler_93();
extern void int_handler_94();
extern void int_handler_95();
extern void int_handler_96();
extern void int_handler_97();
extern void int_handler_98();
extern void int_handler_99();
}

IdtDescriptor idtDesc;
IdtEntry idt[256];

void idtSetGate(unsigned int pos, uint32_t base, uint16_t selector, uint8_t typeAttributes)
{
	idt[pos].baseLower = base & 0xffff;
	idt[pos].selector = selector;
	idt[pos].unused = 0x0;
	idt[pos].typeAttributes = typeAttributes;
	idt[pos].baseHigher = (base >> 16) & 0xffff;
}

void idtInstall()
{
    idtDesc.limit = 256 * sizeof(IdtEntry) - 1;
	idtDesc.base = (uint32_t) (uintptr_t) idt;

	idtSetGate(0, (uint32_t) &int_handler_0, 0x8, 0x8e);
	idtSetGate(1, (uint32_t) &int_handler_1, 0x8, 0x8e);
	idtSetGate(2, (uint32_t) &int_handler_2, 0x8, 0x8e);
	idtSetGate(3, (uint32_t) &int_handler_3, 0x8, 0x8e);
	idtSetGate(4, (uint32_t) &int_handler_4, 0x8, 0x8e);
	idtSetGate(5, (uint32_t) &int_handler_5, 0x8, 0x8e);
	idtSetGate(6, (uint32_t) &int_handler_6, 0x8, 0x8e);
	idtSetGate(7, (uint32_t) &int_handler_7, 0x8, 0x8e);
	idtSetGate(8, (uint32_t) &int_handler_8, 0x8, 0x8e);
	idtSetGate(9, (uint32_t) &int_handler_9, 0x8, 0x8e);
	idtSetGate(10, (uint32_t) &int_handler_10, 0x8, 0x8e);
	idtSetGate(11, (uint32_t) &int_handler_11, 0x8, 0x8e);
	idtSetGate(12, (uint32_t) &int_handler_12, 0x8, 0x8e);
 	idtSetGate(13, (uint32_t) &int_handler_13, 0x8, 0x8e);
	idtSetGate(14, (uint32_t) &int_handler_14, 0x8, 0x8e);
	idtSetGate(15, (uint32_t) &int_handler_15, 0x8, 0x8e);
	idtSetGate(16, (uint32_t) &int_handler_16, 0x8, 0x8e);
	idtSetGate(17, (uint32_t) &int_handler_17, 0x8, 0x8e);
	idtSetGate(18, (uint32_t) &int_handler_18, 0x8, 0x8e);
	idtSetGate(19, (uint32_t) &int_handler_19, 0x8, 0x8e);
	idtSetGate(20, (uint32_t) &int_handler_20, 0x8, 0x8e);
	idtSetGate(21, (uint32_t) &int_handler_21, 0x8, 0x8e);
	idtSetGate(22, (uint32_t) &int_handler_22, 0x8, 0x8e);
	idtSetGate(23, (uint32_t) &int_handler_23, 0x8, 0x8e);
	idtSetGate(24, (uint32_t) &int_handler_24, 0x8, 0x8e);
	idtSetGate(25, (uint32_t) &int_handler_25, 0x8, 0x8e);
	idtSetGate(26, (uint32_t) &int_handler_26, 0x8, 0x8e);
	idtSetGate(27, (uint32_t) &int_handler_27, 0x8, 0x8e);
	idtSetGate(28, (uint32_t) &int_handler_28, 0x8, 0x8e);
	idtSetGate(29, (uint32_t) &int_handler_29, 0x8, 0x8e);
	idtSetGate(30, (uint32_t) &int_handler_30, 0x8, 0x8e);
	idtSetGate(31, (uint32_t) &int_handler_31, 0x8, 0x8e);
	idtSetGate(32, (uint32_t) &int_handler_32, 0x8, 0x8e);
    idtSetGate(33, (uint32_t) &int_handler_33, 0x8, 0x8e);
    idtSetGate(34, (uint32_t) &int_handler_34, 0x8, 0x8e);
    idtSetGate(35, (uint32_t) &int_handler_35, 0x8, 0x8e);
    idtSetGate(36, (uint32_t) &int_handler_36, 0x8, 0x8e);
    idtSetGate(37, (uint32_t) &int_handler_37, 0x8, 0x8e);
    idtSetGate(38, (uint32_t) &int_handler_38, 0x8, 0x8e);
    idtSetGate(39, (uint32_t) &int_handler_39, 0x8, 0x8e);
    idtSetGate(40, (uint32_t) &int_handler_40, 0x8, 0x8e);
    idtSetGate(41, (uint32_t) &int_handler_41, 0x8, 0x8e);
    idtSetGate(42, (uint32_t) &int_handler_42, 0x8, 0x8e);
    idtSetGate(43, (uint32_t) &int_handler_43, 0x8, 0x8e);
    idtSetGate(44, (uint32_t) &int_handler_44, 0x8, 0x8e);
    idtSetGate(45, (uint32_t) &int_handler_45, 0x8, 0x8e);
    idtSetGate(46, (uint32_t) &int_handler_46, 0x8, 0x8e);
    idtSetGate(47, (uint32_t) &int_handler_47, 0x8, 0x8e);
	idtSetGate(48, (uint32_t) &int_handler_48, 0x8, 0x8e);
    idtSetGate(49, (uint32_t) &int_handler_49, 0x8, 0x8e);
    idtSetGate(50, (uint32_t) &int_handler_50, 0x8, 0x8e);
	idtSetGate(51, (uint32_t) &int_handler_51, 0x8, 0x8e);
	idtSetGate(52, (uint32_t) &int_handler_52, 0x8, 0x8e);
	idtSetGate(53, (uint32_t) &int_handler_53, 0x8, 0x8e);
	idtSetGate(54, (uint32_t) &int_handler_54, 0x8, 0x8e);
    idtSetGate(55, (uint32_t) &int_handler_55, 0x8, 0x8e);
    idtSetGate(56, (uint32_t) &int_handler_56, 0x8, 0x8e);
	idtSetGate(57, (uint32_t) &int_handler_57, 0x8, 0x8e);
    idtSetGate(58, (uint32_t) &int_handler_58, 0x8, 0x8e);
    idtSetGate(59, (uint32_t) &int_handler_59, 0x8, 0x8e);
    idtSetGate(60, (uint32_t) &int_handler_60, 0x8, 0x8e);
    idtSetGate(61, (uint32_t) &int_handler_61, 0x8, 0x8e);
    idtSetGate(62, (uint32_t) &int_handler_62, 0x8, 0x8e);
    idtSetGate(63, (uint32_t) &int_handler_63, 0x8, 0x8e);
    idtSetGate(64, (uint32_t) &int_handler_64, 0x8, 0x8e);
    idtSetGate(65, (uint32_t) &int_handler_65, 0x8, 0x8e);
    idtSetGate(66, (uint32_t) &int_handler_66, 0x8, 0x8e);
    idtSetGate(67, (uint32_t) &int_handler_67, 0x8, 0x8e);
    idtSetGate(68, (uint32_t) &int_handler_68, 0x8, 0x8e);
    idtSetGate(69, (uint32_t) &int_handler_69, 0x8, 0x8e);
    idtSetGate(70, (uint32_t) &int_handler_70, 0x8, 0x8e);
    idtSetGate(71, (uint32_t) &int_handler_71, 0x8, 0x8e);
    idtSetGate(72, (uint32_t) &int_handler_72, 0x8, 0x8e);
    idtSetGate(73, (uint32_t) &int_handler_73, 0x8, 0x8e);
    idtSetGate(74, (uint32_t) &int_handler_74, 0x8, 0x8e);
    idtSetGate(75, (uint32_t) &int_handler_75, 0x8, 0x8e);
	idtSetGate(76, (uint32_t) &int_handler_76, 0x8, 0x8e);
	idtSetGate(77, (uint32_t) &int_handler_77, 0x8, 0x8e);
	idtSetGate(78, (uint32_t) &int_handler_78, 0x8, 0x8e);
	idtSetGate(79, (uint32_t) &int_handler_79, 0x8, 0x8e);
    idtSetGate(80, (uint32_t) &int_handler_80, 0x8, 0x8e);
    idtSetGate(81, (uint32_t) &int_handler_81, 0x8, 0x8e);
    idtSetGate(82, (uint32_t) &int_handler_82, 0x8, 0x8e);
    idtSetGate(83, (uint32_t) &int_handler_83, 0x8, 0x8e);
    idtSetGate(84, (uint32_t) &int_handler_84, 0x8, 0x8e);
    idtSetGate(85, (uint32_t) &int_handler_85, 0x8, 0x8e);
    idtSetGate(86, (uint32_t) &int_handler_86, 0x8, 0x8e);
    idtSetGate(87, (uint32_t) &int_handler_87, 0x8, 0x8e);
    idtSetGate(88, (uint32_t) &int_handler_88, 0x8, 0x8e);
    idtSetGate(89, (uint32_t) &int_handler_89, 0x8, 0x8e);
	idtSetGate(90, (uint32_t) &int_handler_90, 0x8, 0x8e);
    idtSetGate(91, (uint32_t) &int_handler_91, 0x8, 0x8e);
    idtSetGate(92, (uint32_t) &int_handler_92, 0x8, 0x8e);
    idtSetGate(93, (uint32_t) &int_handler_93, 0x8, 0x8e);
    idtSetGate(94, (uint32_t) &int_handler_94, 0x8, 0x8e);
    idtSetGate(95, (uint32_t) &int_handler_95, 0x8, 0x8e);
    idtSetGate(96, (uint32_t) &int_handler_96, 0x8, 0x8e);
    idtSetGate(97, (uint32_t) &int_handler_97, 0x8, 0x8e);
    idtSetGate(98, (uint32_t) &int_handler_98, 0x8, 0x8e);
    
	for (int i = 99; i < 256; i++) {
        idtSetGate(i, (uint32_t) &int_handler_99, 0x8, 0x8e);
    }

    picInit(PIC_MASTER_COMMAND, PIC_MASTER_DATA, 0x11, 0x20, 0x04, 0x01);
    picInit(PIC_SLAVE_COMMAND, PIC_SLAVE_DATA, 0x11, 0x28, 0x04, 0x01);

    // Demask
    picDemask(PIC_MASTER_DATA);
    picDemask(PIC_SLAVE_DATA);

    asm volatile("lidt %0" :: "m"(idtDesc));
    asm volatile("xchg %bx, %bx");
    asm volatile("sti");
}

cpuState *intHandler(cpuState *state)
{
    if (state->intr > 31) {
        // IRQ
        irqHandler(state->intr - 32);
    }

    return state;
}