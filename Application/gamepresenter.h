#ifndef GAMEPRESENTER_H
#define GAMEPRESENTER_H

#include <QObject>
#include <QVariant>
#include <QVector>
#include <QColor>
#include <QTimer>

#include "../SystemFunctionality/Utilities/gameutils.h"

class GameFacade;
class GameProxy;

class DataEntryPresenter;

class GamePresenter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObject* dataEntry READ getDataEntryPresenter CONSTANT)

    /* pane visibility properties */
    Q_PROPERTY(bool introPaneVisible READ getIntroPaneVisible NOTIFY currentPaneChanged)
    Q_PROPERTY(bool helpPaneVisible READ getHelpPaneVisible NOTIFY currentPaneChanged)
    Q_PROPERTY(bool mainPaneVisible READ getMainPaneVisible NOTIFY currentPaneChanged)
    Q_PROPERTY(bool dataEntryPaneVisible READ getDataEntryPaneVisible NOTIFY currentPaneChanged)
    Q_PROPERTY(bool promptSaveExitPaneVisible READ getPromptSaveExitPaneVisible NOTIFY currentPaneChanged)
    Q_PROPERTY(bool promptDiscardPaneVisible READ getPromptDiscardPaneVisible NOTIFY currentPaneChanged)

    /* functionality enabling properties */
    Q_PROPERTY(bool playEnabled READ isPlayEnabled NOTIFY playEnabledChanged)
    Q_PROPERTY(bool languageSelectionEnabled READ isLanguageSelectionEnabled NOTIFY languageSelectionEnabledChanged)
    Q_PROPERTY(bool submitMainPaneInputEnabled READ getSubmitMainPaneInputEnabled NOTIFY submitMainPaneInputEnabledChanged)
    Q_PROPERTY(bool clearMainPaneInputEnabled READ getClearMainPaneInputEnabled NOTIFY clearMainPaneInputEnabledChanged)
    Q_PROPERTY(bool mainPaneStatisticsResetEnabled READ getMainPaneStatisticsResetEnabled NOTIFY mainPaneStatisticsResetEnabledChanged)
    Q_PROPERTY(bool persistentModeEnabled READ isPersistentModeEnabled NOTIFY persistentModeEnabledChanged)

    /* game and user input properties */
    Q_PROPERTY(QList<QVariant> mixedWordsPiecesContent READ getMixedWordsPiecesContent NOTIFY mixedWordsChanged)
    Q_PROPERTY(QList<QVariant> mixedWordsPiecesTextColors READ getMixedWordsPiecesTextColors NOTIFY mixedWordsChanged)
    Q_PROPERTY(QList<QVariant> mixedWordsPiecesSelections READ getMixedWordsPiecesSelections NOTIFY selectionChanged)
    Q_PROPERTY(QList<QVariant> firstWordInputPiecesContent READ getFirstWordInputPiecesContent NOTIFY inputChanged)
    Q_PROPERTY(QList<QVariant> firstWordInputPiecesTextColors READ getFirstWordInputPiecesTextColors NOTIFY inputChanged)
    Q_PROPERTY(bool areFirstWordInputPiecesHovered READ getAreFirstWordInputPiecesHovered NOTIFY hoverChanged)
    Q_PROPERTY(int firstWordInputPiecesHoverIndex READ getFirstWordInputPiecesHoverIndex NOTIFY hoverChanged)
    Q_PROPERTY(QList<QVariant> secondWordInputPiecesContent READ getSecondWordInputPiecesContent NOTIFY inputChanged)
    Q_PROPERTY(QList<QVariant> secondWordInputPiecesTextColors READ getSecondWordInputPiecesTextColors NOTIFY inputChanged)
    Q_PROPERTY(bool areSecondWordInputPiecesHovered READ getAreSecondWordInputPiecesHovered NOTIFY hoverChanged)
    Q_PROPERTY(int secondWordInputPiecesHoverIndex READ getSecondWordInputPiecesHoverIndex NOTIFY hoverChanged)
    Q_PROPERTY(int pieceSelectionCursorPosition READ getPieceSelectionCursorPosition NOTIFY pieceSelectionCursorPositionChanged)
    Q_PROPERTY(int piecesRemovalFirstWordCursorPosition READ getPiecesRemovalFirstWordCursorPosition NOTIFY piecesRemovalCursorPositionChanged)
    Q_PROPERTY(int piecesRemovalSecondWordCursorPosition READ getPiecesRemovalSecondWordCursorPosition NOTIFY piecesRemovalCursorPositionChanged)
    // need to use these three properties as Q_ENUM cannot be used with the Level enum (it si not part of the presenter class)
    Q_PROPERTY(int levelEasy READ getLevelEasy CONSTANT)
    Q_PROPERTY(int levelMedium READ getLevelMedium CONSTANT)
    Q_PROPERTY(int levelHard READ getLevelHard CONSTANT)

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
    Q_PROPERTY(QStringList availableLanguages READ getAvailableLanguages NOTIFY languageSelectionHeaderChanged)

    /* miscellaneous */
    Q_PROPERTY(bool errorOccured READ getErrorOccured NOTIFY errorOccuredChanged)
    Q_PROPERTY(bool quitGameDeferred READ getQuitGameDeferred WRITE setQuitGameDeferred NOTIFY quitGameDeferredChanged)
    Q_PROPERTY(int toolTipDelay READ getToolTipDelay CONSTANT)
    Q_PROPERTY(int toolTipTimeout READ getToolTipTimeout CONSTANT)

