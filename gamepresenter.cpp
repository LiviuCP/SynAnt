#include <QGuiApplication>
#include <QDebug>

#include "gamepresenter.h"
#include "wordmixer.h"
#include "scoreitem.h"
#include "gamestrings.h"

GamePresenter::GamePresenter(QObject *parent)
    : QObject(parent)
    , m_IntroPaneVisible {true}
    , m_HelpPaneVisible {false}
    , m_MainPaneVisible {false}
    , m_MainPaneInitialized {false}
    , m_IntroPaneMessage {GameStrings::c_IntroWindowWelcomeMessage}
    , m_HelpPaneMessage {GameStrings::c_HelpWindowMessage}
    , m_MainPaneInstructionsMessage {GameStrings::c_InstructionsMessage}
    , m_MainPaneStatusMessage {GameStrings::c_NoUserInputMessage}
    , m_CurrentPane {Pane::INTRO}
    , m_pWordMixer {new WordMixer{QGuiApplication::applicationDirPath() + "/" + GameStrings::c_FileName, this}}
    , m_pScoreItem {new ScoreItem{this}}
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
    if (!m_MainPaneInitialized)
    {
        _initMainPane();
    }

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
        Q_ASSERT(static_cast<int>(m_CurrentPane) >= 0 && static_cast<int>(m_CurrentPane) < static_cast<int>(Pane::Nr_Of_Panes));
    }

    m_CurrentPane = Pane::MAIN;
    m_MainPaneVisible = true;
    Q_EMIT mainPaneVisibleChanged();
}

void GamePresenter::handleResultsRequest()
{

    _updateStatusMessage(Game::StatusCodes::REQUESTED_BY_USER);
    m_pWordMixer -> mixWords();

    qDebug() << "Words displayed as per user request! New words mixed!";
    qDebug() << "First word:" << m_pWordMixer->getFirstWord();
    qDebug() << "Second word:" << m_pWordMixer->getSecondWord();
    qDebug() << "Mixed words pieces are (in this order): ";
    for (auto piece : m_pWordMixer->getMixedWordsStringArray())
    {
        qDebug() << piece;
    }
}

bool GamePresenter::handleSubmitRequest(const QString &firstWord, const QString &secondWord)
{
    bool clearTextFields{false};

    Game::StatusCodes statusCode{m_pWordMixer->checkWords(firstWord, secondWord)};

    _updateStatusMessage(statusCode);

    if (statusCode == Game::StatusCodes::SUCCESS)
    {
        m_pWordMixer -> mixWords();

        qDebug() << "Words guessed by user correctly! New words mixed!";
        qDebug() << "First word:" << m_pWordMixer->getFirstWord();
        qDebug() << "Second word:" << m_pWordMixer->getSecondWord();
        qDebug() << "Mixed words pieces are (in this order): ";
        for (auto piece : m_pWordMixer->getMixedWordsStringArray())
        {
            qDebug() << piece;
        }

        clearTextFields = true;
    }

    return clearTextFields;
}

void GamePresenter::_initMainPane()
{
    m_MainPaneInitialized = true;
    m_pWordMixer -> mixWords();

    qDebug() << "Main window initialized! Words mixed";
    qDebug() << "First word:" << m_pWordMixer->getFirstWord();
    qDebug() << "Second word:" << m_pWordMixer->getSecondWord();
    qDebug() << "Mixed words pieces are (in this order): ";
    for (auto piece : m_pWordMixer->getMixedWordsStringArray())
    {
        qDebug() << piece;
    }
}

void GamePresenter::_updateStatusMessage(Game::StatusCodes statusCode)
{
    switch (statusCode)
    {
    case Game::StatusCodes::SUCCESS:
        m_MainPaneStatusMessage = GameStrings::c_SuccessMessage;
        m_MainPaneStatusMessage += "\n\nThe two words are:\n\n";
        m_MainPaneStatusMessage += "\t" + m_pWordMixer->getFirstWord() + "\n";
        m_MainPaneStatusMessage += "\t" + m_pWordMixer->getSecondWord() + "\n";
        m_MainPaneStatusMessage += "\nThe words are: \n\n\t";
        m_MainPaneStatusMessage += m_pWordMixer->areSynonyms() ? "synonyms" : "antonyms";
        m_MainPaneStatusMessage += "\n\nNext pair of words is available below.";
        break;
    case Game::StatusCodes::MISSING_WORDS:
        m_MainPaneStatusMessage = GameStrings::c_MissingWordsMessage;
        break;
    case Game::StatusCodes::INCORRECT_WORDS:
        m_MainPaneStatusMessage = GameStrings::c_IncorrectWordsMessage;
        break;
    case Game::StatusCodes::REQUESTED_BY_USER:
        m_MainPaneStatusMessage = "The correct words are: \n\n";
        m_MainPaneStatusMessage += "\t" + m_pWordMixer->getFirstWord() + "\n";
        m_MainPaneStatusMessage += "\t" + m_pWordMixer->getSecondWord() + "\n";
        m_MainPaneStatusMessage += "\nThe words are: \n\n\t";
        m_MainPaneStatusMessage += m_pWordMixer->areSynonyms() ? "synonyms" : "antonyms";
        m_MainPaneStatusMessage += "\n\nNext pair of words is available below.";
        break;
    default:
        ;
    }

    Q_EMIT mainPaneStatusMessageChanged();
}
