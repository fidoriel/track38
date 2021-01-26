#include "traineditpanel.h"
#include "track38App.h"

wxBEGIN_EVENT_TABLE( trainEditPanel, wxPanel )
    EVT_RADIOBOX( ID_ChangeControl, trainEditPanel::OnChangeControler )
    EVT_BUTTON( ID_AddTrain, trainEditPanel::OnAddTrain )
    EVT_BUTTON( ID_UpdateTrain, trainEditPanel::OnUpdateTrain )
    EVT_BUTTON( ID_RemoveTrain, trainEditPanel::OnRemoveTrain )
    EVT_LISTBOX( ID_SelectTrain, trainEditPanel::OnSelectTrain )
wxEND_EVENT_TABLE()

trainEditPanel::trainEditPanel( wxNotebook* parent ) : wxPanel( parent )
{
    panelParent = parent;

    topSizer = new wxBoxSizer( wxHORIZONTAL );

    // init Config
    configTrain = new wxFileConfig( wxGetApp().GetAppName(), wxGetApp().GetVendorName(), wxGetApp().ini_dir + "trains.ini", "", wxCONFIG_USE_GLOBAL_FILE );
    wxConfigBase::Set( configTrain );
    track38ConfigTrain = wxConfigBase::Get();

    //
    // Left Picker/Listbox
    //

    leftBox = new wxStaticBox( this, wxID_ANY, "&Pick Train to edit" );
    leftSizer = new wxStaticBoxSizer( leftBox, wxHORIZONTAL );
    m_trainPicker = new wxListBox( this, ID_SelectTrain, wxDefaultPosition, wxSize( -1, 500 ), 0, NULL );

    leftSizer->Add( m_trainPicker, 1, wxGROW | wxALL, 5 );
    leftSizer->SetMinSize( 200, 0 );

    //
    // Right Edit and Buttons
    //

    rightBox = new wxStaticBox( this, wxID_ANY, "&Train Properties" );
    rightSizer = new wxStaticBoxSizer( rightBox, wxVERTICAL );

    //Control kind picker
    wxArrayString options;
    options.Add( "Lego PowerFunctions" );
    options.Add( "Lego PoweredUp" );
    trainKindPicker = new wxRadioBox( this, ID_ChangeControl, "Train Controller", wxDefaultPosition, wxDefaultSize, options, 2, wxRA_HORIZONTAL );

    //PF edit Panel 
    m_trainEditBox = new pfEditBox( this, wxID_ANY, "&Edit PowerFunctions Settings" );

    // Save Panel
    saveBox = new wxStaticBox( this, wxID_ANY, "&" );
    saveSizer = new wxStaticBoxSizer( saveBox, wxHORIZONTAL );
    m_AddBtn = new wxButton( this, ID_AddTrain, "Add", wxDefaultPosition, wxDefaultSize );
    m_UpdateBtn = new wxButton( this, ID_UpdateTrain, "Update", wxDefaultPosition, wxDefaultSize );
    m_RemoveBtn = new wxButton( this, ID_RemoveTrain, "Remove", wxDefaultPosition, wxDefaultSize );
    saveSizer->Add( m_AddBtn, 0, wxALL | wxALIGN_CENTER | wxSHAPED, 5 );
    saveSizer->Add( m_UpdateBtn, 0, wxALL | wxALIGN_CENTER | wxSHAPED, 5 );
    saveSizer->Add( m_RemoveBtn, 0, wxALL | wxALIGN_CENTER | wxSHAPED, 5 );
    saveSizer->Layout();

    rightSizer->Add( trainKindPicker, 0, wxALL | wxGROW, 5 );
    rightSizer->Add( m_trainEditBox, 0, wxALL | wxGROW, 5 );
    rightSizer->Add( saveSizer, 0, wxALL | wxALIGN_CENTER, 5 );

    topSizer->Add( leftSizer, 1, wxALL, 5 );
    topSizer->Add( rightSizer, 4, wxALL, 5 );

    parent->SetSizer( topSizer );
    parent->Layout();
	topSizer->Fit( this );
    topSizer->SetSizeHints( this );

    track38ConfigTrain->SetPath( "/Train/" );
    int count = track38ConfigTrain->GetNumberOfGroups( false );

    long idx;
    wxString out;
    bool exists = track38ConfigTrain->GetFirstGroup( out, idx );
    if (  exists == true  )
        m_trainPicker->AppendString( out );
    
    while ( exists )
    {
        exists = track38ConfigTrain->GetNextGroup( out, idx );
        if (  exists == true  )
            m_trainPicker->AppendString( out );
    }

    if ( m_trainPicker->GetCount() > 0 )
    {
        m_trainPicker->SetSelection( 0 );
        this->SelectTrain();
    }
      
}

