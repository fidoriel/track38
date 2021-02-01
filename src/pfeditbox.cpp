#include "pfeditbox.h"
#include "track38App.h"

wxBEGIN_EVENT_TABLE( pfEditBox, wxPanel )
    EVT_BUTTON( ID_REFRESHSERIAL, pfEditBox::OnRefreshSerial )
wxEND_EVENT_TABLE()

pfEditBox::pfEditBox( wxPanel* parent, int id, wxString title ) : editBox( parent, id, title )
{   
    topSizerPfEdit = new wxFlexGridSizer( 2, 0, 0 );


    nameTxt = new wxStaticText( this, wxID_ANY, "Train Name:" );
    trainName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 300, -1 ), 0L, wxDefaultValidator, "tName" );

    refreshSizer = new wxBoxSizer( wxHORIZONTAL );
    portTxt = new wxStaticText( this, wxID_ANY, "Arduino ComPort:" );
    this->refreshSerial();
    portPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 300, -1 ), serialArray, 0L, wxDefaultValidator, "pfPort" );

    m_RefreshBtn = new wxButton( this, ID_REFRESHSERIAL, "Refresh", wxDefaultPosition, wxDefaultSize );
    refreshSizer->Add( portPicker, 0, wxALL, 5 );
    refreshSizer->Add( m_RefreshBtn, 0, wxALL, 5 );

    gpioTxt = new wxStaticText( this, wxID_ANY, "GPIO:" );
    gpioPicker = new wxSpinCtrl( this, wxID_ANY, "13", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 13, "pfGpio" );

    channelTxt = new wxStaticText( this, wxID_ANY, "PowerFunctions Channel:" );
    channel.Add( "1" );
    channel.Add( "2" );
    channel.Add( "3" );
    channel.Add( "4" );
    channelPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, channel, 0L, wxDefaultValidator, "pfChannel" );

    subChannelTxt = new wxStaticText( this, wxID_ANY, "PowerFunctions SubCannel:" );
    subChannel.Add( "Red" );
    subChannel.Add( "Blue" );
    subChannelPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, subChannel, 0L, wxDefaultValidator, "pfSubChannel" );

    maxSpeedTxt = new wxStaticText( this, wxID_ANY, "Train Max Speed" );
    maxSpeedPicker = new wxSpinCtrl( this, wxID_ANY, "5", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 7, 5, "pfSpeed" );

    topSizerPfEdit->AddSpacer( 10 );
    topSizerPfEdit->AddSpacer( 10 );
    topSizerPfEdit->Add( nameTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    topSizerPfEdit->Add( trainName, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );

    topSizerPfEdit->Add( portTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    topSizerPfEdit->Add( refreshSizer, 0, wxALL | ( wxALL & ~wxLEFT ), 0 );

    topSizerPfEdit->Add( gpioTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    topSizerPfEdit->Add( gpioPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );

    topSizerPfEdit->Add( channelTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    topSizerPfEdit->Add( channelPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );

    topSizerPfEdit->Add( subChannelTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    topSizerPfEdit->Add( subChannelPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );

    topSizerPfEdit->Add( maxSpeedTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    topSizerPfEdit->Add( maxSpeedPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );

    topSizerPfEdit->AddSpacer( 10 );
    topSizerPfEdit->AddSpacer( 10 );
    
    // Top Sizer Settings
    parent->SetSizer( topSizerPfEdit );
    parent->Layout();
	topSizerPfEdit->Fit( this );
    topSizerPfEdit->SetSizeHints( this );
}

void pfEditBox::OnRefreshSerial( wxCommandEvent& event )
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

void pfEditBox::initConf()
{
    // Init config
    configTrain = new wxFileConfig( wxGetApp().GetAppName(), wxGetApp().GetVendorName(), wxGetApp().ini_dir + "train.ini", "", wxCONFIG_USE_GLOBAL_FILE );
    wxConfigBase::Set( configTrain );
    track38ConfigTrain = wxConfigBase::Get();
}

void pfEditBox::SaveTrain()
{
    this->initConf();

    track38ConfigTrain->SetPath( "/Train/" );

    track38ConfigTrain->SetPath( trainName->GetValue() );
    track38ConfigTrain->Write( "control", "pf" );
    track38ConfigTrain->Write( "gpio", wxString::Format( wxT( "%i" ), gpioPicker->GetValue() ) );
    track38ConfigTrain->Write( "subChannel", subChannelPicker->GetStringSelection() );
    track38ConfigTrain->Write( "maxSpeed", wxString::Format( wxT( "%i" ), maxSpeedPicker->GetValue() ) );
    track38ConfigTrain->Write( "channel", channelPicker->GetStringSelection() );
    track38ConfigTrain->Write( "port", portPicker->GetStringSelection() );
    
    track38ConfigTrain->Flush();
}

void pfEditBox::SelectTrain( wxString trainSel )
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

    for ( size_t idx = 0; idx < subChannelPicker->GetCount(); idx++ )
    {
        if ( track38ConfigTrain->Read( "subChannel", "R" ).IsSameAs( subChannelPicker->GetString( idx ) ) )
            subChannelPicker->SetSelection( idx );            
    }

    trainName->ChangeValue( trainSel );
    maxSpeedPicker->SetValue( track38ConfigTrain->Read( "maxSpeed", "7" ) );

    channelPicker->Refresh();
    subChannelPicker->Refresh();
}