#include "rceditbox.h"
#include "track38App.h"

wxBEGIN_EVENT_TABLE( rcEditBox, wxPanel )
    EVT_BUTTON( ID_REFRESHSERIAL, rcEditBox::OnRefreshSerial )
wxEND_EVENT_TABLE()

rcEditBox::rcEditBox( wxPanel* parent, int id, wxString title ) : editBox( parent, id, title )
{   
    topSizerrcEdit = new wxFlexGridSizer( 2, 0, 0 );

    nameTxt = new wxStaticText( this, wxID_ANY, "Train Name:" );
    trainName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200, -1 ), 0L, wxDefaultValidator, "tName" );
    trainName->SetEditable( false );

    refreshSizer = new wxBoxSizer( wxHORIZONTAL );
    portTxt = new wxStaticText( this, wxID_ANY, "Arduino ComPort:" );
    this->refreshSerial();
    portPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 200, -1 ), serialArray, 0L, wxDefaultValidator, "rcPort" );

    m_RefreshBtn = new wxButton( this, ID_REFRESHSERIAL, "Refresh", wxDefaultPosition, wxDefaultSize );
    refreshSizer->Add( portPicker, 0, wxALL, 5 );
    refreshSizer->Add( m_RefreshBtn, 0, wxALL, 5 );

    gpioTxt = new wxStaticText( this, wxID_ANY, "GPIO:" );
    gpioPicker = new wxSpinCtrl( this, wxID_ANY, "13", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 13, "rcGpio" );

    channelTxt = new wxStaticText( this, wxID_ANY, "RC Channel:" );
    channel.Add( "1" );
    channel.Add( "2" );
    channel.Add( "3" );
    channelPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, channel, 0L, wxDefaultValidator, "rcChannel" );

    maxSpeedTxt = new wxStaticText( this, wxID_ANY, "Train Max Speed" );
    maxSpeedPicker = new wxSpinCtrl( this, wxID_ANY, "5", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 7, 5, "rcSpeed" );

    topSizerrcEdit->AddSpacer( 10 );
    topSizerrcEdit->AddSpacer( 10 );
    topSizerrcEdit->Add( nameTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    topSizerrcEdit->Add( trainName, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );

    topSizerrcEdit->Add( portTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    topSizerrcEdit->Add( refreshSizer, 0, wxALL | ( wxALL & ~wxLEFT ), 0 );

    topSizerrcEdit->Add( gpioTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    topSizerrcEdit->Add( gpioPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );

    topSizerrcEdit->Add( channelTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    topSizerrcEdit->Add( channelPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );

    topSizerrcEdit->Add( maxSpeedTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    topSizerrcEdit->Add( maxSpeedPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );

    topSizerrcEdit->AddSpacer( 10 );
    topSizerrcEdit->AddSpacer( 10 );

    // Top Sizer Settings
    this->SetSizer( topSizerrcEdit );
    this->Layout();
	topSizerrcEdit->Fit( this );
    topSizerrcEdit->SetSizeHints( this );
    this->SendSizeEventToParent();
}

void rcEditBox::OnRefreshSerial( wxCommandEvent& event )
{
    this->refreshSerial();
    portPicker->Clear();
    for (size_t i = 0; i < serialArray.GetCount(); i++)
    {
        portPicker->Append( serialArray.Item( i ) );
    }

    if ( portPicker->GetCount() > 0 )
        portPicker->SetSelection( portPicker->GetCount() - 1 );
}

void rcEditBox::initConf()
{
    // Init config
    configTrain = new wxFileConfig( wxGetApp().GetAppName(), wxGetApp().GetVendorName(), wxGetApp().ini_dir + "trains.ini", "", wxCONFIG_USE_GLOBAL_FILE );
    wxConfigBase::Set( configTrain );
    track38ConfigTrain = wxConfigBase::Get();
}

bool rcEditBox::SaveTrain( bool overwrite )
{
    this->initConf();
    track38ConfigTrain->SetPath( "/Train/" );
    track38ConfigTrain->DeleteGroup( trainName->GetValue() );
    track38ConfigTrain->SetPath( trainName->GetValue() );
    track38ConfigTrain->Write( "control", "rc" );
    track38ConfigTrain->Write( "gpio", wxString::Format( wxT( "%i" ), gpioPicker->GetValue() ) );
    track38ConfigTrain->Write( "maxSpeed", wxString::Format( wxT( "%i" ), maxSpeedPicker->GetValue() ) );
    track38ConfigTrain->Write( "channel", channelPicker->GetStringSelection() );
    track38ConfigTrain->Write( "port", portPicker->GetStringSelection() );
    
    track38ConfigTrain->Flush();

    return true;
}

void rcEditBox::SelectTrain( wxString trainSel )
{
    this->initConf();

    track38ConfigTrain->SetPath( "/Train/" );
    track38ConfigTrain->SetPath( trainSel );

    gpioPicker->SetValue( track38ConfigTrain->Read( "gpio", "13" ) );

    for ( size_t idx = 0; idx < channelPicker->GetCount(); idx++ )
    {
        if ( track38ConfigTrain->Read( "channel", "1" ).IsSameAs( channelPicker->GetString( idx ) ) )
            channelPicker->SetSelection( idx );
    }

    trainName->ChangeValue( trainSel );
    maxSpeedPicker->SetValue( track38ConfigTrain->Read( "maxSpeed", "5" ) );

    channelPicker->Refresh();
}

void rcEditBox::AddTrain( wxString trainName )
{
    this->initConf();
    track38ConfigTrain->SetPath( "/Train/" );
    track38ConfigTrain->SetPath( trainName );
    track38ConfigTrain->Write( "control", "rc" );
    track38ConfigTrain->Flush();

    SelectTrain( trainName );
}

void rcEditBox::SetTrainName( wxString name )
{
    trainName->ChangeValue( name );
}