void trainEditPanel::OnChangeControler( wxCommandEvent& event )
{
    RefreshPanel();
    panelParent->SetSizerAndFit( topSizer );
    panelParent->Layout();
    panelParent->SendSizeEventToParent();

    int sel = trainKindPicker->GetSelection();

    wxTextCtrl* tName;
    wxChoice* tPort;
    wxSpinCtrl* tSpeed;

    switch ( sel )
    {
        // PF
        case 0: 
            tName = ( wxTextCtrl* ) FindWindow( "pfName" );
            break;
        //UP
        case 1:
            tName = ( wxTextCtrl* ) FindWindow( "upName" );
            break;
    }

    wxString trainSel = m_trainPicker->GetString( m_trainPicker->GetSelection() );
    tName->ChangeValue( trainSel );
}

void trainEditPanel::RefreshPanel()
{
    if ( trainKindPicker )
    {
        rightSizer->Remove( 1 );
        delete m_trainEditBox;
        
        int sel = trainKindPicker->GetSelection();
        switch ( sel )
        {
        case 0:
            m_trainEditBox = new pfEditBox( this, wxID_ANY, "&Edit PowerFunctions Settings" );
            break;
        
        case 1:
            m_trainEditBox = new upEditBox( this, wxID_ANY, "&Edit PoweredUP Settings" );
            break;
        }

        rightSizer->Insert( 1, m_trainEditBox, 0, wxALL | wxGROW, 5 );
    }
}

void trainEditPanel::SaveTrain()
{
    track38ConfigTrain->SetPath( "/Train/" );

    if ( trainKindPicker )
    {
        int sel = trainKindPicker->GetSelection();

        wxTextCtrl* tName;
        wxChoice* tPort;
        wxSpinCtrl* tSpeed;
        wxChoice* tChannel;

        switch ( sel )
        {
            // PF
            case 0: 
            {
                tName = ( wxTextCtrl* ) FindWindow( "pfName" );
                tPort = ( wxChoice* ) FindWindow( "pfPort" );
                wxSpinCtrl* tGpio = ( wxSpinCtrl* ) FindWindow( "pfGpio" );
                tChannel = ( wxChoice* ) FindWindow( "pfChannel" );
                wxChoice* tSubChannel = ( wxChoice* ) FindWindow( "pfSubChannel" );
                tSpeed = ( wxSpinCtrl* ) FindWindow( "pfSpeed" );

                track38ConfigTrain->SetPath( tName->GetValue() );
                track38ConfigTrain->Write( "control", "pf" );
                track38ConfigTrain->Write( "gpio", wxString::Format( wxT( "%i" ), tGpio->GetValue() ) );
                track38ConfigTrain->Write( "subChannel", tSubChannel->GetStringSelection() );
                break;
            }
            //UP
            case 1:
            {
                tName = ( wxTextCtrl* ) FindWindow( "upName" );
                tPort = ( wxChoice* ) FindWindow( "upPort" );
                wxTextCtrl* tHubAdress = ( wxTextCtrl* ) FindWindow( "upHubAdress" );
                tChannel = ( wxChoice* ) FindWindow( "upChannel" );
                wxCheckBox* tAreTwoMotorsUsed = ( wxCheckBox* ) FindWindow( "upAreTwoMotorsUsed" );
                tSpeed = ( wxSpinCtrl* ) FindWindow( "upSpeed" );

                track38ConfigTrain->SetPath( tName->GetValue() );
                track38ConfigTrain->Write( "control", "up" );
                track38ConfigTrain->Write( "hubAdress", tHubAdress->GetValue() );
                track38ConfigTrain->Write( "twoMotorsUsed", tAreTwoMotorsUsed->GetValue() );
                break;
            }
        }

        track38ConfigTrain->Write( "maxSpeed", wxString::Format( wxT( "%i" ), tSpeed->GetValue() ) );
        track38ConfigTrain->Write( "channel", tChannel->GetStringSelection() );

        track38ConfigTrain->Write( "port", tPort->GetStringSelection() );
        
        track38ConfigTrain->Flush();
    }
}

