#ifndef GAME_H
#define GAME_H

class GameFacade;

class IGame
{
public:
    virtual GameFacade* getGameFacade() const = 0;
    virtual void releaseResources() = 0;
    virtual ~IGame() = 0;
};

#endif // GAME_H
