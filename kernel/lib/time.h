#include <inttypes.h>

typedef struct Time {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day_of_week;
    uint8_t date_day;
    uint8_t date_month;
    uint8_t date_year;
} Time;