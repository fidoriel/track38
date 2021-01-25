#include "traincontrolbox.h"

trainControlBox::trainControlBox( wxPanel* parent, int id, wxString title, wxString boxName, std::unordered_map< wxString, int > &cons ) : wxStaticBox( parent, id, title, wxDefaultPosition, wxDefaultSize, 0L, boxName )
{
    sizer = new wxStaticBoxSizer( this, wxVERTICAL );
    m_trainControlPanel = new trainControlPanel( parent, wxID_ANY );

    m_trainControlPanel->loadTrains( cons );
    m_trainControlPanel->createControlBox();

    sizer->Add( m_trainControlPanel->stopAllBtn, 0, wxALL | wxALIGN_CENTER, 10 );
    sizer->Add( m_trainControlPanel, 0, wxEXPAND | wxALL, 10 );
}

trainControlBox::~trainControlBox()
{
    delete m_trainControlPanel;
}