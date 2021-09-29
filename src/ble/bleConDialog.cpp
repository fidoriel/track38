#include "bleConDialog.h"

bleConDialog::bleConDialog( wxWindow *parent ) : wxDialog( parent, wxID_ANY, "Search for PoweredUP Hubs", wxDefaultPosition, wxSize(-1, -1), wxDEFAULT_DIALOG_STYLE )
{
    blesearch = new bleSearch();
    if ( blesearch->searchAdapter( 0 ) )
        wxMessageBox("Adapter found");
    else
        wxMessageBox("No Adapter found");

}

bool bleConDialog::Create()
{
    topSizer = new wxBoxSizer( wxVERTICAL );
    buttonSizer = new wxBoxSizer( wxHORIZONTAL );
    bleDeviceList = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxSize(250, 300), 0, NULL );
    cancelButton = new wxButton( this, wxID_ANY, "Cancel" );
    selDevButton = new wxButton( this, wxID_ANY, "Select" );
    rescanButton = new wxButton( this, wxID_ANY, "Repeat scan" );

    buttonSizer->Add( cancelButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10 );
    buttonSizer->Add( rescanButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10 );
    buttonSizer->Add( selDevButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10 );

    topSizer->Add( bleDeviceList, 7, wxALL | wxGROW, 10 );
    topSizer->Add( buttonSizer, 1, wxALL | wxALIGN_CENTER_HORIZONTAL, 0 );

    SetSizerAndFit( topSizer );
    CentreOnScreen();

    return true;
}