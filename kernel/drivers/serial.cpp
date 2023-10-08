#include <drivers/serial.hpp>
#include <io/io.hpp>

#define REG_DATA        0x00
#define REG_DIVISOR_LSB 0x00
#define REG_DIVISOR_MSB 0x01
#define REG_IER         0x01
#define REG_FCR         0x02
#define REG_LCR         0x03
#define REG_MCR         0x04
#define REG_LSR         0x05

#define LCR_DLAB        (1 << 7)

#define MCR_LOOPBACK    (1 << 4)

#define LSR_DR          (1 << 0)
#define LSR_THRE        (1 << 5)

bool SerialPort::transmitEmpty()
{
    uint8_t lsr = inb(port + REG_LSR);

    if (lsr & LSR_THRE) {
        return true;
    }

    return false;
}

bool SerialPort::dataReady()
{
    uint8_t lsr = inb(port + REG_LSR);

    if (lsr & LSR_DR) {
        return true;
    }

    return false;
}

SerialPort::SerialPort(uint16_t port)
{
    this->port = port;

    outb(port + REG_IER, 0x01);         // Enable data interrupts
    outb(port + REG_LCR, LCR_DLAB);     // Unlock divisor registers
    outb(port + REG_DIVISOR_LSB, 12);   // 9600 Baud
    outb(port + REG_DIVISOR_MSB, 0);
    outb(port + REG_LCR, 0x03);         // 8bit data and one stop bit
    outb(port + REG_FCR, 0xC7);

    // First we test the port by sending a random byte and checking if reading returns the same byte
    /*
    outb(port + REG_MCR, MCR_LOOPBACK); // Enable loopback mode
    outb(port + REG_DATA, 0xEF);        // Test byte

    if (inb(port + REG_DATA) != 0xEF) {

    }

    outb(port + REG_MCR, 0x00);         // Disable loopback mode
    */
}

void SerialPort::writeChar(char c)
{
    while (!transmitEmpty()) {}

    if (c == '\n') {
        outb(port + REG_DATA, '\r');
        outb(port + REG_DATA, '\n');

        return;
    }

    outb(port + REG_DATA, c);
}

void SerialPort::writeString(char *str)
{
    while (*str != '\0') {
        writeChar(*str);

        str++;
    }
}

char SerialPort::readChar()
{
    while (!dataReady()) {}

    return (char) inb(port + REG_DATA);
}