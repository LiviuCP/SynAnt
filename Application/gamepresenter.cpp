#include <QGuiApplication>
#include <QVector>

#include "gamepresenter.h"
#include "../Common/gamefacade.h"
#include "../Common/gamestrings.h"
#include "../Common/exceptions.h"

static const QMap<GamePresenter::Pane, QString> c_WindowTitles
{
    {GamePresenter::Pane::INTRO, GameStrings::c_IntroWindowTitle},
    {GamePresenter::Pane::HELP, GameStrings::c_HelpWindowTitle},
    {GamePresenter::Pane::MAIN, GameStrings::c_MainWindowTitle},
    {GamePresenter::Pane::ERROR, GameStrings::c_FatalErrorWindowTitle}
};

static const QMap<Game::PieceTypes, QColor> c_WordPieceTextColors
{
    {Game::PieceTypes::BEGIN_PIECE, GameStrings::c_WordFirstPieceTextColor},
    {Game::PieceTypes::MIDDLE_PIECE, GameStrings::c_WordMiddlePieceTextColor},
    {Game::PieceTypes::END_PIECE, GameStrings::c_WordLastPieceTextColor}
};

GamePresenter::GamePresenter(QObject *parent)
    : QObject(parent)
    , m_IntroPaneVisible {true}
    , m_HelpPaneVisible {false}
    , m_MainPaneVisible {false}
    , m_MainPaneInitialized {false}
    , m_StatisticsResetEnabled {false}
    , m_ErrorOccured {false}
    , m_WindowTitle{GameStrings::c_IntroWindowTitle}
    , m_CurrentPane {Pane::INTRO}
    , m_FirstWordInputHoverIndex{-1}
    , m_SecondWordInputHoverIndex{-1}
    , m_pGameFacade {new GameFacade{QGuiApplication::applicationDirPath(), this}}
{
    Q_ASSERT(m_pGameFacade);

    bool connected{connect(m_pGameFacade, &GameFacade::statisticsChanged, this, &GamePresenter::_onStatisticsChanged)};
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::statusChanged, this, &GamePresenter::_onStatusChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::mixedWordsChanged, this, &GamePresenter::mixedWordsChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::inputChanged, this, &GamePresenter::_onInputChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::selectionChanged, this, &GamePresenter::selectionChanged);
    Q_ASSERT(connected);
    connected = connect(m_pGameFacade, &GameFacade::completionChanged, this, &GamePresenter::submitEnabledChanged);
    Q_ASSERT(connected);
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
                    m_MainPaneInitialized = true;
                    m_pGameFacade -> startGame();
                }
                else
                {
                    m_pGameFacade -> resumeGame();
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
        catch (const GameException& exception)
        {
            _launchErrorPane(exception.getDescription());
        }
    }
}

void GamePresenter::handleResultsRequest()
{
    try
    {
        m_pGameFacade->provideResultsToUser();
    }
    catch (const GameException& exception)
    {
        _launchErrorPane(exception.getDescription());
    }
}

void GamePresenter::handleSubmitRequest()
{
    try
    {
        m_pGameFacade->handleSubmitRequest();
    }
    catch(const GameException& exception)
    {
        _launchErrorPane(exception.getDescription());
    }
}

void GamePresenter::handleResetRequest()
{
    m_pGameFacade -> resetStatistics();
}

void GamePresenter::switchToLevel(int level)
{
    try
    {
        Q_ASSERT(level >= 0 && level < static_cast<int>(Game::Level::NrOfLevels));
        m_pGameFacade->setLevel(static_cast<Game::Level>(level));
    }
    catch (const GameException& exception)
    {
        _launchErrorPane(exception.getDescription());
    }
}

void GamePresenter::selectWordPieceForFirstInputWord(int wordPieceIndex)
{
    m_pGameFacade->selectWordPieceForFirstInputWord(wordPieceIndex);
}

void GamePresenter::selectWordPieceForSecondInputWord(int wordPieceIndex)
{
    m_pGameFacade->selectWordPieceForSecondInputWord(wordPieceIndex);
}

void GamePresenter::removeWordPiecesFromFirstInputWord(int inputRangeStart)
{
    m_pGameFacade->removeWordPiecesFromFirstInputWord(inputRangeStart);
}

