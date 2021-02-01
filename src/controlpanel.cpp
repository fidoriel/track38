#include "controlpanel.h"
#include "track38App.h"

controlPanel::controlPanel( wxNotebook* parent ) : wxPanel( parent )
{
    this->parent = parent;
    topSizer = new wxBoxSizer( wxHORIZONTAL);
    mapSizer = new wxBoxSizer( wxVERTICAL );

    m_trainControlBox = new trainControlBox( this, wxID_ANY, "Train Control", "trainControlBox", cons );
    // m_switchControlBox = new switchControlBox( this, ID_SwitchChange, "Switch Control", "switchControlBox", cons );

    m_switchHandler = new switchHandler();
    m_switchHandler->loadswitchs( cons );

    this->initConf();
    this->LoadMapFromFile();

    mapSizer->Add( map, 1, wxGROW | wxALL, 5 );
    // mapSizer->Add( m_switchControlBox->sizer, 0, wxGROW | wxALL, 5 );
    topSizer->Add( m_trainControlBox->sizer, 0, wxGROW | wxALL, 5 );  
    topSizer->Add( mapSizer, 1, wxGROW | wxALL, 5 ); 

    this->SetSizerAndFit( topSizer );
    this->Layout();
	topSizer->Fit( this );
    topSizer->SetSizeHints( this );
}

void controlPanel::RefreshPanel()
{
    this->CloseAll();
    for (std::pair< wxString, int> element : cons)
    {
        wxMessageBox( element.first );
        wxMessageBox( wxString::Format( wxT("%i"), element.second ) );
    }

    while ( mapSizer->GetItemCount() )
    {
        mapSizer->Detach( 0 );
    }

    while ( topSizer->GetItemCount() )
    {
        topSizer->Detach( 0 );
    }

    delete map;
    delete m_trainControlBox;
    // delete m_switchControlBox;
    delete m_switchHandler;

    m_switchHandler = new switchHandler();
    m_switchHandler->loadswitchs( cons );

    this->initConf();
    this->LoadMapFromFile();

    m_trainControlBox = new trainControlBox( this, wxID_ANY, "Train Control", "trainControlBox", cons );
    // m_switchControlBox = new switchControlBox( this, wxID_ANY, "Switch Control", "switchControlBox", cons );
    
    mapSizer->Add( map, 1, wxGROW | wxALL, 5 );
    // mapSizer->Add( m_switchControlBox->sizer, 0, wxGROW | wxALL, 5 );
    topSizer->Add( m_trainControlBox->sizer, 0, wxGROW | wxALL, 5 );
    topSizer->Add( mapSizer, 1, wxGROW | wxALL, 5 ); 

    this->SetSizerAndFit( topSizer );
    this->Layout();
	topSizer->Fit( this );
    topSizer->SetSizeHints( this );
    this->SendSizeEventToParent();
}

void controlPanel::initConf()
{
    // Init config
    configMap = new wxFileConfig( wxGetApp().GetAppName(), wxGetApp().GetVendorName(), wxGetApp().ini_dir + "map.ini", "", wxCONFIG_USE_GLOBAL_FILE );
    wxConfigBase::Set( configMap );
    track38ConfigMap = wxConfigBase::Get();
}

void controlPanel::LoadMapFromFile()
{
    this->initConf();

    map = new wxGrid( this, ID_MapLClick, wxPoint( 0, 0 ), wxSize( 40, 40) );

    map->Bind( wxEVT_GRID_CELL_LEFT_CLICK, &controlPanel::OnLClickMap, this, ID_MapLClick );

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
    this->RefreshMap();
    this->updateSwitchStatus();
}

void controlPanel::OnRefreshPanel( wxCommandEvent& event )
{
    this->RefreshPanel();
}

void controlPanel::CloseAll()
{
    // Iterate over an unordered_map using range based for loop
    for (std::pair< wxString, int> element : cons)
    {
        close_port( element.second );
    }
    cons.clear();
}

void controlPanel::OnLClickMap( wxGridEvent& event )
{
    cellImageRenderer* cellRenderer = (cellImageRenderer*) map->GetCellRenderer( event.GetRow(), event.GetCol() );

    if ( !( cellRenderer->file.Find( "switch" ) == wxNOT_FOUND ) )
    {
        for ( tswitch* & selswitch : this->m_switchHandler->switches)
        {
            if ( selswitch->con != -1 )
            {
                if( ( selswitch->col == event.GetCol() ) && ( selswitch->row == event.GetRow() ) )
                {
                    selswitch->Toggle();
                }
            }
        }
    }

    this->updateSwitchStatus();
}

void controlPanel::updateSwitchStatus()
{
    for ( tswitch* & selswitch : this->m_switchHandler->switches)
    {
        cellImageRenderer* cellRenderer = (cellImageRenderer*) map->GetCellRenderer( selswitch->row, selswitch->col );
        wxString newPNG = cellRenderer->file;
        int dot = newPNG.find( ".png" );
        if ( selswitch->currentPos == 'n' )
        {
            newPNG.replace( dot -1, dot -1, 'n' );
            newPNG += ".png";
        }

        else if ( selswitch->currentPos == 'S' )
        {
            newPNG.replace( dot -1, dot -1, 's' );
            newPNG += ".png";
        }

        else if ( selswitch->currentPos == 'T' )
        {
            newPNG.replace( dot -1, dot -1, 't' );
            newPNG += ".png";
        }

        map->SetCellRenderer( selswitch->row, selswitch->col, new cellImageRenderer( newPNG, 0, cellRenderer->rotation) );
        // wxMessageBox(newPNG + wxString(selswitch->currentPos) );
    }

    map->Refresh();
}

void controlPanel::RefreshMap()
{
}

controlPanel::~controlPanel()
{
}