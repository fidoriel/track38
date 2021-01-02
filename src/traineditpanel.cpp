#include "wx/wx.h"
#include "wx/sizer.h"
#include "wx/notebook.h"
#include "wx/listbook.h"
#include "wx/radiobox.h"
#include <wx/statbox.h>

#include "pfeditbox.cpp"

class trainEditPanel : public wxPanel
{ 
    public:
        trainEditPanel(wxNotebook* parent);

        enum
        {
            ID_ADD = 901,
            ID_REMOVE = 902,
            ID_UPDATE = 903
        };
};

trainEditPanel::trainEditPanel(wxNotebook* parent) : wxPanel(parent)
{

    wxBoxSizer* topSizer= new wxBoxSizer(wxHORIZONTAL);

    //
    // Left Picker/Listbox
    //

    wxStaticBox* leftBox = new wxStaticBox(this, wxID_ANY, "&Pick Train to edit");
    wxStaticBoxSizer* leftSizer = new wxStaticBoxSizer(leftBox, wxVERTICAL);
    wxListBox* m_trainPicker = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL);

    leftSizer->Add(m_trainPicker, 1, wxGROW | wxALL, 5);
    leftSizer->SetMinSize(200, 0);

    //
    // Right Edit and Buttons
    //

    wxStaticBox* rightBox = new wxStaticBox(this, wxID_ANY, "&Train Properties");
    wxSizer* rightSizer = new wxStaticBoxSizer(rightBox, wxVERTICAL);

    //Control kind picker
    wxArrayString options;
    options.Add("Lego PowerFunctions");
    options.Add("Lego PoweredUp");
    wxRadioBox* trainKindPicker = new wxRadioBox(this, wxID_ANY, "Train Controller", wxDefaultPosition, wxDefaultSize, options, 2, wxRA_HORIZONTAL);

    //PF edit Panel
    pfEditBox* m_pfEditBox = new pfEditBox(this, wxID_ANY, "&Edit PowerFunctin Settings");

    // Save Panel
    wxStaticBox* saveBox = new wxStaticBox(this, wxID_ANY, "&");
    wxSizer* saveSizer = new wxStaticBoxSizer(saveBox, wxHORIZONTAL);
    wxButton* m_AddBtn = new wxButton(this, ID_ADD, "Add", wxDefaultPosition, wxDefaultSize);
    wxButton* m_UpdateBtn = new wxButton(this, ID_UPDATE, "Update", wxDefaultPosition, wxDefaultSize);
    wxButton* m_RemoveBtn = new wxButton(this, ID_REMOVE, "Remove", wxDefaultPosition, wxDefaultSize);
    saveSizer->Add(m_AddBtn, 0, wxALL | wxALIGN_CENTER | wxSHAPED, 5);
    saveSizer->Add(m_UpdateBtn, 0, wxALL | wxALIGN_CENTER | wxSHAPED, 5);
    saveSizer->Add(m_RemoveBtn, 0, wxALL | wxALIGN_CENTER | wxSHAPED, 5);
    saveSizer->Layout();



    rightSizer->Add(trainKindPicker, 0, wxALL | wxGROW, 5);
    rightSizer->Add(m_pfEditBox, 1, wxALL | wxGROW, 5);
    rightSizer->Add(saveSizer, 0, wxALL | wxALIGN_CENTER, 5);

    topSizer->Add(leftSizer, 0, wxGROW | (wxALL & ~wxLEFT), 5);
    topSizer->Add(rightSizer, 1, wxGROW | wxALL, 5);
    parent->SetSizer(topSizer);
    parent->Layout();
	topSizer->Fit(this);
    topSizer->SetSizeHints(this);
}