void trainEditPanel::OnSelectTrain( wxCommandEvent& event )
{
    wxString trainSel = m_trainPicker->GetString( m_trainPicker->GetSelection() );

    track38ConfigTrain->SetPath( "/Train/" );
    track38ConfigTrain->SetPath( trainSel );
    wxString control = track38ConfigTrain->Read( "control", "pf" );
    wxChoice* tPort;

    if ( control.IsSameAs( "pf" ) )
        tPort = ( wxChoice* ) FindWindow( "pfPort" );

    else if ( control.IsSameAs( "pf" ) )
        tPort = ( wxChoice* ) FindWindow( "pfPort" );

    tPort->Refresh();

    // if ( tPort->FindString( track38ConfigTrain->Read( "port", "" ) ) == wxNOT_FOUND )
    //     wxMessageBox( "The saved Port was not found. Please plug in the device.", "Port Error" );

    this->SelectTrain();
}

void trainEditPanel::SelectTrain()
{
    if ( m_trainPicker->GetCount() == 0 )
        return;
    
    wxString trainSel = m_trainPicker->GetString( m_trainPicker->GetSelection() );

    track38ConfigTrain->SetPath( "/Train/" );
    track38ConfigTrain->SetPath( trainSel );
    wxString control = track38ConfigTrain->Read( "control", "pf" );

    wxTextCtrl* tName;
    wxChoice* tPort;
    wxSpinCtrl* tSpeed;

    if ( control.IsSameAs( "pf" ) )
    {
        trainKindPicker->SetSelection( 0 );
        RefreshPanel();

        tName = ( wxTextCtrl* ) FindWindow( "pfName" );
        tPort = ( wxChoice* ) FindWindow( "pfPort" );
        wxSpinCtrl* tGpio = ( wxSpinCtrl* ) FindWindow( "pfGpio" );
        wxChoice* tChannel = ( wxChoice* ) FindWindow( "pfChannel" );
        wxChoice* tSubChannel = ( wxChoice* ) FindWindow( "pfSubChannel" );
        tSpeed = ( wxSpinCtrl* ) FindWindow( "pfSpeed" );

        tGpio->SetValue( track38ConfigTrain->Read( "gpio", "13" ) );

        for ( size_t idx = 0; idx < tChannel->GetCount(); idx++ )
        {
            if ( track38ConfigTrain->Read( "channel", "1" ).IsSameAs( tChannel->GetString( idx ) ) )
                tChannel->SetSelection( idx );
        }

        for ( size_t idx = 0; idx < tSubChannel->GetCount(); idx++ )
        {
            if ( track38ConfigTrain->Read( "subChannel", "R" ).IsSameAs( tSubChannel->GetString( idx ) ) )
                tSubChannel->SetSelection( idx );            
        }
    }

    else if ( control.IsSameAs( "up" ) )
    {
        trainKindPicker->SetSelection( 1 );
        RefreshPanel();

        tName = ( wxTextCtrl* ) FindWindow( "upName" );
        tPort = ( wxChoice* ) FindWindow( "upPort" );
        wxTextCtrl* tHubAdress = ( wxTextCtrl* ) FindWindow( "upHubAdress" );
        wxChoice* tChannel = ( wxChoice* ) FindWindow( "upChannel" );
        wxCheckBox* tAreTwoMotorsUsed = ( wxCheckBox* ) FindWindow( "upAreTwoMotorsUsed" );
        tSpeed = ( wxSpinCtrl* ) FindWindow( "upSpeed" );

        tHubAdress->ChangeValue( track38ConfigTrain->Read( "hubAdress", "" ) );

        for ( size_t idx = 0; idx < tChannel->GetCount(); idx++ )
        {
            if ( track38ConfigTrain->Read( "channel", "1" ).IsSameAs( tChannel->GetString( idx ) ) )
                tChannel->SetSelection( idx );         
        }
        tAreTwoMotorsUsed->SetValue( track38ConfigTrain->Read( "twoMotorsUsed", false ) );
    }

    tName->ChangeValue( trainSel );

    tSpeed->SetValue( track38ConfigTrain->Read( "maxSpeed", "7" ) );

    panelParent->SetSizerAndFit( topSizer );
    panelParent->Layout();
    panelParent->SendSizeEventToParent();

    if ( ( tPort->FindString( track38ConfigTrain->Read( "port", "" ) ) == wxNOT_FOUND ) && ( tPort->FindString( "Please select a new Port" ) == wxNOT_FOUND ) )
    { 
        // tPort->AppendString( "Please select a new Port" );
        // tPort->SetStringSelection( "Please select a new Port" );
    }

    else
    {
        for ( size_t idx = 0; idx < tPort->GetCount(); idx++ )
        {
            if ( track38ConfigTrain->Read( "port", "" ).IsSameAs( tPort->GetString( idx ) ) )
                tPort->SetSelection( idx );   
        }
    }
}

