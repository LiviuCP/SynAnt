#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QShortcut>

#include "appmanager.h"
#include "hintswindow.h"
#include "gamestrings.h"

HintsWindow::HintsWindow(QWidget *parent)
    : QWidget{parent}
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

    bool connected{connect(quitButton,&QPushButton::clicked,qApp,&QApplication::quit)};
    Q_ASSERT(connected);
    connected = connect(quitButtonShortcut,&QShortcut::activated,qApp,&QApplication::quit);
    Q_ASSERT(connected);
    connected = connect(okButton,&QPushButton::clicked,this,&HintsWindow::switchedHintsToMain);
    Q_ASSERT(connected);
    connected = connect(okButtonShortcut,&QShortcut::activated,this,&HintsWindow::switchedHintsToMain);
    Q_ASSERT(connected);
}
