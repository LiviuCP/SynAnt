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

class HintsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit HintsWindow(QWidget *parent = nullptr);
signals:
    void switchedHintsToMain();
};

#endif // HINTSWINDOW_H
