#ifndef GAMEPRESENTER_H
#define GAMEPRESENTER_H

#include <QObject>
#include <QVariant>
#include <QColor>

#include "../Common/game.h"

class GameFacade;

class GamePresenter : public QObject
{
    Q_OBJECT

    // functionality properties
    Q_PROPERTY(bool introPaneVisible READ getIntroPaneVisible NOTIFY introPaneVisibleChanged)
    Q_PROPERTY(bool helpPaneVisible READ getHelpPaneVisible NOTIFY helpPaneVisibleChanged)
    Q_PROPERTY(bool mainPaneVisible READ getMainPaneVisible NOTIFY mainPaneVisibleChanged)
    Q_PROPERTY(bool resetEnabled READ getResetEnabled NOTIFY resetEnabledChanged)
    Q_PROPERTY(bool submitEnabled READ getSubmitEnabled NOTIFY submitEnabledChanged)
    Q_PROPERTY(bool errorOccured READ getErrorOccured NOTIFY errorOccuredChanged)
    Q_PROPERTY(QList<QVariant> mixedWordsPiecesContent READ getMixedWordsPiecesContent NOTIFY mixedWordsChanged)
    Q_PROPERTY(QList<QVariant> mixedWordsPiecesTextColors READ getMixedWordsPiecesTextColors NOTIFY mixedWordsChanged)
    Q_PROPERTY(QList<QVariant> mixedWordsPiecesSelections READ getMixedWordsPiecesSelections NOTIFY selectionChanged)
    Q_PROPERTY(QList<QVariant> firstWordInputPiecesContent READ getFirstWordInputPiecesContent NOTIFY inputChanged)
    Q_PROPERTY(QList<QVariant> firstWordInputPiecesTextColors READ getFirstWordInputPiecesTextColors NOTIFY inputChanged)
    Q_PROPERTY(QList<QVariant> secondWordInputPiecesContent READ getSecondWordInputPiecesContent NOTIFY inputChanged)
    Q_PROPERTY(QList<QVariant> secondWordInputPiecesTextColors READ getSecondWordInputPiecesTextColors NOTIFY inputChanged)
    Q_PROPERTY(int levelEasy READ getLevelEasy CONSTANT)
    Q_PROPERTY(int levelMedium READ getLevelMedium CONSTANT)
    Q_PROPERTY(int levelHard READ getLevelHard CONSTANT)
    Q_PROPERTY(int toolTipDelay READ getToolTipDelay CONSTANT)
    Q_PROPERTY(int toolTipTimeout READ getToolTipTimeout CONSTANT)

    // text properties
    Q_PROPERTY(QString windowTitle READ getWindowTitle NOTIFY windowTitleChanged)

    Q_PROPERTY(QString introPaneMessage READ getIntroPaneMessage CONSTANT)
    Q_PROPERTY(QString helpPaneMessage READ getHelpPaneMessage CONSTANT)
    Q_PROPERTY(QString mainPaneInstructionsMessage READ getMainPaneInstructionsMessage CONSTANT)
    Q_PROPERTY(QString mainPaneStatusMessage READ getMainPaneStatusMessage NOTIFY mainPaneStatusMessageChanged)
    Q_PROPERTY(QString mainPaneScoreMessage READ getMainPaneScoreMessage NOTIFY mainPaneStatisticsMessagesChanged)
    Q_PROPERTY(QString mainPaneWordPairsMessage READ getMainPaneWordPairsMessage NOTIFY mainPaneStatisticsMessagesChanged)
    Q_PROPERTY(QString errorMessage READ getErrorMessage NOTIFY errorMessageChanged)

    Q_PROPERTY(QString playButtonLabel READ getPlayButtonLabel CONSTANT)
    Q_PROPERTY(QString helpButtonLabel READ getHelpButtonLabel CONSTANT)
    Q_PROPERTY(QString quitButtonLabel READ getQuitButtonLabel CONSTANT)
    Q_PROPERTY(QString submitButtonLabel READ getSubmitButtonLabel CONSTANT)
    Q_PROPERTY(QString resultsButtonLabel READ getResultsButtonLabel CONSTANT)
    Q_PROPERTY(QString resetButtonLabel READ getResetButtonLabel CONSTANT)
    Q_PROPERTY(QString okButtonLabel READ getOkButtonLabel CONSTANT)
    Q_PROPERTY(QString levelEasyButtonLabel READ getLevelEasyButtonLabel CONSTANT)
    Q_PROPERTY(QString levelMediumButtonLabel READ getLevelMediumButtonLabel CONSTANT)
    Q_PROPERTY(QString levelHardButtonLabel READ getLevelHardButtonLabel CONSTANT)
    Q_PROPERTY(QString closeButtonLabel READ getCloseButtonLabel CONSTANT)

