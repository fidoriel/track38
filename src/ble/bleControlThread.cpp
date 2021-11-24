#include "bleControlThread.h"

bleControlThread::bleControlThread() : wxThread(wxTHREAD_JOINABLE)
{
}

wxThread::ExitCode bleControlThread::Entry()
{
    // tuples.push_back(std::make_tuple(18, 17, 19));
    // for (auto [ X, Y, Z ] : tuples)
    // {
    // }

    auto adapter_list = SimpleBLE::Adapter::get_adapters();
    if (adapter_list.size() > 0) {
        adapter = new SimpleBLE::Adapter( adapter_list[ 0 ] );
    }

    return NULL;
}

void bleControlThread::queHub( std::string hub )
{
    hubQue.push_back( hub );
}

void bleControlThread::sendCommand()
{
    // std::string message((const char*) ary, contents.length()/2);
    // peripheral.write_command(uuids[selection].first, uuids[selection].second, message);
}

bleControlThread::~bleControlThread()
{
}