#include "bleSearch.h"

bleSearch::bleSearch( wxDialog* parent, wxListBox* bleDeviceList ) : wxThread(wxTHREAD_JOINABLE)
{
    this->resultBox = bleDeviceList;
    this->parent = parent;

    auto adapter_list = SimpleBLE::Adapter::get_adapters();
    if (adapter_list.size() > 0) {
        adapter = new SimpleBLE::Adapter( adapter_list[ 0 ] );
    }
    else
        adapter = nullptr;
}

int bleSearch::scan()
{
    adapter->set_callback_on_scan_found([&](SimpleBLE::Peripheral peripheral) {
        wxMutexGuiEnter();

        if ( peripheral.identifier().length() > 0 )
            resultBox->AppendString( wxString( peripheral.identifier() ) );
        else
            resultBox->AppendString( wxString( peripheral.address() ) );
        
        resultBox->Refresh();
        wxMutexGuiLeave();

        wxMilliSleep(50);
    });

    //wxMessageBox( "Thread Start" );
    int ctr = 0;

    this->adapter->scan_start();
    while ( ctr <= 5000 )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        ctr++;

        if ( TestDestroy() || CancelationReqested() )
        {
            this->adapter->scan_stop();
            return 0;
        }
    }
    this->adapter->scan_stop();

    ctr = 0;
    while ( ctr <= 1000 )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        ctr++;

        if ( TestDestroy() || CancelationReqested() )
            return 0;
    }

    results = adapter->scan_get_results();

    // notify the main thread
    //wxCommandEvent event( wxEVT_COMMAND_BUTTON_CLICKED, FINISHED_BLE_ID );
    //event.SetInt(n);  // pass some data along the event, a number in this case
    //this->parent->GetEventHandler()->AddPendingEvent( event );

    wxThreadEvent event( wxEVT_THREAD, FINISHED_BLE_ID );
    event.SetInt(-1); // that's all
    wxQueueEvent( parent, event.Clone() );

    return 0;
}

bool bleSearch::CancelationReqested()
{
    wxCriticalSectionLocker lock(m_csCancelled);
    return this->threadTerminated;
}

void bleSearch::RequestTermination()
{
    wxCriticalSectionLocker lock(m_csCancelled);
    this->threadTerminated = true;
}

wxThread::ExitCode bleSearch::Entry()
{
    if ( adapter != nullptr )
    {
        this->scan();
    }

    this->threadTerminated = true;
    //wxMessageBox( "Thread End" );
    return NULL;
}

void bleSearch::startScan()
{
    resultBox->Clear();
    this->Run();
}

bleSearch::~bleSearch()
{
}