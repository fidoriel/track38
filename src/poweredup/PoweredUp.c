#include "PoweredUp.h"

int motor_speed(uint8_t **cmd, uint8_t port, int8_t speed)
{   
    int len = 8;
    *cmd = malloc( sizeof(uint8_t) * len );
    uint8_t tmp[8] = {0x08, 0x00, 0x81, port, 0x11, 0x51, 0x00, (uint8_t)speed}; //static_cast<uint8_t>
    memcpy( *cmd, tmp, 8);
    return len;
}

int set_color(uint8_t **cmd, uint8_t port, uint8_t color)
{    
    int len = 8;
    *cmd = malloc( sizeof(uint8_t) * len );
    uint8_t tmp[8] = {0x08, 0x00, 0x81, port, 0x11, 0x51, 0x00, color};
    memcpy( *cmd, tmp, 8);
    return len;
}

int subscribe(uint8_t **cmd, uint8_t port, uint8_t mode, bool subscribe)
{
    int len = 10;
    *cmd = malloc( sizeof(uint8_t) * 10 );
    uint8_t tmp[10] = {0x0a, 0x00, 0x41, port, mode, 0x01, 0x00, 0x00, 0x00, (uint8_t)subscribe};
    memcpy( *cmd, tmp, 10);

    return len;
}
