#ifndef GAMEPRESENTER_H
#define GAMEPRESENTER_H

#include <QObject>
#include <QVariant>
#include <QVector>
#include <QColor>
#include <QTimer>

#include "gameutils.h"

class GameFacade;
class GameProxy;

class DataEntryPresenter;

class GamePresenter : public QObject
{
    Q_OBJECT

    /* current pane and data entry presenter */
    Q_PROPERTY(Panes currentPane READ getCurrentPane WRITE setCurrentPane NOTIFY currentPaneChanged)
    Q_PROPERTY(QObject* dataEntry READ getDataEntryPresenter CONSTANT)

    /* functionality enabling properties */
    Q_PROPERTY(bool playEnabled READ isPlayEnabled NOTIFY playEnabledChanged)
    Q_PROPERTY(bool languageSelectionEnabled READ isLanguageSelectionEnabled NOTIFY languageSelectionEnabledChanged)
    Q_PROPERTY(bool submitMainPaneInputEnabled READ getSubmitMainPaneInputEnabled NOTIFY submitMainPaneInputEnabledChanged)
    Q_PROPERTY(bool clearMainPaneInputEnabled READ getClearMainPaneInputEnabled NOTIFY clearMainPaneInputEnabledChanged)
    Q_PROPERTY(bool mainPaneStatisticsResetEnabled READ getMainPaneStatisticsResetEnabled NOTIFY mainPaneStatisticsResetEnabledChanged)
    Q_PROPERTY(bool persistentModeEnabled READ isPersistentModeEnabled NOTIFY persistentModeEnabledChanged)
    Q_PROPERTY(bool timeLimitEnabled READ isTimeLimitEnabled NOTIFY timeLimitEnabledChanged)

    /* game and user input properties */
    Q_PROPERTY(QList<QVariant> mixedWordsPiecesContent READ getMixedWordsPiecesContent NOTIFY mixedWordsChanged)
    Q_PROPERTY(QList<QVariant> mixedWordsPiecesTextColors READ getMixedWordsPiecesTextColors NOTIFY mixedWordsChanged)
    Q_PROPERTY(QList<QVariant> mixedWordsPiecesSelections READ getMixedWordsPiecesSelections NOTIFY selectionChanged)
    Q_PROPERTY(QList<QVariant> firstWordInputPiecesContent READ getFirstWordInputPiecesContent NOTIFY inputChanged)
    Q_PROPERTY(QList<QVariant> firstWordInputPiecesTextColors READ getFirstWordInputPiecesTextColors NOTIFY inputChanged)
    Q_PROPERTY(int languageIndex READ getLanguageIndex NOTIFY languageChanged)
    Q_PROPERTY(QStringList remainingTimeMinSec READ getRemainingTime NOTIFY remainingTimeChanged)
    Q_PROPERTY(bool areFirstWordInputPiecesHovered READ getAreFirstWordInputPiecesHovered NOTIFY hoverChanged)
    Q_PROPERTY(int firstWordInputPiecesHoverIndex READ getFirstWordInputPiecesHoverIndex NOTIFY hoverChanged)
    Q_PROPERTY(QList<QVariant> secondWordInputPiecesContent READ getSecondWordInputPiecesContent NOTIFY inputChanged)
    Q_PROPERTY(QList<QVariant> secondWordInputPiecesTextColors READ getSecondWordInputPiecesTextColors NOTIFY inputChanged)
    Q_PROPERTY(bool areSecondWordInputPiecesHovered READ getAreSecondWordInputPiecesHovered NOTIFY hoverChanged)
    Q_PROPERTY(int secondWordInputPiecesHoverIndex READ getSecondWordInputPiecesHoverIndex NOTIFY hoverChanged)
    Q_PROPERTY(int pieceSelectionCursorPosition READ getPieceSelectionCursorPosition NOTIFY pieceSelectionCursorPositionChanged)
    Q_PROPERTY(int piecesRemovalFirstWordCursorPosition READ getPiecesRemovalFirstWordCursorPosition NOTIFY piecesRemovalCursorPositionChanged)
    Q_PROPERTY(int piecesRemovalSecondWordCursorPosition READ getPiecesRemovalSecondWordCursorPosition NOTIFY piecesRemovalCursorPositionChanged)