void GamePresenter::removeWordPiecesFromSecondInputWord(int inputRangeStart)
{
    m_pGameFacade->removeWordPiecesFromSecondInputWord(inputRangeStart);
}

void GamePresenter::updateFirstWordInputHoverIndex(int index)
{
    m_FirstWordInputHoverIndex = index;
    Q_EMIT hoverChanged();
}

void GamePresenter::updateSecondWordInputHoverIndex(int index)
{
    m_SecondWordInputHoverIndex = index;
    Q_EMIT hoverChanged();
}

void GamePresenter::clearWordInputHoverIndexes()
{
    m_FirstWordInputHoverIndex = -1;
    m_SecondWordInputHoverIndex = -1;

    Q_EMIT hoverChanged();
}

bool GamePresenter::getIntroPaneVisible() const
{
    return m_IntroPaneVisible;
}

bool GamePresenter::getHelpPaneVisible() const
{
    return m_HelpPaneVisible;
}

bool GamePresenter::getMainPaneVisible() const
{
    return m_MainPaneVisible;
}

bool GamePresenter::getResetEnabled() const
{
    return m_StatisticsResetEnabled;
}

bool GamePresenter::getSubmitEnabled() const
{
    return m_pGameFacade->isInputComplete();
}

bool GamePresenter::getErrorOccured() const
{
    return m_ErrorOccured;
}

QList<QVariant> GamePresenter::getMixedWordsPiecesContent() const
{
    QList<QVariant> mixedWordsPiecesContent;

    for (auto piece : m_pGameFacade->getMixedWordsPieces())
    {
        mixedWordsPiecesContent.append(piece.content);
    }

    return mixedWordsPiecesContent;
}

QList<QVariant> GamePresenter::getMixedWordsPiecesTextColors() const
{
    QList<QVariant> mixedWordsPiecesTextColors;

    for (auto piece : m_pGameFacade->getMixedWordsPieces())
    {
        mixedWordsPiecesTextColors.append(c_WordPieceTextColors[piece.pieceType]);
    }

    return mixedWordsPiecesTextColors;
}

QList<QVariant> GamePresenter::getMixedWordsPiecesSelections() const
{
    QList<QVariant> mixedWordsPiecesSelections;

    for (auto piece : m_pGameFacade->getMixedWordsPieces())
    {
        mixedWordsPiecesSelections.append(piece.isSelected);
    }

    return mixedWordsPiecesSelections;
}

QList<QVariant> GamePresenter::getFirstWordInputPiecesContent() const
{
    QList<QVariant> firstWordInputPiecesContent;

    for (auto index : m_pGameFacade->getFirstWordInputIndexes())
    {
        firstWordInputPiecesContent.append(m_pGameFacade->getMixedWordsPieces().at(index).content);
    }

    return firstWordInputPiecesContent;
}

QList<QVariant> GamePresenter::getFirstWordInputPiecesTextColors() const
{
    QList<QVariant> firstWordInputPiecesTextColors;

    for (auto index : m_pGameFacade->getFirstWordInputIndexes())
    {
        firstWordInputPiecesTextColors.append(c_WordPieceTextColors[m_pGameFacade->getMixedWordsPieces().at(index).pieceType]);
    }

    return firstWordInputPiecesTextColors;
}

int GamePresenter::getFirstWordInputHoverIndex() const
{
    return m_FirstWordInputHoverIndex;
}

bool GamePresenter::getIsFirstWordInputHovered() const
{
    return (m_FirstWordInputHoverIndex != -1);
}

QList<QVariant> GamePresenter::getSecondWordInputPiecesContent() const
{
    QList<QVariant> secondWordInputPiecesContent;

    for (auto index : m_pGameFacade->getSecondWordInputIndexes())
    {
        secondWordInputPiecesContent.append(m_pGameFacade->getMixedWordsPieces().at(index).content);
    }

    return secondWordInputPiecesContent;
}

QList<QVariant> GamePresenter::getSecondWordInputPiecesTextColors() const
{
    QList<QVariant> secondWordInputPiecesTextColors;

    for (auto index : m_pGameFacade->getSecondWordInputIndexes())
    {
        secondWordInputPiecesTextColors.append(c_WordPieceTextColors[m_pGameFacade->getMixedWordsPieces().at(index).pieceType]);
    }

    return secondWordInputPiecesTextColors;
}

