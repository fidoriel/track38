#include "mapeditpanel.h"
#include "track38App.h"

wxBEGIN_EVENT_TABLE( mapEditPanel, wxPanel )
    EVT_BUTTON( ID_RefreshSerial, mapEditPanel::OnRefreshSerial )
    EVT_BUTTON( ID_RenameSwitch, mapEditPanel::OnRenameSwitch )
    EVT_BUTTON( ID_RemoveSwitch, mapEditPanel::OnRemoveSwitch )
    EVT_LISTBOX( ID_SelectSwitch, mapEditPanel::OnSelectSwitch )
    EVT_GRID_CMD_CELL_LEFT_CLICK( ID_DragPicker, mapEditPanel::OnDragCellPicker )
    EVT_GRID_CMD_CELL_LEFT_CLICK( ID_Map, mapEditPanel::OnLClickMap )
    EVT_GRID_CMD_CELL_RIGHT_CLICK( ID_Map, mapEditPanel::OnRClickMap )
wxEND_EVENT_TABLE()

mapEditPanel::mapEditPanel( wxNotebook* parent ) : wxPanel( parent )
{
    //
    // Topsizer
    //
    topSizer = new wxBoxSizer( wxVERTICAL );
    bottomSizer = new wxBoxSizer( wxHORIZONTAL );

    this->initConfig();
    //
    // Map
    //

    // Options
    this->clickToDrag = false;
    
    map = new wxGrid( this, ID_Map, wxPoint( 0, 0 ), wxSize( 40, 40) );
    this->LoadMapFromFile();

    //
    // Switch Edit Panel
    //

    // Picker
    switchPickerBox = new editBox( this, wxID_ANY, "Pick Switch to edit" );
    switchPickerBoxSizer = new wxStaticBoxSizer( switchPickerBox, wxHORIZONTAL );
    m_switchPicker = new wxListBox( this, ID_SelectSwitch, wxDefaultPosition, wxSize( 100, -1 ), 0, NULL );

    // Edit Controls
    editSizer = new wxFlexGridSizer( 2, 0, 0 );

    labelName = new wxStaticText( this, wxID_ANY, "Switch Name" );
    switchName = new wxTextCtrl( this, wxID_ANY, "", wxDefaultPosition, wxSize( 200, -1 ) );
    switchName->SetEditable( false );

    refreshSizer = new wxBoxSizer( wxHORIZONTAL );
    labelPort = new wxStaticText( this, wxID_ANY, "Arduino ComPort:" );
    this->switchPickerBox->refreshSerial();
    portPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 200, -1 ), switchPickerBox->serialArray, 0L, wxDefaultValidator, "switchPort" );
    m_RefreshBtn = new wxButton( this, ID_RefreshSerial, "Refresh", wxDefaultPosition, wxDefaultSize );
    refreshSizer->Add( portPicker, 0, wxALL, 5 );
    refreshSizer->Add( m_RefreshBtn, 0, wxALL, 5 );

    labelGpio = new wxStaticText( this, wxID_ANY, "GPIO:" );
    gpioPicker = new wxSpinCtrl( this, wxID_ANY, "2", wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 2, "switchGpio" );

    labelDir = new wxStaticText( this, wxID_ANY, "Switch Direction" );
    dirList.Add( "L" );
    dirList.Add( "R" );
    dirPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 60, -1 ), dirList, 0L, wxDefaultValidator, "switchDir" );

    labelManufacturer = new wxStaticText( this, wxID_ANY, "Manufacturer:" );
    manufacturerList.Add( "4d Brixx" );
    manufacturerPicker = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, manufacturerList, 0L, wxDefaultValidator, "manufacturerPicker" );

    editSizer->Add( labelName, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    editSizer->Add( switchName, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );

    editSizer->Add( labelPort, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    editSizer->Add( refreshSizer, 0, wxALL | ( wxALL & ~wxLEFT ), 0 );

    editSizer->Add( labelGpio, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    editSizer->Add( gpioPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );

    editSizer->Add( labelDir, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    editSizer->Add( dirPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );

    editSizer->Add( labelManufacturer, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );
    editSizer->Add( manufacturerPicker, 0, wxALL | ( wxALL & ~wxLEFT ), 5 );

    // Save Sizer
    wxBoxSizer* saveSizer = new wxBoxSizer( wxHORIZONTAL );
    wxButton* m_UpdateBtn = new wxButton( this, ID_RenameSwitch, "Rename", wxDefaultPosition, wxDefaultSize );
    wxButton* m_RemoveBtn = new wxButton( this, ID_RemoveSwitch, "Remove", wxDefaultPosition, wxDefaultSize );
    saveSizer->Add( m_UpdateBtn, 0, wxALL | wxALIGN_CENTER | wxSHAPED, 5 );
    saveSizer->Add( m_RemoveBtn, 0, wxALL | wxALIGN_CENTER | wxSHAPED, 5 );

    pickerSizer = new wxBoxSizer( wxVERTICAL );
    pickerSizer->Add( m_switchPicker, 1, wxALL | wxEXPAND, 0 );
    pickerSizer->Add( saveSizer, 0, wxALL, 0 );

    switchPickerBoxSizer->Add( pickerSizer, 0, wxEXPAND | wxALL, 5 );
    switchPickerBoxSizer->Add( editSizer, 0, wxEXPAND | wxALL, 5 );

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

    pickerGrid->SetCellRenderer( 0, 0, new cellImageRenderer( "tracks/straightn.png", 2 ) );
    pickerGrid->SetCellRenderer( 1, 0, new cellImageRenderer( "tracks/curven.png", 2 ) );
    pickerGrid->SetCellRenderer( 2, 0, new cellImageRenderer( "tracks/switchLn.png", 2 ) );
    pickerGrid->SetCellRenderer( 3, 0, new cellImageRenderer( "tracks/switchRn.png", 2 ) );
    pickerGrid->SetCellRenderer( 4, 0, new cellImageRenderer( "tracks/stopn.png", 2 ) );
    pickerGrid->SetCellRenderer( 5, 0, new cellImageRenderer( "tracks/crossn.png", 2 ) );

    mapPickerBoxSizer->Add( pickerGrid, 1, wxEXPAND | wxALL, 5 );

    // 
    // Topsizer
    //

    bottomSizer->Add( switchPickerBoxSizer, 4, wxALIGN_CENTER | wxALL, 5 );
    bottomSizer->Add( mapPickerBoxSizer, 0, wxEXPAND | wxALL, 5 );

    topSizer->Add( map, 3, wxEXPAND | wxALL, 5 );
    topSizer->Add( bottomSizer, 0, wxALIGN_CENTER | wxALL, 5 );

    this->SetSizerAndFit( topSizer );
    this->Layout();
    topSizer->Fit( this );
    topSizer->SetSizeHints( this );

    this->loadSwitches();
    if ( m_switchPicker->GetSelection() > 0 )
        m_switchPicker->SetSelection( 0 );
    this->SelectSwitch();
}

void mapEditPanel::OnRefreshSerial( wxCommandEvent& event )
{
    this->switchPickerBox->refreshSerial();
    portPicker->Clear();
    for (size_t i = 0; i < switchPickerBox->serialArray.GetCount(); i++)
    {
        portPicker->Append( switchPickerBox->serialArray.Item( i ) );
    }

    if ( portPicker->GetCount() > 0 )
        portPicker->SetSelection( portPicker->GetCount() - 1 );
}

void mapEditPanel::initConfig()
{
    // init Config
    configSwitch = new wxFileConfig( wxGetApp().GetAppName(), wxGetApp().GetVendorName(), wxGetApp().ini_dir + "switches.ini", "", wxCONFIG_USE_GLOBAL_FILE );
    wxConfigBase::Set( configSwitch );
    track38ConfigSwitch = wxConfigBase::Get();
    track38ConfigSwitch->Flush();

    configMap = new wxFileConfig( wxGetApp().GetAppName(), wxGetApp().GetVendorName(), wxGetApp().ini_dir + "map.ini", "", wxCONFIG_USE_GLOBAL_FILE );
    wxConfigBase::Set( configMap );
    track38ConfigMap = wxConfigBase::Get();
    track38ConfigMap->Flush();
}

void mapEditPanel::DragSwitchToMap( int row, int col )
{
    this->saveSwitch();
    this->initConfig();
    wxString name = "Switch ";
    int i = 1;
    while ( !( m_switchPicker->FindString( name + wxString::Format( wxT( "%i" ), i ) ) == wxNOT_FOUND ) )
        i += 1;

    track38ConfigSwitch->SetPath( "/Switch/" );
    track38ConfigSwitch->SetPath( name + wxString::Format( wxT( "%i" ), i )  );
    track38ConfigSwitch->Write( "gpio", "" );
    track38ConfigSwitch->Write( "dir", "" );
    track38ConfigSwitch->Write( "port", "" );
    track38ConfigSwitch->Write( "row", row );
    track38ConfigSwitch->Write( "col", col );
    track38ConfigSwitch->Write( "manufacturer", "" );
    track38ConfigSwitch->Flush();
    m_switchPicker->Append( name + wxString::Format( wxT( "%i" ), i ) );
    m_switchPicker->Select( m_switchPicker->FindString( name + wxString::Format( wxT( "%i" ), i ) ) );
    this->SelectSwitch();
}

void mapEditPanel::saveSwitch()
{
    this->saveSwitch( m_switchPicker->GetStringSelection() );
}

void mapEditPanel::saveSwitch( wxString name )
{
    if ( m_switchPicker->GetCount() == 0 )
        return;

    this->initConfig();

    track38ConfigSwitch->SetPath( "/Switch/" );
    track38ConfigSwitch->SetPath( name );
    track38ConfigSwitch->Write( "gpio", wxString::Format( wxT( "%i" ), gpioPicker->GetValue() ) );
    track38ConfigSwitch->Write( "dir", dirPicker->GetStringSelection() );
    track38ConfigSwitch->Write( "port", portPicker->GetStringSelection() );
    track38ConfigSwitch->Write( "manufacturer", manufacturerPicker->GetStringSelection() );
    track38ConfigSwitch->Flush();
}

void mapEditPanel::OnRemoveSwitch( wxCommandEvent& event )
{
    if ( m_switchPicker->GetCount() == 0 )
    {
        switchName->SetValue( "" );
        gpioPicker->SetValue( 2 );
    }

    this->RemoveSwitch();
}

void mapEditPanel::RemoveSwitch()
{
    if ( m_switchPicker->GetCount() == 0 )
        return; 

    this->initConfig();

    wxMessageDialog dialog( this, "Do you want to remove the Switch?", "Remove?", wxYES_NO | wxICON_INFORMATION );
    switch ( dialog.ShowModal() )
    {
        case wxID_YES:
            {
                track38ConfigSwitch->SetPath( "/Switch/" );
                track38ConfigSwitch->SetPath( m_switchPicker->GetStringSelection() );
                map->SetCellRenderer( wxAtoi ( track38ConfigSwitch->Read( "row", "" ) ), wxAtoi( track38ConfigSwitch->Read( "col", "" ) ), new cellImageRenderer() );
                track38ConfigMap->SetPath( "/map/" );
                track38ConfigMap->DeleteEntry( "cell" + track38ConfigSwitch->Read( "row", "" ) + "_" + track38ConfigSwitch->Read( "col", "" ) );
                track38ConfigSwitch->SetPath( "/Switch/" );
                track38ConfigSwitch->DeleteGroup( switchName->GetValue() );
                track38ConfigSwitch->Flush();
                m_switchPicker->Delete( m_switchPicker->GetSelection() );
                switchName->SetValue( "" );

                // Deselect all
                for ( int col = 0; col < map->GetNumberCols(); col++ )
                {
                    for ( int  row = 0; row < map->GetNumberRows(); row++ )
                    {
                        map->SetCellBackgroundColour( row, col, *wxWHITE );
                    }
                }

                map->Refresh();

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
    if ( !selectedSwitch.IsEmpty() )
        this->saveSwitch( selectedSwitch );

    this->SelectSwitch();
    selectedSwitch = m_switchPicker->GetStringSelection();
}

void mapEditPanel::SelectSwitch( int row, int col )
{
    if ( !selectedSwitch.IsEmpty() )
        this->saveSwitch( selectedSwitch );

    this->initConfig();

    long idx;
    wxString out;
    wxString toSelect;
    track38ConfigSwitch->SetPath( "/Switch/" );
    bool exists = track38ConfigSwitch->GetFirstGroup( out, idx );
    if (  exists == true  )
    {   
        track38ConfigSwitch->SetPath( out );
        if ( ( wxAtoi( track38ConfigSwitch->Read( "row", "") ) == row ) && ( wxAtoi( track38ConfigSwitch->Read( "col", "") ) == col ) )
        {
            toSelect = out;
        }
        track38ConfigSwitch->SetPath( "/Switch/" );
    }
    
    while ( exists )
    {
        exists = track38ConfigSwitch->GetNextGroup( out, idx );
        if (  exists == true  )
        {   
            track38ConfigSwitch->SetPath( out );
            if ( ( wxAtoi( track38ConfigSwitch->Read( "row", "") ) == row ) && ( wxAtoi( track38ConfigSwitch->Read( "col", "") ) == col ) )
            {
                toSelect = out;
            }
            track38ConfigSwitch->SetPath( "/Switch/" );
        }
    }

    m_switchPicker->SetStringSelection( toSelect );
    this->SelectSwitch();
}

void mapEditPanel::OnEditSwitch( wxCommandEvent& event )
{
    this->SelectSwitch( eventCellRow, eventCellCol );
}

void mapEditPanel::SelectSwitch()
{
    if ( !selectedSwitch.IsEmpty() )
        this->saveSwitch( selectedSwitch );

    this->initConfig();

    if ( m_switchPicker->GetCount() == 0 )
        return; 
    
    track38ConfigSwitch->SetPath( "/Switch/" );
    track38ConfigSwitch->SetPath( m_switchPicker->GetStringSelection() );

    switchName->SetValue( m_switchPicker->GetStringSelection() );

    if ( ( portPicker->FindString( track38ConfigSwitch->Read( "port", "" ) ) == wxNOT_FOUND ) && ( portPicker->FindString( "Please select a new Port" ) == wxNOT_FOUND ) )
    {
        // portPicker->AppendString( "Please select a new Port" );
        // portPicker->SetStringSelection( "Please select a new Port" );
    }
    else
    {
        for ( size_t idx = 0; idx < portPicker->GetCount(); idx++ )
        {
            if ( track38ConfigSwitch->Read( "port", "" ).IsSameAs( portPicker->GetString( idx ) ) )
                portPicker->SetSelection( idx );   
        }
    }

    gpioPicker->SetValue( track38ConfigSwitch->Read( "gpio", "2" ) );

    for ( size_t idx = 0; idx < dirPicker->GetCount(); idx++ )
    {
        if ( track38ConfigSwitch->Read( "dir", "1" ).IsSameAs( dirPicker->GetString( idx ) ) )
            dirPicker->SetSelection( idx );         
    }

    for ( size_t idx = 0; idx < manufacturerPicker->GetCount(); idx++ )
    {
        if ( track38ConfigSwitch->Read( "manufacturer", "1" ).IsSameAs( manufacturerPicker->GetString( idx ) ) )
            manufacturerPicker->SetSelection( idx );         
    }

    // Deselect all
    for ( int col = 0; col < map->GetNumberCols(); col++ )
    {
        for ( int  row = 0; row < map->GetNumberRows(); row++ )
        {
            map->SetCellBackgroundColour( row, col, *wxWHITE );
        }
    }
    map->SetCellBackgroundColour( wxAtoi( track38ConfigSwitch->Read( "row", "" ) ), wxAtoi( track38ConfigSwitch->Read( "col", "" ) ), *wxLIGHT_GREY );
    map->Refresh();
    selectedSwitch = m_switchPicker->GetStringSelection();
}

void mapEditPanel::loadSwitches()
{
    this->initConfig();

    track38ConfigSwitch->SetPath( "/Switch/" );
    int count = track38ConfigSwitch->GetNumberOfGroups( false );

    long idx;
    wxString out;
    bool exists = track38ConfigSwitch->GetFirstGroup( out, idx );
    if (  exists == true  )
        m_switchPicker->AppendString( out );
    
    while ( exists )
    {
        exists = track38ConfigSwitch->GetNextGroup( out, idx );
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
    cellImageRenderer* cellRenderer = (cellImageRenderer*) pickerGrid->GetCellRenderer( event.GetRow(), event.GetCol() );
    wxTextDataObject myData( cellRenderer->file );
    wxDropSource dragSource( this );
    dragSource.SetData( myData );
    wxDragResult result = dragSource.DoDragDrop( wxDrag_AllowMove );
}

void mapEditPanel::OnLClickMap( wxGridEvent& event )
{
    this->initConfig();
    bool wasSelected = false;
    if ( map->GetCellBackgroundColour( event.GetRow(), event.GetCol() ) == *wxLIGHT_GREY )
        wasSelected = true;

    cellImageRenderer* cellRenderer = ( cellImageRenderer* ) map->GetCellRenderer( event.GetRow(), event.GetCol() );
    if ( this->clickToDrag && !cellRenderer->isEmptyCell)
    {
        wxString fileRot = cellRenderer->file;
        fileRot += "|";
        fileRot += wxString::Format( wxT( "%i" ), cellRenderer->rotation );

        track38ConfigSwitch->SetPath( "/Switch/" );
        int count = track38ConfigSwitch->GetNumberOfGroups( false );

        long idx;
        wxString out;
        bool exists = track38ConfigSwitch->GetFirstGroup( out, idx );
        if (  exists == true  )
        {   
            track38ConfigSwitch->SetPath( out );
            if ( ( wxAtoi( track38ConfigSwitch->Read( "row", "") ) == event.GetRow() ) && ( wxAtoi( track38ConfigSwitch->Read( "col", "") ) == event.GetCol() ) )
            {
                fileRot += "|";
                fileRot += out;
            }
            track38ConfigSwitch->SetPath( "/Switch/" );
        }
        
        while ( exists )
        {
            exists = track38ConfigSwitch->GetNextGroup( out, idx );
            if (  exists == true  )
            {   
                track38ConfigSwitch->SetPath( out );
                if ( ( wxAtoi( track38ConfigSwitch->Read( "row", "") ) == event.GetRow() ) && ( wxAtoi( track38ConfigSwitch->Read( "col", "") ) == event.GetCol() ) )
                {
                    fileRot += "|";
                    fileRot += out;
                }
                track38ConfigSwitch->SetPath( "/Switch/" );
            }
        }

        map->SetCellRenderer( event.GetRow(), event.GetCol(), new cellImageRenderer() );
        map->SetCellBackgroundColour( event.GetRow(), event.GetCol(), *wxWHITE );
        map->ForceRefresh();

        wxTextDataObject myData( fileRot );
        wxDropSource dragSource( this );
        dragSource.SetData( myData );
        wxDragResult result = dragSource.DoDragDrop( wxDrag_AllowMove );

        // wxMessageBox( wxString::Format( wxT( "%i" ), result ) );

        if ( ( result == wxDragError ) || ( result == wxDragCancel ) || ( result == wxDragNone ) || wxGetApp().vetoDND )
        {
            map->SetCellRenderer( event.GetRow(), event.GetCol(), new cellImageRenderer( fileRot ) );
            if ( !( fileRot.Find( "switch" ) == wxNOT_FOUND ) )
                if ( wasSelected )
                    map->SetCellBackgroundColour( event.GetRow(), event.GetCol(), *wxLIGHT_GREY ); 
            // wxMessageBox("error");
            map->ForceRefresh();

            wxGetApp().vetoDND = false;
        }
    }

    else
    {
        this->turn( event.GetRow(), event.GetCol() );
    }
}

void mapEditPanel::OnRClickMap( wxGridEvent& event )
{
    eventCellRow = event.GetRow();
    eventCellCol = event.GetCol();
    mapMenu = new wxMenu();
 	mapMenu->Append( ID_RMenuTurnCW, "Rotate Clockwise" );
 	mapMenu->Append( ID_RMenuTurnCC, "Rotate Counter Clockwise" );
    mapMenu->AppendSeparator();
    mapMenu->AppendCheckItem( ID_DragMode, "DragMode" );
 	mapMenu->AppendSeparator();
    mapMenu->AppendCheckItem( ID_RMenuEdit, "Edit" );
    mapMenu->Append( ID_CellRemove, "Remove" );


    mapMenu->Check( ID_DragMode, this->clickToDrag );
    cellImageRenderer* cellRenderer = (cellImageRenderer*) map->GetCellRenderer( eventCellRow, eventCellCol );
    if ( cellRenderer->file.Find( "switch" ) == wxNOT_FOUND )
        mapMenu->Enable( ID_RMenuEdit, false );

    Bind( wxEVT_COMMAND_MENU_SELECTED, &mapEditPanel::OnDragMode, this, ID_DragMode );
    Bind( wxEVT_COMMAND_MENU_SELECTED, &mapEditPanel::OnMapRemove, this, ID_CellRemove );
    Bind( wxEVT_COMMAND_MENU_SELECTED, &mapEditPanel::OnTurnCW, this, ID_RMenuTurnCW );
    Bind( wxEVT_COMMAND_MENU_SELECTED, &mapEditPanel::OnTurnCC, this, ID_RMenuTurnCC );
    Bind( wxEVT_COMMAND_MENU_SELECTED, &mapEditPanel::OnEditSwitch, this, ID_RMenuEdit );

 	PopupMenu( mapMenu );
}

void mapEditPanel::LoadMapFromFile()
{
    track38ConfigMap->SetPath( "/map/" );
    map->CreateGrid( track38ConfigMap->Read( "rows", 25 ), track38ConfigMap->Read( "cols", 50 ) );
    map->HideColLabels();
    map->HideRowLabels();
    map->EnableEditing( false );
    map->EnableGridLines( false );
    map->EnableDragGridSize( false );
    map->DisableCellEditControl();
    map->SetMargins( 0 , 0 );
    map->SetCellHighlightColour( *wxWHITE );
    map->SetCellHighlightROPenWidth( 0 );

    map->SetDropTarget( new mapDropTarget( map ) );

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
            wxString selCell = "cell";
            selCell << row;
            selCell += "_";
            selCell << col;
            if ( track38ConfigMap->Exists( selCell ) )
                map->SetCellRenderer( row, col, new cellImageRenderer( track38ConfigMap->Read( selCell, "") ) );
            else
                map->SetCellRenderer( row, col, new cellImageRenderer() );
        }
    }
}

void mapEditPanel::SaveMapToFile()
{
    this->initConfig();

    track38ConfigMap->DeleteGroup( "/map/" );
    track38ConfigMap->SetPath( "/map/" );
    track38ConfigMap->Write( "rows", map->GetNumberRows() );
    track38ConfigMap->Write( "cols", map->GetNumberCols() );

    for ( size_t col = 0; col < map->GetNumberCols(); col++ )
    {
        for ( size_t row = 0; row < map->GetNumberRows(); row++ )
        {
            map->SetReadOnly( row, col );
            map->SetCellBackgroundColour( row, col, *wxWHITE );
            map->SetCellTextColour( row, col, *wxBLACK );

            cellImageRenderer* cellRenderer = (cellImageRenderer*) map->GetCellRenderer( row, col );

            if ( !cellRenderer->isEmptyCell )
            {
                wxString fileRot = cellRenderer->file;
                fileRot += "|";
                fileRot += wxString::Format( wxT( "%i" ), cellRenderer->rotation );

                wxString selCell = "cell";
                selCell << row;
                selCell += "_";
                selCell << col;

                track38ConfigMap->Write( selCell, fileRot );
            }
        }
    }
    track38ConfigMap->Flush();
}

void mapEditPanel::OnTurnCC( wxCommandEvent& event )
{
    this->turn( eventCellRow, eventCellCol, false );
}

void mapEditPanel::OnTurnCW( wxCommandEvent& event )
{
    this->turn( eventCellRow, eventCellCol, true );
}

void mapEditPanel::turn( int row, int col, bool clockwise )
{
    cellImageRenderer* cellRenderer = (cellImageRenderer*) map->GetCellRenderer( row, col );
    if ( !cellRenderer->isEmptyCell )
        {
            wxString filename = cellRenderer->file;
            int degree = cellRenderer->rotation;
            cellRenderer->DecRef();

            if (clockwise)
                degree += 90;
            else
            {
                if ( degree == 0)
                    degree = 270;
                else
                    degree -= 90;
            }

            map->SetCellRenderer( row, col, new cellImageRenderer( filename, 0, degree ) );
            map->ForceRefresh();
        }
}

void mapEditPanel::OnMapRemove( wxCommandEvent& event )
{
    cellImageRenderer* cellRenderer = (cellImageRenderer*) map->GetCellRenderer( eventCellRow, eventCellCol );

    cellRenderer->DecRef();
    map->SetCellRenderer( eventCellRow, eventCellCol, new cellImageRenderer() );

    if ( !cellRenderer->isEmptyCell && !( cellRenderer->file.Find( "switch" ) == wxNOT_FOUND ) )
    {
        this->RemoveMap( eventCellRow, eventCellCol );
    }

    map->ForceRefresh();
}

void mapEditPanel::RemoveMap( int row, int col)
{
    this->initConfig();
    
    // Remove Switch

    long idx;
    wxString out;
    wxString toRemove;
    bool exists = track38ConfigSwitch->GetFirstGroup( out, idx );
    if (  exists == true  )
    {   
        track38ConfigSwitch->SetPath( out );
        if ( ( wxAtoi( track38ConfigSwitch->Read( "row", "") ) == row ) && ( wxAtoi( track38ConfigSwitch->Read( "col", "") ) == col ) )
        {
            toRemove = out;
        }
        track38ConfigSwitch->SetPath( "/Switch/" );
    }
    
    while ( exists )
    {
        exists = track38ConfigSwitch->GetNextGroup( out, idx );
        if (  exists == true  )
        {   
            track38ConfigSwitch->SetPath( out );
            if ( ( wxAtoi( track38ConfigSwitch->Read( "row", "") ) == row ) && ( wxAtoi( track38ConfigSwitch->Read( "col", "") ) == col ) )
            {
                toRemove = out;
            }
            track38ConfigSwitch->SetPath( "/Switch/" );
        }
    }
    
    track38ConfigSwitch->SetPath( "/Switch/" );
    track38ConfigSwitch->SetPath( toRemove );
    map->SetCellRenderer( row, col, new cellImageRenderer() );
    track38ConfigMap->SetPath( "/map/" );
    track38ConfigMap->DeleteEntry( "cell" + track38ConfigSwitch->Read( "row", "" ) + "_" + track38ConfigSwitch->Read( "col", "" ) );
    track38ConfigSwitch->SetPath( "/Switch/" );
    track38ConfigSwitch->DeleteGroup( switchName->GetValue() );
    track38ConfigSwitch->DeleteGroup( toRemove );
    track38ConfigSwitch->Flush();
    // m_switchPicker->Delete( m_switchPicker->FindString( toRemove ) );
    m_switchPicker->Delete( m_switchPicker->GetSelection() );

    // Deselect all
    for ( int col = 0; col < map->GetNumberCols(); col++ )
    {
        for ( int  row = 0; row < map->GetNumberRows(); row++ )
        {
            map->SetCellBackgroundColour( row, col, *wxWHITE );
        }
    }

    if ( m_switchPicker->GetCount() > 0 )
    {
        m_switchPicker->SetSelection( 0 );
        this->SelectSwitch();
    }
    map->ForceRefresh();
}

void mapEditPanel::OnDragMode( wxCommandEvent& event )
{
    this->clickToDrag = !this->clickToDrag;
    mapMenu->Check( ID_DragMode, this->clickToDrag );
}

void mapEditPanel::OnRenameSwitch( wxCommandEvent& event )
{
    if ( m_switchPicker->GetCount() == 0 )
        return;

    this->initConfig();
    track38ConfigSwitch->SetPath( "/Switch/" );

    wxString oldName = m_switchPicker->GetStringSelection();
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
            else if ( track38ConfigSwitch->Exists( newName ) )
            {
                wxMessageBox( "The entered Name already exists. Please remove the switch first." );
            }

            else
            {
                track38ConfigSwitch->RenameGroup( oldName, newName );
                int oldPos = m_switchPicker->FindString( oldName );
                m_switchPicker->Delete( oldPos );
                m_switchPicker->Insert( newName, oldPos );
            }
            
            break;

        case wxID_CANCEL:
            return;
            break;
    }
    track38ConfigSwitch->Flush();
    this->m_switchPicker->SetStringSelection( newName ); 
    this->SelectSwitch();
}


mapDropTarget::mapDropTarget(wxGrid *grid)
{
    m_grid = grid;
}

bool mapDropTarget::OnDropText(wxCoord x, wxCoord y, const wxString& text)
{
    wxGridCellCoords coordinates = m_grid->XYToCell( x, y );

    wxString send;
    wxArrayString ary;
    int row = coordinates.GetRow() + m_grid->GetFirstFullyVisibleRow();
    int col = coordinates.GetCol() + m_grid->GetFirstFullyVisibleColumn();
    // m_grid->SetCellValue(row,col, text);

    if ( !( text.Find( "|" ) == wxNOT_FOUND ) )
    {
        ary = wxArrayString( wxSplit( text, '|' ) );
        send = ary.Item( 0 );
        send += "|";
        send += ary.Item( 1 );
    }
    else
        send = text;
    // wxMessageBox( wxString::Format( wxT( "R %i \n C %i"), row, col ) );

    mapEditPanel* parent = ( mapEditPanel* ) m_grid->GetParent();
    cellImageRenderer* cellRenderer = ( cellImageRenderer* ) m_grid->GetCellRenderer( row, col );
    if ( !cellRenderer->isEmptyCell && !( cellRenderer->file.Find( "switch" ) == wxNOT_FOUND ) )
    {
        wxMessageBox( "At the drop destination a switch does already exists. You are not allowed to drag a track element on a switch. Please (re)move the switch first." );
        wxGetApp().vetoDND = true;
        return false;
    }

    m_grid->SetCellRenderer( row, col, new cellImageRenderer( send ) );

    // with rotation
    if ( ary.GetCount() == 3 )
    {
        wxConfigBase::Set( parent->configSwitch );
        wxConfigBase* track38ConfigSwitch = wxConfigBase::Get();
        track38ConfigSwitch->SetPath( "/Switch/" );
        track38ConfigSwitch->SetPath( ary.Item( 2 ) );
        track38ConfigSwitch->Write( "row", row );
        track38ConfigSwitch->Write( "col", col );
        track38ConfigSwitch->Flush();

        // Deselect all
        for ( int col = 0; col < m_grid->GetNumberCols(); col++ )
        {
            for ( int  row = 0; row < m_grid->GetNumberRows(); row++ )
            {
                m_grid->SetCellBackgroundColour( row, col, *wxWHITE );
            }
        }
        m_grid->SetCellBackgroundColour( row, col, *wxLIGHT_GREY );
        parent->m_switchPicker->SetStringSelection( ary.Item( 2 ) );
        parent->SelectSwitch();
    }

    // 
    else if ( !( text.Find( "switch" ) == wxNOT_FOUND ) )
    {
        mapEditPanel* parent = ( mapEditPanel* ) m_grid->GetParent();
        parent->DragSwitchToMap( row, col );
    }

    m_grid->ForceRefresh();
    return true;
}