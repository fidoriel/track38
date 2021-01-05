#include "traincontrol.h"

trainControlBox::trainControlBox(wxPanel* parent, int id, wxString title) : wxStaticBox(parent, id, title)
{
    /*
    wxBoxSizer* trainControlSizer = new wxBoxSizer(wxVERTICAL);
    button = new wxButton(this, wxID_ANY, "testbutton");

    trainControlSizer->Add(button, 1, wxALL);

    parent->SetSizer(trainControlSizer);
    parent->Layout();
	trainControlSizer->Fit(this);
    trainControlSizer->SetSizeHints(this);
    */

   wxFlexGridSizer* topSizer = new wxFlexGridSizer(3, 0, 0);

    //
    // Left Picker/Listbox
    //

    wxStaticText* trainName = new wxStaticText(this, wxID_ANY, "Train Name:");
    topSizer->Add(trainName, 1, wxALL, 10);

    wxSlider* speedSlider = new wxSlider(this, wxID_ANY, 0, -7, 7, wxDefaultPosition, wxSize(200, -1), wxSL_AUTOTICKS);
    topSizer->Add(speedSlider, 1, wxALL , 10);

    wxButton* stopBtn = new wxButton(this, wxID_ANY, "Stop", wxDefaultPosition, wxDefaultSize);
    topSizer->Add(stopBtn, 1, wxALL , 10);


    wxButton* stopAllBtn = new wxButton(this, wxID_ANY, "Stop All", wxDefaultPosition, wxDefaultSize);
    topSizer->AddSpacer(20);
    topSizer->Add(stopAllBtn, 0, wxALL | wxALIGN_CENTER_HORIZONTAL , 10);

    parent->SetSizer(topSizer);
    parent->Layout();
	topSizer->Fit(this);
    topSizer->SetSizeHints(this);
}