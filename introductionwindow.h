                                                                                                    /*

                                                                                                       This class implements the first program window, namely the introduction window

                                                                                                       From this window the user can execute following actions:
                                                                                                       1) Jump to the game window by clicking the Play button
                                                                                                       2) Jump to the hints (help) window by clicking the Hints button
                                                                                                       3) Quit the game by clicking the Quit button

                                                                                                    */

#ifndef INTRODUCTIONWINDOW_H
#define INTRODUCTIONWINDOW_H

#include <QWidget>
#include <QPushButton>
#include "hintswindow.h"
#include "maingamewindow.h"

class IntroductionWindow : public QWidget
{
    Q_OBJECT
public:
    explicit IntroductionWindow(QWidget *parent = 0);                                               // constructor will build all window items and connect the button signals to slots
    void updateHintsWinPtr(HintsWindow *hw);                                                        // links the introduction window to hints window by assigning the pointer to the HintsWindow object
    void updateMainGameWinPtr(MainGameWindow *mgw);                                                 // links the introduction window to main game window, enables the transition between these when button Play is clicked
private slots:
    void slotButtonPlayClicked();                                                                   // executes when button Play is clicked
    void slotButtonHintsClicked();                                                                  // executes when button Hints is clicked
private:
    HintsWindow *hintsWindow;                                                                       // for opening the hints window when the user presses the Hints button
    MainGameWindow *mainGameWindow;                                                                 // for opening the main game window when user presses the Play button
};

#endif // INTRODUCTIONWINDOW_H
