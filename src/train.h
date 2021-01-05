#ifndef train_h
#define train_h

#include <string>

class train
{ 
    public:
        train();
        bool isPf();
        bool isUp();

        std::string* name;
        std::string* control;
        std::string* port;
        int maxTrainSpeed;

        //UP
        std::string* upHubAdress;
        char upChannel;
        bool upTwoMotorsUsed;

        //PF
        int pfChannel;
        char pfSubChannel;
        int pfGpio;
};

#endif