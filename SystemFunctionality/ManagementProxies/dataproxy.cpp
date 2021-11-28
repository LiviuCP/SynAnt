#include "dataproxy.h"
#include "gamemanager.h"

DataProxy::DataProxy(QObject *parent)
    : QObject(parent)
{
}

DataEntryFacade* DataProxy::getDataEntryFacade() const
{
    return GameManager::getManager()->getDataEntryFacade();
}
