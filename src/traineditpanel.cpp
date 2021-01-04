#include "traineditpanel.h"

wxBEGIN_EVENT_TABLE(trainEditPanel, wxPanel)
    EVT_RADIOBOX(ID_ChangeControl, trainEditPanel::OnChangeControler)
    EVT_BUTTON(ID_AddTrain, trainEditPanel::OnAddTrain)
wxEND_EVENT_TABLE()

trainEditPanel::trainEditPanel(wxNotebook* parent) : wxPanel(parent)
{
    panelParent = parent;

    topSizer = new wxBoxSizer(wxHORIZONTAL);

    //
    // Left Picker/Listbox
    //

    leftBox = new wxStaticBox(this, wxID_ANY, "&Pick Train to edit");
    leftSizer = new wxStaticBoxSizer(leftBox, wxVERTICAL);
    m_trainPicker = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_HSCROLL);

    leftSizer->Add(m_trainPicker, 1, wxGROW | wxALL, 5);
    leftSizer->SetMinSize(200, 0);

    //
    // Right Edit and Buttons
    //

    rightBox = new wxStaticBox(this, wxID_ANY, "&Train Properties");
    rightSizer = new wxStaticBoxSizer(rightBox, wxVERTICAL);

    //Control kind picker
    wxArrayString options;
    options.Add("Lego PowerFunctions");
    options.Add("Lego PoweredUp");
    trainKindPicker = new wxRadioBox(this, ID_ChangeControl, "Train Controller", wxDefaultPosition, wxDefaultSize, options, 2, wxRA_HORIZONTAL);

    //PF edit Panel 
    m_trainEditBox = new pfEditBox(this, wxID_ANY, "&Edit PowerFunctions Settings");

    // Save Panel
    saveBox = new wxStaticBox(this, wxID_ANY, "&");
    saveSizer = new wxStaticBoxSizer(saveBox, wxHORIZONTAL);
    m_AddBtn = new wxButton(this, ID_AddTrain, "Add", wxDefaultPosition, wxDefaultSize);
    m_UpdateBtn = new wxButton(this, ID_UPDATE, "Update", wxDefaultPosition, wxDefaultSize);
    m_RemoveBtn = new wxButton(this, ID_REMOVE, "Remove", wxDefaultPosition, wxDefaultSize);
    saveSizer->Add(m_AddBtn, 0, wxALL | wxALIGN_CENTER | wxSHAPED, 5);
    saveSizer->Add(m_UpdateBtn, 0, wxALL | wxALIGN_CENTER | wxSHAPED, 5);
    saveSizer->Add(m_RemoveBtn, 0, wxALL | wxALIGN_CENTER | wxSHAPED, 5);
    saveSizer->Layout();

    rightSizer->Add(trainKindPicker, 0, wxALL | wxGROW, 5);
    rightSizer->Add(m_trainEditBox, 0, wxALL | wxGROW, 5);
    rightSizer->Add(saveSizer, 0, wxALL | wxALIGN_CENTER, 5);

    topSizer->Add(leftSizer, 1, wxGROW | wxALL, 5);
    topSizer->Add(rightSizer, 4, wxGROW | wxALL, 5);
    parent->SetSizer(topSizer);
    parent->Layout();
	topSizer->Fit(this);
    topSizer->SetSizeHints(this);
}


void trainEditPanel::OnChangeControler(wxCommandEvent& event)
{
    RefreshPanel();
}

void trainEditPanel::RefreshPanel()
{
    if (trainKindPicker)
    {
        rightSizer->Remove(1);
        delete m_trainEditBox;
        
        int sel = trainKindPicker->GetSelection();
        switch (sel)
        {
        case 0:
            m_trainEditBox = new pfEditBox(this, wxID_ANY, "&Edit PowerFunctions Settings");
            break;
        
        case 1:
            m_trainEditBox = new upEditBox(this, wxID_ANY, "&Edit PoweredUP Settings");
            break;
        }

        rightSizer->Insert(1, m_trainEditBox, 0, wxALL | wxGROW, 5);

        panelParent->SetSizerAndFit(topSizer);
        panelParent->Layout();
        panelParent->SendSizeEventToParent();
    }
}


void trainEditPanel::OnAddTrain(wxCommandEvent& event)
{
    wxConfigBase* track38ConfigTrain = wxConfigBase::Get();

    wxWindowList panelChild = m_trainEditBox->GetChildren();

    track38ConfigTrain->SetPath("/TrainTest");

    if (trainKindPicker)
    {
        int sel = trainKindPicker->GetSelection();
        switch (sel)
        {
            // PF
            case 0: 
            {
                wxTextCtrl* pfName = (wxTextCtrl*) FindWindow("pfName");
                wxChoice* pfPort = (wxChoice*) FindWindow("pfPort");
                wxSpinCtrl* pfGpio = (wxSpinCtrl*) FindWindow("pfGpio");
                wxChoice* pfChannel = (wxChoice*) FindWindow("pfChannel");
                wxChoice* pfSubChannel = (wxChoice*) FindWindow("pfSubChannel");
                wxSpinCtrl* pfSpeed = (wxSpinCtrl*) FindWindow("pfSpeed");

                m_trainPicker->AppendString(pfName->GetValue());
                m_trainPicker->SetStringSelection(pfName->GetValue());

                track38ConfigTrain->SetPath(pfName->GetValue());
                track38ConfigTrain->Write("control", "pf");
                track38ConfigTrain->Write("port", pfPort->GetStringSelection());
                track38ConfigTrain->Write("channel", pfChannel->GetStringSelection());
                track38ConfigTrain->Write("subChannel", pfSubChannel->GetStringSelection());
                track38ConfigTrain->Write("maxSpeed", pfSpeed->GetValue());

                break;
            }
            //UP
            case 1:
            {
                wxTextCtrl* upName = (wxTextCtrl*) FindWindow("upName");
                wxChoice* upPort = (wxChoice*) FindWindow("upPort");
                wxTextCtrl* upHubAdress = (wxTextCtrl*) FindWindow("upHubAdress");
                wxChoice* upChannel = (wxChoice*) FindWindow("upChannel");
                wxCheckBox* upAreTwoMotorsUsed = (wxCheckBox*) FindWindow("upAreTwoMotorsUsed");
                wxSpinCtrl* upSpeed = (wxSpinCtrl*) FindWindow("upSpeed");

                m_trainPicker->AppendString(upName->GetValue());
                m_trainPicker->SetStringSelection(upName->GetValue());

                track38ConfigTrain->SetPath(upName->GetValue());
                track38ConfigTrain->Write("control", "up");
                track38ConfigTrain->Write("port", upPort->GetStringSelection());
                track38ConfigTrain->Write("hubAdress", upHubAdress->GetStringSelection());
                track38ConfigTrain->Write("channel", upChannel->GetStringSelection());
                track38ConfigTrain->Write("twoMotorsUsed", upAreTwoMotorsUsed->GetValue());
                track38ConfigTrain->Write("maxSpeed", upSpeed->GetValue());
                break;
            }
        }
    }
}