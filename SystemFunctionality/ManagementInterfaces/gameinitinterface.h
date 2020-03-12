#ifndef GAMEINIT_H
#define GAMEINIT_H

#include <QString>

class IGameInit
{
public:
    virtual void setEnvironment(const QString& dataDirPath) = 0;
    virtual ~IGameInit() = 0;
};

#endif // GAMEINIT_H
