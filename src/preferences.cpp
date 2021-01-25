#include "preferences.h"

track38PreferencePanel::track38PreferencePanel(wxWindow *parent) : wxPanel(parent)
{

    wxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add( new wxCheckBox( this, wxID_ANY, "track38 empty setting" ), wxSizerFlags().Border());

    SetSizerAndFit(sizer);
}