#ifndef DATA_H
#define DATA_H

#include "../Management/dataentryfacade.h"

class IData
{
public:
    virtual DataEntryFacade* getDataEntryFacade() const = 0;
    virtual ~IData() = 0;
};

#endif // DATA_H
