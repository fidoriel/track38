#include "controlpanel.h"

controlPanel::controlPanel(wxNotebook* parent) : wxPanel(parent)
{
    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticBox* m_trainControlBox = new trainControlBox(this, wxID_ANY, "test");
    topSizer->Add(m_trainControlBox, 2, wxGROW | wxALL, 10);

    parent->SetSizerAndFit(topSizer);
    parent->Layout();
	topSizer->Fit(this);
    topSizer->SetSizeHints(this);
}
