#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QShortcut>
#include "fatalerrors.h"

FatalErrors::FatalErrors(QWidget *parent)                                                           // constructor
    : QWidget{parent},
      m_ErrorText{}                                                                                   // initially empty, will be filled in when writing the error message by calling setFatalErrorText()
{
                                                                                                    // first create the upper part of the window, namely the box with the message requesting input
    QHBoxLayout *messageLayout{new QHBoxLayout{}};
    m_ErrorMessage = new QLabel{};                                                                    // initially the label is empty. A text will be setup only if a fatal error occurs
    m_ErrorMessage -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_ErrorMessage -> setFrameStyle(QFrame::StyledPanel);
    messageLayout -> addWidget(m_ErrorMessage,1);
                                                                                                    // second, create the lower part of the window, namely the Ok button
    QHBoxLayout *okButtonLayout{new QHBoxLayout{}};
    QPushButton *okButton{new QPushButton{"Ok"}};
    okButton -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    okButton -> setToolTip("Click or hit ALT+O/ENTER to exit the game.");
    QShortcut *okButtonShortcut{new QShortcut{QKeySequence{"ALT+O"},this}};                         // Ok button primary key combination (shortcut)
    QShortcut *okButtonAltShortcut{new QShortcut{QKeySequence{"RETURN"},this}};                     // Ok button alternative key combination (shortcut)
    okButtonLayout -> addWidget(okButton,1);
                                                                                                    // third, everything to be added to the main widget layout
    QVBoxLayout *mainLayout{new QVBoxLayout{}};
    mainLayout -> addLayout(messageLayout,1);
    mainLayout -> addLayout(okButtonLayout);
    setLayout(mainLayout);
    setToolTip("A fatal error occurred. Corrective actions are required.");
                                                                                                    // fourth, connect signals to slots
    connect(okButton,&QPushButton::clicked,qApp,&QApplication::quit);
    connect(okButtonShortcut,&QShortcut::activated,qApp,&QApplication::quit);
    connect(okButtonAltShortcut,&QShortcut::activated,qApp,&QApplication::quit);
}
                                                                                                    // this function is called from outside the class when the exception is encountered
void FatalErrors::setFatalErrorText(const QString &text)
{
    m_ErrorText = text;
    m_ErrorMessage->setText(m_ErrorText);
}
