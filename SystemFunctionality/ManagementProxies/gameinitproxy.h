/* Used by GameManager to avoid exposing full functionality to the main() function */

#ifndef GAMEINITPROXY_H
#define GAMEINITPROXY_H

#include <QObject>

#include "../ManagementInterfaces/gameinit.h"

class GameManager;

class GameInitProxy : public QObject, public IGameInit
{
public:
    explicit GameInitProxy(QObject *parent = nullptr);
    void setDataSource(const QString& dataDirPath);
    ~GameInitProxy();
};

#endif // GAMEINITPROXY_H
