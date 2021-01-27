#include "track38App.h"
#include "track38Frame.h"

// Give wxWidgets the means to create a track38 object
IMPLEMENT_APP( track38App );

bool track38App::OnInit()
{
    SetVendorName( vendorName );
    SetAppName( appName );

    track38Frame* m_frame = new track38Frame();
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