#include "mapeditpanel.h"

wxBEGIN_EVENT_TABLE( mapEditPanel, wxPanel )
    EVT_BUTTON( ID_RefreshSerial, mapEditPanel::OnRefreshSerial )
    EVT_BUTTON( ID_AddSwitch, mapEditPanel::OnAddSwitch )
    EVT_BUTTON( ID_UpdateSwitch, mapEditPanel::OnUpdateSwitch )
    EVT_BUTTON( ID_RemoveSwitch, mapEditPanel::OnRemoveSwitch )
    EVT_LISTBOX( ID_SelectSwitch, mapEditPanel::OnSelectSwitch )
    EVT_GRID_CMD_CELL_LEFT_CLICK( ID_DragPicker, mapEditPanel::OnDragCellPicker )
    EVT_GRID_CMD_CELL_LEFT_CLICK( ID_Map, mapEditPanel::OnDragCellMap )
    EVT_GRID_CMD_CELL_LEFT_DCLICK( ID_Map, mapEditPanel::OnDClickMap )
    EVT_GRID_CMD_CELL_RIGHT_CLICK( ID_Map, mapEditPanel::OnRClickMap )
wxEND_EVENT_TABLE()

mapEditPanel::mapEditPanel( wxNotebook* parent ) : wxPanel( parent )
{
    //
    // Topsizer
    //
    topSizer = new wxBoxSizer( wxVERTICAL );
    bottomSizer = new wxBoxSizer( wxHORIZONTAL );

    //
    // Map
    //
    
    map = new wxGrid( this, ID_Map, wxPoint( 0, 0 ), wxSize( 40, 40) );

    map->CreateGrid( 25, 50);
    map->HideColLabels();
    map->HideRowLabels();
    map->EnableEditing( false );
    // map->EnableGridLines( false );
    map->EnableDragGridSize( false );
    map->DisableCellEditControl();
    map->SetMargins( 0 , 0 );
    map->SetCellHighlightColour( *wxWHITE );
    map->SetCellHighlightROPenWidth( 0 );

    map->SetDropTarget( new gridTextDropTarget( map ) );

    for ( size_t i = 0; i < map->GetNumberRows(); i++ )
    {
        map->SetRowSize( i, 50 );
    }

    for ( size_t i = 0; i < map->GetNumberCols(); i++ )
    {
        map->SetColSize( i, 50 );
    }

    for ( size_t col = 0; col < map->GetNumberCols(); col++ )
    {
        for ( size_t row = 0; row < map->GetNumberRows(); row++ )
        {
            map->SetReadOnly( row, col );
            map->SetCellBackgroundColour( row, col, *wxWHITE );
            map->SetCellTextColour( row, col, *wxBLACK );
        }
    }

    map->SetCellRenderer( 0, 0, new cellImageRenderer( "straightn.png" ) );
    map->SetCellRenderer( 0, 1, new cellImageRenderer( "straightn.png" ) );
    map->SetCellRenderer( 0, 2, new cellImageRenderer( "curven.png" ) );
    map->SetCellRenderer( 1, 1, new cellImageRenderer( "straightn.png" ) );
    map->SetCellRenderer( 2, 2, new cellImageRenderer( "straightn.png" ) );
    map->SetCellRenderer( 3, 3, new cellImageRenderer( "straightn.png" ) );

    //
    // Switch Edit Panel
    //

    // Picker
    switchPickerBox = new editBox( this, wxID_ANY, "Pick Switch to edit" );
    switchPickerBoxSizer = new wxStaticBoxSizer( switchPickerBox, wxHORIZONTAL );
    m_switchPicker = new wxListBox( this, ID_SelectSwitch, wxDefaultPosition, wxDefaultSize, 0, NULL );

    // Edit Controls
    editSizer = new wxFlexGridSizer( 2, 0, 0 );

    labelName = new wxStaticText( this, wxID_ANY, "Switch Name" );
    switchName = new wxTextCtrl( this, wxID_ANY, "", wxDefaultPosition, wxSize( 300, -1 ) );

    refreshSizer = new wxBoxSizer( wxHORIZONTAL );
    labelPort = new wxStaticText( this, wxID_ANY, "Arduino ComPort:" );
    this->switchPickerBox->refreshSerial();
    portPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 300, -1 ), switchPickerBox->serialArray, 0L, wxDefaultValidator, "switchPort" );
    m_RefreshBtn = new wxButton( this, ID_RefreshSerial, "Refresh", wxDefaultPosition, wxDefaultSize );
    refreshSizer->Add( portPicker, 0, wxALL, 10 );
    refreshSizer->Add( m_RefreshBtn, 0, wxALL, 10 );

    labelGpio = new wxStaticText( this, wxID_ANY, "GPIO:" );
    gpioPicker = new wxSpinCtrl( this, wxID_ANY, "2", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 2, "switchGpio" );

    labelDir = new wxStaticText( this, wxID_ANY, "Switch Direction" );
    dirList.Add( "L" );
    dirList.Add( "R" );
    dirPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 60, -1 ), dirList, 0L, wxDefaultValidator, "switchDir" );

    labelManufacturer = new wxStaticText( this, wxID_ANY, "Manufacturer:" );
    manufacturerList.Add( "4d Brixx" );
    manufacturerPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, manufacturerList, 0L, wxDefaultValidator, "manufacturerPicker" );

    editSizer->Add( labelName, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );
    editSizer->Add( switchName, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );

    editSizer->Add( labelPort, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );
    editSizer->Add( refreshSizer, 0, wxALL | ( wxALL & ~wxLEFT ), 0 );

    editSizer->Add( labelGpio, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );
    editSizer->Add( gpioPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );

    editSizer->Add( labelDir, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );
    editSizer->Add( dirPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );

    editSizer->Add( labelManufacturer, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );
    editSizer->Add( manufacturerPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 10 );

    // Save Sizer
    wxStaticBox* saveBox = new wxStaticBox( this, wxID_ANY, "" );
    wxStaticBoxSizer* saveSizer = new wxStaticBoxSizer( saveBox, wxVERTICAL );
    wxButton* m_AddBtn = new wxButton( this, ID_AddSwitch, "Add", wxDefaultPosition, wxDefaultSize );
    wxButton* m_UpdateBtn = new wxButton( this, ID_UpdateSwitch, "Update", wxDefaultPosition, wxDefaultSize );
    wxButton* m_RemoveBtn = new wxButton( this, ID_RemoveSwitch, "Remove", wxDefaultPosition, wxDefaultSize );
    saveSizer->Add( m_AddBtn, 0, wxALL | wxALIGN_CENTER | wxSHAPED, 5);
    saveSizer->Add( m_UpdateBtn, 0, wxALL | wxALIGN_CENTER | wxSHAPED, 5 );
    saveSizer->Add( m_RemoveBtn, 0, wxALL | wxALIGN_CENTER | wxSHAPED, 5 );
    saveSizer->Layout();

    switchPickerBoxSizer->Add( m_switchPicker, 0, wxEXPAND | wxALL, 10 );
    switchPickerBoxSizer->Add( editSizer, 0, wxEXPAND | wxALL, 10 );
    switchPickerBoxSizer->Add( saveSizer, 0, wxALL | wxALIGN_CENTER | wxSHAPED, 10 );

    //
    // Map Picker
    //

    mapPickerBox = new wxStaticBox( this, wxID_ANY, "Symbol Picker");
    mapPickerBoxSizer = new wxStaticBoxSizer( mapPickerBox, wxVERTICAL );

    pickerGrid = new wxGrid( this, ID_DragPicker, wxPoint( 0, 0 ), wxSize( 10, 10) );

    // Grid
    pickerGrid->CreateGrid( 6, 1 );
    pickerGrid->HideColLabels();
    pickerGrid->HideRowLabels();
    pickerGrid->EnableEditing( false );
    pickerGrid->EnableGridLines( false );
    pickerGrid->EnableDragGridSize( false );
    pickerGrid->DisableCellEditControl();
    pickerGrid->SetMargins( 0 - wxSYS_VSCROLL_X , 0 );
    pickerGrid->SetCellHighlightColour( *wxWHITE );
    pickerGrid->SetCellHighlightROPenWidth( 0 );


    for ( size_t col = 0; col < pickerGrid->GetNumberCols(); col++ )
    {
        for ( size_t row = 0; row < pickerGrid->GetNumberRows(); row++ )
        {
            pickerGrid->SetReadOnly( row, col );
            pickerGrid->SetRowSize( row, ( pickerGrid->GetColSize( 0 ) - wxSYS_VSCROLL_X ) );
            pickerGrid->SetCellBackgroundColour( row, col, *wxWHITE );
        }
    }

    pickerGrid->SetCellRenderer( 0, 0, new cellImageRenderer( "straightn.png", 2 ) );
    pickerGrid->SetCellRenderer( 1, 0, new cellImageRenderer( "curven.png", 2 ) );
    pickerGrid->SetCellRenderer( 2, 0, new cellImageRenderer( "switchLn.png", 2 ) );
    pickerGrid->SetCellRenderer( 3, 0, new cellImageRenderer( "switchRn.png", 2 ) );
    pickerGrid->SetCellRenderer( 4, 0, new cellImageRenderer( "stopn.png", 2 ) );
    pickerGrid->SetCellRenderer( 5, 0, new cellImageRenderer( "crossn.png", 2 ) );

    mapPickerBoxSizer->Add( pickerGrid, 1, wxEXPAND | wxALL, 5 );

    // 
    // Topsizer
    //

    bottomSizer->Add( switchPickerBoxSizer, 4, wxALIGN_CENTER | wxALL, 5);
    bottomSizer->Add( mapPickerBoxSizer, 0, wxEXPAND | wxALL, 5 );

    topSizer->Add( map, 3, wxEXPAND | wxALL, 5 );
    topSizer->Add( bottomSizer, 0, wxALIGN_CENTER | wxALL, 5);

    this->SetSizerAndFit( topSizer );
    this->Layout();
    topSizer->Fit( this );
    topSizer->SetSizeHints( this );

    this->loadSwitches();
}

