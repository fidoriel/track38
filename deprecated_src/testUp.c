#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../src/poweredup/PoweredUp.h"
#include "../src/poweredup/PoweredUp.c"

#define service = "00001623-1212-EFDE-1623-785FEABCD123"
#define characteristic = "00001624-1212-EFDE-1623-785FEABCD123"

void deleteAry(uint8_t *ary, int len)
{
    for (size_t i = 0; i < len; i++)
    {
        free( ary+i );
    }
}

int main()
{
    uint8_t *ary;
    int len = subscribe( &ary, voltage, 0, true);
    printf("subscribe ");
    for (size_t i = 0; i < len; i++)
    {
        printf( "%x ", *(ary+i) );
    }
    printf("\n");


    len = motor_speed( &ary, portA, -100);
    printf("speed ");
    for (size_t i = 0; i < len; i++)
    {
        printf( "%x ", *(ary+i) );
    }
    printf("\n");


    len = set_color( &ary, rgbLED, lightBlue);
    printf("color ");
    for (size_t i = 0; i < len; i++)
    {
        printf( "%x ", *(ary+i) );
    }
    printf("\n");
}