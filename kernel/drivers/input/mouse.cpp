#include <drivers/input/mouse.hpp>
#include <io/io.hpp>
#include <cpu/irq.hpp>
#include <debug.hpp>

#define COMMAND_DISABLE_SECOND_PORT 0xA7
#define COMMAND_ENABLE_SECOND_PORT  0xA8
#define COMMAND_DISABLE_FIRST_PORT  0xAD
#define COMMAND_ENABLE_FIRST_PORT   0xAE
#define COMMAND_SET_DEFAULTS        0xF6
#define COMMAND_RESET               0xFF

mouseInputState state;

void mouseHandler(void *privateData)
{
	static uint16_t mouseReceiveByte[3];
    static uint8_t mouseReceiveCounter=0;

	uint8_t data = inb(0x60);

	switch(mouseReceiveCounter) {
		case 0:
	 		// Flags, buttons
			mouseReceiveByte[0] = data;
			mouseReceiveCounter++;

			break;

		case 1:
			// X Movement
			mouseReceiveByte[1] = data;
			mouseReceiveCounter++;
			break;

		case 2:
			// Y Movement
			mouseReceiveByte[2] = data;

			state.x = mouseReceiveByte[1] - ((mouseReceiveByte[0] << 4) & 0x100);
			state.y = mouseReceiveByte[2] - ((mouseReceiveByte[0] << 3) & 0x100);

			mouseReceiveCounter=0;
			break;
	}
}

void mouseWrite(uint8_t data)
{
	// Tell controller we're sending a command to the mouse
	outb(0x64, 0xD4);
	outb(0x60, data);

	while (inb(0x60) != 0xFA) {
		// Wait for ack
	}
}

void mouseInit()
{
	// Enable auxiliary mouse device
	outb(0x64, 0xA8);

	// Enable irq
    outb(0x64, 0x20);

    uint8_t status = inb(0x60) | 2;
    outb(0x64, 0x60);
    outb(0x60, status);

	// Use default settings
	mouseWrite(0xF6);

	// Enable mouse
	mouseWrite(0xF4);

    inb(0x60);

	irqInstallHandler(12, mouseHandler, nullptr);
}

mouseInputState mouseRead()
{
	mouseInputState inputState = state;

	state.x = 0;
	state.y = 0;

    return inputState;
}