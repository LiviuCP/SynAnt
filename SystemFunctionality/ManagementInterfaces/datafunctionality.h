#ifndef DATAFUNCTIONALITY_H
#define DATAFUNCTIONALITY_H

#include "../ManagementProxies/dataentryproxy.h"

class IDataFunctionality
{
public:
    virtual DataEntryProxy* getDataEntryProxy() const = 0;
    virtual ~IDataFunctionality() = 0;
};

#endif // DATAFUNCTIONALITY_H
