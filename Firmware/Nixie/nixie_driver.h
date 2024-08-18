#ifndef NIXIE_DRIVER_H
#define NIXIE_DRIVER_H

#include <stdint.h>

#define PORT_0_ADD 0x20
#define PORT_1_ADD 0x21
#define PORT0_CLK_PIN 2
#define PORT1_CLK_PIN 3
#define HV_CNTRL_PIN 9

class NixieDriver
{
    public:
        NixieDriver(void);
        void begin(void);
        void high_voltage_switch(uint8_t onOff);
        byte get_high_voltage_status(void);
        void set_Nixie(uint8_t n, uint8_t value);

    private:
        int _port0_add;
        int _port1_add;
        byte _nixie_val[4];
};

#endif
