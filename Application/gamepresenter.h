#ifndef GAMEPRESENTER_H
#define GAMEPRESENTER_H

#include <QObject>
#include <QVariant>
#include <QVector>
#include <QColor>
#include <QTimer>

#include "../SystemFunctionality/Utilities/game.h"

class GameFacade;
class GameProxy;

class GamePresenter : public QObject
{
    Q_OBJECT

    /* pane visibility properties */
    Q_PROPERTY(bool introPaneVisible READ getIntroPaneVisible NOTIFY currentPaneChanged)
    Q_PROPERTY(bool helpPaneVisible READ getHelpPaneVisible NOTIFY currentPaneChanged)
    Q_PROPERTY(bool mainPaneVisible READ getMainPaneVisible NOTIFY currentPaneChanged)
    Q_PROPERTY(bool dataEntryPaneVisible READ getDataEntryPaneVisible NOTIFY currentPaneChanged)
    Q_PROPERTY(bool promptSaveExitPaneVisible READ getPromptSaveExitPaneVisible NOTIFY currentPaneChanged)

    /* functionality enabling properties */
    Q_PROPERTY(bool playEnabled READ isPlayEnabled NOTIFY playEnabledChanged)
    Q_PROPERTY(bool dataEntryEnabled READ isDataEntryEnabled NOTIFY dataEntryEnabledChanged)
    Q_PROPERTY(bool addWordsPairEnabled READ isAddWordsPairEnabled NOTIFY addWordsPairEnabledChanged)
    Q_PROPERTY(bool discardAddedWordPairsEnabled READ isDiscardAddedWordPairsEnabled NOTIFY discardAddedWordPairsEnabledChanged)
    Q_PROPERTY(bool saveAddedWordPairsEnabled READ isSaveAddedWordPairsEnabled NOTIFY saveAddedWordPairsEnabledChanged)
    Q_PROPERTY(bool submitMainPaneInputEnabled READ getSubmitMainPaneInputEnabled NOTIFY submitMainPaneInputEnabledChanged)
    Q_PROPERTY(bool clearMainPaneInputEnabled READ getClearMainPaneInputEnabled NOTIFY clearMainPaneInputEnabledChanged)
    Q_PROPERTY(bool mainPaneStatisticsResetEnabled READ getMainPaneStatisticsResetEnabled NOTIFY mainPaneStatisticsResetEnabledChanged)

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
    // need to use these three properties as Q_ENUM cannot be used with the Level enum (it si not part of the presenter class)
    Q_PROPERTY(int levelEasy READ getLevelEasy CONSTANT)
    Q_PROPERTY(int levelMedium READ getLevelMedium CONSTANT)
    Q_PROPERTY(int levelHard READ getLevelHard CONSTANT)

