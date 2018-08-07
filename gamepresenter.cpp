#include <QGuiApplication>
#include <QDebug>

#include "gamepresenter.h"
#include "wordmixer.h"
#include "scoreitem.h"
#include "gamestrings.h"

static constexpr int c_ToolTipDelay{1000};

GamePresenter::GamePresenter(QObject *parent)
    : QObject(parent)
    , m_IntroPaneVisible {true}
    , m_HelpPaneVisible {false}
    , m_MainPaneVisible {false}
    , m_MainPaneInitialized {false}
    , m_ResetEnabled {false}
    , m_ErrorOccured {false}
    , m_WindowTitle{GameStrings::c_IntroWindowTitle}
    , m_MainPaneStatusMessage {GameStrings::c_NoUserInputMessage}
    , m_CurrentPane {Pane::INTRO}
    , m_pWordMixer {nullptr}
    , m_pScoreItem {new ScoreItem{this}}
{
    try
    {
        m_pWordMixer = new WordMixer{QGuiApplication::applicationDirPath() + "/" + GameStrings::c_FileName, this};

        bool connected{connect(this,&GamePresenter::levelChanged,m_pWordMixer,&WordMixer::setWordPieceSize)};
        Q_ASSERT(connected);
        connected = connect(this,&GamePresenter::levelChanged,m_pScoreItem,&ScoreItem::setScoreIncrement);
        Q_ASSERT(connected);
        connected = connect(m_pScoreItem,&ScoreItem::statisticsUpdated,this,&GamePresenter::_onStatisticsUpdated);
        Q_ASSERT(connected);

        _onStatisticsUpdated();
    }
    catch(const QString& errorMessage)
    {
        _launchErrorPane(errorMessage);
    }
}

void GamePresenter::switchToPane(Pane pane)
{
    Q_ASSERT(static_cast<int>(pane) >= 0 && static_cast<int>(pane) < static_cast<int>(Pane::Nr_Of_Panes));

    if (m_CurrentPane != pane)
    {
        try
        {
            switch(m_CurrentPane)
            {
            case Pane::INTRO:
                m_IntroPaneVisible = false;
                Q_EMIT introPaneVisibleChanged();
                break;
            case Pane::HELP:
                m_HelpPaneVisible = false;
                Q_EMIT helpPaneVisibleChanged();
                break;
            case Pane::MAIN:
                m_MainPaneVisible = false;
                Q_EMIT mainPaneVisibleChanged();
                break;
            default:
                Q_ASSERT(false);
            }

            switch(pane)
            {
            case Pane::INTRO:
                Q_ASSERT(false);
                break;
            case Pane::HELP:
                m_HelpPaneVisible = true;
                Q_EMIT helpPaneVisibleChanged();
                break;
            case Pane::MAIN:
                if (!m_MainPaneInitialized)
                {
                    _initMainPane();
                }
                m_MainPaneVisible = true;
                Q_EMIT mainPaneVisibleChanged();
                break;
            default:
                Q_ASSERT(false);
            }

            m_CurrentPane = pane;
            Q_EMIT windowTitleChanged();
        }
        catch (const QString& errorMessage)
        {
            _launchErrorPane(errorMessage);
        }
    }
}

void GamePresenter::handleResultsRequest()
{
    try {
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
        Q_EMIT mixedWordsPiecesChanged();
    }
    catch (const QString& errorMessage)
    {
        _launchErrorPane(errorMessage);
    }
}

bool GamePresenter::handleSubmitRequest(const QString &firstWord, const QString &secondWord)
{
    try
    {
        bool clearTextFields{false};

        Game::StatusCodes statusCode{_checkWords(firstWord, secondWord)};

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
            Q_EMIT mixedWordsPiecesChanged();

            clearTextFields = true;
        }

        return clearTextFields;
    }
    catch(const QString& errorMessage)
    {
        _launchErrorPane(errorMessage);
    }
}

void GamePresenter::handleResetRequest()
{
    if (m_ResetEnabled)
    {
        qDebug() << "=====================================================";
        qDebug() << "Statistics reset! Same mixed words to be guessed by user";

        m_pScoreItem -> resetStatistics();
        _updateStatusMessage(Game::StatusCodes::STATISTICS_RESET);
        m_ResetEnabled = false;
        Q_EMIT resetEnabledChanged();
    }
}

void GamePresenter::switchToLevel(int level)
{
    try
    {
        Q_ASSERT(level >= 0 && level < static_cast<int>(Game::Level::NrOfLevels));

        qDebug() << "=====================================================";
        qDebug() << "Level changed! New words mixed";

        _setLevel(static_cast<Game::Level>(level));
    }
    catch (const QString& errorMessage)
    {
        _launchErrorPane(errorMessage);
    }
}

QString GamePresenter::getWindowTitle() const
{
    QString windowTitle;
    switch(m_CurrentPane)
    {
    case Pane::INTRO:
        windowTitle = GameStrings::c_IntroWindowTitle;
        break;
    case Pane::HELP:
        windowTitle = GameStrings::c_HelpWindowTitle;
        break;
    case Pane::MAIN:
        windowTitle = GameStrings::c_MainWindowTitle;
        break;
    //reserved for future use
    default:
        ;
    }
    return windowTitle;
}