    Q_PROPERTY(QString playButtonShortcut READ getPlayButtonShortcut CONSTANT)
    Q_PROPERTY(QString helpButtonShortcut READ getHelpButtonShortcut CONSTANT)
    Q_PROPERTY(QString quitButtonShortcut READ getQuitButtonShortcut CONSTANT)
    Q_PROPERTY(QString okButtonShortcut READ getOkButtonShortcut CONSTANT)
    Q_PROPERTY(QString submitButtonShortcut READ getSubmitButtonShortcut CONSTANT)
    Q_PROPERTY(QString resultsButtonShortcut READ getResultsButtonShortcut CONSTANT)
    Q_PROPERTY(QString resetButtonShortcut READ getResetButtonShortcut CONSTANT)
    Q_PROPERTY(QString levelEasyButtonShortcut READ getLevelEasyButtonShortcut CONSTANT)
    Q_PROPERTY(QString levelMediumButtonShortcut READ getLevelMediumButtonShortcut CONSTANT)
    Q_PROPERTY(QString levelHardButtonShortcut READ getLevelHardButtonShortcut CONSTANT)
    Q_PROPERTY(QString closeButtonShortcut READ getCloseButtonShortcut CONSTANT)

    Q_PROPERTY(QString introPaneToolTip READ getIntroPaneToolTip CONSTANT)
    Q_PROPERTY(QString helpPaneToolTip READ getHelpPaneToolTip CONSTANT)
    Q_PROPERTY(QString mainPaneToolTip READ getMainPaneToolTip CONSTANT)
    Q_PROPERTY(QString errorPaneToolTip READ getErrorPaneToolTip CONSTANT)
    Q_PROPERTY(QString highscoresToolTip READ getHighscoresToolTip CONSTANT)
    Q_PROPERTY(QString wordPairsToolTip READ getWordPairsToolTip CONSTANT)
    Q_PROPERTY(QString gameInstructionsToolTip READ getGameInstructionsToolTip CONSTANT)
    Q_PROPERTY(QString gameStatusToolTip READ getGameStatusToolTip CONSTANT)
    Q_PROPERTY(QString playButtonToolTip READ getPlayButtonToolTip CONSTANT)
    Q_PROPERTY(QString helpButtonToolTip READ getHelpButtonToolTip CONSTANT)
    Q_PROPERTY(QString quitButtonToolTip READ getQuitButtonToolTip CONSTANT)
    Q_PROPERTY(QString okButtonToolTip READ getOkButtonToolTip CONSTANT)
    Q_PROPERTY(QString submitButtonToolTip READ getSubmitButtonToolTip CONSTANT)
    Q_PROPERTY(QString resultsButtonToolTip READ getResultsButtonToolTip CONSTANT)
    Q_PROPERTY(QString resetButtonToolTip READ getResetButtonToolTip CONSTANT)
    Q_PROPERTY(QString levelButtonsToolTip READ getLevelButtonsToolTip CONSTANT)
    Q_PROPERTY(QString closeButtonToolTip READ getCloseButtonToolTip CONSTANT)

    // color properties
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor CONSTANT)
    Q_PROPERTY(QColor pushButtonColor READ getPushButtonColor CONSTANT)
    Q_PROPERTY(QColor borderColor READ getBorderColor CONSTANT)
    Q_PROPERTY(QColor textColor READ getTextColor CONSTANT)
    Q_PROPERTY(QColor fatalErrorTextColor READ getFatalErrorTextColor CONSTANT)
    Q_PROPERTY(QColor wordPieceSelectedColor READ getWordPieceSelectedColor CONSTANT)

