#include "PoweredUpp.h"

PoweredUpp::PoweredUpp()
{
}

int PoweredUpp::motor_start(uint8_t port, int8_t speed)
{    
    uint8_t cmd[8] = {0x08, 0x00, 0x81, port, 0x11, 0x51, 0x00, speed};
    return 0;
}

int PoweredUpp::set_color(uint8_t port, uint8_t color)
{    
    uint8_t cmd[8] = {0x08, 0x00, 0x81, port, 0x11, 0x51, 0x00, color};
    return 0;
}

int PoweredUpp::read(uint8_t property, uint8_t operation)
{    
    uint8_t cmd[6] = {0x06, 0x00, 0x01, property, operation, 0x00};
    return 0;
}

int PoweredUpp::subscribe(uint8_t port, uint8_t mode, bool subscribe)
{
    uint8_t cmd[10] = {0x0a, 0x00, 0x41, port, mode, 0x01, 0x00, 0x00, 0x00, subscribe};
    return 0;
}

PoweredUpp::~PoweredUpp()
{
}
