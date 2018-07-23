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
    , m_ResetEnabled {false}
    , m_IntroPaneMessage {GameStrings::c_IntroWindowWelcomeMessage}
    , m_HelpPaneMessage {GameStrings::c_HelpWindowMessage}
    , m_MainPaneInstructionsMessage {GameStrings::c_InstructionsMessage}
    , m_MainPaneStatusMessage {GameStrings::c_NoUserInputMessage}
    , m_CurrentPane {Pane::INTRO}
    , m_pWordMixer {new WordMixer{QGuiApplication::applicationDirPath() + "/" + GameStrings::c_FileName, this}}
    , m_pScoreItem {new ScoreItem{this}}
{
    bool connected{connect(this,&GamePresenter::levelChanged,m_pWordMixer,&WordMixer::setWordPieceSize)};
    Q_ASSERT(connected);
    connected = connect(this,&GamePresenter::levelChanged,m_pScoreItem,&ScoreItem::setScoreIncrement);
    Q_ASSERT(connected);
    connected = connect(m_pScoreItem,&ScoreItem::statisticsUpdated,this,&GamePresenter::_onStatisticsUpdated);
    Q_ASSERT(connected);

    _onStatisticsUpdated();
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
        Q_ASSERT(static_cast<int>(m_CurrentPane) >= 0 && static_cast<int>(m_CurrentPane) < static_cast<int>(Pane::Nr_Of_Panes));
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
    qDebug() << "=====================================================";
    qDebug() << "Words displayed as per user request! New words mixed";

    _updateStatusMessage(Game::StatusCodes::REQUESTED_BY_USER);
    m_pScoreItem -> updateStatistics(Game::StatisticsUpdate::PARTIAL_UPDATE);

    if (!m_ResetEnabled)
    {
        m_ResetEnabled = true;
        Q_EMIT resetEnabledChanged();
    }

    m_pWordMixer -> mixWords();
}

bool GamePresenter::handleSubmitRequest(const QString &firstWord, const QString &secondWord)
{
    bool clearTextFields{false};

    Game::StatusCodes statusCode{m_pWordMixer->checkWords(firstWord, secondWord)};

    _updateStatusMessage(statusCode);

    if (statusCode == Game::StatusCodes::SUCCESS)
    {
        qDebug() << "=====================================================";
        qDebug() << "Words guessed by user correctly! New words mixed";

        m_pScoreItem -> updateStatistics(Game::StatisticsUpdate::FULL_UPDATE);

        if (!m_ResetEnabled)
        {
            m_ResetEnabled = true;
            Q_EMIT resetEnabledChanged();
        }

        m_pWordMixer -> mixWords();

        clearTextFields = true;
    }

    return clearTextFields;
}

void GamePresenter::handleResetRequest()
{
    qDebug() << "=====================================================";
    qDebug() << "Statistics reset! Same mixed words to be guessed by user";

    m_pScoreItem -> resetStatistics();
    _updateStatusMessage(Game::StatusCodes::STATISTICS_RESET);
    m_ResetEnabled = false;
    Q_EMIT resetEnabledChanged();
}

void GamePresenter::switchToEasyLevel()
{
    qDebug() << "=====================================================";
    qDebug() << "Level changed to easy! New words mixed";

    _setLevel(Game::Level::EASY);
}

void GamePresenter::switchToMediumLevel()
{
    qDebug() << "=====================================================";
    qDebug() << "Level changed to medium! New words mixed";

    _setLevel(Game::Level::MEDIUM);
}

void GamePresenter::switchToHardLevel()
{
    qDebug() << "=====================================================";
    qDebug() << "Level changed to hard! New words mixed";

    _setLevel(Game::Level::HARD);
}

void GamePresenter::_onStatisticsUpdated()
{
    QVector<int> scoresPairs{m_pScoreItem -> getStatistics()};

    m_MainPaneScoreMessage = "High-score: " + QString::number(scoresPairs[0]) + "/" + QString::number(scoresPairs[1]);
    m_MainPaneNrOfPairsMessage = "Word pairs: " + QString::number(scoresPairs[2]) + "/" + QString::number(scoresPairs[3]);

    Q_EMIT mainPaneScoreMessageChanged();
    Q_EMIT mainPaneNrOfPairsMessageChanged();
}

void GamePresenter::_initMainPane()
{
    qDebug() << "=====================================================";
    qDebug() << "Main window initialized! First 2 words mixed";

    m_MainPaneInitialized = true;
    m_pWordMixer -> mixWords();
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
    case Game::StatusCodes::STATISTICS_RESET:
        m_MainPaneStatusMessage = GameStrings::c_ScoresResetMessage;
        break;
    case Game::StatusCodes::LEVEL_CHANGED:
        m_MainPaneStatusMessage = GameStrings::c_LevelChangedMessage;
    //reserved for future use
    default:
        ;
    }

    Q_EMIT mainPaneStatusMessageChanged();
}

void GamePresenter::_setLevel(Game::Level level)
{
    _updateStatusMessage(Game::StatusCodes::LEVEL_CHANGED);
    Q_EMIT levelChanged(level);
    m_pWordMixer -> mixWords();
}