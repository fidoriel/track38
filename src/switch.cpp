#include "switch.h"

tswitch::tswitch(wxString wxName)
{
    this->name = wxName;
}

void tswitch::createControls( wxStaticBox* parent )
{
    this->tswitchName = new wxStaticText( parent, wxID_ANY, this->name );
    this->straightBtn = new wxButton( parent, ID_turn, "Turn", wxDefaultPosition, wxDefaultSize );
    this->turnBtn = new wxButton( parent, ID_straight, "Straight", wxDefaultPosition, wxDefaultSize );
}

void tswitch::Turn()
{
    this->ChangePos( 'T' );
}

void tswitch::Straight()
{
    this->ChangePos( 'S' );
}

void tswitch::ChangePos( char newPos )
{
    if ( this->currentPos == newPos )
        return;

    string serialSignal = "<S";

    if ( sGPIO < 10 )
    {
        serialSignal += '0';
    }

    serialSignal += dir;
    serialSignal += newPos;

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

void tswitch::setGPIO( int sGPIO )
{
    this->sGPIO = sGPIO;
}

void tswitch::setDir( wxString wxDir )
{
    this->dir = string(wxDir.mb_str())[0];
}

void tswitch::setManufacturer( wxString manufacturer )
{
    this->manufacturer = manufacturer;
}

void tswitch::OnToggle( wxCommandEvent& event )
{
    if ( currentPos == 'T' )
        this->ChangePos( 'S' );

    else if ( this->currentPos == 'S' )
        this->ChangePos( 'T' );
}

void tswitch::OnStraight( wxCommandEvent& event )
{
    this->Straight();
}

void tswitch::OnTurn( wxCommandEvent& event )
{
    this->Turn();
}

bool tswitch::isTurn()
{
    return ( this->currentPos == 'T' );
}

bool tswitch::isStraight()
{
    return ( this->currentPos == 'S' );
}

void tswitch::CloseCon()
{
    close_port( this->con );
}

tswitch::~tswitch()
{
    this->CloseCon();
}