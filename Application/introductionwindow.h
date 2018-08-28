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

class IntroductionWindow : public QWidget
{
    Q_OBJECT
public:
    explicit IntroductionWindow(QWidget *parent = nullptr);
signals:
    void switchedIntroToHints();
    void switchedIntroToMain();
};

#endif // INTRODUCTIONWINDOW_H