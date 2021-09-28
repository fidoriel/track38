#include "bleConDialog.h"
// ----------------------------------------------------------------------------
// bleConDialog
// ----------------------------------------------------------------------------

/* static */
const bleConDialog::BtnInfo bleConDialog::ms_btnInfo[] =
{
    { wxYES,    "&Yes"    },
    { wxNO,     "&No"     },
    { wxOK,     "&Ok"     },
    { wxCANCEL, "&Cancel" },
    { wxHELP,   "&Help"   },
};

wxBEGIN_EVENT_TABLE(bleConDialog, wxDialog)
    EVT_BUTTON(wxID_APPLY, bleConDialog::OnApply)
    EVT_BUTTON(wxID_CLOSE, bleConDialog::OnClose)
wxEND_EVENT_TABLE()

bleConDialog::bleConDialog(wxWindow *parent)
                    : wxDialog(parent, wxID_ANY, "Message Box Test Dialog",
                               wxDefaultPosition, wxDefaultSize,
                               wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
}

bool bleConDialog::Create()
{
    wxSizer * const sizerTop = new wxBoxSizer(wxVERTICAL);

    // this sizer allows to configure the messages shown in the message box
    wxSizer * const
        sizerMsgs = new wxStaticBoxSizer(wxVERTICAL, this, "&Messages");
    sizerMsgs->Add(new wxStaticText(this, wxID_ANY, "&Title:"));
    m_textTitle = new wxTextCtrl(this, wxID_ANY, "Test Message Box");
    sizerMsgs->Add(m_textTitle, wxSizerFlags().Expand().Border(wxBOTTOM));

    sizerMsgs->Add(new wxStaticText(this, wxID_ANY, "&Main message:"));
    m_textMsg = new wxTextCtrl(this, wxID_ANY, "Hello from a box!",
                               wxDefaultPosition, wxDefaultSize,
                               wxTE_MULTILINE);
    sizerMsgs->Add(m_textMsg, wxSizerFlags(1).Expand().Border(wxBOTTOM));

    sizerMsgs->Add(new wxStaticText(this, wxID_ANY, "&Extended message:"));
    m_textExtMsg = new wxTextCtrl(this, wxID_ANY, "",
                                  wxDefaultPosition, wxDefaultSize,
                                  wxTE_MULTILINE);
    sizerMsgs->Add(m_textExtMsg, wxSizerFlags().Expand());

    sizerTop->Add(sizerMsgs, wxSizerFlags(1).Expand().Border());

    // if a derived class provides more message configurations, add these.
    AddAdditionalTextOptions(sizerTop);

    // this one is for configuring the buttons
    wxSizer * const
    sizerBtnsBox = new wxStaticBoxSizer(wxVERTICAL, this, "&Buttons");

    wxFlexGridSizer * const sizerBtns = new wxFlexGridSizer(2, 5, 5);
    sizerBtns->AddGrowableCol(1);

    sizerBtns->Add(new wxStaticText(this, wxID_ANY, "Button(s)"));
    sizerBtns->Add(new wxStaticText(this, wxID_ANY, "Custom label"));

    for ( int n = 0; n < Btn_Max; n++ )
    {
        m_buttons[n] = new wxCheckBox(this, wxID_ANY, ms_btnInfo[n].name);
        sizerBtns->Add(m_buttons[n], wxSizerFlags().Centre().Left());

        m_labels[n] = new wxTextCtrl(this, wxID_ANY);
        sizerBtns->Add(m_labels[n], wxSizerFlags().Expand());

        m_labels[n]->Bind(wxEVT_UPDATE_UI,
                          &bleConDialog::OnUpdateLabelUI, this);
    }

    sizerBtnsBox->Add(sizerBtns, wxSizerFlags().Expand());
    sizerTop->Add(sizerBtnsBox, wxSizerFlags().Expand().Border());


    // icon choice
    const wxString icons[] =
    {
        "&Not specified",
        "E&xplicitly none",
        "&Information icon",
        "&Question icon",
        "&Warning icon",
        "&Error icon",
        "A&uth needed icon"
    };

   wxCOMPILE_TIME_ASSERT( WXSIZEOF(icons) == MsgDlgIcon_Max, IconMismatch );

    m_icons = new wxRadioBox(this, wxID_ANY, "&Icon style",
                             wxDefaultPosition, wxDefaultSize,
                             WXSIZEOF(icons), icons,
                             2, wxRA_SPECIFY_ROWS);
    // Make the 'Information' icon the default one:
    m_icons->SetSelection(MsgDlgIcon_Info);
    sizerTop->Add(m_icons, wxSizerFlags().Expand().Border());


    // miscellaneous other stuff
    wxSizer * const
        sizerFlags = new wxStaticBoxSizer(wxHORIZONTAL, this, "&Other flags");

    m_chkNoDefault = new wxCheckBox(this, wxID_ANY, "Make \"No\" &default");
    m_chkNoDefault->Bind(wxEVT_UPDATE_UI,
                         &bleConDialog::OnUpdateNoDefaultUI, this);
    sizerFlags->Add(m_chkNoDefault, wxSizerFlags().Border());

    m_chkCentre = new wxCheckBox(this, wxID_ANY, "Centre on &parent");
    sizerFlags->Add(m_chkCentre, wxSizerFlags().Border());

    // add any additional flag from subclasses
    AddAdditionalFlags(sizerFlags);

    sizerTop->Add(sizerFlags, wxSizerFlags().Expand().Border());

    // add the currently unused zone for displaying the dialog result
    m_labelResult = new wxStaticText(this, wxID_ANY, "",
                                     wxDefaultPosition, wxDefaultSize,
                                     wxST_NO_AUTORESIZE | wxALIGN_CENTRE);
    m_labelResult->SetForegroundColour(*wxBLUE);
    sizerTop->Add(m_labelResult, wxSizerFlags().Expand().DoubleBorder());

    // finally buttons to show the resulting message box and close this dialog
    sizerTop->Add(CreateStdDialogButtonSizer(wxAPPLY | wxCLOSE),
                  wxSizerFlags().Right().Border());

    SetSizerAndFit(sizerTop);

    m_buttons[Btn_Ok]->SetValue(true);

    CentreOnScreen();

    return true;
}

