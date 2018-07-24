#ifndef GAMEPRESENTER_H
#define GAMEPRESENTER_H

#include <QObject>

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
    Q_PROPERTY(bool introPaneVisible READ getIntroPaneVisible NOTIFY introPaneVisibleChanged)
    Q_PROPERTY(bool helpPaneVisible READ getHelpPaneVisible NOTIFY helpPaneVisibleChanged)
    Q_PROPERTY(bool mainPaneVisible READ getMainPaneVisible NOTIFY mainPaneVisibleChanged)
    Q_PROPERTY(QString windowTitle READ getWindowTitle NOTIFY windowTitleChanged)
    Q_PROPERTY(QString introPaneMessage READ getIntroPaneMessage CONSTANT)
    Q_PROPERTY(QString helpPaneMessage READ getHelpPaneMessage CONSTANT)
    Q_PROPERTY(QString mainPaneInstructionsMessage READ getMainPaneInstructionsMessage CONSTANT)
    Q_PROPERTY(QString mainPaneStatusMessage READ getMainPaneStatusMessage NOTIFY mainPaneStatusMessageChanged)
    Q_PROPERTY(QString mainPaneScoreMessage READ getMainPaneScoreMessage NOTIFY mainPaneScoreMessageChanged)
    Q_PROPERTY(QString mainPaneNrOfPairsMessage READ getMainPaneNrOfPairsMessage NOTIFY mainPaneNrOfPairsMessageChanged)
    Q_PROPERTY(int toolTipDelay READ getToolTipDelay CONSTANT)
    Q_PROPERTY(QString playButtonToolTip READ getPlayButtonToolTip CONSTANT)
    Q_PROPERTY(QString helpButtonToolTip READ getHelpButtonToolTip CONSTANT)
    Q_PROPERTY(QString quitButtonToolTip READ getQuitButtonToolTip CONSTANT)
    Q_PROPERTY(QString okButtonToolTip READ getOkButtonToolTip CONSTANT)
    Q_PROPERTY(QString submitButtonToolTip READ getSubmitButtonToolTip CONSTANT)
    Q_PROPERTY(QString resultsButtonToolTip READ getResultsButtonToolTip CONSTANT)
    Q_PROPERTY(QString resetButtonToolTip READ getResetButtonToolTip CONSTANT)
    Q_PROPERTY(QString levelButtonsToolTip READ getLevelButtonsToolTip CONSTANT)
    Q_PROPERTY(bool resetEnabled READ getResetEnabled NOTIFY resetEnabledChanged)

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

    bool getIntroPaneVisible() const {return m_IntroPaneVisible;}
    bool getHelpPaneVisible() const {return m_HelpPaneVisible;}
    bool getMainPaneVisible() const {return m_MainPaneVisible;}
    bool getResetEnabled() const {return m_ResetEnabled;}

    QString getWindowTitle() const;
    QString getIntroPaneMessage() const {return m_IntroPaneMessage;}
    QString getHelpPaneMessage() const {return m_HelpPaneMessage;}
    QString getMainPaneInstructionsMessage() const {return m_MainPaneInstructionsMessage;}
    QString getMainPaneStatusMessage() const {return m_MainPaneStatusMessage;}
    QString getMainPaneScoreMessage() const {return m_MainPaneScoreMessage;}
    QString getMainPaneNrOfPairsMessage() const {return m_MainPaneNrOfPairsMessage;}

    int getToolTipDelay() const {return m_ToolTipDelay;}
    QString getPlayButtonToolTip() const {return GameStrings::c_PlayButtonToolTip;}
    QString getHelpButtonToolTip() const {return GameStrings::c_HelpButtonToolTip;}
    QString getQuitButtonToolTip() const {return GameStrings::c_QuitButtonToolTip;}
    QString getOkButtonToolTip() const {return GameStrings::c_OkButtonToolTip;}
    QString getSubmitButtonToolTip() const {return GameStrings::c_SubmitButtonToolTip;}
    QString getResultsButtonToolTip() const {return GameStrings::c_ResultsButtonToolTip;}
    QString getResetButtonToolTip() const {return GameStrings::c_ResetButtonToolTip;}
    QString getLevelButtonsToolTip() const {return GameStrings::c_LevelButtonsToolTip;}

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

signals:
    Q_SIGNAL void windowTitleChanged();
    Q_SIGNAL void introPaneVisibleChanged();
    Q_SIGNAL void helpPaneVisibleChanged();
    Q_SIGNAL void mainPaneVisibleChanged();
    Q_SIGNAL void resetEnabledChanged();
    Q_SIGNAL void mainPaneStatusMessageChanged();
    Q_SIGNAL void mainPaneScoreMessageChanged();
    Q_SIGNAL void mainPaneNrOfPairsMessageChanged();
    Q_SIGNAL void levelChanged(Game::Level level);

private slots:
    void _onStatisticsUpdated();
private:
    void _initMainPane();
    void _updateStatusMessage(Game::StatusCodes statusCode);
    void _setLevel(Game::Level level);

    enum class Pane
    {
        INTRO,
        HELP,
        MAIN,
        Nr_Of_Panes
    };

    bool m_IntroPaneVisible;
    bool m_HelpPaneVisible;
    bool m_MainPaneVisible;
    bool m_MainPaneInitialized;

    bool m_ResetEnabled;

    QString m_WindowTitle;
    QString m_IntroPaneMessage;
    QString m_HelpPaneMessage;
    QString m_MainPaneInstructionsMessage;
    QString m_MainPaneStatusMessage;
    QString m_MainPaneScoreMessage;
    QString m_MainPaneNrOfPairsMessage;

    Pane m_CurrentPane;

    WordMixer* m_pWordMixer;
    ScoreItem* m_pScoreItem;

    int m_ToolTipDelay;
};

#endif // GAMEPRESENTER_H
