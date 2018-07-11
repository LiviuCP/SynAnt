#include "gamepresenter.h"
#include "gamestrings.h"

GamePresenter::GamePresenter(QObject *parent)
    : QObject(parent)
    , m_IntroPaneVisible {true}
    , m_HelpPaneVisible {false}
    , m_MainPaneVisible {false}
    , m_IntroPaneMessage {GameStrings::c_IntroWindowWelcomeMessage}
    , m_HelpPaneMessage {GameStrings::c_HelpWindowMessage}
    , m_MainPaneInstructionsMessage {GameStrings::c_InstructionsMessage}
    , m_MainPaneStatusMessage {GameStrings::c_NoUserInputMessage}
    , m_CurrentPane {Pane::INTRO}
{

}

void GamePresenter::switchToHelpPane()
{
    switch (m_CurrentPane) {
    case Pane::INTRO:
        m_IntroPaneVisible = false;
        Q_EMIT introPaneVisibleChanged();
        break;
    case Pane::MAIN:
        m_MainPaneVisible = false;
        Q_EMIT mainPaneVisibleChanged();
        break;
    default:
        qDebug("No valid window. Please check!");
    }
    m_CurrentPane = Pane::HELP;
    m_HelpPaneVisible = true;
    Q_EMIT helpPaneVisibleChanged();
}

void GamePresenter::switchToMainPane()
{
    switch (m_CurrentPane) {
    case Pane::INTRO:
        m_IntroPaneVisible = false;
        Q_EMIT introPaneVisibleChanged();
        break;
    case Pane::HELP:
        m_HelpPaneVisible = false;
        Q_EMIT helpPaneVisibleChanged();
        break;
    default:
        qDebug("No valid window. Please check!");
    }
    m_CurrentPane = Pane::MAIN;
    m_MainPaneVisible = true;
    Q_EMIT mainPaneVisibleChanged();
}