int GamePresenter::getToolTipDelay() const
{
    return c_ToolTipDelay;
}

QList<QVariant> GamePresenter::getMixedWordsPieces() const
{
    QList<QVariant> mixedWordsPieces;
    QVector<QString> mixedWordsStringArray{m_pWordMixer->getMixedWordsStringArray()};
    for (auto piece : mixedWordsStringArray)
    {
        mixedWordsPieces.append(piece);
    }
    return mixedWordsPieces;
}

int GamePresenter::getFirstWordBeginIndex() const
{
    return m_pWordMixer->getFirstWordBeginIndex();
}

int GamePresenter::getFirstWordEndIndex() const
{
    return m_pWordMixer->getFirstWordEndIndex();
}

int GamePresenter::getSecondWordBeginIndex() const
{
    return m_pWordMixer->getSecondWordBeginIndex();
}

int GamePresenter::getSecondWordEndIndex() const
{
    return m_pWordMixer->getSecondWordEndIndex();
}

void GamePresenter::_onStatisticsUpdated()
{
    m_MainPaneScoreMessage = GameStrings::c_HighscoresMessage.arg(m_pScoreItem->getObtainedScore())
                                                             .arg(m_pScoreItem->getTotalAvailableScore());
    m_MainPaneWordPairsMessage = GameStrings::c_WordPairsMessage.arg(m_pScoreItem->getGuessedWordPairs())
                                                                .arg(m_pScoreItem->getTotalWordPairs());

    Q_EMIT mainPaneScoreMessageChanged();
    Q_EMIT mainPaneWordPairsMessageChanged();
}

Game::StatusCodes GamePresenter::_checkWords(const QString &firstWord, const QString &secondWord)
{
    Game::StatusCodes statusCode;

    const QString firstWordRef{m_pWordMixer->getFirstWord()};
    const QString secondWordRef{m_pWordMixer->getSecondWord()};

    if (firstWord.isEmpty() || secondWord.isEmpty())
    {
        statusCode = Game::StatusCodes::MISSING_WORDS;
    }

    else if (((firstWord == firstWordRef) && (secondWord == secondWordRef)) || ((firstWord == secondWordRef) && (secondWord == firstWordRef)))
    {
        statusCode = Game::StatusCodes::SUCCESS;
    }
    else
    {
        statusCode = Game::StatusCodes::INCORRECT_WORDS;
    }
    return statusCode;
}

void GamePresenter::_initMainPane()
{

    qDebug() << "=====================================================";
    qDebug() << "Main window initialized! First 2 words mixed";

    m_MainPaneInitialized = true;
    m_pWordMixer -> mixWords();
    Q_EMIT mixedWordsPiecesChanged();
}

void GamePresenter::_updateStatusMessage(Game::StatusCodes statusCode)
{
    switch (statusCode)
    {
    case Game::StatusCodes::SUCCESS:
        m_MainPaneStatusMessage = GameStrings::c_SuccessMessage.arg(m_pWordMixer->getFirstWord())
                                                               .arg(m_pWordMixer->getSecondWord())
                                                               .arg(m_pWordMixer->areSynonyms() ? GameStrings::c_Synonyms : GameStrings::c_Antonyms);
        break;
    case Game::StatusCodes::MISSING_WORDS:
        m_MainPaneStatusMessage = GameStrings::c_MissingWordsMessage;
        break;
    case Game::StatusCodes::INCORRECT_WORDS:
        m_MainPaneStatusMessage = GameStrings::c_IncorrectWordsMessage;
        break;
    case Game::StatusCodes::REQUESTED_BY_USER:
        m_MainPaneStatusMessage = GameStrings::c_RequestedByUserMessage.arg(m_pWordMixer->getFirstWord())
                                                                       .arg(m_pWordMixer->getSecondWord())
                                                                       .arg(m_pWordMixer->areSynonyms() ? GameStrings::c_Synonyms : GameStrings::c_Antonyms);
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
    Q_EMIT mixedWordsPiecesChanged();
}

void GamePresenter::_launchErrorPane(const QString& errorMessage)
{
    qDebug() << "Error message: " << errorMessage;

    switch (m_CurrentPane) {
    case Pane::INTRO:
        m_IntroPaneVisible = false;
        Q_EMIT introPaneVisibleChanged();
        break;
    case Pane::HELP:
        m_HelpPaneVisible = false;
        Q_EMIT helpPaneVisibleChanged();
        break;
    case Pane::MAIN:
        m_MainPaneVisible = false;
        Q_EMIT mainPaneVisibleChanged();
        break;
    default:
        Q_ASSERT(static_cast<int>(m_CurrentPane) >= 0 && static_cast<int>(m_CurrentPane) < static_cast<int>(Pane::Nr_Of_Panes));
    }

    m_CurrentPane = Pane::ERROR;
    m_ErrorMessage = errorMessage;
    m_ErrorOccured = true;

    Q_EMIT errorMessageChanged();
    Q_EMIT errorOccuredChanged();
}
