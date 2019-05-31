#include "gameinitproxy.h"
#include "../Management/gamemanager.h"

GameInitProxy::GameInitProxy(QObject *parent)
    : QObject(parent)
{
}

void GameInitProxy::setDataSource(const QString &dataDirPath)
{
    GameManager::getManager()->setDataSource(dataDirPath);
}
