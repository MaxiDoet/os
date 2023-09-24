#include <inttypes.h>

#ifndef SERIAL_HPP
#define SERIAL_HPP

class SerialPort {
    private:
        uint16_t port;

        bool transmitEmpty();
    public:
        SerialPort(uint16_t port);

        void writeChar(char c);
        void writeString(char *str);
};

#endif