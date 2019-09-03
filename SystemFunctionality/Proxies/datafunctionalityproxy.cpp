#include "datafunctionalityproxy.h"
#include "../Management/gamemanager.h"
#include "../Proxies/dataentryproxy.h"

DataFunctionalityProxy::DataFunctionalityProxy(QObject *parent)
    : QObject(parent)
{
}

DataEntryProxy *DataFunctionalityProxy::getDataEntryProxy() const
{
    return GameManager::getManager()->getDataEntryProxy();
}
