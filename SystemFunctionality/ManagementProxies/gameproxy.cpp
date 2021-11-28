#include "gameproxy.h"
#include "gamemanager.h"

GameProxy::GameProxy(QObject *parent)
    : QObject(parent)
{
}

GameFacade *GameProxy::getGameFacade() const
{
    return GameManager::getManager()->getGameFacade();
}

void GameProxy::releaseResources()
{
    GameManager::getManager()->releaseResources();
}