public:
    enum class Pane
    {
        INTRO,
        HELP,
        MAIN,
        ERROR,
        Nr_Of_Panes
    };
    Q_ENUM(Pane)

    explicit GamePresenter(QObject *parent = nullptr);

    Q_INVOKABLE void switchToPane(Pane pane);
    Q_INVOKABLE void handleResultsRequest();
    Q_INVOKABLE void handleSubmitRequest();
    Q_INVOKABLE void handleResetRequest();
    Q_INVOKABLE void switchToLevel(int level);
    Q_INVOKABLE void selectWordPieceForFirstInputWord(int wordPieceIndex);
    Q_INVOKABLE void selectWordPieceForSecondInputWord(int wordPieceIndex);
    Q_INVOKABLE void removeWordPiecesFromFirstInputWord(int inputRangeStart);
    Q_INVOKABLE void removeWordPiecesFromSecondInputWord(int inputRangeStart);

    bool getIntroPaneVisible() const;
    bool getHelpPaneVisible() const;
    bool getMainPaneVisible() const;
    bool getResetEnabled() const;
    bool getSubmitEnabled() const;
    bool getErrorOccured() const;

    QList<QVariant> getMixedWordsPiecesContent() const;
    QList<QVariant> getMixedWordsPiecesTextColors() const;
    QList<QVariant> getMixedWordsPiecesSelections() const;
    QList<QVariant> getFirstWordInputPiecesContent() const;
    QList<QVariant> getFirstWordInputPiecesTextColors() const;
    QList<QVariant> getSecondWordInputPiecesContent() const;
    QList<QVariant> getSecondWordInputPiecesTextColors() const;

    int getLevelEasy() const;
    int getLevelMedium() const;
    int getLevelHard() const;
    int getToolTipDelay() const;
    int getToolTipTimeout() const;

    QString getWindowTitle() const;

    QString getIntroPaneMessage() const;
    QString getHelpPaneMessage() const;
    QString getMainPaneInstructionsMessage() const;
    QString getMainPaneStatusMessage() const;
    QString getMainPaneScoreMessage() const;
    QString getMainPaneWordPairsMessage() const;
    QString getErrorMessage() const;

    QString getPlayButtonLabel() const;
    QString getHelpButtonLabel() const;
    QString getQuitButtonLabel() const;
    QString getSubmitButtonLabel() const;
    QString getResultsButtonLabel() const;
    QString getResetButtonLabel() const;
    QString getLevelEasyButtonLabel() const;
    QString getLevelMediumButtonLabel() const;
    QString getLevelHardButtonLabel() const;
    QString getOkButtonLabel() const;
    QString getCloseButtonLabel() const;

    QString getPlayButtonShortcut() const;
    QString getHelpButtonShortcut() const;
    QString getQuitButtonShortcut() const;
    QString getSubmitButtonShortcut() const;
    QString getResultsButtonShortcut() const;
    QString getResetButtonShortcut() const;
    QString getLevelEasyButtonShortcut() const;
    QString getLevelMediumButtonShortcut() const;
    QString getLevelHardButtonShortcut() const;
    QString getOkButtonShortcut() const;
    QString getCloseButtonShortcut() const;

    QString getIntroPaneToolTip() const;
    QString getHelpPaneToolTip() const;
    QString getMainPaneToolTip() const;
    QString getErrorPaneToolTip() const;
    QString getHighscoresToolTip() const;
    QString getWordPairsToolTip() const;
    QString getGameInstructionsToolTip() const;
    QString getGameStatusToolTip() const;
    QString getPlayButtonToolTip() const;
    QString getHelpButtonToolTip() const;
    QString getQuitButtonToolTip() const;
    QString getSubmitButtonToolTip() const;
    QString getResultsButtonToolTip() const;
    QString getResetButtonToolTip() const;
    QString getLevelButtonsToolTip() const;
    QString getOkButtonToolTip() const;
    QString getCloseButtonToolTip() const;

    QColor getBackgroundColor() const;
    QColor getPushButtonColor() const;
    QColor getBorderColor() const;
    QColor getTextColor() const;
    QColor getFatalErrorTextColor() const;
    QColor getWordPieceSelectedColor() const;

signals:
    Q_SIGNAL void introPaneVisibleChanged();
    Q_SIGNAL void helpPaneVisibleChanged();
    Q_SIGNAL void mainPaneVisibleChanged();
    Q_SIGNAL void resetEnabledChanged();
    Q_SIGNAL void submitEnabledChanged();
    Q_SIGNAL void errorOccuredChanged();
    Q_SIGNAL void mixedWordsChanged();
    Q_SIGNAL void inputChanged();
    Q_SIGNAL void selectionChanged();
    Q_SIGNAL void windowTitleChanged();
    Q_SIGNAL void mainPaneStatusMessageChanged();
    Q_SIGNAL void mainPaneStatisticsMessagesChanged();
    Q_SIGNAL void errorMessageChanged();

private slots:
    void _onStatisticsChanged();
    void _onStatusChanged(Game::StatusCodes statusCode);

private:
    void _launchErrorPane(const QString& errorMessage);

    bool m_IntroPaneVisible;
    bool m_HelpPaneVisible;
    bool m_MainPaneVisible;
    bool m_MainPaneInitialized;

    bool m_StatisticsResetEnabled;
    bool m_ErrorOccured;

    QString m_WindowTitle;
    QString m_MainPaneStatusMessage;
    QString m_MainPaneScoreMessage;
    QString m_MainPaneWordPairsMessage;
    QString m_ErrorMessage;

    Pane m_CurrentPane;

    GameFacade* m_pGameFacade;
};

#endif // GAMEPRESENTER_H
