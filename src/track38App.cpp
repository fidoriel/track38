#include "track38App.h"
#include "track38Frame.h"

// Give wxWidgets the means to create a track38 object
IMPLEMENT_APP( track38App );

bool checkIfValid()
{
    // int Vday = 31;
    // int Vmonth = 12;
    // int Vyear = 2021;

    // time_t ttime = time(0);
    // tm *local_time = localtime(&ttime);

    // // std::cout << "year " << 1900 + local_time->tm_year << endl;
    // // std::cout << "month " << 1 + local_time->tm_mon << endl;
    // // std::cout << "day " << local_time->tm_mday << endl;

    // if ( ( 1900 + local_time->tm_year <= Vyear ) && ( ( 1 + local_time->tm_mon ) <= Vmonth ) && ( local_time->tm_mday <= Vday ) )
    //     return true;
    
    // else
    //     return false;
    return true;
}

bool track38App::OnInit()
{
    if ( !checkIfValid() )
    {
        wxMessageBox( "This beta copy is not valid anymore. track38 will close now." );
        return false;
    }

    SetVendorName( vendorName );
    SetAppName( appName );

    this->blutoothPermission = bleIsAutorized();
    if (this->blutoothPermission)
    wxMessageBox( "acces" );
    else
    wxMessageBox( "no acces" );

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