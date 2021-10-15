#include "bleConDialog.h"

BEGIN_EVENT_TABLE( bleConDialog, wxDialog )
    EVT_BUTTON( ID_REFRESHSCAN, bleConDialog::triggerScan )
    EVT_LISTBOX( ID_SELECTIONLISTBOX, bleConDialog::getSelection )
    EVT_CLOSE( bleConDialog::OnClose )
END_EVENT_TABLE()

bleConDialog::bleConDialog( wxWindow *parent ) : wxDialog( parent, wxID_ANY, "Search for PoweredUP Hubs", wxDefaultPosition, wxSize(-1, -1), wxCAPTION )
{
    blesearch = new bleSearch();
    if ( !blesearch->searchAdapter( 0 ) )
        wxMessageBox( "No Blutooth Adapter found" );
}

bool bleConDialog::Create()
{
    topSizer = new wxBoxSizer( wxVERTICAL );
    buttonSizer = new wxBoxSizer( wxHORIZONTAL );
    bleDeviceList = new wxListBox( this, ID_SELECTIONLISTBOX, wxDefaultPosition, wxSize(250, 300) );
    cancelButton = new wxButton( this, wxID_CANCEL, "Cancel" );
    selDevButton = new wxButton( this, wxID_OK, "Select" );
    rescanButton = new wxButton( this, ID_REFRESHSCAN, "Repeat scan" );

    buttonSizer->Add( cancelButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10 );
    buttonSizer->Add( rescanButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10 );
    buttonSizer->Add( selDevButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10 );

    topSizer->Add( bleDeviceList, 7, wxALL | wxGROW, 10 );
    topSizer->Add( buttonSizer, 1, wxALL | wxALIGN_CENTER_HORIZONTAL, 0 );

    SetSizerAndFit( topSizer );
    CentreOnScreen();
    this->DlgIsOpen = true;
    
    std::thread t(&bleConDialog::scan, this);
    t.detach();

    return true;
}

void bleConDialog::OnClose( wxCloseEvent& event )
{
    this->DlgIsOpen = false;
}

void bleConDialog::triggerScan( wxCommandEvent& event )
{
    std::thread t(&bleConDialog::scan, this);
    t.detach();
}

void bleConDialog::getSelection( wxCommandEvent& event )
{
    if( bleDeviceList->GetCount() > 0)
        this->selection = bleDeviceList->GetSelection();
    
    else
        this->selection = -1;
}

void bleConDialog::scan()
{
    if ( this->IsScanning )
        return;

    this->IsScanning = true;
    bleDeviceList->Clear();
    int devCount = blesearch->scan();
    this->IsScanning = false;

    if ( ( this->IsScanning == true ) || ( devCount <= 0) )
        return;

    if( ( this->DlgIsOpen ) && ( this->bleDeviceList != NULL ) )
        for (int i = 0; i < (devCount - 1); i++)
            if (blesearch->peripherals[i].identifier().length() > 0)
                bleDeviceList->AppendString( wxString( blesearch->peripherals[i].identifier() ) );
            else
                bleDeviceList->AppendString( wxString( blesearch->peripherals[i].address() ) );   
}