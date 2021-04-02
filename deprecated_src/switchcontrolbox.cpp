#include "switchcontrolbox.h"

switchControlBox::switchControlBox( wxPanel* parent, int id, wxString title, wxString boxName, std::unordered_map< wxString, int > &cons ) : wxStaticBox( parent, id, title, wxDefaultPosition, wxDefaultSize, 0L, boxName )
{
    sizer = new wxStaticBoxSizer( this, wxVERTICAL );
    m_switchControlPanel = new switchControlPanel( parent, wxID_ANY );

    m_switchControlPanel->loadswitchs( cons );
    m_switchControlPanel->createControlBox();

    sizer->Add( m_switchControlPanel, 0, wxEXPAND | wxALL, 5 );
}

switchControlBox::~switchControlBox()
{
    delete m_switchControlPanel;
}