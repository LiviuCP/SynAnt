#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QShortcut>

#include "appmanager.h"
#include "introductionwindow.h"
#include "gamestrings.h"

IntroductionWindow::IntroductionWindow(QWidget *parent)
    : QWidget{parent}
{
    QHBoxLayout *welcomeMsgLayout{new QHBoxLayout{}};
    QLabel *welcomeMessage{new QLabel{}};
    welcomeMessage -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    welcomeMsgLayout -> addWidget(welcomeMessage,1);
    welcomeMessage -> setText(GameStrings::c_IntroWindowWelcomeMessage);

    QHBoxLayout *buttonsLayout{new QHBoxLayout{}};
    buttonsLayout -> setSpacing(25);
    QPushButton *playButton{new QPushButton{GameStrings::c_PlayButtonLabel}};
    playButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    playButton -> setToolTip(GameStrings::c_PlayButtonToolTip);
    QShortcut *playButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_PlayButtonShortcut},this}};
    QShortcut *playButtonAltShortcut{new QShortcut{QKeySequence{GameStrings::c_PlayButtonAltShortcut},this}};
    QPushButton *hintsButton{new QPushButton{GameStrings::c_HelpButtonLabel}};
    hintsButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    hintsButton -> setToolTip(GameStrings::c_HelpButtonToolTip);
    QShortcut *hintsButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_HelpButtonShortcut},this}};
    QPushButton *quitButton{new QPushButton{GameStrings::c_QuitButtonLabel}};
    quitButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    quitButton -> setToolTip(GameStrings::c_QuitButtonToolTip);
    QShortcut *quitButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_QuitButtonShortcut},this}};
    buttonsLayout -> addWidget(playButton);
    buttonsLayout -> addWidget(hintsButton);
    buttonsLayout -> addWidget(quitButton);

    QVBoxLayout *mainLayout{new QVBoxLayout{}};
    mainLayout -> addLayout(welcomeMsgLayout,1);
    mainLayout -> addLayout(buttonsLayout,0);
    setLayout(mainLayout);
    setToolTip(GameStrings::c_IntroWindowToolTip);

    bool connected{connect(quitButton,&QPushButton::clicked,qApp,&QApplication::quit)};
    Q_ASSERT(connected);
    connected = connect(quitButtonShortcut,&QShortcut::activated,qApp,&QApplication::quit);
    Q_ASSERT(connected);
    connected = connect(hintsButton,&QPushButton::clicked,this,&IntroductionWindow::switchedIntroToHints);
    Q_ASSERT(connected);
    connected = connect(hintsButtonShortcut,&QShortcut::activated,this,&IntroductionWindow::switchedIntroToHints);
    Q_ASSERT(connected);
    connected = connect(playButton,&QPushButton::clicked,this,&IntroductionWindow::switchedIntroToMain);
    Q_ASSERT(connected);
    connected = connect(playButtonShortcut,&QShortcut::activated,this,&IntroductionWindow::switchedIntroToMain);
    Q_ASSERT(connected);
    connected = connect(playButtonAltShortcut,&QShortcut::activated,this,&IntroductionWindow::switchedIntroToMain);
    Q_ASSERT(connected);
}



