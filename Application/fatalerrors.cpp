#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QShortcut>

#include "fatalerrors.h"
#include "gamestrings.h"

FatalErrors::FatalErrors(QWidget *parent)
    : QWidget{parent},
      m_ErrorText{}
{
    QHBoxLayout *messageLayout{new QHBoxLayout{}};
    m_pErrorMessage = new QLabel{};
    m_pErrorMessage -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_pErrorMessage -> setFrameStyle(QFrame::StyledPanel);
    messageLayout -> addWidget(m_pErrorMessage,1);

    QHBoxLayout *closeButtonLayout{new QHBoxLayout{}};
    QPushButton *closeButton{new QPushButton{GameStrings::c_FatalErrorQuitButtonLabel}};
    closeButton -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    closeButton -> setToolTip(GameStrings::c_FatalErrorQuitButtonToolTip);
    QShortcut *closeButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_FatalErrorQuitButtonShortcut},this}};
    closeButtonLayout -> addWidget(closeButton,1);

    QVBoxLayout *mainLayout{new QVBoxLayout{}};
    mainLayout -> addLayout(messageLayout,1);
    mainLayout -> addLayout(closeButtonLayout);
    setLayout(mainLayout);
    setToolTip(GameStrings::c_FatalErrorWindowToolTip);

    bool connected{connect(closeButton,&QPushButton::clicked,qApp,&QApplication::quit)};
    Q_ASSERT(connected);
    connected = connect(closeButtonShortcut,&QShortcut::activated,qApp,&QApplication::quit);
    Q_ASSERT(connected);
}

void FatalErrors::setFatalErrorText(const QString &text)
{
    m_ErrorText = text;
    m_pErrorMessage->setText(m_ErrorText);
}
