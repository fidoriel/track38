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
    bool scan();
    ~bleSearch();

    SimpleBLE::Adapter* adapter;
};

#endif