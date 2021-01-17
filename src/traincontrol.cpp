#include "traincontrol.h"

trainControlBox::trainControlBox( wxPanel* parent, int id, wxString title, wxString boxName ) : wxStaticBox( parent, id, title, wxDefaultPosition, wxDefaultSize, 0L, boxName )
{

    this->parent = parent;

    //Sizer
    topSizer = new wxFlexGridSizer( 3, 0, 0 );

    this->loadTrains();
    this->createControlBox();
}

void trainControlBox::RefreshPanel()
{      

    while ( !topSizer->IsEmpty() )
    {
        topSizer->Remove(0);
    }
    
    delete stopAllBtn;

    while ( !trains.empty() )
    {
        delete trains.front()->speedSlider;
        delete trains.front()->trainName;
        delete trains.front()->stopBtn;
        delete trains.front();
        trains.pop_front();
    }

    if ( trains.empty() )
    {
        this->loadTrains();
        this->createControlBox();
    }
}

void trainControlBox::createControlBox()
{
    topSizer->AddSpacer(10);
    topSizer->AddSpacer(10);
    topSizer->AddSpacer(10);
    for (train* & selTrain : this->trains)
    {
        selTrain->createControls(this);
        topSizer->Add( selTrain->trainName, 1, wxALL, 10 );
        topSizer->Add( selTrain->speedSlider, 1, wxALL , 10 );
        topSizer->Add( selTrain->stopBtn, 1, wxALL , 10 );

        selTrain->stopBtn->Bind( wxEVT_BUTTON, &train::OnStop, selTrain, selTrain->ID_StopTrain );
        selTrain->speedSlider->Bind( wxEVT_SCROLL_THUMBRELEASE, &train::OnChangeSpeed, selTrain, selTrain->ID_ChangeSpeed );
    }

    stopAllBtn = new wxButton( this, ID_StopAll, "Stop All", wxDefaultPosition, wxDefaultSize );
    stopAllBtn->Bind( wxEVT_BUTTON, &trainControlBox::OnStopAll, this, ID_StopAll);
    
    topSizer->AddSpacer( 20 );
    topSizer->Add( stopAllBtn, 0, wxALL | wxALIGN_CENTER_HORIZONTAL , 10 );

    parent->SetSizer( topSizer );
    parent->Layout();
	topSizer->Fit( this );
    topSizer->SetSizeHints( this );
    parent->SendSizeEventToParent();
}

void trainControlBox::loadTrains()
{
    //init Config
    wxConfigBase* track38ConfigTrain = wxConfigBase::Get();
    track38ConfigTrain->SetPath( "/Train/" );
    trains.clear();

    // Read Train Names
    long idx;
    wxString out;
    bool exists = track38ConfigTrain->GetFirstGroup( out, idx );
    if (  exists == true  )
        trains.push_back( new train(out) );
    
    while ( exists )
    {
        exists = track38ConfigTrain->GetNextGroup( out, idx );
        if (  exists == true  )
            trains.push_back( new train(out) );
    }

    //Fill tain information
    for (train* & selTrain : trains)
    {
        // Access the object through iterator
        track38ConfigTrain->SetPath( "/Train/" );
        track38ConfigTrain->SetPath( selTrain->name );

        selTrain->setControl( track38ConfigTrain->Read( "control", "pf" ) );
        selTrain->setMaxSpeed( track38ConfigTrain->Read( "maxSpeed", "7" ) );
        selTrain->setPort( track38ConfigTrain->Read( "port", "" ) );

        if ( selTrain->isPf() )
        {
            selTrain->setPfGPIO( track38ConfigTrain->Read( "gpio", "13" ) );
            selTrain->setPfChannel( track38ConfigTrain->Read( "channel", "1" ) );
            selTrain->setPfSubChannel( track38ConfigTrain->Read( "subChannel", "R" ) );        
        }

        else if ( selTrain->isUp() )
        {
            selTrain->setHubAdress( track38ConfigTrain->Read( "hubAdress", "" ) );
            selTrain->setUpChannel( track38ConfigTrain->Read( "channel", "1" ) );
            selTrain->setTwoMotors( track38ConfigTrain->Read( "twoMotorsUsed", false ) );
        }

        //Debug

        //wxMessageBox( selTrain->name );
        //wxMessageBox( track38ConfigTrain->Read( "port", "" ) );
        //wxMessageBox( wxString::Format(wxT("%i"), selTrain->maxTrainSpeed ) );
    }
}

void trainControlBox::OnStopAll( wxCommandEvent& event )
{
    this->StopAll();
}

void trainControlBox::StopAll()
{
    for (train* & selTrain : this->trains)
    {
        selTrain->Stop();
    }  
}

trainControlBox::~trainControlBox()
{
}