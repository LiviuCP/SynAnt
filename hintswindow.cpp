#include <QHBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QShortcut>
#include "hintswindow.h"
#include "gamestrings.h"


HintsWindow::HintsWindow(QWidget *parent)
    : QWidget{parent},
      m_MainGameWindow{nullptr}
{
    QHBoxLayout *hintsMsgLayout{new QHBoxLayout{}};
    QScrollArea *hintsMsgScrollArea{new QScrollArea{}};
    QLabel *hintsMessage{new QLabel{}};
    hintsMessage -> setText(GameStrings::c_HelpWindowMessage);
    hintsMsgScrollArea -> setWidget(hintsMessage);
    hintsMsgScrollArea -> setWidgetResizable(true);
    hintsMsgLayout->addWidget(hintsMsgScrollArea,1);

    QHBoxLayout *buttonsLayout{new QHBoxLayout{}};
    buttonsLayout -> setSpacing(25);
    QPushButton *okButton{new QPushButton{GameStrings::c_OkButtonLabel}};
    okButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    okButton -> setToolTip(GameStrings::c_OkButtonToolTip);
    QShortcut *okButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_OkButtonShortcut},this}};
    QShortcut *okButtonAltShortcut{new QShortcut{QKeySequence{GameStrings::c_OkButtonAltShortcut},this}};
    QPushButton *quitButton{new QPushButton{GameStrings::c_QuitButtonLabel}};
    quitButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    quitButton -> setToolTip(GameStrings::c_QuitButtonToolTip);
    QShortcut *quitButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_QuitButtonShortcut},this}};
    buttonsLayout -> addWidget(okButton);
    buttonsLayout -> addWidget(quitButton);

    QVBoxLayout *mainLayout{new QVBoxLayout{}};
    mainLayout -> addLayout(hintsMsgLayout,1);
    mainLayout -> addLayout(buttonsLayout);
    setLayout(mainLayout);
    setToolTip(GameStrings::c_HelpWindowToolTip);

    connect(quitButton,&QPushButton::clicked,qApp,&QApplication::quit);
    connect(quitButtonShortcut,&QShortcut::activated,qApp,&QApplication::quit);
    connect(okButton,&QPushButton::clicked,this,&HintsWindow::_onButtonOkClicked);
    connect(okButtonShortcut,&QShortcut::activated,this,&HintsWindow::_onButtonOkClicked);
    connect(okButtonAltShortcut,&QShortcut::activated,this,&HintsWindow::_onButtonOkClicked);
}

void HintsWindow::updateMainGameWinPtr(MainGameWindow *mgw)
{
    m_MainGameWindow=mgw;
}

void HintsWindow::_onButtonOkClicked()
{
    hide();
    if (!(m_MainGameWindow -> windowAlreadyAccessed()))
    {
        m_MainGameWindow -> getFirstTwoWords();
    }
    m_MainGameWindow -> show();
}

