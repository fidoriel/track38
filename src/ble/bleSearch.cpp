#include "bleSearch.h"

bleSearch::bleSearch(/* args */)
{
}

bool bleSearch::searchAdapter( int adapterNumber )
{
    auto adapter_list = SimpleBLE::Adapter::get_adapters();
    if (adapter_list.size() == 0) {
        return false;
    }

    adapter = new SimpleBLE::Adapter( adapter_list[ adapterNumber ] );
    return true;
}

bool bleSearch::scan()
{
    this->adapter->scan_for(5000);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::vector<SimpleBLE::Peripheral> peripherals = adapter->scan_get_results();
    if ( peripherals.size() )
    {
        return true;
    }
    return false;
}

bleSearch::~bleSearch()
{
}