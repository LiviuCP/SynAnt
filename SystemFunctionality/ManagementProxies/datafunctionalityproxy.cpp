#include "datafunctionalityproxy.h"
#include "../Management/gamemanager.h"
#include "../ManagementProxies/dataentryproxy.h"

DataFunctionalityProxy::DataFunctionalityProxy(QObject *parent)
    : QObject(parent)
{
}

DataEntryProxy *DataFunctionalityProxy::getDataEntryProxy() const
{
    return GameManager::getManager()->getDataEntryProxy();
}