void mapEditPanel::OnRefreshSerial( wxCommandEvent& event )
{
    refreshSizer->Detach( 0 );
    delete portPicker;
    this->switchPickerBox->refreshSerial();
    portPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 300, -1 ), switchPickerBox->serialArray, 0L, wxDefaultValidator, "switchPort" );
    refreshSizer->Insert( 0, portPicker, 0, wxALL, 10 );
    refreshSizer->Layout();
}

void mapEditPanel::OnAddSwitch( wxCommandEvent& event )
{
    if ( m_switchPicker->FindString( switchName->GetValue() ) != wxNOT_FOUND )
    {
        wxMessageDialog dialog( this, "The Switch does already Exists. Do you want to Overwrite?", "Overwrite?", wxYES_NO | wxICON_INFORMATION );
        switch ( dialog.ShowModal() )
        {
            case wxID_YES:
                this->AddSwitch();
                break;

            case wxID_NO:
                return;
                break;
        }
    }
    else
    {
        this->AddSwitch();
        m_switchPicker->AppendString( switchName->GetValue() );
        m_switchPicker->SetStringSelection( switchName->GetValue() );
    }
}

void mapEditPanel::AddSwitch()
{
    wxConfigBase* track38ConfigMap = wxConfigBase::Get();

    track38ConfigMap->SetPath( "/Switch/" );
    track38ConfigMap->SetPath( switchName->GetValue() );
    track38ConfigMap->Write( "gpio", wxString::Format( wxT( "%i" ), gpioPicker->GetValue() ) );
    track38ConfigMap->Write( "dir", dirPicker->GetStringSelection() );
    track38ConfigMap->Write( "port", portPicker->GetStringSelection() );
    track38ConfigMap->Write( "manufacturer", manufacturerPicker->GetStringSelection() );
    track38ConfigMap->Flush();
}

