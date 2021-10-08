#ifndef serial_win_h
#define serial_win_h

#include <stdio.h>
#include <string.h>

#if defined(WIN32)

#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif

int connect_port(char* comPort);
int read_port(int device, unsigned char *buffer, int size);
int write_port(int device, const char *string);
void close_port( int device );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

#endif