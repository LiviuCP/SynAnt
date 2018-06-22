                                                                                                    /*

                                                                                                       This class implements the hints (help) window

                                                                                                       This window can be accessed from 2 possible locations, in both cases by clicking the Hints button:
                                                                                                        - from the introduction window
                                                                                                        - from the main game window

                                                                                                       The role of this window is to provide instructions about how the game needs to be played

                                                                                                       From this window the user can execute following actions:
                                                                                                       1) Jump to the main game window by clicking the Ok button
                                                                                                       2) Quit the game by clicking the Quit button

                                                                                                    */

#ifndef HINTSWINDOW_H
#define HINTSWINDOW_H

#include <QWidget>
#include <QPushButton>
#include "maingamewindow.h"

class MainGameWindow;                                                                               // forward declaration necessary (HW contains pointer to MGW and viceversa)
class HintsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit HintsWindow(QWidget *parent = 0);                                                      // constructor will build all window items and connect the button signals to slots
    void updateMainGameWinPtr(MainGameWindow *mgw);                                                 // links the hints window to main game window, enables the transition between these when the Ok button is clicked
private slots:
    void slotButtonOkClicked();                                                                     // executes when button Play is clicked
private:
    MainGameWindow *mainGameWindow;                                                                 // for opening the main game window when user presses the Ok button
};

#endif // HINTSWINDOW_H