    /* text properties */
    Q_PROPERTY(QString windowTitle READ getWindowTitle NOTIFY currentPaneChanged)
    Q_PROPERTY(QString introPaneMessage READ getIntroPaneMessage NOTIFY introPaneMessageChanged)
    Q_PROPERTY(QString helpPaneMessage READ getHelpPaneMessage NOTIFY helpPaneContentChanged)
    Q_PROPERTY(QString helpPaneToolTip READ getHelpPaneToolTip NOTIFY helpPaneContentChanged)
    Q_PROPERTY(QString helpButtonToolTip READ getHelpButtonToolTip NOTIFY currentPaneChanged)
    Q_PROPERTY(QString mainPaneStatusMessage READ getMainPaneStatusMessage NOTIFY mainPaneStatusMessageChanged)
    Q_PROPERTY(QString obtainedScore READ getObtainedScore NOTIFY mainPaneStatisticsChanged)
    Q_PROPERTY(QString totalAvailableScore READ getTotalAvailableScore NOTIFY mainPaneStatisticsChanged)
    Q_PROPERTY(QString guessedWordPairs READ getGuessedWordPairs NOTIFY mainPaneStatisticsChanged)
    Q_PROPERTY(QString totalWordPairs READ getTotalWordPairs NOTIFY mainPaneStatisticsChanged)
    Q_PROPERTY(QString errorMessage READ getErrorMessage NOTIFY errorMessageChanged)

    /* miscellaneous */
    Q_PROPERTY(bool dataFetchingInProgress READ getDataFetchingInProgress NOTIFY dataFetchingInProgressChanged)
    Q_PROPERTY(bool errorOccured READ getErrorOccured NOTIFY errorOccuredChanged)
    Q_PROPERTY(bool quitGameDeferred READ getQuitGameDeferred WRITE setQuitGameDeferred NOTIFY quitGameDeferredChanged)

public:
    enum class Panes
    {
        INTRO_PANE,
        HELP_PANE,
        MAIN_PANE,
        DATA_ENTRY_PANE,
        PROMPT_SAVE_EXIT_PANE,
        PROMPT_DISCARD_PANE,
        ERROR_PANE,
        NrOfPanes
    };
    Q_ENUM(Panes)

    // need this enum class as Game::Levels cannot be registered for QML usage (doesn't belong to presenter)
    enum class Levels
    {
        LEVEL_EASY,
        LEVEL_MEDIUM,
        LEVEL_HARD,
        LEVEL_NONE
    };
    Q_ENUM(Levels)

    explicit GamePresenter(QObject *parent = nullptr);

    void setCurrentPane(Panes pane);

    Q_INVOKABLE void goBack();
    Q_INVOKABLE void promptForSavingAddedWordPairs();
    Q_INVOKABLE void promptForDiscardingAddedWordPairs();
    Q_INVOKABLE void handleDisplayCorrectWordsPairRequest();
    Q_INVOKABLE void handleSubmitMainPaneInputRequest();
    Q_INVOKABLE void handleMainPaneStatisticsResetRequest();
    Q_INVOKABLE void setLevel(Levels level);
    Q_INVOKABLE void handleLanguageChangeRequest(int newLanguageIndex, bool revertLanguageWhenDataUnavailable = false);
    Q_INVOKABLE void enableCursor();
    Q_INVOKABLE void disableCursor();
    Q_INVOKABLE void moveCursorToNextContainer();
    Q_INVOKABLE void moveCursorLeft();
    Q_INVOKABLE void moveCursorRight();
    Q_INVOKABLE void executeFirstCursorAction();
    Q_INVOKABLE void executeSecondCursorAction();
    Q_INVOKABLE void setTimeLimitEnabled(bool enabled);
    Q_INVOKABLE void selectPieceForFirstInputWord(int wordPieceIndex);
    Q_INVOKABLE void selectPieceForSecondInputWord(int wordPieceIndex);
    Q_INVOKABLE void removePiecesFromFirstInputWord(int inputRangeStart);
    Q_INVOKABLE void removePiecesFromSecondInputWord(int inputRangeStart);
    Q_INVOKABLE void clearMainPaneInput();
    Q_INVOKABLE void clearMainPaneFirstInputWord();
    Q_INVOKABLE void clearMainPaneSecondInputWord();
    Q_INVOKABLE void updateFirstWordInputHoverIndex(int index);
    Q_INVOKABLE void updateSecondWordInputHoverIndex(int index);
    Q_INVOKABLE void clearWordInputHoverIndexes();
    Q_INVOKABLE void quit();
    Q_INVOKABLE void handleSaveAndQuit();

    QObject* getDataEntryPresenter() const;

    Panes getCurrentPane() const;

    bool isLanguageSelectionEnabled() const;
    bool isPlayEnabled() const;
    bool getMainPaneStatisticsResetEnabled() const;
    bool isPersistentModeEnabled() const;
    bool isTimeLimitEnabled() const;
    bool getClearMainPaneInputEnabled() const;
    bool getSubmitMainPaneInputEnabled() const;
    bool getDataFetchingInProgress() const;
    bool getErrorOccured() const;
    bool getQuitGameDeferred() const;

