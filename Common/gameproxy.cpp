#include "gameproxy.h"
#include "gamemanager.h"

GameProxy::GameProxy(QObject *parent)
    : QObject(parent)
{
}

GameFacade *GameProxy::getFacade() const
{
    return GameManager::getManager()->getFacade();
}

void GameProxy::releaseResources()
{
    GameManager::getManager()->releaseResources();
}