void bleConDialog::OnUpdateLabelUI(wxUpdateUIEvent& event)
{
    for ( int n = 0; n < Btn_Max; n++ )
    {
        if ( event.GetEventObject() == m_labels[n] )
        {
            event.Enable( m_buttons[n]->IsChecked() );
            return;
        }
    }

    wxFAIL_MSG( "called for unknown label" );
}

void bleConDialog::OnUpdateNoDefaultUI(wxUpdateUIEvent& event)
{
    event.Enable( m_buttons[Btn_No]->IsChecked() );
}

long bleConDialog::GetStyle()
{
    long style = 0;

    for ( int n = 0; n < Btn_Max; n++ )
    {
        if ( m_buttons[n]->IsChecked() )
            style |= ms_btnInfo[n].flag;
    }

    switch ( m_icons->GetSelection() )
    {
        case MsgDlgIcon_Max:
            wxFAIL_MSG( "unexpected selection" );
            wxFALLTHROUGH;

        case MsgDlgIcon_No:
            break;

        case MsgDlgIcon_None:
            style |= wxICON_NONE;
            break;

        case MsgDlgIcon_Info:
            style |= wxICON_INFORMATION;
            break;

        case MsgDlgIcon_Question:
            style |= wxICON_QUESTION;
            break;

        case MsgDlgIcon_Warning:
            style |= wxICON_WARNING;
            break;

        case MsgDlgIcon_Error:
            style |= wxICON_ERROR;
            break;

        case MsgDlgIcon_AuthNeeded:
            style |= wxICON_AUTH_NEEDED;
            break;
    }

    if ( m_chkCentre->IsChecked() )
        style |= wxCENTRE;

    if ( m_chkNoDefault->IsEnabled() && m_chkNoDefault->IsChecked() )
        style |= wxNO_DEFAULT;

    return style;
}

void bleConDialog::PrepareMessageDialog(wxMessageDialogBase &dlg)
{
    long style = dlg.GetMessageDialogStyle();

    if ( !m_textExtMsg->IsEmpty() )
        dlg.SetExtendedMessage(m_textExtMsg->GetValue());

    if ( style & wxYES_NO )
    {
        if ( style & wxCANCEL )
        {
            dlg.SetYesNoCancelLabels(m_labels[Btn_Yes]->GetValue(),
                                     m_labels[Btn_No]->GetValue(),
                                     m_labels[Btn_Cancel]->GetValue());
        }
        else
        {
            dlg.SetYesNoLabels(m_labels[Btn_Yes]->GetValue(),
                               m_labels[Btn_No]->GetValue());
        }
    }
    else
    {
        if ( style & wxCANCEL )
        {
            dlg.SetOKCancelLabels(m_labels[Btn_Ok]->GetValue(),
                                  m_labels[Btn_Cancel]->GetValue());
        }
        else
        {
            dlg.SetOKLabel(m_labels[Btn_Ok]->GetValue());
        }
    }

    if ( style & wxHELP )
    {
        dlg.SetHelpLabel(m_labels[Btn_Help]->GetValue());
    }
}

void bleConDialog::OnApply(wxCommandEvent& WXUNUSED(event))
{
    wxMessageDialog dlg(this, GetMessage(), GetBoxTitle(), GetStyle());
    PrepareMessageDialog(dlg);

    ShowResult(dlg.ShowModal());
}

void bleConDialog::ShowResult(int res)
{
    wxString btnName;
    switch ( res )
    {
        case wxID_OK:
            btnName = "OK";
            break;

        case wxID_CANCEL:
            btnName = "Cancel";
            break;

        case wxID_YES:
            btnName = "Yes";
            break;

        case wxID_NO:
            btnName = "No";
            break;

        case wxID_HELP:
            btnName = "Help";
            break;

        default:
            btnName = "Unknown";
    }

    m_labelResult->SetLabel(
        wxString::Format("Dialog was closed with the \"%s\" button.", btnName)
    );
}

void bleConDialog::OnClose(wxCommandEvent& WXUNUSED(event))
{
    EndModal(wxID_CANCEL);
}