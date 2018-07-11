#ifndef GAMEPRESENTER_H
#define GAMEPRESENTER_H

#include <QObject>

class GamePresenter : public QObject
{
    Q_OBJECT
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
    enum class Pane
    {
        INTRO,
        HELP,
        MAIN,
        NR_OF_PANES
    };

    bool m_IntroPaneVisible;
    bool m_HelpPaneVisible;
    bool m_MainPaneVisible;

    QString m_IntroPaneMessage;
    QString m_HelpPaneMessage;
    QString m_MainPaneInstructionsMessage;
    QString m_MainPaneStatusMessage;

    Pane m_CurrentPane;
};

#endif // GAMEPRESENTER_H
