#include "traineditpanel.h"
#include "track38App.h"

wxBEGIN_EVENT_TABLE( trainEditPanel, wxPanel )
    EVT_RADIOBOX( ID_ChangeControl, trainEditPanel::OnChangeControler )
    EVT_LISTBOX( ID_SelectTrain, trainEditPanel::OnSelectTrain )
    EVT_BUTTON( ID_NewTrain, trainEditPanel::OnNewTrain )
    EVT_BUTTON( ID_RemoveTrain, trainEditPanel::OnRemoveTrain )
    EVT_BUTTON( ID_RenameTrain, trainEditPanel::OnRenameTrain )
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
    m_trainPicker = new wxListBox( this, ID_SelectTrain, wxDefaultPosition, wxSize( -1, 400 ), 0, NULL );

    // Save Panel
    saveSizer = new wxBoxSizer( wxHORIZONTAL );
    m_NewBtn = new wxButton( this, ID_NewTrain, "New", wxDefaultPosition, wxDefaultSize );
    m_RenameBtn = new wxButton( this, ID_RenameTrain, "Rename", wxDefaultPosition, wxDefaultSize );
    m_RemoveBtn = new wxButton( this, ID_RemoveTrain, "Remove", wxDefaultPosition, wxDefaultSize );
    saveSizer->Add( m_NewBtn, 0, wxALL | wxALIGN_CENTER | wxSHAPED, 5 );
    saveSizer->Add( m_RenameBtn, 0, wxALL | wxALIGN_CENTER | wxSHAPED, 5 );
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

    this->initConf();
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
    this->RefreshPanel();
    wxString trainSel = m_trainPicker->GetString( m_trainPicker->GetSelection() );

    if ( trainKindPicker->GetSelection() == 0 )
    {
        pfEditBox* tmpPtr = ( pfEditBox* ) m_trainEditBox;
        tmpPtr->SetTrainName( trainSel );
        tmpPtr->SaveTrain();
    }

    else if ( trainKindPicker->GetSelection() == 1 )
    {
        upEditBox* tmpPtr = ( upEditBox* ) m_trainEditBox;
        tmpPtr->SetTrainName( trainSel );
        tmpPtr->SaveTrain();
    }

    if ( m_trainPicker->GetCount() == 0 )
        return;
}

void trainEditPanel::RefreshPanel()
{
    if ( trainKindPicker )
    {
        rightSizer->Remove( 1 );
        delete m_trainEditBox;

        this->initConf();
        track38ConfigTrain->SetPath( "/Train/" );
        track38ConfigTrain->SetPath( m_trainPicker->GetStringSelection() );
        
        int sel = trainKindPicker->GetSelection();
        switch ( sel )
        {
        case 0:
            m_trainEditBox = new pfEditBox( this, wxID_ANY, "Edit PowerFunctions Settings" );
            track38ConfigTrain->Write( "control", "pf" );
            break;
        
        case 1:
            m_trainEditBox = new upEditBox( this, wxID_ANY, "Edit PoweredUP Settings" );
            track38ConfigTrain->Write( "control", "up" );
            break;
        }

        rightSizer->Insert( 1, m_trainEditBox, 0, wxALL | wxGROW, 5 );
    }

    rightSizer->Layout();
    panelParent->SendSizeEvent();
}

void trainEditPanel::initConf()
{
    // Init config
    configTrain = new wxFileConfig( wxGetApp().GetAppName(), wxGetApp().GetVendorName(), wxGetApp().ini_dir + "trains.ini", "", wxCONFIG_USE_GLOBAL_FILE );
    wxConfigBase::Set( configTrain );
    track38ConfigTrain = wxConfigBase::Get();
}

void trainEditPanel::OnNewTrain( wxCommandEvent& event )
{
    int i = 1;
    while ( !( m_trainPicker->FindString( "Train " + wxString::Format( wxT( "%i" ), i ) ) == wxNOT_FOUND ) )
        i += 1;

    wxString trainName = "Train " + wxString::Format( wxT( "%i" ), i );

    m_trainPicker->Append( trainName );

    trainKindPicker->SetSelection( 0 );
    RefreshPanel();
    pfEditBox* tmpPtr = ( pfEditBox* ) m_trainEditBox;
    
    tmpPtr->AddTrain( trainName );
    m_trainPicker->SetStringSelection( trainName );
}

