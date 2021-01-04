#include "traineditpanel.h"

wxBEGIN_EVENT_TABLE(trainEditPanel, wxPanel)
    EVT_RADIOBOX(ID_ChangeControl, trainEditPanel::OnChangeControler)
    EVT_BUTTON(ID_AddTrain, trainEditPanel::OnAddTrain)
    EVT_LISTBOX(ID_SelectTrain, trainEditPanel::OnTrainSelect)
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
    m_trainPicker = new wxListBox(this, ID_SelectTrain, wxDefaultPosition, wxDefaultSize, 0, NULL);

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

    wxConfigBase* track38ConfigTrain = wxConfigBase::Get();

    track38ConfigTrain->SetPath("/TrainTest/");
    int count = track38ConfigTrain->GetNumberOfGroups(false);

    long idx;
    wxString out;
    bool exists = track38ConfigTrain->GetFirstGroup(out, idx);
    if ( exists == true ) m_trainPicker->AppendString(out);;
    
    while (exists)
    {
        exists = track38ConfigTrain->GetNextGroup(out, idx);
        if ( exists == true ) m_trainPicker->AppendString(out);;
    }
      
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
                track38ConfigTrain->Write("gpio", pfGpio->GetValue());
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
                track38ConfigTrain->Write("hubAdress", upHubAdress->GetValue());
                track38ConfigTrain->Write("channel", upChannel->GetStringSelection());
                track38ConfigTrain->Write("twoMotorsUsed", upAreTwoMotorsUsed->GetValue());
                track38ConfigTrain->Write("maxSpeed", upSpeed->GetValue());
                break;
            }
            track38ConfigTrain->Flush();
        }
    }
}

void trainEditPanel::OnTrainSelect(wxCommandEvent& event)
{
    wxString trainSel = m_trainPicker->GetString(m_trainPicker->GetSelection());

    wxConfigBase* track38ConfigTrain = wxConfigBase::Get();
    track38ConfigTrain->SetPath("/TrainTest/");
    track38ConfigTrain->SetPath(trainSel);
    wxString control = track38ConfigTrain->Read("control", "pf");

    wxTextCtrl* tName;
    wxChoice* tPort;
    wxSpinCtrl* tSpeed;

    if (control.IsSameAs("pf"))
    {
        trainKindPicker->SetSelection(0);
        RefreshPanel();

        tName = (wxTextCtrl*) FindWindow("pfName");
        tPort = (wxChoice*) FindWindow("pfPort");
        wxSpinCtrl* tGpio = (wxSpinCtrl*) FindWindow("pfGpio");
        wxChoice* tChannel = (wxChoice*) FindWindow("pfChannel");
        wxChoice* tSubChannel = (wxChoice*) FindWindow("pfSubChannel");
        tSpeed = (wxSpinCtrl*) FindWindow("pfSpeed");

        tGpio->SetValue(track38ConfigTrain->Read("gpio", "13"));

        for (size_t idx = 0; idx < tChannel->GetCount(); idx++)
        {
            if (track38ConfigTrain->Read("channel", "1").IsSameAs(tChannel->GetString(idx)))
            {
                tChannel->SetSelection(idx);
            }
            
        }

        for (size_t idx = 0; idx < tSubChannel->GetCount(); idx++)
        {
            if (track38ConfigTrain->Read("subChannel", "1").IsSameAs(tSubChannel->GetString(idx)))
            {
                tSubChannel->SetSelection(idx);
            }
            
        }
    }

    else if (control.IsSameAs("up"))
    {
        trainKindPicker->SetSelection(1);
        RefreshPanel();

        tName = (wxTextCtrl*) FindWindow("upName");
        tPort = (wxChoice*) FindWindow("upPort");
        wxTextCtrl* tHubAdress = (wxTextCtrl*) FindWindow("upHubAdress");
        wxChoice* tChannel = (wxChoice*) FindWindow("upChannel");
        wxCheckBox* tAreTwoMotorsUsed = (wxCheckBox*) FindWindow("upAreTwoMotorsUsed");
        tSpeed = (wxSpinCtrl*) FindWindow("upSpeed");

        tHubAdress->ChangeValue(track38ConfigTrain->Read("hubAdress", ""));

        for (size_t idx = 0; idx < tChannel->GetCount(); idx++)
        {
            if (track38ConfigTrain->Read("channel", "1").IsSameAs(tChannel->GetString(idx)))
            {
                tChannel->SetSelection(idx);
            }
            
        }
        
        tAreTwoMotorsUsed->SetValue(track38ConfigTrain->Read("twoMotorsUsed", false));
    }

    tName->ChangeValue(trainSel);

    if (tPort->FindString(track38ConfigTrain->Read("port", "")) == wxNOT_FOUND)
    {
        wxMessageBox("The saved Port was not found. Please plug in the device.", "Port Error");
    }
    else
    {
        for (size_t idx = 0; idx < tPort->GetCount(); idx++)
        {
            if (track38ConfigTrain->Read("port", "").IsSameAs(tPort->GetString(idx)))
            {
                tPort->SetSelection(idx);
            }
            
        }
    }

    tSpeed->SetValue(track38ConfigTrain->Read("maxSpeed", "7"));
}