void mapEditPanel::OnUpdateSwitch( wxCommandEvent& event )
{
    m_switchPicker->Delete( m_switchPicker->GetSelection() );
    this->AddSwitch();
    m_switchPicker->AppendString( switchName->GetValue() );
    m_switchPicker->SetStringSelection( switchName->GetValue() );
}

void mapEditPanel::OnRemoveSwitch( wxCommandEvent& event )
{
    wxMessageDialog dialog( this, "Do you want to remove the Train?", "Remove?", wxYES_NO | wxICON_INFORMATION );
    switch ( dialog.ShowModal() )
    {
        case wxID_YES:
            {
                wxConfigBase* track38ConfigMap = wxConfigBase::Get();

                track38ConfigMap->SetPath( "/Switch/" );
                track38ConfigMap->DeleteGroup( switchName->GetValue() );
                track38ConfigMap->DeleteGroup( m_switchPicker->GetString( m_switchPicker->GetSelection() ) );
                track38ConfigMap->Flush();
                m_switchPicker->Delete( m_switchPicker->GetSelection() );

                if ( m_switchPicker->GetCount() )
                {
                    m_switchPicker->SetSelection( 0 );
                    this->SelectSwitch();
                }
            }
            break;

        case wxID_NO:
            return;
            break;
    }
}

