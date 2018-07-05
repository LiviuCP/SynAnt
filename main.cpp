                                                                                                    /*
                                                                                                       This program will implement a game which is played as follows:
                                                                                                       - the application reads 2 words from a file
                                                                                                       - the words are divided into small pieces
                                                                                                       - all pieces are mixed into one single word
                                                                                                       - the user will have to enter the 2 words
                                                                                                       - the application will advise if the correct words were entered

                                                                                                       The program consists of three window types. Only one will be displayed at any moment. The window types are:
                                                                                                       - the introduction window: class IntroductionWindow
                                                                                                       - the main game window: class MainGameWindow
                                                                                                       - the hints window: class HintsWindow

                                                                                                       An additional class (WordMixer) will be used for:
                                                                                                       - reading the words and mixing them into a single one
                                                                                                       - verifying if the words entered by user match the actual words
                                                                                                    */

#include <QApplication>
#include <QWidget>
#include "introductionwindow.h"
#include "hintswindow.h"
#include "fatalerrors.h"
#include "gamestrings.h"

int main(int argc, char *argv[]) {
    QApplication app(argc,argv);
    qApp -> setStyleSheet(GameStrings::c_ApplicationStyle);
    try {
        WordMixer wordMixer{nullptr, QApplication::applicationDirPath() + "/data"};                 /* application core, handles the mixing of the read words pair and the checking of the user input
                                                                                                       the file from which words are read will be in the same directory with the application, so its path
                                                                                                       will be built using the application directory path as base
                                                                                                    */
        IntroductionWindow introductionWindow{};                                                    // first application window, launched when the program starts
        introductionWindow.setWindowTitle(GameStrings::c_IntroWindowTitle);
        HintsWindow hintsWindow{};                                                                  // second application window, launched by clicking on the Hints button
        hintsWindow.setWindowTitle(GameStrings::c_HelpWindowTitle);
        introductionWindow.updateHintsWinPtr(&hintsWindow);                                         // link introduction window to hints window
        MainGameWindow mainGameWindow{};                                                            // third application window, launched by clicking the Play or Ok buttons
        mainGameWindow.setWindowTitle(GameStrings::c_MainWindowTitle);
        introductionWindow.updateMainGameWinPtr(&mainGameWindow);                                   // link introduction window to main game window
        hintsWindow.updateMainGameWinPtr(&mainGameWindow);                                          // link hints window to main game window
        mainGameWindow.updateHintsWinPtr(&hintsWindow);                                             // link main game window to hints window
        mainGameWindow.assignWordMixer(&wordMixer);                                                 // link the word mixer to the main game window
        introductionWindow.show();                                                                  // finally, show the introduction window and wait for user action
        return app.exec();
    }
    catch (const QString fatalErrorMessage) {                                                       // this block handles all possible fatal errors that occur in the application
        FatalErrors fatalError{};
        fatalError.setWindowTitle(GameStrings::c_FatalErrorWindowTitle);
        fatalError.setFatalErrorText(fatalErrorMessage);
        fatalError.show();
        return app.exec();
    }
}
