#ifndef GAMEPRESENTER_H
#define GAMEPRESENTER_H

#include <QObject>
#include <QVariant>

#include "game.h"
#include "gamestrings.h"

class WordMixer;
class ScoreItem;

class GamePresenter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int levelEasy READ getLevelEasy CONSTANT)
    Q_PROPERTY(int levelMedium READ getLevelMedium CONSTANT)
    Q_PROPERTY(int levelHard READ getLevelHard CONSTANT)
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
    Q_PROPERTY(bool introPaneVisible READ getIntroPaneVisible NOTIFY introPaneVisibleChanged)
    Q_PROPERTY(bool helpPaneVisible READ getHelpPaneVisible NOTIFY helpPaneVisibleChanged)
    Q_PROPERTY(bool mainPaneVisible READ getMainPaneVisible NOTIFY mainPaneVisibleChanged)
    Q_PROPERTY(QString windowTitle READ getWindowTitle NOTIFY windowTitleChanged)
    Q_PROPERTY(QString introPaneMessage READ getIntroPaneMessage CONSTANT)
    Q_PROPERTY(QString helpPaneMessage READ getHelpPaneMessage CONSTANT)
    Q_PROPERTY(QString mainPaneInstructionsMessage READ getMainPaneInstructionsMessage CONSTANT)
    Q_PROPERTY(QString mainPaneStatusMessage READ getMainPaneStatusMessage NOTIFY mainPaneStatusMessageChanged)
    Q_PROPERTY(QString mainPaneScoreMessage READ getMainPaneScoreMessage NOTIFY mainPaneScoreMessageChanged)
    Q_PROPERTY(QString mainPaneWordPairsMessage READ getMainPaneWordPairsMessage NOTIFY mainPaneWordPairsMessageChanged)
    Q_PROPERTY(QString errorMessage READ getErrorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(int toolTipDelay READ getToolTipDelay CONSTANT)
    Q_PROPERTY(QString introPaneToolTip READ getIntroPaneToolTip CONSTANT)
    Q_PROPERTY(QString helpPaneToolTip READ getHelpPaneToolTip CONSTANT)
    Q_PROPERTY(QString mainPaneToolTip READ getMainPaneToolTip CONSTANT)
    Q_PROPERTY(QString errorPaneToolTip READ getErrorPaneToolTip CONSTANT)
    Q_PROPERTY(QString highscoresToolTip READ getHighscoresToolTip CONSTANT)
    Q_PROPERTY(QString wordPairsToolTip READ getWordPairsToolTip CONSTANT)
    Q_PROPERTY(QString gameInstructionsToolTip READ getGameInstructionsToolTip CONSTANT)
    Q_PROPERTY(QString gameStatusToolTip READ getGameStatusToolTip CONSTANT)
    Q_PROPERTY(QString firstWordToolTip READ getFirstWordToolTip CONSTANT)
    Q_PROPERTY(QString secondWordToolTip READ getSecondWordToolTip CONSTANT)
    Q_PROPERTY(QString playButtonToolTip READ getPlayButtonToolTip CONSTANT)
    Q_PROPERTY(QString helpButtonToolTip READ getHelpButtonToolTip CONSTANT)
    Q_PROPERTY(QString quitButtonToolTip READ getQuitButtonToolTip CONSTANT)
    Q_PROPERTY(QString okButtonToolTip READ getOkButtonToolTip CONSTANT)
    Q_PROPERTY(QString submitButtonToolTip READ getSubmitButtonToolTip CONSTANT)
    Q_PROPERTY(QString resultsButtonToolTip READ getResultsButtonToolTip CONSTANT)
    Q_PROPERTY(QString resetButtonToolTip READ getResetButtonToolTip CONSTANT)
    Q_PROPERTY(QString levelButtonsToolTip READ getLevelButtonsToolTip CONSTANT)
    Q_PROPERTY(QString closeButtonToolTip READ getCloseButtonToolTip CONSTANT)
    Q_PROPERTY(bool resetEnabled READ getResetEnabled NOTIFY resetEnabledChanged)
    Q_PROPERTY(bool errorOccured READ getErrorOccured NOTIFY errorOccuredChanged)
    Q_PROPERTY(QList<QVariant> mixedWordsPieces READ getMixedWordsPieces NOTIFY mixedWordsPiecesChanged)
    Q_PROPERTY(int firstWordBeginIndex READ getFirstWordFirstPieceIndex NOTIFY mixedWordsPiecesChanged)
    Q_PROPERTY(int firstWordEndIndex READ getFirstWordLastPieceIndex NOTIFY mixedWordsPiecesChanged)
    Q_PROPERTY(int secondWordBeginIndex READ getSecondWordFirstPieceIndex NOTIFY mixedWordsPiecesChanged)
    Q_PROPERTY(int secondWordEndIndex READ getSecondWordLastPieceIndex NOTIFY mixedWordsPiecesChanged)

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
    Q_INVOKABLE bool handleSubmitRequest(const QString& firstWord, const QString& secondWord);
    Q_INVOKABLE void handleResetRequest();
    Q_INVOKABLE void switchToLevel(int level);

    int getLevelEasy() const;
    int getLevelMedium() const;
    int getLevelHard() const;

    QString getPlayButtonLabel() const;
    QString getHelpButtonLabel() const;
    QString getQuitButtonLabel() const;
    QString getSubmitButtonLabel() const;
    QString getResultsButtonLabel() const;
    QString getResetButtonLabel() const;
    QString getOkButtonLabel() const;
    QString getLevelEasyButtonLabel() const;
    QString getLevelMediumButtonLabel() const;
    QString getLevelHardButtonLabel() const;
    QString getCloseButtonLabel() const;

    bool getIntroPaneVisible() const;
    bool getHelpPaneVisible() const;
    bool getMainPaneVisible() const;
    bool getResetEnabled() const;
    bool getErrorOccured() const;

    QString getWindowTitle() const;
    QString getIntroPaneMessage() const;
    QString getHelpPaneMessage() const;
    QString getMainPaneInstructionsMessage() const;
    QString getMainPaneStatusMessage() const;
    QString getMainPaneScoreMessage() const;
    QString getMainPaneWordPairsMessage() const;
    QString getErrorMessage() const;

    int getToolTipDelay() const;
    QString getIntroPaneToolTip() const;
    QString getHelpPaneToolTip() const;
    QString getMainPaneToolTip() const;
    QString getErrorPaneToolTip() const;
    QString getHighscoresToolTip() const;
    QString getWordPairsToolTip() const;
    QString getGameInstructionsToolTip() const;
    QString getGameStatusToolTip() const;
    QString getFirstWordToolTip() const;
    QString getSecondWordToolTip() const;
    QString getPlayButtonToolTip() const;
    QString getHelpButtonToolTip() const;
    QString getQuitButtonToolTip() const;
    QString getOkButtonToolTip() const;
    QString getSubmitButtonToolTip() const;
    QString getResultsButtonToolTip() const;
    QString getResetButtonToolTip() const;
    QString getLevelButtonsToolTip() const;
    QString getCloseButtonToolTip() const;

    QString getPlayButtonShortcut() const;
    QString getHelpButtonShortcut() const;
    QString getQuitButtonShortcut() const;
    QString getOkButtonShortcut() const;
    QString getSubmitButtonShortcut() const;
    QString getResultsButtonShortcut() const;
    QString getResetButtonShortcut() const;
    QString getLevelEasyButtonShortcut() const;
    QString getLevelMediumButtonShortcut() const;
    QString getLevelHardButtonShortcut() const;
    QString getCloseButtonShortcut() const;

    QList<QVariant> getMixedWordsPieces() const;
    int getFirstWordFirstPieceIndex() const;
    int getFirstWordLastPieceIndex() const;
    int getSecondWordFirstPieceIndex() const;
    int getSecondWordLastPieceIndex() const;

signals:
    Q_SIGNAL void windowTitleChanged();
    Q_SIGNAL void introPaneVisibleChanged();
    Q_SIGNAL void helpPaneVisibleChanged();
    Q_SIGNAL void mainPaneVisibleChanged();
    Q_SIGNAL void resetEnabledChanged();
    Q_SIGNAL void errorOccuredChanged();
    Q_SIGNAL void mainPaneStatusMessageChanged();
    Q_SIGNAL void mainPaneScoreMessageChanged();
    Q_SIGNAL void errorMessageChanged();
    Q_SIGNAL void mainPaneWordPairsMessageChanged();
    Q_SIGNAL void levelChanged(Game::Level level);
    Q_SIGNAL void mixedWordsPiecesChanged();

private slots:
    void _onStatisticsUpdated();

private:
    Game::StatusCodes _checkWords(const QString &firstWord, const QString &secondWord);
    void _initMainPane();
    void _updateStatusMessage(Game::StatusCodes statusCode);
    void _setLevel(Game::Level level);
    void _launchErrorPane(const QString& errorMessage);

    bool m_IntroPaneVisible;
    bool m_HelpPaneVisible;
    bool m_MainPaneVisible;
    bool m_MainPaneInitialized;

    bool m_ResetEnabled;
    bool m_ErrorOccured;

    QString m_WindowTitle;
    QString m_MainPaneStatusMessage;
    QString m_MainPaneScoreMessage;
    QString m_MainPaneWordPairsMessage;
    QString m_ErrorMessage;

    Pane m_CurrentPane;

    WordMixer* m_pWordMixer;
    ScoreItem* m_pScoreItem;
};

#endif // GAMEPRESENTER_H
