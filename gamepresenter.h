#ifndef GAMEPRESENTER_H
#define GAMEPRESENTER_H

#include <QObject>

class GamePresenter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool introPaneVisible READ getIntroPaneVisible NOTIFY introPaneVisibleChanged)
    Q_PROPERTY(bool helpPaneVisible READ getHelpPaneVisible NOTIFY helpPaneVisibleChanged)
    Q_PROPERTY(bool mainPaneVisible READ getMainPaneVisible NOTIFY mainPaneVisibleChanged)

public:
    explicit GamePresenter(QObject *parent = nullptr);

    Q_INVOKABLE void switchToHelpPane();
    Q_INVOKABLE void switchToMainPane();

    bool getIntroPaneVisible() {return m_IntroPaneVisible;}
    bool getHelpPaneVisible() {return m_HelpPaneVisible;}
    bool getMainPaneVisible() {return m_MainPaneVisible;}

signals:
    Q_SIGNAL void introPaneVisibleChanged();
    Q_SIGNAL void helpPaneVisibleChanged();
    Q_SIGNAL void mainPaneVisibleChanged();

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

    Pane m_CurrentPane;
};

#endif // GAMEPRESENTER_H
