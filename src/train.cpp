#include "train.h"

train::train()
{

}

bool train::isPf()
{
    if (this->control->compare("pf") == 0)
       return true;
    return false;    
}

bool train::isUp()
{
    if (this->control->compare("up") == 0)
       return true;
    return false;
}