bool GamePresenter::getIsSecondWordInputHovered() const
{
    return (m_SecondWordInputHoverIndex != -1);
}

int GamePresenter::getSecondWordInputHoverIndex() const
{
    return m_SecondWordInputHoverIndex;
}

int GamePresenter::getLevelEasy() const
{
    return static_cast<int>(Game::Level::EASY);
}

int GamePresenter::getLevelMedium() const
{
    return static_cast<int>(Game::Level::MEDIUM);
}

int GamePresenter::getLevelHard() const
{
    return static_cast<int>(Game::Level::HARD);
}

int GamePresenter::getToolTipDelay() const
{
    return Game::c_ToolTipDelay;
}

int GamePresenter::getToolTipTimeout() const
{
    return Game::c_ToolTipTimeout;
}

QString GamePresenter::getWindowTitle() const
{
    return c_WindowTitles[m_CurrentPane];
}

QString GamePresenter::getIntroPaneMessage() const
{
    return GameStrings::c_IntroWindowWelcomeMessage;
}

QString GamePresenter::getHelpPaneMessage() const
{
    return GameStrings::c_HelpWindowMessage;
}

QString GamePresenter::getMainPaneInstructionsMessage() const
{
    return GameStrings::c_InstructionsMessage;
}

QString GamePresenter::getMainPaneStatusMessage() const
{
    return m_MainPaneStatusMessage;
}

QString GamePresenter::getMainPaneScoreMessage() const
{
    return m_MainPaneScoreMessage;
}

QString GamePresenter::getMainPaneWordPairsMessage() const
{
    return m_MainPaneWordPairsMessage;
}

QString GamePresenter::getErrorMessage() const
{
    return m_ErrorMessage;
}

QString GamePresenter::getPlayButtonLabel() const
{
    return GameStrings::c_PlayButtonLabel;
}

QString GamePresenter::getHelpButtonLabel() const
{
    return GameStrings::c_HelpButtonLabel;
}

QString GamePresenter::getQuitButtonLabel() const
{
    return GameStrings::c_QuitButtonLabel;
}

QString GamePresenter::getSubmitButtonLabel() const
{
    return GameStrings::c_SubmitButtonLabel;
}

QString GamePresenter::getResultsButtonLabel() const
{
    return GameStrings::c_ResultsButtonLabel;
}

QString GamePresenter::getResetButtonLabel() const
{
    return GameStrings::c_ResetButtonLabel;
}

QString GamePresenter::getLevelEasyButtonLabel() const
{
    return GameStrings::c_LevelEasyButtonLabel;
}

QString GamePresenter::getLevelMediumButtonLabel() const
{
    return GameStrings::c_LevelMediumButtonLabel;
}

QString GamePresenter::getLevelHardButtonLabel() const
{
    return GameStrings::c_LevelHardButtonLabel;
}

QString GamePresenter::getOkButtonLabel() const
{
    return GameStrings::c_OkButtonLabel;
}

QString GamePresenter::getCloseButtonLabel() const
{
    return GameStrings::c_FatalErrorQuitButtonLabel;
}

QString GamePresenter::getPlayButtonShortcut() const
{
    return GameStrings::c_PlayButtonShortcut;
}

QString GamePresenter::getHelpButtonShortcut() const
{
    return GameStrings::c_HelpButtonShortcut;
}

QString GamePresenter::getQuitButtonShortcut() const
{
    return GameStrings::c_QuitButtonShortcut;
}

QString GamePresenter::getSubmitButtonShortcut() const
{
    return GameStrings::c_SubmitButtonShortcut;
}

QString GamePresenter::getResultsButtonShortcut() const
{
    return GameStrings::c_ResultsButtonShortcut;
}

QString GamePresenter::getResetButtonShortcut() const
{
    return GameStrings::c_ResetShortcut;
}

QString GamePresenter::getLevelEasyButtonShortcut() const
{
    return GameStrings::c_LevelEasyButtonShortcut;
}

