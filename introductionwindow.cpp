#include <QHBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QVBoxLayout>
#include <QShortcut>
#include "introductionwindow.h"
#include "gamestrings.h"
                                                                                                    // constructor creates the layout of the introduction window and connects button signals to slots

IntroductionWindow::IntroductionWindow(QWidget *parent)
    : QWidget{parent},
      m_HintsWindow{nullptr},                                                                         // will be assigned a new value when connecting the introduction window to hints window by calling updateHintsWindowPtr()
      m_MainGameWindow{nullptr}                                                                       // will be assigned a new value when connecting the introduction window to main game window by calling updateMainGameWindowPtr()
{
                                                                                                    // first create the upper part of the window, namely the box with the welcome message; set the welcome message
    QHBoxLayout *welcomeMsgLayout{new QHBoxLayout{}};
    QLabel *welcomeMessage{new QLabel{}};
    welcomeMessage -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    welcomeMsgLayout -> addWidget(welcomeMessage,1);
    welcomeMessage -> setText(GameStrings::c_IntroWindowWelcomeMessage);
                                                                                                    // second, create the lower part of the window, namely the buttons (Play, Hints, Quit)
    QHBoxLayout *buttonsLayout{new QHBoxLayout{}};
    buttonsLayout -> setSpacing(25);
    QPushButton *playButton{new QPushButton{GameStrings::c_PlayButtonLabel}};                        // button used for launching the actual game (main game window opens)
    playButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    playButton -> setToolTip(GameStrings::c_PlayButtonToolTip);
    QShortcut *playButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_PlayButtonShortcut},this}};    // Play button primary key combination (shortcut)
    QShortcut *playButtonAltShortcut{new QShortcut{QKeySequence{GameStrings::c_PlayButtonAltShortcut},this}}; // Play button alternative key combination (shortcut)
    QPushButton *hintsButton{new QPushButton{GameStrings::c_HelpButtonLabel}};                       // button used for accessing the game instructions (hints window opens)
    hintsButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    hintsButton -> setToolTip(GameStrings::c_HelpButtonToolTip);
    QShortcut *hintsButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_HelpButtonShortcut},this}}; // Help (hints) button key combination (shortcut)
    QPushButton *quitButton{new QPushButton{GameStrings::c_QuitButtonLabel}};                        // for quitting the application (window closes)
    quitButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    quitButton -> setToolTip(GameStrings::c_QuitButtonToolTip);
    QShortcut *quitButtonShortcut{new QShortcut{QKeySequence{GameStrings::c_QuitButtonShortcut},this}}; // Quit button key combination (shortcut)
    buttonsLayout -> addWidget(playButton);
    buttonsLayout -> addWidget(hintsButton);
    buttonsLayout -> addWidget(quitButton);
                                                                                                    // third, everything to be added to the main widget layout
    QVBoxLayout *mainLayout{new QVBoxLayout{}};
    mainLayout -> addLayout(welcomeMsgLayout,1);
    mainLayout -> addLayout(buttonsLayout,0);
    setLayout(mainLayout);
    setToolTip(GameStrings::c_IntroWindowToolTip);
                                                                                                    // fourth, connect the button and shortcut signals to slots
    connect(quitButton,&QPushButton::clicked,qApp,&QApplication::quit);
    connect(quitButtonShortcut,&QShortcut::activated,qApp,&QApplication::quit);
    connect(hintsButton,&QPushButton::clicked,this,&IntroductionWindow::_onButtonHintsClicked);
    connect(hintsButtonShortcut,&QShortcut::activated,this,
                                                   &IntroductionWindow::_onButtonHintsClicked);
    connect(playButton,&QPushButton::clicked,this,&IntroductionWindow::_onButtonPlayClicked);
    connect(playButtonShortcut,&QShortcut::activated,this,
                                                    &IntroductionWindow::_onButtonPlayClicked);
    connect(playButtonAltShortcut,&QShortcut::activated,this,
                                                    &IntroductionWindow::_onButtonPlayClicked);
}
                                                                                                    // this function links the introduction window to hints window by assigning the pointer to the HintsWindow object
void IntroductionWindow::updateHintsWinPtr(HintsWindow *hw)
{
    m_HintsWindow = hw;
}
                                                                                                    // this function links the introduction window to main game window, enables the transition between these when the Play button is clicked
void IntroductionWindow::updateMainGameWinPtr(MainGameWindow *mgw)
{
    m_MainGameWindow = mgw;
}
                                                                                                    // slot handles the button Play click
void IntroductionWindow::_onButtonPlayClicked()
{
    hide();                                                                                         // step 1: hide the introduction window
    m_MainGameWindow -> getFirstTwoWords();                                                           // step 2: retrieve words, mix them, update main game window with mixed words, update results/errors section
    m_MainGameWindow -> show();                                                                       // step 3: show the main game window, control passed to user (who should enter the words and/or click a button)
}
                                                                                                    // slot handles the button Hints click
void IntroductionWindow::_onButtonHintsClicked()
{
    hide();
    m_HintsWindow -> show();
}



