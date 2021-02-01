#include "traineditpanel.h"
#include "track38App.h"

wxBEGIN_EVENT_TABLE( trainEditPanel, wxPanel )
    //EVT_RADIOBOX( ID_ChangeControl, trainEditPanel::OnChangeControler )
    //EVT_LISTBOX( ID_SelectTrain, trainEditPanel::OnSelectTrain )
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

    leftBox = new wxStaticBox( this, wxID_ANY, "Pick Train to edit" );
    leftSizer = new wxStaticBoxSizer( leftBox, wxVERTICAL );
    m_trainPicker = new wxListBox( this, ID_SelectTrain, wxDefaultPosition, wxSize( -1, 500 ), 0, NULL );

    // Save Panel
    saveSizer = new wxBoxSizer( wxHORIZONTAL );
    m_NewBtn = new wxButton( this, ID_AddTrain, "New", wxDefaultPosition, wxDefaultSize );
    m_RemoveBtn = new wxButton( this, ID_RemoveTrain, "Remove", wxDefaultPosition, wxDefaultSize );
    saveSizer->Add( m_NewBtn, 0, wxALL | wxALIGN_CENTER | wxSHAPED, 5 );
    saveSizer->Add( m_RemoveBtn, 0, wxALL | wxALIGN_CENTER | wxSHAPED, 5 );
    saveSizer->Layout();

    leftSizer->Add( m_trainPicker, 1, wxGROW | wxALL, 5 );
    leftSizer->Add( saveSizer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5 );
    leftSizer->SetMinSize( 200, 0 );

    //
    // Right Edit and Buttons
    //

    rightBox = new wxStaticBox( this, wxID_ANY, "Train Properties" );
    rightSizer = new wxStaticBoxSizer( rightBox, wxVERTICAL );

    //Control kind picker
    wxArrayString options;
    options.Add( "Lego PowerFunctions" );
    options.Add( "Lego PoweredUp" );
    options.Add( "Lego 9V RC" );
    // options.Add( "Buwizz" );
    // options.Add( "SBrick" );
    trainKindPicker = new wxRadioBox( this, ID_ChangeControl, "Train Controller", wxDefaultPosition, wxDefaultSize, options, 3, wxRA_HORIZONTAL );
    trainKindPicker->Enable( 1, false );
    trainKindPicker->Enable( 2, false );
    // trainKindPicker->Enable( 3, false );
    // trainKindPicker->Enable( 4, false );

    //PF edit Panel 
    m_trainEditBox = new pfEditBox( this, wxID_ANY, "Edit PowerFunctions Settings" );

    rightSizer->Add( trainKindPicker, 0, wxALL | wxGROW, 5 );
    rightSizer->Add( m_trainEditBox, 0, wxALL | wxGROW, 5 );
    rightSizer->Layout();

    topSizer->Add( leftSizer, 1, wxALL, 5 );
    topSizer->Add( rightSizer, 4, wxALL, 5 );

    parent->SetSizer( topSizer );
    parent->Layout();
	topSizer->Fit( this );

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
        // this->SelectTrain();
    }
}

void trainEditPanel::OnChangeControler( wxCommandEvent& event )
{
    this->RefreshPanel();
    panelParent->SendSizeEvent();

    int sel = trainKindPicker->GetSelection();

    wxTextCtrl* tName;

    switch ( sel )
    {
        // PF
        case 0: 
            tName = ( wxTextCtrl* ) FindWindow( "tName" );
            break;
        //UP
        case 1:
            tName = ( wxTextCtrl* ) FindWindow( "tName" );
            break;
    }
    if ( m_trainPicker->GetCount() == 0 )
        return;

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
            m_trainEditBox = new pfEditBox( this, wxID_ANY, "Edit PowerFunctions Settings" );
            break;
        
        case 1:
            m_trainEditBox = new upEditBox( this, wxID_ANY, "Edit PoweredUP Settings" );
            break;
        }

        rightSizer->Insert( 1, m_trainEditBox, 0, wxALL | wxGROW, 5 );
    }
    rightSizer->Layout();
}

void trainEditPanel::initConf()
{
    // Init config
    configTrain = new wxFileConfig( wxGetApp().GetAppName(), wxGetApp().GetVendorName(), wxGetApp().ini_dir + "train.ini", "", wxCONFIG_USE_GLOBAL_FILE );
    wxConfigBase::Set( configTrain );
    track38ConfigTrain = wxConfigBase::Get();
}