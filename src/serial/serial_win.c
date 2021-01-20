#include "serial_win.h"

#ifdef __WIN32__

int connect_port(char* comPort)
{
    char options[ 256 ];
    strcpy( options, "baud=57600" );
    strcat( options, " data=8" );
    strcat( options, " parity=n" );
    strcat( options, " stop=1" );
    strcat( options, " xon=off to=off odsr=off dtr=on rts=on" );

    char port[12] = "\\\\.\\";
    strcat( port, comPort );

    HANDLE device = CreateFileA( port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL );

    DCB port_settings;
    memset( &port_settings, 0, sizeof( port_settings ) );  /* clear the new struct  */
    port_settings.DCBlength = sizeof( port_settings );

    BuildCommDCBA( options, &port_settings );

    SetCommState( device, &port_settings );

    COMMTIMEOUTS Cptimeouts;

    Cptimeouts.ReadIntervalTimeout         = MAXDWORD;
    Cptimeouts.ReadTotalTimeoutMultiplier  = 0;
    Cptimeouts.ReadTotalTimeoutConstant    = 0;
    Cptimeouts.WriteTotalTimeoutMultiplier = 0;
    Cptimeouts.WriteTotalTimeoutConstant   = 0;

    SetCommTimeouts( device, &Cptimeouts );

    return ( int ) device;
}

int read_port( int device, unsigned char *buffer, int size )
{
    HANDLE con = ( HANDLE ) device;
    int error;
    ReadFile( con, buffer, size, ( LPDWORD )( ( void * ) &error ), NULL );
    return error;
}

int write_port(int device, const char *string)
{
    HANDLE con = ( HANDLE ) device;
    int error;
  	while( *string !=  0)
	{
        int bytesWritten;
        error = WriteFile( con, ( string++ ), 1, ( LPDWORD ) &bytesWritten, NULL );
	}
    if ( error < 0 )
        return 1;
	return 0;
}

void close_port( int device )
{
    HANDLE con = ( HANDLE ) device;
    CloseHandle( con );
}

#endif