QString GamePresenter::getLevelMediumButtonShortcut() const
{
    return GameStrings::c_LevelMediumButtonShortcut;
}

QString GamePresenter::getLevelHardButtonShortcut() const
{
    return GameStrings::c_LevelHardButtonShortcut;
}

QString GamePresenter::getOkButtonShortcut() const
{
    return GameStrings::c_OkButtonShortcut;
}

QString GamePresenter::getCloseButtonShortcut() const
{
    return GameStrings::c_FatalErrorQuitButtonShortcut;
}

QString GamePresenter::getIntroPaneToolTip() const
{
    return GameStrings::c_IntroWindowToolTip;
}

QString GamePresenter::getHelpPaneToolTip() const
{
    return GameStrings::c_HelpWindowToolTip;
}

QString GamePresenter::getMainPaneToolTip() const
{
    return GameStrings::c_MainWindowToolTip;
}

QString GamePresenter::getErrorPaneToolTip() const
{
    return GameStrings::c_FatalErrorWindowToolTip;
}

QString GamePresenter::getHighscoresToolTip() const
{
    return GameStrings::c_HighscoresToolTip;
}

QString GamePresenter::getWordPairsToolTip() const
{
    return GameStrings::c_WordPairsToolTip;
}

QString GamePresenter::getGameInstructionsToolTip() const
{
    return GameStrings::c_GameInstructionsToolTip;
}

QString GamePresenter::getGameStatusToolTip() const
{
    return GameStrings::c_GameStatusToolTip;
}

QString GamePresenter::getSelectWordPieceToolTip() const
{
    return GameStrings::c_SelectWordPieceToolTip;
}

QString GamePresenter::getWordPieceAlreadySelectedToolTip() const
{
    return GameStrings::c_WordPieceAlreadySelectedToolTip;
}

QString GamePresenter::getFirstWordInputToolTip() const
{
    return GameStrings::c_FirstWordInputToolTip;
}

QString GamePresenter::getSecondWordInputToolTip() const
{
    return GameStrings::c_SecondWordInputToolTip;
}

QString GamePresenter::getPlayButtonToolTip() const
{
    return GameStrings::c_PlayButtonToolTip;
}

QString GamePresenter::getHelpButtonToolTip() const
{
    return GameStrings::c_HelpButtonToolTip;
}

QString GamePresenter::getQuitButtonToolTip() const
{
    return GameStrings::c_QuitButtonToolTip;
}

QString GamePresenter::getSubmitButtonToolTip() const
{
    return GameStrings::c_SubmitButtonToolTip;
}

QString GamePresenter::getResultsButtonToolTip() const
{
    return GameStrings::c_ResultsButtonToolTip;
}

QString GamePresenter::getResetButtonToolTip() const
{
    return GameStrings::c_ResetButtonToolTip;
}

QString GamePresenter::getLevelButtonsToolTip() const
{
    return GameStrings::c_LevelButtonsToolTip;
}

QString GamePresenter::getOkButtonToolTip() const
{
    return GameStrings::c_OkButtonToolTip;
}

QString GamePresenter::getCloseButtonToolTip() const
{
    return GameStrings::c_FatalErrorQuitButtonToolTip;
}

QColor GamePresenter::getBackgroundColor() const
{
    return QColor{GameStrings::c_BackgroundColor};
}

QColor GamePresenter::getBackgroundSelectedColor() const
{
    return QColor{GameStrings::c_BackgroundSelectedColor};
}

QColor GamePresenter::getPushButtonColor() const
{
    return QColor{GameStrings::c_PushButtonColor};
}

QColor GamePresenter::getBorderColor() const
{
    return QColor{GameStrings::c_BorderColor};
}

QColor GamePresenter::getTextColor() const
{
    return QColor{GameStrings::c_TextColor};
}

QColor GamePresenter::getFatalErrorTextColor() const
{
    return QColor{GameStrings::c_FatalErrorTextColor};
}

QColor GamePresenter::getWordPieceSelectedColor() const
{
    return QColor{GameStrings::c_WordPieceSelectedBackgroundColor};
}

void GamePresenter::_onInputChanged()
{
    clearWordInputHoverIndexes();
    Q_EMIT inputChanged();
}