wxString trainEditPanel::GetTrainControl( wxString trainSel )
{
    this->initConf();
    track38ConfigTrain->SetPath( "/Train/" );
    track38ConfigTrain->SetPath( trainSel );
    return track38ConfigTrain->Read( "control", "" );
}

void trainEditPanel::OnRemoveTrain( wxCommandEvent& event )
{
    if ( m_trainPicker->GetCount() == 0 )
        return;

    wxString msg = "Do you want to remove " + m_trainPicker->GetStringSelection() + "?";
    wxString hl = "Remove " + m_trainPicker->GetStringSelection() + "?";
    wxMessageDialog dialog( this, msg, hl, wxYES_NO | wxICON_INFORMATION );
    switch ( dialog.ShowModal() )
    {
        case wxID_YES:
        {
            this->initConf();
            track38ConfigTrain->SetPath( "/Train/" );
            track38ConfigTrain->DeleteGroup( m_trainPicker->GetStringSelection() );
            m_trainPicker->Delete( m_trainPicker->GetSelection() );
            track38ConfigTrain->Flush();

            if ( m_trainPicker->GetCount() > 0 )
            {
                m_trainPicker->SetSelection( 0 );
                this->SelectTrain();
            }
        }
            break;

        case wxID_NO:
            return;
            break;
    }
}

void trainEditPanel::OnSelectTrain( wxCommandEvent& event )
{
    this->SaveTrain();
    this->SelectTrain();
}

void trainEditPanel::SaveTrain()
{
    if ( m_trainPicker->GetCount() == 0 )
        return;
    
    if ( trainKindPicker->GetSelection() == 0 )
    {
        pfEditBox* tmpPtr = ( pfEditBox* ) m_trainEditBox;
        tmpPtr->SaveTrain();
    }

    else if ( trainKindPicker->GetSelection() == 1 )
    {
        upEditBox* tmpPtr = ( upEditBox* ) m_trainEditBox;
        tmpPtr->SaveTrain();
    }
}

void trainEditPanel::SelectTrain()
{
    if ( m_trainPicker->GetCount() == 0 )
        return;

    if ( this->GetTrainControl( m_trainPicker->GetStringSelection() ).IsSameAs( "pf" ) )
    {
        trainKindPicker->SetSelection( 0 );
        this->RefreshPanel();
        pfEditBox* tmpPtr = ( pfEditBox* ) m_trainEditBox;
        tmpPtr->SelectTrain( m_trainPicker->GetStringSelection() );
    }

    else if ( this->GetTrainControl( m_trainPicker->GetStringSelection() ).IsSameAs( "up" ) )
    {
        trainKindPicker->SetSelection( 1 );
        this->RefreshPanel();
        upEditBox* tmpPtr = ( upEditBox* ) m_trainEditBox;
        tmpPtr->SelectTrain( m_trainPicker->GetStringSelection() );
    }
}

void trainEditPanel::OnRenameTrain( wxCommandEvent& event )
{
    if ( m_trainPicker->GetCount() == 0 )
        return;

    this->initConf();
    track38ConfigTrain->SetPath( "/Train/" );

    wxString oldName = m_trainPicker->GetStringSelection();
    wxString newName;

    wxTextEntryDialog dlg( this, "Please enter new Name", "Rename", oldName );
    switch ( dlg.ShowModal() )
    {
        case wxID_OK:
            newName = dlg.GetValue();

            // Same Name
            if ( newName.IsSameAs( oldName ) )
                return;

            // Other Name But exists
            else if ( track38ConfigTrain->Exists( newName ) )
            {
                wxMessageDialog dialog( this, "The enterd train exists alredy. Do you want to replace it?", "Overwrite " + newName + "?", wxYES_NO | wxICON_INFORMATION );
                switch ( dialog.ShowModal() )
                {
                    case wxID_YES:
                        track38ConfigTrain->DeleteGroup( newName );
                        track38ConfigTrain->RenameGroup( oldName, newName );
                        m_trainPicker->Delete( m_trainPicker->FindString( oldName ) );
                        break;

                    case wxID_NO:
                        return;
                        break;
                }
            }

            else
            {
                track38ConfigTrain->RenameGroup( oldName, newName );
                int oldPos = m_trainPicker->FindString( oldName );
                m_trainPicker->Delete( oldPos );
                m_trainPicker->Insert( newName, oldPos );
            }
            
            break;

        case wxID_CANCEL:
            return;
            break;
    }
    track38ConfigTrain->Flush();
    this->m_trainPicker->SetStringSelection( newName ); 
    this->SelectTrain();
}