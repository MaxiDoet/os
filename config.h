#define BUILD_INFO "++++++++++++++++++++++++++++++++++\r\n+  Kernel built on February 4th 2022  +\r\n++++++++++++++++++++++++++++++++++\r\n"

/* Debug */
#define DEBUG_PORT 0x3F8
#define DEBUG_IRQ_INSTALL
//#define DEBUG_IRQ_PRINT
//#define DEBUG_IRQ_PRINT_ROUTINE
//#define DEBUG_KEYBOARD_EVENT
#define DEBUG_PCI
//#define DEBUG_ATA
//#define DEBUG_EXT2
#define DEBUG_NET_ARP

#define PAGE_SIZE 4096
#define HEAP_SIZE 10485760

//#define DESKTOP