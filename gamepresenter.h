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
    Q_PROPERTY(bool introPaneVisible READ getIntroPaneVisible NOTIFY introPaneVisibleChanged)
    Q_PROPERTY(bool helpPaneVisible READ getHelpPaneVisible NOTIFY helpPaneVisibleChanged)
    Q_PROPERTY(bool mainPaneVisible READ getMainPaneVisible NOTIFY mainPaneVisibleChanged)
    Q_PROPERTY(QString introPaneMessage READ getIntroPaneMessage CONSTANT)
    Q_PROPERTY(QString helpPaneMessage READ getHelpPaneMessage CONSTANT)
    Q_PROPERTY(QString mainPaneInstructionsMessage READ getMainPaneInstructionsMessage CONSTANT)
    Q_PROPERTY(QString mainPaneStatusMessage READ getMainPaneStatusMessage NOTIFY mainPaneStatusMessageChanged)

public:
    explicit GamePresenter(QObject *parent = nullptr);

    Q_INVOKABLE void switchToHelpPane();
    Q_INVOKABLE void switchToMainPane();
    Q_INVOKABLE void handleResultsRequest();
    Q_INVOKABLE bool handleSubmitRequest(const QString& firstWord, const QString& secondWord);
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

    QString getIntroPaneMessage() const {return m_IntroPaneMessage;}
    QString getHelpPaneMessage() const {return m_HelpPaneMessage;}
    QString getMainPaneInstructionsMessage() const {return m_MainPaneInstructionsMessage;}
    QString getMainPaneStatusMessage() const {return m_MainPaneStatusMessage;}

signals:
    Q_SIGNAL void introPaneVisibleChanged();
    Q_SIGNAL void helpPaneVisibleChanged();
    Q_SIGNAL void mainPaneVisibleChanged();
    Q_SIGNAL void mainPaneStatusMessageChanged();

public slots:

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

    QString m_IntroPaneMessage;
    QString m_HelpPaneMessage;
    QString m_MainPaneInstructionsMessage;
    QString m_MainPaneStatusMessage;

    Pane m_CurrentPane;

    WordMixer* m_pWordMixer;
    ScoreItem* m_pScoreItem;
};

#endif // GAMEPRESENTER_H
