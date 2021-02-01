#include "upeditbox.h"

wxBEGIN_EVENT_TABLE( upEditBox, wxPanel )
    EVT_BUTTON( ID_REFRESHSERIAL, upEditBox::OnRefreshSerial )
wxEND_EVENT_TABLE()

upEditBox::upEditBox( wxPanel* parent, int id, wxString title ) : editBox( parent, id, title )
{
    topSizerUpEdit = new wxFlexGridSizer( 2, 0, 0 );

    nameTxt = new wxStaticText( this, wxID_ANY, "Train Name:" );
    trainName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 300, -1 ), 0L, wxDefaultValidator, "tName" );

    refreshSizer = new wxBoxSizer( wxHORIZONTAL );
    portTxt = new wxStaticText( this, wxID_ANY, "Arduino ComPort:" );
    this->refreshSerial();
    portPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 300, -1 ), serialArray, 0L, wxDefaultValidator, "upPort" );

    m_RefreshBtn = new wxButton( this, ID_REFRESHSERIAL, "Refresh", wxDefaultPosition, wxDefaultSize );
    refreshSizer->Add( portPicker, 0, wxALL, 5 );
    refreshSizer->Add( m_RefreshBtn, 0, wxALL, 5 );

    hubAdressTxt = new wxStaticText( this, wxID_ANY, "Hub Adress:" );
    hubAdress = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 300, -1 ), 0L, wxDefaultValidator, "upHubAdress" );

    channelTxt = new wxStaticText( this, wxID_ANY, "PoweredUP Hub Channel:" );
    channel.Add( "A" );
    channel.Add( "B" );
    channelPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, channel, 0L, wxDefaultValidator, "upChannel" );

    hasTwoMotorsTxt = new wxStaticText( this, wxID_ANY, "Two Motors used:" );
    hasTwoMotors = new wxCheckBox( this, wxID_ANY, wxT( "" ), wxDefaultPosition, wxDefaultSize, 0L, wxDefaultValidator, "upAreTwoMotorsUsed" );

    maxSpeedTxt = new wxStaticText( this, wxID_ANY, "Train Max Speed" );
    maxSpeedPicker = new wxSpinCtrl( this, wxID_ANY, "7", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10, 7, "upSpeed" );

    topSizerUpEdit->AddSpacer( 10 );
    topSizerUpEdit->AddSpacer( 10 );
    
    topSizerUpEdit->Add( nameTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    topSizerUpEdit->Add( trainName, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );

    topSizerUpEdit->Add( portTxt, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
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
    parent->SetSizer( topSizerUpEdit );
    parent->Layout();
	topSizerUpEdit->Fit( this );
    topSizerUpEdit->SetSizeHints( this );
}

void upEditBox::OnRefreshSerial( wxCommandEvent& event )
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