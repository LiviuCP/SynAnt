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
    Q_PROPERTY(int firstWordBeginIndex READ getFirstWordBeginIndex NOTIFY mixedWordsPiecesChanged)
    Q_PROPERTY(int firstWordEndIndex READ getFirstWordEndIndex NOTIFY mixedWordsPiecesChanged)
    Q_PROPERTY(int secondWordBeginIndex READ getSecondWordBeginIndex NOTIFY mixedWordsPiecesChanged)
    Q_PROPERTY(int secondWordEndIndex READ getSecondWordEndIndex NOTIFY mixedWordsPiecesChanged)

public:
    explicit GamePresenter(QObject *parent = nullptr);

    Q_INVOKABLE void switchToHelpPane();
    Q_INVOKABLE void switchToMainPane();
    Q_INVOKABLE void handleResultsRequest();
    Q_INVOKABLE bool handleSubmitRequest(const QString& firstWord, const QString& secondWord);
    Q_INVOKABLE void handleResetRequest();
    Q_INVOKABLE void switchToEasyLevel();
    Q_INVOKABLE void switchToMediumLevel();
    Q_INVOKABLE void switchToHardLevel();

    QString getPlayButtonLabel() const {return GameStrings::c_PlayButtonLabel;}
    QString getHelpButtonLabel() const {return GameStrings::c_HelpButtonLabel;}
    QString getQuitButtonLabel() const {return GameStrings::c_QuitButtonLabel;}
    QString getSubmitButtonLabel() const {return GameStrings::c_SubmitButtonLabel;}
    QString getResultsButtonLabel() const {return GameStrings::c_ResultsButtonLabel;}
    QString getResetButtonLabel() const {return GameStrings::c_ResetButtonLabel;}
    QString getOkButtonLabel() const {return GameStrings::c_OkButtonLabel;}
    QString getLevelEasyButtonLabel() const {return GameStrings::c_LevelEasyButtonLabel;}
    QString getLevelMediumButtonLabel() const {return GameStrings::c_LevelMediumButtonLabel;}
    QString getLevelHardButtonLabel() const {return GameStrings::c_LevelHardButtonLabel;}
    QString getCloseButtonLabel() const {return GameStrings::c_FatalErrorQuitButtonLabel;}

    bool getIntroPaneVisible() const {return m_IntroPaneVisible;}
    bool getHelpPaneVisible() const {return m_HelpPaneVisible;}
    bool getMainPaneVisible() const {return m_MainPaneVisible;}
    bool getResetEnabled() const {return m_ResetEnabled;}
    bool getErrorOccured() const {return m_ErrorOccured;}

    QString getWindowTitle() const;
    QString getIntroPaneMessage() const {return GameStrings::c_IntroWindowWelcomeMessage;}
    QString getHelpPaneMessage() const {return GameStrings::c_HelpWindowMessage;}
    QString getMainPaneInstructionsMessage() const {return GameStrings::c_InstructionsMessage;}
    QString getMainPaneStatusMessage() const {return m_MainPaneStatusMessage;}
    QString getMainPaneScoreMessage() const {return m_MainPaneScoreMessage;}
    QString getMainPaneWordPairsMessage() const {return m_MainPaneWordPairsMessage;}
    QString getErrorMessage() const {return m_ErrorMessage;}

    int getToolTipDelay() const;
    QString getIntroPaneToolTip() const {return GameStrings::c_IntroWindowToolTip;}
    QString getHelpPaneToolTip() const {return GameStrings::c_HelpWindowToolTip;}
    QString getMainPaneToolTip() const {return GameStrings::c_MainWindowToolTip;}
    QString getErrorPaneToolTip() const {return GameStrings::c_FatalErrorWindowToolTip;}
    QString getHighscoresToolTip() const {return GameStrings::c_HighscoresToolTip;}
    QString getWordPairsToolTip() const {return GameStrings::c_WordPairsToolTip;}
    QString getGameInstructionsToolTip() const {return GameStrings::c_GameInstructionsToolTip;}
    QString getGameStatusToolTip() const {return GameStrings::c_GameStatusToolTip;}
    QString getFirstWordToolTip() const {return GameStrings::c_FirstWordToolTip;}
    QString getSecondWordToolTip() const {return GameStrings::c_SecondWordToolTip;}
    QString getPlayButtonToolTip() const {return GameStrings::c_PlayButtonToolTip;}
    QString getHelpButtonToolTip() const {return GameStrings::c_HelpButtonToolTip;}
    QString getQuitButtonToolTip() const {return GameStrings::c_QuitButtonToolTip;}
    QString getOkButtonToolTip() const {return GameStrings::c_OkButtonToolTip;}
    QString getSubmitButtonToolTip() const {return GameStrings::c_SubmitButtonToolTip;}
    QString getResultsButtonToolTip() const {return GameStrings::c_ResultsButtonToolTip;}
    QString getResetButtonToolTip() const {return GameStrings::c_ResetButtonToolTip;}
    QString getLevelButtonsToolTip() const {return GameStrings::c_LevelButtonsToolTip;}
    QString getCloseButtonToolTip() const {return GameStrings::c_FatalErrorQuitButtonToolTip;}

    QString getPlayButtonShortcut() const {return GameStrings::c_PlayButtonShortcut;}
    QString getHelpButtonShortcut() const {return GameStrings::c_HelpButtonShortcut;}
    QString getQuitButtonShortcut() const {return GameStrings::c_QuitButtonShortcut;}
    QString getOkButtonShortcut() const {return GameStrings::c_OkButtonShortcut;}
    QString getSubmitButtonShortcut() const {return GameStrings::c_SubmitButtonShortcut;}
    QString getResultsButtonShortcut() const {return GameStrings::c_ResultsButtonShortcut;}
    QString getResetButtonShortcut() const {return GameStrings::c_ResetShortcut;}
    QString getLevelEasyButtonShortcut() const {return GameStrings::c_LevelEasyButtonShortcut;}
    QString getLevelMediumButtonShortcut() const {return GameStrings::c_LevelMediumButtonShortcut;}
    QString getLevelHardButtonShortcut() const {return GameStrings::c_LevelHardButtonShortcut;}
    QString getCloseButtonShortcut() const {return GameStrings::c_FatalErrorQuitButtonShortcut;}

    QList<QVariant> getMixedWordsPieces() const;
    int getFirstWordBeginIndex() const;
    int getFirstWordEndIndex() const;
    int getSecondWordBeginIndex() const;
    int getSecondWordEndIndex() const;

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
    void _initMainPane();
    void _updateStatusMessage(Game::StatusCodes statusCode);
    void _setLevel(Game::Level level);
    void _launchErrorPane(const QString& errorMessage);

    enum class Pane
    {
        INTRO,
        HELP,
        MAIN,
        ERROR,
        Nr_Of_Panes
    };

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
