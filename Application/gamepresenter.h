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

    // functionality properties
    Q_PROPERTY(bool introPaneVisible READ getIntroPaneVisible NOTIFY currentPaneChanged)
    Q_PROPERTY(bool helpPaneVisible READ getHelpPaneVisible NOTIFY currentPaneChanged)
    Q_PROPERTY(bool mainPaneVisible READ getMainPaneVisible NOTIFY currentPaneChanged)
    Q_PROPERTY(bool dataEntryPaneVisible READ getDataEntryPaneVisible NOTIFY currentPaneChanged)
    Q_PROPERTY(bool playEnabled READ isPlayEnabled NOTIFY playEnabledChanged)
    Q_PROPERTY(bool dataEntryEnabled READ isDataEntryEnabled NOTIFY dataEntryEnabledChanged)
    Q_PROPERTY(bool addDataEntryEnabled READ isAddDataEntryEnabled NOTIFY addWordsPairEnabledChanged)
    Q_PROPERTY(bool saveEntriesEnabled READ isSaveDataEntriesEnabled NOTIFY saveNewPairsEnabledChanged)
    Q_PROPERTY(bool resetEnabled READ getResetEnabled NOTIFY resetEnabledChanged)
    Q_PROPERTY(bool clearInputEnabled READ getClearInputEnabled NOTIFY clearInputEnabledChanged)
    Q_PROPERTY(bool submitEnabled READ getSubmitEnabled NOTIFY submitEnabledChanged)
    Q_PROPERTY(bool errorOccured READ getErrorOccured NOTIFY errorOccuredChanged)
    Q_PROPERTY(QList<QVariant> mixedWordsPiecesContent READ getMixedWordsPiecesContent NOTIFY mixedWordsChanged)
    Q_PROPERTY(QList<QVariant> mixedWordsPiecesTextColors READ getMixedWordsPiecesTextColors NOTIFY mixedWordsChanged)
    Q_PROPERTY(QList<QVariant> mixedWordsPiecesSelections READ getMixedWordsPiecesSelections NOTIFY selectionChanged)
    Q_PROPERTY(QList<QVariant> firstWordInputPiecesContent READ getFirstWordInputPiecesContent NOTIFY inputChanged)
    Q_PROPERTY(QList<QVariant> firstWordInputPiecesTextColors READ getFirstWordInputPiecesTextColors NOTIFY inputChanged)
    Q_PROPERTY(bool isFirstWordInputHovered READ getIsFirstWordInputHovered NOTIFY hoverChanged)
    Q_PROPERTY(int firstWordInputHoverIndex READ getFirstWordInputHoverIndex NOTIFY hoverChanged)
    Q_PROPERTY(QList<QVariant> secondWordInputPiecesContent READ getSecondWordInputPiecesContent NOTIFY inputChanged)
    Q_PROPERTY(QList<QVariant> secondWordInputPiecesTextColors READ getSecondWordInputPiecesTextColors NOTIFY inputChanged)
    Q_PROPERTY(bool isSecondWordInputHovered READ getIsSecondWordInputHovered NOTIFY hoverChanged)
    Q_PROPERTY(int secondWordInputHoverIndex READ getSecondWordInputHoverIndex NOTIFY hoverChanged)
    Q_PROPERTY(int levelEasy READ getLevelEasy CONSTANT)
    Q_PROPERTY(int levelMedium READ getLevelMedium CONSTANT)
    Q_PROPERTY(int levelHard READ getLevelHard CONSTANT)
    Q_PROPERTY(int toolTipDelay READ getToolTipDelay CONSTANT)
    Q_PROPERTY(int toolTipTimeout READ getToolTipTimeout CONSTANT)

    // text properties
    Q_PROPERTY(QString windowTitle READ getWindowTitle NOTIFY windowTitleChanged)
    Q_PROPERTY(QString introPaneMessage READ getIntroPaneMessage NOTIFY introPaneMessageChanged)
    Q_PROPERTY(QString helpPaneMessage READ getHelpPaneMessage CONSTANT) // if feasible: to be moved later to GameStrings.qml
    Q_PROPERTY(QString mainPaneStatusMessage READ getMainPaneStatusMessage NOTIFY mainPaneStatusMessageChanged)
    Q_PROPERTY(QString mainPaneScoreMessage READ getMainPaneScoreMessage NOTIFY mainPaneStatisticsMessagesChanged)
    Q_PROPERTY(QString mainPaneWordPairsMessage READ getMainPaneWordPairsMessage NOTIFY mainPaneStatisticsMessagesChanged)
    Q_PROPERTY(QString dataEntryPaneStatusMessage READ getDataEntryPaneStatusMessage NOTIFY dataEntryPaneStatusMessageChanged)
    Q_PROPERTY(QString errorMessage READ getErrorMessage NOTIFY errorMessageChanged)

