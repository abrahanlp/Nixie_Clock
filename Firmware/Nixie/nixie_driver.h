#ifndef NIXIE_DRIVER_H
#define NIXIE_DRIVER_H

#include <stdint.h>

/*
NIXIE HOURS (UNITS)
I2C-8bit 0x20 -> D0-D7 -> IO3

NIXIE MINUTES (UNITS)
I2C-8bit 0x21 -> D0-D7 -> IO2

NIXIE TENS
2 0 1 3
D C B A | OUT | NIBBLE | NIXIE |
________________________________
0 0 0 0 |  0  |  0x0   |   3
0 0 0 1 |  1  |  0x8   |   4
0 0 1 0 |  2  |  0x2   |   0
0 0 1 1 |  3  |  0xA   |   9
0 1 0 0 |  4  |  0x1   |   6
0 1 0 1 |  5  |  0x9   |   5
0 1 1 0 |  6  |  0x3   |   7
0 1 1 1 |  7  |  0xB   |   8
1 0 0 0 |  8  |  0x4   |   2
1 0 0 1 |  9  |  0xC   |   1
________________________________

NIXIE UNITS
5 7 6 4
D C B A | OUT | NIBBLE | NIXIE |
--------------------------------
0 0 0 0 |  0  |  0x0   |   3
0 0 0 1 |  1  |  0x1   |   4
0 0 1 0 |  2  |  0x4   |   0
0 0 1 1 |  3  |  0x5   |   9
0 1 0 0 |  4  |  0x8   |   6
0 1 0 1 |  5  |  0x9   |   5
0 1 1 0 |  6  |  0xC   |   7
0 1 1 1 |  7  |  0xD   |   8
1 0 0 0 |  8  |  0x2   |   2
1 0 0 1 |  9  |  0x3   |   1
_______________________________

*/

#define PORT_0_ADD 0x20
#define PORT0_CLK_PIN 3

#define PORT_1_ADD 0x21
#define PORT1_CLK_PIN 2

#define HV_CNTRL_PIN 9

typedef struct NIXIE_DRIVER{
  int minutes_address;
  uint8_t minutes_clk;
  int hours_address;
  uint8_t hours_clk;
  byte tube_value[4];
}nixie_driver_t;

class NixieDriver
{
  public:
    NixieDriver(void);
    void begin(void);
    void high_voltage_switch(uint8_t onOff);
    byte get_high_voltage_status(void);
    uint8_t set_Nixie(uint8_t n, uint8_t value);

  private:
    nixie_driver_t _nixie_driver;
};

#endif
