#include "gameinitproxy.h"
#include "../Management/gamemanager.h"

GameInitProxy::GameInitProxy(QObject *parent)
    : QObject(parent)
{
}

void GameInitProxy::setEnvironment(const QString &dataDirPath)
{
    GameManager::getManager()->setEnvironment(dataDirPath);
}

GameInitProxy::~GameInitProxy()
{

}
