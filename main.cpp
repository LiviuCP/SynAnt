/*

   This program implements a small game which is played as follows:
   - the application reads 2 words from a file
   - the words are divided into small pieces
   - all pieces are mixed into a single string array
   - the user will have to guess and enter the 2 words
   - the application will advise if the words were correctly guessed

   The program consists of three window types. Only one will be displayed at any moment. The window types are:
   - the introduction window: class IntroductionWindow
   - the main game window: class MainGameWindow
   - the hints window: class HintsWindow

   An additional class (WordMixer) will be used for:
   - reading the words and mixing them into a single one
   - verifying if the words entered by user match the actual words

   A second additional class (ScoreItem) will be used for keeping track of the score.

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
        IntroductionWindow introductionWindow{};
        introductionWindow.setWindowTitle(GameStrings::c_IntroWindowTitle);

        HintsWindow hintsWindow{};
        hintsWindow.setWindowTitle(GameStrings::c_HelpWindowTitle);

        WordMixer* wordMixer{new WordMixer{nullptr, QApplication::applicationDirPath() + "/data"}};
        MainGameWindow mainGameWindow{wordMixer};
        mainGameWindow.setWindowTitle(GameStrings::c_MainWindowTitle);

        introductionWindow.updateHintsWinPtr(&hintsWindow);
        introductionWindow.updateMainGameWinPtr(&mainGameWindow);
        hintsWindow.updateMainGameWinPtr(&mainGameWindow);
        mainGameWindow.updateHintsWinPtr(&hintsWindow);

        introductionWindow.show();

        return app.exec();
    }
    catch (const QString fatalErrorMessage) {
        FatalErrors fatalError{};
        fatalError.setWindowTitle(GameStrings::c_FatalErrorWindowTitle);
        fatalError.setFatalErrorText(fatalErrorMessage);
        fatalError.show();
        return app.exec();
    }
}