    /* text properties */
    Q_PROPERTY(QString windowTitle READ getWindowTitle NOTIFY currentPaneChanged)
    Q_PROPERTY(QString introPaneMessage READ getIntroPaneMessage NOTIFY introPaneMessageChanged)
    Q_PROPERTY(QString helpPaneMessage READ getHelpPaneMessage CONSTANT) // if feasible: to be moved later to GameStrings.qml
    Q_PROPERTY(QString mainPaneStatusMessage READ getMainPaneStatusMessage NOTIFY mainPaneStatusMessageChanged)
    Q_PROPERTY(QString mainPaneScoreMessage READ getMainPaneScoreMessage NOTIFY mainPaneStatisticsMessagesChanged)
    Q_PROPERTY(QString mainPaneWordPairsMessage READ getMainPaneWordPairsMessage NOTIFY mainPaneStatisticsMessagesChanged)
    Q_PROPERTY(QString dataEntryPaneStatusMessage READ getDataEntryPaneStatusMessage NOTIFY dataEntryPaneStatusMessageChanged)
    Q_PROPERTY(QString errorMessage READ getErrorMessage NOTIFY errorMessageChanged)

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
        ERROR,
        Nr_Of_Panes
    };
    Q_ENUM(Pane)

    explicit GamePresenter(QObject *parent = nullptr);

    Q_INVOKABLE void switchToPane(Pane pane);
    Q_INVOKABLE void goBack();
    Q_INVOKABLE void handleAddWordsPairRequest(const QString& firstWord, const QString& secondWord, bool areSynonyms);
    Q_INVOKABLE void handleClearAddedWordPairsRequest();
    Q_INVOKABLE void handleSaveAddedWordPairsRequest();
    Q_INVOKABLE void promptForSavingAddedWordPairs();
    Q_INVOKABLE void handleDisplayCorrectWordsPairRequest();
    Q_INVOKABLE void handleSubmitMainPaneInputRequest();
    Q_INVOKABLE void handleMainPaneStatisticsResetRequest();
    Q_INVOKABLE void switchToLevel(int level);
    Q_INVOKABLE void selectWordPieceForFirstInputWord(int wordPieceIndex);
    Q_INVOKABLE void selectWordPieceForSecondInputWord(int wordPieceIndex);
    Q_INVOKABLE void removeWordPiecesFromFirstInputWord(int inputRangeStart);
    Q_INVOKABLE void removeWordPiecesFromSecondInputWord(int inputRangeStart);
    Q_INVOKABLE void clearMainPaneInput();
    Q_INVOKABLE void clearMainPaneFirstInputWord();
    Q_INVOKABLE void clearMainPaneSecondInputWord();
    Q_INVOKABLE void updateFirstWordInputHoverIndex(int index);
    Q_INVOKABLE void updateSecondWordInputHoverIndex(int index);
    Q_INVOKABLE void clearWordInputHoverIndexes();
    Q_INVOKABLE void quit();

    bool getIntroPaneVisible() const;
    bool getHelpPaneVisible() const;
    bool getMainPaneVisible() const;
    bool getDataEntryPaneVisible() const;
    bool getPromptSaveExitPaneVisible() const;
    bool isPlayEnabled() const;
    bool isDataEntryEnabled() const;
    bool isAddWordsPairEnabled() const;
    bool isDiscardAddedWordPairsEnabled() const;
    bool isSaveAddedWordPairsEnabled() const;
    bool getMainPaneStatisticsResetEnabled() const;
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

    int getLevelEasy() const;
    int getLevelMedium() const;
    int getLevelHard() const;
    int getToolTipDelay() const;
    int getToolTipTimeout() const;

    QString getWindowTitle() const;

    QString getIntroPaneMessage() const;
    QString getHelpPaneMessage() const;
    QString getMainPaneStatusMessage() const;
    QString getMainPaneScoreMessage() const;
    QString getMainPaneWordPairsMessage() const;
    QString getDataEntryPaneStatusMessage() const;
    QString getErrorMessage() const;

    virtual ~GamePresenter();

signals:
    Q_SIGNAL void currentPaneChanged();
    Q_SIGNAL void playEnabledChanged();
    Q_SIGNAL void dataEntryEnabledChanged();
    Q_SIGNAL void addWordsPairEnabledChanged();
    Q_SIGNAL void discardAddedWordPairsEnabledChanged();
    Q_SIGNAL void saveAddedWordPairsEnabledChanged();
    Q_SIGNAL void mainPaneStatisticsResetEnabledChanged();
    Q_SIGNAL void clearMainPaneInputEnabledChanged();
    Q_SIGNAL void submitMainPaneInputEnabledChanged();
    Q_SIGNAL void errorOccuredChanged();
    Q_SIGNAL void quitGameDeferredChanged();
    Q_SIGNAL void mixedWordsChanged();
    Q_SIGNAL void inputChanged();
    Q_SIGNAL void selectionChanged();
    Q_SIGNAL void introPaneMessageChanged();
    Q_SIGNAL void mainPaneStatusMessageChanged();
    Q_SIGNAL void mainPaneStatisticsMessagesChanged();
    Q_SIGNAL void dataEntryPaneStatusMessageChanged();
    Q_SIGNAL void dataEntrySucceeded();
    Q_SIGNAL void errorMessageChanged();
    Q_SIGNAL void hoverChanged();

private slots:
    void _onInputChanged();
    void _onStatisticsChanged();
    void _onStatusChanged(Game::StatusCodes statusCode);

private:
    void _switchToPane(Pane pane);
    void _updateStatusMessage(const QString& message, Pane pane, int delay);
    void _updateMessage();
    void _launchErrorPane(const QString& errorMessage);

    bool m_IntroPaneVisible;
    bool m_HelpPaneVisible;
    bool m_MainPaneVisible;
    bool m_DataEntryPaneVisible;
    bool m_PromptSaveExitPaneVisible;
    bool m_MainPaneInitialized;

    bool m_MainPaneStatisticsResetEnabled;
    bool m_ClearMainPaneInputEnabled;
    bool m_ErrorOccured;
    bool m_QuitDeferred;

    QString m_WindowTitle;
    QString m_IntroPaneMessage;
    QString m_MainPaneStatusMessage;
    QString m_MainPaneScoreMessage;
    QString m_MainPaneWordPairsMessage;
    QString m_DataEntryPaneStatusMessage;
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
