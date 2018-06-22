#include <QHBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QShortcut>
#include "hintswindow.h"

                                                                                                    // constructor creates the layout of the hints window and connects button signals to slots
HintsWindow::HintsWindow(QWidget *parent)
    : QWidget{parent},
      mainGameWindow{nullptr}                                                                       // will be assigned a new value when connecting the hints window to main game window by calling updateMainGameWindowPtr()
{
                                                                                                    // first create the upper part of the window, namely the box with the hints message (located in a scroll area); set the hints message
    QHBoxLayout *hintsMsgLayout{new QHBoxLayout{}};
    QScrollArea *hintsMsgScrollArea{new QScrollArea{}};
    QLabel *hintsMessage{new QLabel{}};
    QString hintsMessageText{"\n\n 1) The words are either in Romanian or English language.\n"};
    hintsMessageText += "\n 2) Minimum word size is 5 characters. \n";
    hintsMessageText += "\n 3) Each word is divided in equal parts, ";
    hintsMessageText += "except the last part, which might have a smaller number of characters.\n";
    hintsMessageText += "\n 4) The parts from both words are mixed with each other.\n";
    hintsMessageText += "\n 5) The first (beginning) part of each word is marked blue.\n";
    hintsMessageText += "\n 6) The last (end) part of each word is marked red.\n";
    hintsMessageText += "\n 7) The words can either be synonyms or antonyms.\n";
    hintsMessageText += "\n 8) Please enter only small letter characters. No numbers, spaces, ";
    hintsMessageText += "capital letters or any other characters are allowed.\n";
    hintsMessageText += "\n 9) You need to enter both words to win the round.\n";
    hintsMessageText += "\n10) After entering the 2 words, press Submit for validating input.\n";
    hintsMessageText += "\n11) If the input is not correct, the user will be asked ";
    hintsMessageText += "to re-enter the words.\n";
    hintsMessageText += "\n12) If the 2 words have been correctly determined, ";
    hintsMessageText += "the program will display 2 new mixed words for guessing.\n";
    hintsMessageText += "\n13) 2 new words will also be presented for guessing ";
    hintsMessageText += "if the Show results button is pressed.\n";
    hintsMessageText += "\n14) The status box is located on the right side of the radio buttons. ";
    hintsMessageText += "It displays if the words have been correctly entered or not.\n";
    hintsMessageText += "\n15) The status box also displays the right solution ";
    hintsMessageText += "when the Show results button is pressed.\n";
    hintsMessageText += "\n16) The scores and number of word pairs are displayed in the upper part ";
    hintsMessageText += "of the window.\n";
    hintsMessageText += "\n17) The format of the score and number of word pairs is: ";
    hintsMessageText += "available/total.\n";
    hintsMessageText += "\n18) The available score and number of pairs are increased only when ";
    hintsMessageText += "a correct guess is made.\n";
    hintsMessageText += "\n19) Press the Reset button to set the scores and number of pairs to 0.\n";
    hintsMessageText += "\n20) Choose the desired difficulty level by using the radio buttons.\n";
    hintsMessageText += "\n21) The difficulty level determines the size of the parts in which ";
    hintsMessageText += "the words are divided.\n";
    hintsMessageText += "\n22) It also determines the obtained score for correctly guessing ";
    hintsMessageText += "a pair of words.\n";
    hintsMessageText += "\n23) The obtained score per pair is: 1 point (easy), 2 points (medium), ";
    hintsMessageText += "4 points (hard).\n";
    hintsMessageText += "\n24) When changing the difficulty level, the score and number of word ";
    hintsMessageText += "pairs remain unchanged.\n";
    hintsMessageText += "\n25) However, when the level is changed, a new pair of words ";
    hintsMessageText += "(divided according to the new level) will be presented to the user.\n";
    hintsMessageText += "\n26) Each button has an appropriate key combination (shortcut).\n";
    hintsMessageText += "\n27) For all buttons (except Show results) the key combination is ";
    hintsMessageText += "ALT+[first character of the button name].\n";
    hintsMessageText += "\n28) For example the key combination for Quit is ALT+Q.\n";
    hintsMessageText += "\n29) For the Show results button the key combination is ALT+B.\n";
    hintsMessageText += "\n30) For buttons Play, Ok and Submit the alternative key combination is ";
    hintsMessageText += "ENTER.\n";
    hintsMessageText += "\n31) Hit ALT+1/2/3 for setting the difficulty levels to ";
    hintsMessageText += "easy/medium/hard.\n";
    hintsMessageText += "\n\n\n\n\t\t\t\tPress Ok to proceed (or return) to game.\n";
    hintsMessage -> setText(hintsMessageText);
    hintsMsgScrollArea -> setWidget(hintsMessage);
    hintsMsgScrollArea -> setWidgetResizable(true);
    hintsMsgLayout->addWidget(hintsMsgScrollArea,1);
                                                                                                    // second, create the lower part of the window, namely the buttons (Ok, Quit)
    QHBoxLayout *buttonsLayout{new QHBoxLayout{}};
    buttonsLayout -> setSpacing(25);
    QPushButton *okButton{new QPushButton{"Ok"}};                                                   // button used for going back to the main game window
    okButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    okButton -> setToolTip("Click or hit ALT+O/ENTER to return to game.");
    QShortcut *okButtonShortcut{new QShortcut{QKeySequence{"ALT+O"},this}};                         // Ok button primary key combination (shortcut)
    QShortcut *okButtonAltShortcut{new QShortcut{QKeySequence{"RETURN"},this}};                     // Ok button alternative key combination (shortcut)
    QPushButton *quitButton{new QPushButton{"Quit"}};                                               // button used for quitting the application (window closes)
    quitButton -> setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    quitButton -> setToolTip("Click or hit ALT+Q to exit application.");
    QShortcut *quitButtonShortcut{new QShortcut{QKeySequence{"ALT+Q"},this}};                       // Quit button key combination (shortcut)
    buttonsLayout -> addWidget(okButton);
    buttonsLayout -> addWidget(quitButton);
                                                                                                    // third, everything to be added to the main widget layout
    QVBoxLayout *mainLayout{new QVBoxLayout{}};
    mainLayout -> addLayout(hintsMsgLayout,1);
    mainLayout -> addLayout(buttonsLayout);
    setLayout(mainLayout);
    setToolTip("Please read the detailed game instructions.");
                                                                                                    // fourth, connect the button signals to slots
    connect(quitButton,&QPushButton::clicked,qApp,&QApplication::quit);
    connect(quitButtonShortcut,&QShortcut::activated,qApp,&QApplication::quit);
    connect(okButton,&QPushButton::clicked,this,&HintsWindow::slotButtonOkClicked);
    connect(okButtonShortcut,&QShortcut::activated,this,&HintsWindow::slotButtonOkClicked);
    connect(okButtonAltShortcut,&QShortcut::activated,this,&HintsWindow::slotButtonOkClicked);
}
                                                                                                    // this function links the hints window to main game window, enables the transition between these when the Ok button is clicked
void HintsWindow::updateMainGameWinPtr(MainGameWindow *mgw)
{
    mainGameWindow=mgw;
}
                                                                                                    // slot handles the button Ok click
void HintsWindow::slotButtonOkClicked()
{
    hide();                                                                                         // step 1: hide the hints window
    if (!(mainGameWindow -> windowAlreadyAccessed()))                                               // step 2: check if the main game window has already been accessed by user
    {
        mainGameWindow -> getFirstTwoWords();                                                       // if not, retrieve the first 2 words, mix them, update the main game window with the mixed words, update errors/results box
    }
    mainGameWindow -> show();                                                                       // step 3: show updated main game window, pass control to user (who should enter the words and/or press a button)
}

