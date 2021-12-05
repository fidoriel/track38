#include "bleControlThread.h"

bleControlThread::bleControlThread( string sadr, string sname ) : wxThread(wxTHREAD_JOINABLE)
{
    this->sadr = sadr;
    this->sname = sname;
    auto adapter_list = SimpleBLE::Adapter::get_adapters();
    if (adapter_list.size() > 0) {
        adapter = new SimpleBLE::Adapter( adapter_list[ 0 ] );
    }
    else
        adapter = nullptr;
}

void bleControlThread::setCallbackFunc(std::function<void(bool)> onConStatusChange)
{
    this->onStatusChangeCallback = onConStatusChange;
}

void bleControlThread::connect()
{
    this->Run();
}

void bleControlThread::RequestTermination()
{
    wxCriticalSectionLocker lock(m_csCancelled);
    this->threadTerminated = true;
}

void bleControlThread::idle()
{
    while ( 1 )
    {
        if ( this->cmdInPipe )
        {
            this->hub->write_request(uuids[1].first, uuids[1].second, this->message );
            this->cmdInPipe = false;
            std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
        }

        if ( TestDestroy() || this->threadTerminated )
        {
            break;
        }

        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }

    this->hub->disconnect();
    this->onStatusChangeCallback(false);
}

wxThread::ExitCode bleControlThread::Entry()
{
    if ( this->adapter && this->connectionAttempt() )
    {
        this->idle();
    }

    return NULL;
}

bool bleControlThread::connectionAttempt()
{
    adapter->set_callback_on_scan_found([&](SimpleBLE::Peripheral peripheral) {
        if ( ( peripheral.address().compare( this->sadr ) == 0 ) || ( peripheral.identifier().compare( this->sname ) == 0 ) )
        {
            //wxMessageBox("found device");
            this->hubDetected = true;
        }
    });

    //wxMessageBox( "Thread Start" );
    int ctr = 0;

    this->adapter->scan_start();
    //wxMessageBox("scanning started");
    while ( ( ctr <= 5000 ) && !this->hubDetected )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        ctr++;

        if ( TestDestroy() )
        {
            this->adapter->scan_stop();
            return 0;
        }
    }
    this->adapter->scan_stop();
    //wxMessageBox("scanning stoped");

    ctr = 0;
    while ( ctr <= 100 )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        ctr++;

        if ( TestDestroy() )
            return 0;
    }

    results = adapter->scan_get_results();

    // setting hub
    for (int i = 0; i < results.size(); i++) {
        if ( ( results[ i ].address().compare( this->sadr ) == 0 ) || ( results[ i ].identifier().compare( this->sname ) == 0 ) )
            this->hub = &results[ i ];
    }

    if ( this->hub )
    {
        this->hub->connect();
        if ( this->hub->is_connected() )
        {
            this->onStatusChangeCallback(true);

            for (auto service : this->hub->services())
            {
                for (auto characteristic : service.characteristics)
                {
                    uuids.push_back(std::make_pair(service.uuid, characteristic));
                }
            }

            for (int i = 0; i < uuids.size(); i++) {
                string sID(serviceUUID);
                string cID(characteristicUUID);
                if ( strcasecmp(uuids[ i ].first.c_str(), sID.c_str()) == 0)
                    if ( strcasecmp(uuids[ i ].second.c_str(), cID.c_str()) == 0)
                    {
                        this->bleCharIDX = i;
                        break;
                    }
            }

            if ( this->bleCharIDX == -1 )
            {
                this->hub->disconnect();
                this->onStatusChangeCallback(false);
                this->threadTerminated = true;
            }

            return true;
        }
    }
    else
    {
        this->onStatusChangeCallback(false);
        this->threadTerminated = true;
    }

    return false;
}

void bleControlThread::sendCommand( uint8_t ary[], int len )
{
    wxCriticalSectionLocker lock(m_csCancelled);
    this->message = string((const char*) ary, len);
    this->cmdInPipe = true;
}

bleControlThread::~bleControlThread()
{
}