    void setQuitGameDeferred(bool deferred);

    QList<QVariant> getMixedWordsPiecesContent() const;
    QList<QVariant> getMixedWordsPiecesTextColors() const;
    QList<QVariant> getMixedWordsPiecesSelections() const;
    QList<QVariant> getFirstWordInputPiecesContent() const;
    QList<QVariant> getFirstWordInputPiecesTextColors() const;
    bool getAreFirstWordInputPiecesHovered() const;
    int getFirstWordInputPiecesHoverIndex() const;
    QList<QVariant> getSecondWordInputPiecesContent() const;
    QList<QVariant> getSecondWordInputPiecesTextColors() const;
    bool getAreSecondWordInputPiecesHovered() const;
    int getSecondWordInputPiecesHoverIndex() const;
    int getPieceSelectionCursorPosition() const;
    int getPiecesRemovalFirstWordCursorPosition() const;
    int getPiecesRemovalSecondWordCursorPosition() const;

    int getLanguageIndex() const;

    QStringList getRemainingTime() const;

    QString getWindowTitle() const;

    QString getIntroPaneMessage() const;
    QString getHelpPaneMessage() const;
    QString getHelpPaneToolTip() const;
    QString getHelpButtonToolTip() const;
    QString getMainPaneStatusMessage() const;
    QString getObtainedScore() const;
    QString getTotalAvailableScore() const;
    QString getGuessedWordPairs() const;
    QString getTotalWordPairs() const;
    QString getErrorMessage() const;

    virtual ~GamePresenter();

signals:
    Q_SIGNAL void currentPaneChanged();
    Q_SIGNAL void languageSelectionEnabledChanged();
    Q_SIGNAL void languageChanged();
    Q_SIGNAL void remainingTimeChanged();
    Q_SIGNAL void playEnabledChanged();
    Q_SIGNAL void mainPaneStatisticsResetEnabledChanged();
    Q_SIGNAL void persistentModeEnabledChanged();
    Q_SIGNAL void timeLimitEnabledChanged();
    Q_SIGNAL void clearMainPaneInputEnabledChanged();
    Q_SIGNAL void submitMainPaneInputEnabledChanged();
    Q_SIGNAL void errorOccuredChanged();
    Q_SIGNAL void quitGameDeferredChanged();
    Q_SIGNAL void mixedWordsChanged();
    Q_SIGNAL void inputChanged();
    Q_SIGNAL void selectionChanged();
    Q_SIGNAL void introPaneMessageChanged();
    Q_SIGNAL void helpPaneContentChanged();
    Q_SIGNAL void mainPaneStatusMessageChanged();
    Q_SIGNAL void mainPaneStatisticsChanged();
    Q_SIGNAL void errorMessageChanged();
    Q_SIGNAL void hoverChanged();
    Q_SIGNAL void pieceSelectionCursorPositionChanged();
    Q_SIGNAL void piecesRemovalCursorPositionChanged();
    Q_SIGNAL void dataFetchingInProgressChanged();

private slots:
    void _onInputChanged();
    void _onStatisticsChanged();
    void _onDataSaveInProgress();
    void _onStatusChanged();

private:
    bool _canResetGameStatistics() const;
    void _switchToPane(Panes pane);
    void _setDataEntryHelpMenuActive(bool isDataEntryHelpMenuRequired);
    void _updateStatusMessage(const QString& message, Panes pane, int delay);
    void _updateMessage();
    void _launchErrorPane(const QString& errorMessage);

    static const QMap<GamePresenter::Panes, QString> sc_WindowTitles;
    static const QMap<Game::PieceTypes, QColor> sc_WordPieceTextColors;

    static constexpr int sc_PaneSwitchingDelay{350};
    static constexpr int sc_GameQuitDelay{200};

    QObject* m_pDataEntryPresenter;

    bool m_MainPaneInitialized;
    bool m_IsDataEntryHelpMenuActive;

    bool m_MainPaneStatisticsResetEnabled;
    bool m_ClearMainPaneInputEnabled;
    bool m_ErrorOccured;
    bool m_QuitDeferred;

    QString m_WindowTitle;
    QString m_IntroPaneMessage;
    QString m_MainPaneStatusMessage;
    QString m_ErrorMessage;
    QString m_CurrentStatusMessage;

    Panes m_CurrentPane;
    Panes m_StatusUpdatePane;
    QVector<Panes> m_PreviousPanesStack;

    int m_FirstWordInputPiecesHoverIndex;
    int m_SecondWordInputPiecesHoverIndex;

    GameFacade* m_pGameFacade;
    GameProxy* m_pGameProxy;

    QTimer* m_pStatusUpdateTimer;
};

#endif // GAMEPRESENTER_H
