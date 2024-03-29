#include "upeditbox.h"
#include "track38App.h"

wxBEGIN_EVENT_TABLE( upEditBox, wxPanel )
    EVT_BUTTON( ID_SCANBLE, upEditBox::OnBleMenue )
wxEND_EVENT_TABLE()

upEditBox::upEditBox( wxPanel* parent, int id, wxString title ) : editBox( parent, id, title )
{
    parentPanel = parent;
    topSizerUpEdit = new wxFlexGridSizer( 2, 0, 0 );

    nameTxt = new wxStaticText( this, wxID_ANY, "Train Name:" );
    trainName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200, -1 ), 0L, wxDefaultValidator, "tName" );
    trainName->SetEditable( false );

    refreshSizer = new wxBoxSizer( wxHORIZONTAL );
    bleDevTxt = new wxStaticText( this, wxID_ANY, "Blutooth Device:" );
    bleDevName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200, -1 ), 0L, wxDefaultValidator, "upPort" );
    bleDevName->SetEditable(false);

    m_RefreshBtn = new wxButton( this, ID_SCANBLE, "Scan for Hubs", wxDefaultPosition, wxDefaultSize );
    refreshSizer->Add( bleDevName, 0, wxALL, 5 );
    refreshSizer->Add( m_RefreshBtn, 0, wxALL, 5 );

    hubAdressTxt = new wxStaticText( this, wxID_ANY, "Hub Adress:" );
    hubAdress = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200, -1 ), 0L, wxDefaultValidator, "upHubAdress" );
    hubAdress->SetEditable(false);

    channelTxt = new wxStaticText( this, wxID_ANY, "PoweredUP Hub Channel:" );
    channel.Add( "A" );
    channel.Add( "B" );
    channelPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, channel, 0L, wxDefaultValidator, "upChannel" );

    hasTwoMotorsTxt = new wxStaticText( this, wxID_ANY, "Two Motors used:" );
    hasTwoMotors = new wxCheckBox( this, wxID_ANY, wxT( "" ), wxDefaultPosition, wxDefaultSize, 0L, wxDefaultValidator, "upAreTwoMotorsUsed" );
    hasTwoMotors->Disable();

    maxSpeedTxt = new wxStaticText( this, wxID_ANY, "Train Max Speed" );
    maxSpeedPicker = new wxSpinCtrl( this, wxID_ANY, "7", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 7, "upSpeed" );

    topSizerUpEdit->AddSpacer( 10 );
    topSizerUpEdit->AddSpacer( 10 );
    
    topSizerUpEdit->Add( nameTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    topSizerUpEdit->Add( trainName, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );

    topSizerUpEdit->Add( bleDevTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    topSizerUpEdit->Add( refreshSizer, 0, wxALL | ( wxALL & ~wxLEFT ), 0 );

    topSizerUpEdit->Add( hubAdressTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    topSizerUpEdit->Add( hubAdress, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );

    topSizerUpEdit->Add( channelTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    topSizerUpEdit->Add( channelPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );

    topSizerUpEdit->Add( hasTwoMotorsTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    topSizerUpEdit->Add( hasTwoMotors, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );

    topSizerUpEdit->Add( maxSpeedTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    topSizerUpEdit->Add( maxSpeedPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );

    topSizerUpEdit->AddSpacer( 10 );
    topSizerUpEdit->AddSpacer( 10 );
    
    // Top Sizer Settings
    this->SetSizer( topSizerUpEdit );
    this->Layout();
	topSizerUpEdit->Fit( this );
    topSizerUpEdit->SetSizeHints( this );
    this->SendSizeEventToParent();
}

void upEditBox::OnBleMenue( wxCommandEvent& event )
{

    if ( !bleIsAutorized() )
    {
        wxMessageBox("Blutooth ist not authorized.");
        return;
    }

    dlg = new bleConDialog( wxGetApp().m_frame );
    dlg->Create();

    if( dlg->ShowModal() == wxID_OK )
    {
        //SimpleBLE::Peripheral peripheral = dlg.getSelectedPeripheral();
        //wxMessageBox( dlg.blesearch->peripherals[ dlg.selection ].identifier() );
        //wxMessageBox("ok");
        if ( dlg->selection != -1 )
        {
            bleDevName->SetValue( dlg->getBleId( std::string( dlg->bleDeviceList->GetString( dlg->selection ).mb_str() ) ) );
            hubAdress->SetValue( dlg->getBleAdr( std::string( dlg->bleDeviceList->GetString( dlg->selection ).mb_str() ) ) );
        }
    }

    delete dlg;
}

void upEditBox::initConf()
{
    // Init config
    configTrain = new wxFileConfig( wxGetApp().GetAppName(), wxGetApp().GetVendorName(), wxGetApp().ini_dir + "trains.ini", "", wxCONFIG_USE_GLOBAL_FILE );
    wxConfigBase::Set( configTrain );
    track38ConfigTrain = wxConfigBase::Get();
}

bool upEditBox::SaveTrain( bool overwrite )
{
    this->initConf();

    track38ConfigTrain->SetPath( "/Train/" );
    track38ConfigTrain->DeleteGroup( trainName->GetValue() );
    track38ConfigTrain->SetPath( trainName->GetValue() );
    track38ConfigTrain->Write( "control", "up" );
    track38ConfigTrain->Write( "maxSpeed", wxString::Format( wxT( "%i" ), maxSpeedPicker->GetValue() ) );
    track38ConfigTrain->Write( "channel", channelPicker->GetStringSelection() );
    // track38ConfigTrain->Write( "port", portPicker->GetStringSelection() );
    track38ConfigTrain->Write( "hubadress", hubAdress->GetValue() );
    track38ConfigTrain->Write( "hubname", bleDevName->GetValue() );

    if (hasTwoMotors->GetValue())
        track38ConfigTrain->Write( "hastwomotors", "1" );
    else
        track38ConfigTrain->Write( "hastwomotors", "0" );

    
    track38ConfigTrain->Flush();

    return true;
}

void upEditBox::SelectTrain( wxString trainSel )
{
    this->initConf();

    track38ConfigTrain->SetPath( "/Train/" );
    track38ConfigTrain->SetPath( trainSel );

    for ( size_t idx = 0; idx < channelPicker->GetCount(); idx++ )
    {
        if ( track38ConfigTrain->Read( "channel", "1" ).IsSameAs( channelPicker->GetString( idx ) ) )
            channelPicker->SetSelection( idx );
    }

    trainName->ChangeValue( trainSel );
    maxSpeedPicker->SetValue( track38ConfigTrain->Read( "maxSpeed", "5" ) );

    hubAdress->SetValue( track38ConfigTrain->Read( "hubadress", "" ) );
    bleDevName->SetValue( track38ConfigTrain->Read( "hubname", "" ) );
    hasTwoMotors->SetValue( bool( atoi( track38ConfigTrain->Read( "hastwomotors", "0" ) ) ) );
}

void upEditBox::AddTrain( wxString trainName )
{
    this->initConf();
    track38ConfigTrain->SetPath( "/Train/" );
    track38ConfigTrain->SetPath( trainName );
    track38ConfigTrain->Write( "control", "up" );
    track38ConfigTrain->Flush();

    SelectTrain( trainName );
}

void upEditBox::SetTrainName( wxString name )
{
    trainName->ChangeValue( name );
}