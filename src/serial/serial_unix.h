#ifndef serial_unix_h
#define serial_unix_h

#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <Windows.h>
#endif

#if defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
#include <termios.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)

int connect_port(char* device);
int read_port(int device, unsigned char *buffer, int size);
int write_port(int device, const char *string);
void close_port( int device );

#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif