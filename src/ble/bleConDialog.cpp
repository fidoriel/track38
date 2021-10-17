#include "bleConDialog.h"

BEGIN_EVENT_TABLE( bleConDialog, wxDialog )
    EVT_BUTTON( ID_REFRESHSCAN, bleConDialog::OnTriggerScan )
    EVT_BUTTON( ID_CANCEL, bleConDialog::OnClose )
    EVT_LISTBOX( ID_SELECTIONLISTBOX, bleConDialog::OnGetSelection )
    EVT_THREAD( FINISHED_BLE_ID, bleConDialog::OnScanFinished )
    //EVT_CLOSE( bleConDialog::OnClose )
END_EVENT_TABLE()

bleConDialog::bleConDialog( wxWindow *parent ) : wxDialog( parent, wxID_ANY, "Search for PoweredUP Hubs", wxDefaultPosition, wxSize(-1, -1), wxCAPTION )
{
    auto adapter_list = SimpleBLE::Adapter::get_adapters();
    if (adapter_list.size() == 0)
    {
        wxMessageBox( "No Blutooth Adapter found" );
        this->adapterExists = false;
    }
    else    
        this->adapterExists = true;
}

bool bleConDialog::Create()
{
    topSizer = new wxBoxSizer( wxVERTICAL );
    buttonSizer = new wxBoxSizer( wxHORIZONTAL );
    bleDeviceList = new wxListBox( this, ID_SELECTIONLISTBOX, wxDefaultPosition, wxSize(250, 300) );
    cancelButton = new wxButton( this, ID_CANCEL, "Cancel" );
    selDevButton = new wxButton( this, wxID_OK, "Select" );
    rescanButton = new wxButton( this, ID_REFRESHSCAN, "Repeat scan" );

    buttonSizer->Add( cancelButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10 );
    buttonSizer->Add( rescanButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10 );
    buttonSizer->Add( selDevButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10 );

    topSizer->Add( bleDeviceList, 7, wxALL | wxGROW, 10 );
    topSizer->Add( buttonSizer, 1, wxALL | wxALIGN_CENTER_HORIZONTAL, 0 );

    SetSizerAndFit( topSizer );
    CentreOnScreen();
    if ( blesearch == nullptr )
        blesearch = new bleSearch( this, bleDeviceList );
    
    this->triggerScan();

    return true;
}

void bleConDialog::OnClose( wxCommandEvent& event )
{
    blesearch->RequestTermination();
    wxMilliSleep(100);
    this->Close();
}

void bleConDialog::OnTriggerScan( wxCommandEvent& event )
{
    this->triggerScan();
}

void bleConDialog::triggerScan()
{    
    if ( blesearch->threadTerminated )
    {
        delete blesearch;
        blesearch = new bleSearch( this, bleDeviceList );
    }

    this->rescanButton->Disable();
    this->selDevButton->Disable();


    if ( adapterExists )
    {
        blesearch->startScan();
    }
}

std::string bleConDialog::getBleId( std::string tag )
{
    for (size_t i = 0; i < blesearch->results.size(); i++)
        if ( !tag.compare( blesearch->results[i].identifier() ) || !tag.compare( blesearch->results[i].address() ) )
            return blesearch->results[i].identifier();
}

std::string bleConDialog::getBleAdr( std::string tag )
{
    for (size_t i = 0; i < blesearch->results.size(); i++)
        if ( !tag.compare( blesearch->results[i].identifier() ) || !tag.compare( blesearch->results[i].address() ) )
            return blesearch->results[i].address();
}

void bleConDialog::OnGetSelection( wxCommandEvent& event )
{
    if( this->bleDeviceList->GetCount() > 0)
        this->selection = bleDeviceList->GetSelection();
    
    else
        this->selection = -1;
}

void bleConDialog::OnScanFinished( wxThreadEvent& event )
{
    this->rescanButton->Enable();

    if( this->bleDeviceList->GetCount() > 0)
        this->selDevButton->Enable();
}

bleConDialog::~bleConDialog()
{
    // delete topSizer;;
    // delete buttonSizer;
    // delete bleDeviceList;
    // delete cancelButton;
    // delete selDevButton;
    // delete rescanButton;

    // delete blesearch;
}

// void bleConDialog::scan()
// {
//     if ( this->IsScanning )
//         return;

//     this->IsScanning = true;
//     bleDeviceList->Clear();
//     int devCount = blesearch->scan();
//     this->IsScanning = false;

//     if ( ( this->IsScanning == true ) || ( devCount <= 0) )
//         return;

//     auto tmp = *peripheralsPtr.get();

//     if( ( this->DlgIsOpen ) && ( this->bleDeviceList != NULL ) )
//         for (int i = 0; i < (devCount - 1); i++)
//             if ( tmp[i].identifier().length() > 0 )
//                 bleDeviceList->AppendString( wxString( tmp[i].identifier() ) );
//             else
//                 bleDeviceList->AppendString( wxString( tmp[i].address() ) );
// }