public:
    enum class Pane
    {
        INTRO,
        HELP,
        MAIN,
        DATA_ENTRY,
        PROMPT_SAVE_EXIT,
        PROMPT_DISCARD,
        ERROR,
        Nr_Of_Panes
    };
    Q_ENUM(Pane)

    explicit GamePresenter(QObject *parent = nullptr);

    Q_INVOKABLE void switchToPane(Pane pane);
    Q_INVOKABLE void goBack();
    Q_INVOKABLE void promptForSavingAddedWordPairs();
    Q_INVOKABLE void promptForDiscardingAddedWordPairs();
    Q_INVOKABLE void handleDataSaveInProgress();
    Q_INVOKABLE void handleDisplayCorrectWordsPairRequest();
    Q_INVOKABLE void handleSubmitMainPaneInputRequest();
    Q_INVOKABLE void handleMainPaneStatisticsResetRequest();
    Q_INVOKABLE void switchToLevel(int level);
    Q_INVOKABLE void enableCursor();
    Q_INVOKABLE void disableCursor();
    Q_INVOKABLE void moveCursorToNextContainer();
    Q_INVOKABLE void moveCursorLeft();
    Q_INVOKABLE void moveCursorRight();
    Q_INVOKABLE void executeFirstCursorAction();
    Q_INVOKABLE void executeSecondCursorAction();
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

    QObject* getDataEntryPresenter() const;

    bool getIntroPaneVisible() const;
    bool getHelpPaneVisible() const;
    bool getMainPaneVisible() const;
    bool getDataEntryPaneVisible() const;
    bool getPromptSaveExitPaneVisible() const;
    bool getPromptDiscardPaneVisible() const;
    bool isLanguageSelectionEnabled() const;
    bool isPlayEnabled() const;
    bool getMainPaneStatisticsResetEnabled() const;
    bool isPersistentModeEnabled() const;
    bool getClearMainPaneInputEnabled() const;
    bool getSubmitMainPaneInputEnabled() const;
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

    int getLevelEasy() const;
    int getLevelMedium() const;
    int getLevelHard() const;
    int getToolTipDelay() const;
    int getToolTipTimeout() const;

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
    QStringList getAvailableLanguages() const;

    virtual ~GamePresenter();

signals:
    Q_SIGNAL void currentPaneChanged();
    Q_SIGNAL void languageSelectionEnabledChanged();
    Q_SIGNAL void languageSelectionHeaderChanged();
    Q_SIGNAL void playEnabledChanged();
    Q_SIGNAL void mainPaneStatisticsResetEnabledChanged();
    Q_SIGNAL void persistentModeEnabledChanged();
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

private slots:
    void _onInputChanged();
    void _onStatisticsChanged();
    void _onStatusChanged(Game::StatusCodes statusCode);

private:
    bool _canResetGameStatistics() const;
    void _switchToPane(Pane pane);
    void _setDataEntryHelpMenuActive(bool isDataEntryHelpMenuRequired);
    void _updateStatusMessage(const QString& message, Pane pane, int delay);
    void _updateMessage();
    void _launchErrorPane(const QString& errorMessage);

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

    Pane m_CurrentPane;
    Pane m_StatusUpdatePane;
    QVector<Pane> m_PreviousPanesStack;

    int m_FirstWordInputPiecesHoverIndex;
    int m_SecondWordInputPiecesHoverIndex;

    GameFacade* m_pGameFacade;
    GameProxy* m_pGameProxy;

    QTimer* m_pStatusUpdateTimer;
};

#endif // GAMEPRESENTER_H