void mapEditPanel::OnSelectSwitch( wxCommandEvent& event )
{
    this->SelectSwitch();

    wxConfigBase* track38ConfigMap = wxConfigBase::Get();
    if ( portPicker->FindString( track38ConfigMap->Read( "port", "" ) ) == wxNOT_FOUND )
        wxMessageBox( "The saved Port was not found. Please plug in the device.", "Port Error" );
}

void mapEditPanel::SelectSwitch()
{
    if ( m_switchPicker->GetCount() == 0 )
        return; 
    
    wxConfigBase* track38ConfigMap = wxConfigBase::Get();

    track38ConfigMap->SetPath( "/Switch/" );
    track38ConfigMap->SetPath( m_switchPicker->GetStringSelection() );

    switchName->SetValue( m_switchPicker->GetStringSelection() );

    if ( ( portPicker->FindString( track38ConfigMap->Read( "port", "" ) ) == wxNOT_FOUND ) && ( portPicker->FindString( "Please select a new Port" ) == wxNOT_FOUND ) )
    {
        portPicker->AppendString( "Please select a new Port" );
        portPicker->SetStringSelection( "Please select a new Port" );
    }
    else
    {
        for ( size_t idx = 0; idx < portPicker->GetCount(); idx++ )
        {
            if ( track38ConfigMap->Read( "port", "" ).IsSameAs( portPicker->GetString( idx ) ) )
                portPicker->SetSelection( idx );   
        }
    }

    gpioPicker->SetValue( track38ConfigMap->Read( "gpio", "2" ) );

    for ( size_t idx = 0; idx < dirPicker->GetCount(); idx++ )
    {
        if ( track38ConfigMap->Read( "dir", "1" ).IsSameAs( dirPicker->GetString( idx ) ) )
            dirPicker->SetSelection( idx );         
    }

    for ( size_t idx = 0; idx < manufacturerPicker->GetCount(); idx++ )
    {
        if ( track38ConfigMap->Read( "manufacturer", "1" ).IsSameAs( manufacturerPicker->GetString( idx ) ) )
            manufacturerPicker->SetSelection( idx );         
    }
}

void mapEditPanel::loadSwitches()
{
    wxConfigBase* track38ConfigMap = wxConfigBase::Get();

    track38ConfigMap->SetPath( "/Switch/" );
    int count = track38ConfigMap->GetNumberOfGroups( false );

    long idx;
    wxString out;
    bool exists = track38ConfigMap->GetFirstGroup( out, idx );
    if (  exists == true  )
        m_switchPicker->AppendString( out );
    
    while ( exists )
    {
        exists = track38ConfigMap->GetNextGroup( out, idx );
        if (  exists == true  )
            m_switchPicker->AppendString( out );
    }

    if ( m_switchPicker->GetCount() != 0 )
    {
        m_switchPicker->SetSelection( 0 );
        this->SelectSwitch();
    }
}

void mapEditPanel::OnDragCellPicker( wxGridEvent& event )
{
    wxTextDataObject myData(wxT("This text will be dragged."));
    wxDropSource dragSource(this);
    dragSource.SetData(myData);
    wxDragResult result = dragSource.DoDragDrop(wxDrag_AllowMove);
}

void mapEditPanel::OnDragCellMap( wxGridEvent& event )
{
    wxTextDataObject myData(wxT(""));
    wxDropSource dragSource(this);
    dragSource.SetData(myData);
    wxDragResult result = dragSource.DoDragDrop(wxDrag_AllowMove);
}

void mapEditPanel::OnDClickMap( wxGridEvent& event )
{

}

void mapEditPanel::OnRClickMap( wxGridEvent& event )
{
    
}