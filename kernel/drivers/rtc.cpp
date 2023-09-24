#include <drivers/rtc.hpp>
#include <io/io.hpp>

#define ADDRESS_PORT    0x70
#define DATA_PORT       0x71

#define REG_SECONDS     0x0
#define REG_MINUTES     0x2
#define REG_HOURS       0x4
#define REG_DAY_OF_WEEK 0x6
#define REG_DATE_DAY    0x7
#define REG_DATE_MONTH  0x8
#define REG_DATE_YEAR   0x9
#define REG_STATUS_A    0xA
#define REG_STATUS_B    0xB

#define STATUS_B_2412   (1 << 1)
#define STATUS_B_DM     (1 << 2)

uint8_t RtcDevice::readReg(uint8_t reg)
{
    if (reg == REG_STATUS_A || reg == REG_STATUS_A) {
        outb(ADDRESS_PORT, reg);
        return inb(DATA_PORT);
    }

    outb(ADDRESS_PORT, REG_STATUS_B);
    uint8_t statusB = inb(DATA_PORT);
    bool statusB2412 = statusB & STATUS_B_2412;
    bool statusBDm = statusB & STATUS_B_DM;

    outb(ADDRESS_PORT, reg);
    uint32_t result = inb(DATA_PORT);

    if (statusBDm) {
        // Binary mode
        return result;
    } else {
        // BCD mode
        return (result & 0xF) + ((result / 16) * 10);
    }
}

Time RtcDevice::read()
{
    Time time;

    time.seconds = readReg(REG_SECONDS);
    time.minutes = readReg(REG_MINUTES);
    time.hours = readReg(REG_HOURS);
    time.day_of_week = readReg(REG_DAY_OF_WEEK);
    time.date_day = readReg(REG_DATE_DAY);
    time.date_month = readReg(REG_DATE_MONTH);
    time.date_year = readReg(REG_DATE_YEAR);

    return time;
}