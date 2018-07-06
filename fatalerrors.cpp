#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QShortcut>
#include "fatalerrors.h"
#include "gamestrings.h"

FatalErrors::FatalErrors(QWidget *parent)
    : QWidget{parent},
      m_ErrorText{}
{
    QHBoxLayout *messageLayout{new QHBoxLayout{}};
    m_ErrorMessage = new QLabel{};
    m_ErrorMessage -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_ErrorMessage -> setFrameStyle(QFrame::StyledPanel);
    messageLayout -> addWidget(m_ErrorMessage,1);

    QHBoxLayout *okButtonLayout{new QHBoxLayout{}};
    QPushButton *okButton{new QPushButton{GameStrings::c_FatalErrorQuitButtonLabel}};
    okButton -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    okButton -> setToolTip(GameStrings::c_FatalErrorQuitButtonToolTip);
    QShortcut *okButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_FatalErrorQuitButtonShortcut},this}};
    QShortcut *okButtonAltShortcut{new QShortcut{QKeySequence{GameStrings::c_FatalErrorQuitButtonAltShortcut},this}};
    okButtonLayout -> addWidget(okButton,1);

    QVBoxLayout *mainLayout{new QVBoxLayout{}};
    mainLayout -> addLayout(messageLayout,1);
    mainLayout -> addLayout(okButtonLayout);
    setLayout(mainLayout);
    setToolTip(GameStrings::c_FatalErrorWindowToolTip);

    connect(okButton,&QPushButton::clicked,qApp,&QApplication::quit);
    connect(okButtonShortcut,&QShortcut::activated,qApp,&QApplication::quit);
    connect(okButtonAltShortcut,&QShortcut::activated,qApp,&QApplication::quit);
}

void FatalErrors::setFatalErrorText(const QString &text)
{
    m_ErrorText = text;
    m_ErrorMessage->setText(m_ErrorText);
}
