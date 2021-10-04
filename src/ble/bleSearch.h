#ifndef bleSearch_h
#define bleSearch_h

#include <thread>
#include <chrono>

#include "simpleble/SimpleBLE.h"
#include "simpleble/Adapter.h"

class bleSearch
{
public:
    bleSearch(/* args */);
    bool searchAdapter( int adapterNumber );
    int scan();
    ~bleSearch();

    std::vector<SimpleBLE::Peripheral> peripherals;
    SimpleBLE::Adapter* adapter;
};

#endif