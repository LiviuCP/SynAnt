#include "dataproxy.h"
#include "../Management/gamemanager.h"

DataProxy::DataProxy(QObject *parent)
    : QObject(parent)
{
}

DataEntryFacade* DataProxy::getDataEntryFacade() const
{
    return GameManager::getManager()->getDataEntryFacade();
}
