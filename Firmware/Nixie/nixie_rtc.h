#ifndef NIXIE_RTC_H
#define NIXIE_RTC_H

#include <stdint.h>

#define RTC_ADD 0x68

typedef struct rtc_timestamp_t{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t week_day;
    uint8_t day;
    uint8_t month;
    uint8_t year;

    uint8_t minute_bcd;
    uint8_t hour_bcd;
}rtc_timestamp_t;

class RTC
{
  public:
    RTC(void);
    void begin(void);
    uint8_t refresh(void);
    void set_time(rtc_timestamp_t timestamp);
    uint8_t get_second(void);
    uint8_t get_minute(void);
    uint8_t get_minute_bcd(void);
    uint8_t get_hour(void);
    uint8_t get_hour_bcd(void);
    uint8_t get_week_day(void);
    uint8_t get_day(void);
    uint8_t get_month(void);
    uint8_t get_year(void);
    void set_SQW(uint8_t value);
    void turn_on_periodic_SQW(void);

  private:
    int _rtc_add;
    rtc_timestamp_t _timestamp;
};

/**
 * @brief Convert binary to BCD
 * @param bin 
 * @return 
 */
uint8_t bin2bcd(uint8_t bin);

/**
 * @brief Convert BCD to binary
 * @param bcd 
 * @return 
 */
uint8_t bcd2bin(uint8_t bcd);

#endif