public:
    enum class Pane
    {
        INTRO,
        HELP,
        MAIN,
        DATA_ENTRY,
        ERROR,
        Nr_Of_Panes
    };
    Q_ENUM(Pane)

    explicit GamePresenter(QObject *parent = nullptr);

    Q_INVOKABLE void switchToPane(Pane pane);
    Q_INVOKABLE void goBack();
    Q_INVOKABLE void handleAddWordsPairRequest(const QString& firstWord, const QString& secondWord, bool areSynonyms);
    Q_INVOKABLE void handleSaveNewWordPairsRequest();
    Q_INVOKABLE void handleResultsRequest();
    Q_INVOKABLE void handleSubmitRequest();
    Q_INVOKABLE void handleResetRequest();
    Q_INVOKABLE void switchToLevel(int level);
    Q_INVOKABLE void selectWordPieceForFirstInputWord(int wordPieceIndex);
    Q_INVOKABLE void selectWordPieceForSecondInputWord(int wordPieceIndex);
    Q_INVOKABLE void removeWordPiecesFromFirstInputWord(int inputRangeStart);
    Q_INVOKABLE void removeWordPiecesFromSecondInputWord(int inputRangeStart);
    Q_INVOKABLE void clearInput();
    Q_INVOKABLE void clearFirstInputWord();
    Q_INVOKABLE void clearSecondInputWord();
    Q_INVOKABLE void updateFirstWordInputHoverIndex(int index);
    Q_INVOKABLE void updateSecondWordInputHoverIndex(int index);
    Q_INVOKABLE void clearWordInputHoverIndexes();
    Q_INVOKABLE void quit();

    bool getIntroPaneVisible() const;
    bool getHelpPaneVisible() const;
    bool getMainPaneVisible() const;
    bool getDataEntryPaneVisible() const;
    bool isPlayEnabled() const;
    bool isDataEntryEnabled() const;
    bool isAddDataEntryEnabled() const;
    bool isSaveDataEntriesEnabled() const;
    bool getResetEnabled() const;
    bool getClearInputEnabled() const;
    bool getSubmitEnabled() const;
    bool getErrorOccured() const;

    QList<QVariant> getMixedWordsPiecesContent() const;
    QList<QVariant> getMixedWordsPiecesTextColors() const;
    QList<QVariant> getMixedWordsPiecesSelections() const;
    QList<QVariant> getFirstWordInputPiecesContent() const;
    QList<QVariant> getFirstWordInputPiecesTextColors() const;
    bool getIsFirstWordInputHovered() const;
    int getFirstWordInputHoverIndex() const;
    QList<QVariant> getSecondWordInputPiecesContent() const;
    QList<QVariant> getSecondWordInputPiecesTextColors() const;
    bool getIsSecondWordInputHovered() const;
    int getSecondWordInputHoverIndex() const;

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
    Q_SIGNAL void saveNewPairsEnabledChanged();
    Q_SIGNAL void resetEnabledChanged();
    Q_SIGNAL void clearInputEnabledChanged();
    Q_SIGNAL void submitEnabledChanged();
    Q_SIGNAL void errorOccuredChanged();
    Q_SIGNAL void mixedWordsChanged();
    Q_SIGNAL void inputChanged();
    Q_SIGNAL void selectionChanged();
    Q_SIGNAL void windowTitleChanged();
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
    bool m_MainPaneInitialized;

    bool m_StatisticsResetEnabled;
    bool m_ClearInputEnabled;
    bool m_ErrorOccured;

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

    int m_FirstWordInputHoverIndex;
    int m_SecondWordInputHoverIndex;

    GameFacade* m_pGameFacade;
    GameProxy* m_pGameProxy;

    QTimer* m_pStatusUpdateTimer;
};

#endif // GAMEPRESENTER_H
