#include "switch.h"

tswitch::tswitch(wxString wxName)
{
    this->name = wxName;
}

void tswitch::createControls( wxStaticBox* parent )
{
}

void tswitch::ChangePos( char dir )
{
    string serialSignal = "<S";

    if ( sGPIO < 10 )
    {
        serialSignal += '0';
    }

    serialSignal += to_string( sGPIO );     

    serialSignal += '>';

    copy( serialSignal.begin(), serialSignal.end(), sendSignal );

    #if defined(__linux__) || defined(__FreeBSD__) || defined(__APPLE__)
    char str_send[ 2 ][ 128 ];
    strcpy( str_send[ 1 ], sendSignal );
    write_port( con, str_send[ 1 ] );
    #endif
}

void tswitch::setPort(wxString wxPort)
{
    string stdString = string(wxPort.mb_str());
    strcpy(port, stdString.c_str());
    this->portPoint = port;
}

void tswitch::CloseCon()
{
    close_port( this->con );
}

tswitch::~tswitch()
{
    this->CloseCon();
}