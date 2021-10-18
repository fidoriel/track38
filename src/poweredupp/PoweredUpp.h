#ifndef PoweredUpp_h
#define PoweredUpp_h

#include <cstdint>
#include <cstddef>

#include <stdio.h>
#include <string.h>

#define service = "00001623-1212-EFDE-1623-785FEABCD123"
#define characteristic = "00001624-1212-EFDE-1623-785FEABCD123"

enum readOperations
{
    advertisingName = 0x01,
    button = 0x02,
    firmwareVersion = 0x03,
    batteryVoltage = 0x06
};

enum operations
{
    set = 0x01,
    enableUpdates = 0x02,
    disableUpdates = 0x03,
    reset = 0x04,
    requestUpdate = 0x05,
    update = 0x06
};

enum colors
{
    none = 0xff,
    black = 0x00,
    pink = 0x01,
    purple = 0x02,
    blue = 0x03,
    lightBlue = 0x04,
    lightGreen = 0x05,
    green = 0x06,
    yellow = 0x07,
    orange = 0x08,
    red = 0x09,
    white = 0x0a
};

enum UpIO
{
    rgb = 0x32,
    current = 0x3b,
    voltage = 0x3c,
    portA = 0x00,
    portB = 0x01,
    portAB = 0x39
};

class PoweredUpp
{
    private:
        int cmdLen;
        
    public:
        PoweredUpp();

        int motor_start(uint8_t port, uint8_t speed);
        int motor_start(uint8_t port, int8_t speed);
        int set_color(uint8_t port, uint8_t color);
        int read(uint8_t property, uint8_t operation);
        int subscribe(uint8_t cmd[], uint8_t port, uint8_t mode, bool subscribe);

        ~PoweredUpp();
};

#endif