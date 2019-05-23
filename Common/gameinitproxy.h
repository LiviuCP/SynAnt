/* Used by GameManager to avoid exposing full functionality to the main() function */

#ifndef GAMEINITPROXY_H
#define GAMEINITPROXY_H

#include <QObject>

class GameManager;

class GameInitProxy : public QObject
{
public:
    explicit GameInitProxy(QObject *parent = nullptr);
    void setDataSource(const QString& dataDirPath);
};

#endif // GAMEINITPROXY_H
