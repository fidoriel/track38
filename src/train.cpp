#include "train.h"

train::train(wxString wxName)
{
    this->name = wxName;
}

void train::createControls(wxStaticBox* parent)
{
    this->trainName = new wxStaticText( parent, wxID_ANY, this->name );
    this->speedSlider = new wxSlider( parent, wxID_ANY, 0, this->maxTrainSpeed*(-1), this->maxTrainSpeed, wxDefaultPosition, wxSize( 200, -1 ), wxSL_AUTOTICKS );
    this-> stopBtn = new wxButton( parent, wxID_ANY, "Stop", wxDefaultPosition, wxDefaultSize );
}

void train::setControl(wxString wxControl)
{
    this->control = string(wxControl.mb_str());
}

void train::setPort(wxString wxPort)
{
    this->port = string(wxPort.mb_str());
}

void train::setMaxSpeed(wxString wxSpeed)
{
    this->maxTrainSpeed = wxAtoi(wxSpeed);
}

void train::setHubAdress(wxString wxHub)
{
    this->upHubAdress = string(wxHub.mb_str());
}

void train::setUpChannel(wxString wxCh)
{
    this->upChannel = string(wxCh.mb_str())[0];
}

void train::setTwoMotors(bool is)
{
    this->upTwoMotorsUsed = is;
}

void train::setPfChannel(wxString wxCh)
{
    this->pfChannel = wxAtoi(wxCh);
}

void train::setPfSubChannel(wxString wxSub)
{
    this->pfSubChannel = string(wxSub.mb_str())[0];
}

void train::setPfGPIO(wxString wxGPIO)
{
    this->pfGpio = wxAtoi(wxGPIO);
}

wxString train::getName()
{
    return name;
}

bool train::isPf()
{
    if ( this->control.compare( "pf" ) == 0 )
       return true;
    return false;    
}

bool train::isUp()
{
    if ( this->control.compare( "up" ) == 0 )
       return true;
    return false;
}

train::~train()
{
}