void trainEditPanel::OnAddTrain( wxCommandEvent& event )
{

    int sel = trainKindPicker->GetSelection();
    wxTextCtrl* tName;
    switch ( sel )
    {
        // PF
        case 0: 
            tName = ( wxTextCtrl* ) FindWindow( "pfName" );
            break;
        // UP
        case 1:
            tName = ( wxTextCtrl* ) FindWindow( "upName" );
            break;          
    }

    if ( m_trainPicker->FindString( tName->GetValue() ) != wxNOT_FOUND )
    {
        wxMessageDialog dialog( this, "The Train does already Exists. Do you want to Overwrite?", "Overwrite?", wxYES_NO | wxICON_INFORMATION );
        switch ( dialog.ShowModal() )
        {
            case wxID_YES:
                SaveTrain();
                break;

            case wxID_NO:
                return;
                break;
        }
    }
    else
    {
        SaveTrain();
        m_trainPicker->AppendString( tName->GetValue() );
        m_trainPicker->SetStringSelection( tName->GetValue() );
    }
    
}

void trainEditPanel::OnUpdateTrain( wxCommandEvent& event )
{
    m_trainPicker->Delete( m_trainPicker->GetSelection() );
    SaveTrain();

    int sel = trainKindPicker->GetSelection();
    wxTextCtrl* tName;
    switch ( sel )
    {
        // PF
        case 0: 
            tName = ( wxTextCtrl* ) FindWindow( "pfName" );
            break;
        // UP
        case 1:
            tName = ( wxTextCtrl* ) FindWindow( "upName" );
            break;          
    }

    m_trainPicker->AppendString( tName->GetValue() );
    m_trainPicker->SetStringSelection( tName->GetValue() );
}

void trainEditPanel::OnRemoveTrain( wxCommandEvent& event )
{
    wxMessageDialog dialog( this, "Do you want to remove the Train?", "Remove?", wxYES_NO | wxICON_INFORMATION );
    switch ( dialog.ShowModal() )
    {
        case wxID_YES:
            RemoveTrain();
            if ( m_trainPicker->GetCount() )
            {
                m_trainPicker->SetSelection( 0 );
                OnSelectTrain( event );
            }
            break;

        case wxID_NO:
            return;
            break;
    }
}

void trainEditPanel::RemoveTrain()
{
    int sel = trainKindPicker->GetSelection();
    wxTextCtrl* tName;
    switch ( sel )
    {
        // PF
        case 0: 
            tName = ( wxTextCtrl* ) FindWindow( "pfName" );
            break;
        // UP
        case 1:
            tName = ( wxTextCtrl* ) FindWindow( "upName" );
            break;          
    }

    track38ConfigTrain->SetPath( "/Train/" );
    track38ConfigTrain->DeleteGroup( tName->GetValue() );
    track38ConfigTrain->DeleteGroup( m_trainPicker->GetString( m_trainPicker->GetSelection() ) );
    track38ConfigTrain->Flush();

    /*for ( size_t idx = 0; idx < m_trainPicker->GetCount(); idx++ )
    {
        if ( tName->GetValue().IsSameAs( m_trainPicker->GetString( idx ) ) )
            m_trainPicker->Delete( idx );         
    }*/
    m_trainPicker->Delete( m_trainPicker->GetSelection() );
}