void GamePresenter::_onStatisticsChanged()
{
    int obtainedScore{m_pGameFacade->getObtainedScore()};
    int totalAvailableScore{m_pGameFacade->getTotalAvailableScore()};
    int guessedWordPairs{m_pGameFacade->getGuessedWordPairs()};
    int totalWordPairs{m_pGameFacade->getTotalWordPairs()};

    bool emptyStatistics{obtainedScore == 0 && totalAvailableScore == 0 && guessedWordPairs == 0 && totalWordPairs == 0};

    if (!m_StatisticsResetEnabled && !emptyStatistics)
    {
        m_StatisticsResetEnabled = true;
        Q_EMIT resetEnabledChanged();
    }
    else if (m_StatisticsResetEnabled && emptyStatistics)
    {
        m_StatisticsResetEnabled = false;
        Q_EMIT resetEnabledChanged();
    }

    m_MainPaneScoreMessage = GameStrings::c_HighscoresMessage.arg(obtainedScore)
                                                             .arg(totalAvailableScore);
    m_MainPaneWordPairsMessage = GameStrings::c_WordPairsMessage.arg(guessedWordPairs)
                                                                .arg(totalWordPairs);

    Q_EMIT mainPaneStatisticsMessagesChanged();
}

void GamePresenter::_onStatusChanged(Game::StatusCodes statusCode)
{
    switch (statusCode)
    {
    case Game::StatusCodes::GAME_STARTED:
        m_MainPaneStatusMessage = GameStrings::c_GameStartedMessage;
        break;
    case Game::StatusCodes::GAME_RESUMED:
        m_MainPaneStatusMessage = GameStrings::c_GameResumedMessage;
        break;
    case Game::StatusCodes::SUCCESS:
        m_MainPaneStatusMessage = GameStrings::c_SuccessMessage.arg(m_pGameFacade->getFirstReferenceWord())
                                                               .arg(m_pGameFacade->getSecondReferenceWord())
                                                               .arg(m_pGameFacade->areSynonyms() ? GameStrings::c_Synonyms : GameStrings::c_Antonyms);
        break;
    case Game::StatusCodes::INCORRECT_WORDS:
        m_MainPaneStatusMessage = GameStrings::c_IncorrectWordsMessage;
        break;
    case Game::StatusCodes::REQUESTED_BY_USER:
        m_MainPaneStatusMessage = GameStrings::c_RequestedByUserMessage.arg(m_pGameFacade->getFirstReferenceWord())
                                                                       .arg(m_pGameFacade->getSecondReferenceWord())
                                                                       .arg(m_pGameFacade->areSynonyms() ? GameStrings::c_Synonyms : GameStrings::c_Antonyms);
        break;
    case Game::StatusCodes::STATISTICS_RESET:
        m_MainPaneStatusMessage = GameStrings::c_ScoresResetMessage;
        break;
    case Game::StatusCodes::LEVEL_CHANGED:
        m_MainPaneStatusMessage = GameStrings::c_LevelChangedMessage;
        break;
    case Game::StatusCodes::PIECE_ALREADY_ADDED:
        m_MainPaneStatusMessage = GameStrings::c_PieceAlreadyAddedMessage;
        break;
    case Game::StatusCodes::PIECE_NOT_ADDED:
        m_MainPaneStatusMessage = GameStrings::c_PieceNotAddedMessage;
        break;
    case Game::StatusCodes::PIECE_SUCCESSFULLY_ADDED:
        m_MainPaneStatusMessage = GameStrings::c_PieceSuccessfullyAddedMessage;
        break;
    case Game::StatusCodes::PIECES_REMOVED:
        m_MainPaneStatusMessage = GameStrings::c_PiecesRemovedMessage;
        break;
    case Game::StatusCodes::ALL_PIECES_SELECTED:
        m_MainPaneStatusMessage = GameStrings::c_AllPiecesAddedMessage;
        break;
    default:
        m_MainPaneStatusMessage = GameStrings::c_DefaultStatusMessage;
    }

    Q_EMIT mainPaneStatusMessageChanged();
}

void GamePresenter::_launchErrorPane(const QString& errorMessage)
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
    Q_EMIT windowTitleChanged();
}
