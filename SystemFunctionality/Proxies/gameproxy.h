/* Used by GameManager to avoid exposing full functionality to the Presenter */

#ifndef GAMEPROXY_H
#define GAMEPROXY_H

#include <QObject>

class GameFacade;
class GameManager;

class GameProxy : public QObject
{
    Q_OBJECT
public:
    explicit GameProxy(QObject *parent = nullptr);

    GameFacade* getGameFacade() const;
    void releaseResources();
};

#endif // GAMEPROXY_H
