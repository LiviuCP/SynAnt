/*

  This class is responsible for managing the windows of the game.
  Exception is the fatal errors window, which is currently managed by the main() function.

  The managed windows are:
  - introduction window
  - help (hints) window
  - the main game window

  The manager creates the windows and connects them with each other.

*/
#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QObject>

#include <memory>

#include "../UIResources/BuildingBlocks/introductionwindow.h"
#include "../UIResources/BuildingBlocks/hintswindow.h"
#include "../UIResources/BuildingBlocks/maingamewindow.h"

class WordMixer;

class AppManager : public QObject
{
    Q_OBJECT
public:
    explicit AppManager(QObject *parent = nullptr);
    void init(const QString& filename);
    void hideActiveWindow();

signals:

public slots:
    void _onSwitchedIntroToHints();
    void _onSwitchedIntroToMain();
    void _onSwitchedHintsToMain();
    void _onSwitchedMainToHints();

private:
    bool m_IsInitialized;

    WordMixer* m_pWordMixer;

    std::unique_ptr<IntroductionWindow> m_pIntroductionWindow;
    std::unique_ptr<HintsWindow> m_pHintsWindow;
    std::unique_ptr<MainGameWindow> m_pMainGameWindow;
};

#endif // APPMANAGER_H
