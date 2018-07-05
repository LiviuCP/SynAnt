#include <QHBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QShortcut>
#include "hintswindow.h"
#include "gamestrings.h"

                                                                                                    // constructor creates the layout of the hints window and connects button signals to slots
HintsWindow::HintsWindow(QWidget *parent)
    : QWidget{parent},
      m_MainGameWindow{nullptr}                                                                       // will be assigned a new value when connecting the hints window to main game window by calling updateMainGameWindowPtr()
{
                                                                                                    // first create the upper part of the window, namely the box with the hints message (located in a scroll area); set the hints message
    QHBoxLayout *hintsMsgLayout{new QHBoxLayout{}};
    QScrollArea *hintsMsgScrollArea{new QScrollArea{}};
    QLabel *hintsMessage{new QLabel{}};
    hintsMessage -> setText(GameStrings::c_HelpWindowMessage);
    hintsMsgScrollArea -> setWidget(hintsMessage);
    hintsMsgScrollArea -> setWidgetResizable(true);
    hintsMsgLayout->addWidget(hintsMsgScrollArea,1);
                                                                                                    // second, create the lower part of the window, namely the buttons (Ok, Quit)
    QHBoxLayout *buttonsLayout{new QHBoxLayout{}};
    buttonsLayout -> setSpacing(25);
    QPushButton *okButton{new QPushButton{GameStrings::c_OkButtonLabel}};                            // button used for going back to the main game window
    okButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    okButton -> setToolTip(GameStrings::c_OkButtonToolTip);
    QShortcut *okButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_OkButtonShortcut},this}}; // Ok button primary key combination (shortcut)
    QShortcut *okButtonAltShortcut{new QShortcut{QKeySequence{GameStrings::c_OkButtonAltShortcut},this}};// Ok button alternative key combination (shortcut)
    QPushButton *quitButton{new QPushButton{GameStrings::c_QuitButtonLabel}};                        // button used for quitting the application (window closes)
    quitButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    quitButton -> setToolTip(GameStrings::c_QuitButtonToolTip);
    QShortcut *quitButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_QuitButtonShortcut},this}}; // Quit button key combination (shortcut)
    buttonsLayout -> addWidget(okButton);
    buttonsLayout -> addWidget(quitButton);
                                                                                                    // third, everything to be added to the main widget layout
    QVBoxLayout *mainLayout{new QVBoxLayout{}};
    mainLayout -> addLayout(hintsMsgLayout,1);
    mainLayout -> addLayout(buttonsLayout);
    setLayout(mainLayout);
    setToolTip(GameStrings::c_HelpWindowToolTip);
                                                                                                    // fourth, connect the button signals to slots
    connect(quitButton,&QPushButton::clicked,qApp,&QApplication::quit);
    connect(quitButtonShortcut,&QShortcut::activated,qApp,&QApplication::quit);
    connect(okButton,&QPushButton::clicked,this,&HintsWindow::_onButtonOkClicked);
    connect(okButtonShortcut,&QShortcut::activated,this,&HintsWindow::_onButtonOkClicked);
    connect(okButtonAltShortcut,&QShortcut::activated,this,&HintsWindow::_onButtonOkClicked);
}
                                                                                                    // this function links the hints window to main game window, enables the transition between these when the Ok button is clicked
void HintsWindow::updateMainGameWinPtr(MainGameWindow *mgw)
{
    m_MainGameWindow=mgw;
}
                                                                                                    // slot handles the button Ok click
void HintsWindow::_onButtonOkClicked()
{
    hide();                                                                                         // step 1: hide the hints window
    if (!(m_MainGameWindow -> windowAlreadyAccessed()))                                               // step 2: check if the main game window has already been accessed by user
    {
        m_MainGameWindow -> getFirstTwoWords();                                                       // if not, retrieve the first 2 words, mix them, update the main game window with the mixed words, update errors/results box
    }
    m_MainGameWindow -> show();                                                                       // step 3: show updated main game window, pass control to user (who should enter the words and/or press a button)
}

