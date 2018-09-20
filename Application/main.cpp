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

   An application manager (AppManager) will be responsible for creating and connecting the windows.

*/

#include <QApplication>

#include "appmanager.h"
#include "../UIResources/BuildingBlocks/fatalerrors.h"
#include "../UIResources/OtherResources/styles.h"
#include "../Common/gamestrings.h"
#include "../Common/exceptions.h"

int main(int argc, char *argv[]) {
    QApplication app(argc,argv);
    AppManager appManager{};

    qApp -> setStyleSheet(Styles::c_ApplicationStyle);

    try {
        appManager.init(QApplication::applicationDirPath() + "/" + GameStrings::c_FileName);
        return app.exec();
    }
    catch (const GameException& exception) {
        FatalErrors fatalError{};
        appManager.hideActiveWindow();
        fatalError.setWindowTitle(GameStrings::c_FatalErrorWindowTitle);
        fatalError.setFatalErrorText(exception.getDescription());
        fatalError.show();
        return app.exec();
    }
}
