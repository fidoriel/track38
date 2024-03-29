#include "track38App.h"
#include "track38Frame.h"

// Give wxWidgets the means to create a track38 object
IMPLEMENT_APP( track38App );

bool checkIfValid()
{
    return true;
}

bool track38App::OnInit()
{
    if ( !checkIfValid() )
    {
        return false;
    }

    SetVendorName( vendorName );
    SetAppName( appName );

    askForPermission();
    this->blutoothPermission = bleIsAutorized();
    if ( !this->blutoothPermission )
        wxMessageBox( "track38 needs permissions for Blutooth acces to be able to connect to Powered UP hubs. This acces was not granted. Please go to the Settings and grant the permission. Otherwise Bluetooth cannot be used." );

    m_frame = new track38Frame();
    m_frame->Show();
    return true;
}

void track38App::ShowPreferencesEditor( wxWindow* parent )
{
    if ( !m_prefEditor )
    {
        m_prefEditor.reset( new wxPreferencesEditor );
        m_prefEditor->AddPage( new track38PreferencePageGeneral );
    }

    m_prefEditor->Show( parent );
}