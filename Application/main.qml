import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import GameManagers 1.0
import GameUtils 1.0

ApplicationWindow {
    id: game

    title: gamePresenter.windowTitle
    visible: true
    color: Styles.backgroundColor

    screen: Qt.application.screens[0]

    x: (Qt.application.screens[0].width - minimumWidth)/2
    y: (Qt.application.screens[0].height - minimumHeight)/2

    width: 640
    height: 480
    minimumWidth: 640
    minimumHeight: 480

    property double windowMargins: width * 0.01

    GamePresenter {
        id: gamePresenter
    }

    IntroPane {
        id: introPane

        presenter: gamePresenter
        visible: gamePresenter.introPaneVisible

        anchors {
            fill: parent
            margins: windowMargins
        }
    }

    MainPane {
        id: mainPane

        presenter: gamePresenter
        visible: gamePresenter.mainPaneVisible

        anchors {
            fill: parent
            margins: windowMargins
        }
    }

    DataEntryPane {
        id: dataEntryPane

        presenter: gamePresenter
        visible: gamePresenter.dataEntryPaneVisible

        anchors {
            fill: parent
            margins: windowMargins
        }
    }

    PromptSaveExitPane {
        id: promptSaveExitPane

        presenter: gamePresenter
        visible: gamePresenter.promptSaveExitPaneVisible

        anchors {
            fill: parent
            margins: windowMargins
        }
    }

    HelpPane {
        id: helpPane

        presenter: gamePresenter
        visible: gamePresenter.helpPaneVisible

        anchors {
            fill: parent
            margins: windowMargins
        }
    }

    ErrorPane {
        id: errorPane

        presenter: gamePresenter
        visible: gamePresenter.errorOccured

        anchors {
            fill: parent
            margins: windowMargins
        }
    }

    // keys

    Item {
        id: mainPaneKeys

        anchors.fill: parent
        focus: mainPane.visible

        onFocusChanged: {
            if (mainPane.visible) {
                forceActiveFocus();
            }
        }

        Keys.onPressed: {
            if (event.key === Qt.Key_S) {
                if (gamePresenter.pieceSelectionCursorPosition === -1) {
                    gamePresenter.enablePieceSelectionFromKeyboard();
                } else {
                    gamePresenter.disablePieceSelectionFromKeyboard();
                }

                event.accepted = true;
            }
        }

        Keys.onLeftPressed: {
            if (gamePresenter.pieceSelectionCursorPosition !== -1) {
                gamePresenter.movePieceSelectionCursorToLeft();
            } else if (gamePresenter.piecesRemovalFirstWordCursorPosition !== -1 || gamePresenter.piecesRemovalSecondWordCursorPosition != -1) {
                gamePresenter.movePiecesRemovalCursorToLeft();
            }
        }

        Keys.onRightPressed: {
            if (gamePresenter.pieceSelectionCursorPosition !== -1) {
                gamePresenter.movePieceSelectionCursorToRight();
            } else if (gamePresenter.piecesRemovalFirstWordCursorPosition !== -1 || gamePresenter.piecesRemovalSecondWordCursorPosition != -1) {
                gamePresenter.movePiecesRemovalCursorToRight();
            }
        }

        Keys.onDownPressed: {
            gamePresenter.selectPieceForFirstWordFromCursor();
        }

        Keys.onUpPressed: {
            gamePresenter.selectPieceForSecondWordFromCursor();
        }

        Keys.onDigit1Pressed: {
            if (gamePresenter.piecesRemovalFirstWordCursorPosition === -1) {
                gamePresenter.enableFirstWordPiecesRemovalFromKeyboard();
            } else {
                gamePresenter.disablePiecesRemovalFromKeyboard();
            }
        }

        Keys.onDigit2Pressed: {
            if (gamePresenter.piecesRemovalSecondWordCursorPosition === -1) {
                gamePresenter.enableSecondWordPiecesRemovalFromKeyboard();
            } else {
                gamePresenter.disablePiecesRemovalFromKeyboard();
            }
        }

        function removeWordPieces() {
            if (gamePresenter.piecesRemovalFirstWordCursorPosition !== -1) {
                gamePresenter.removeWordPiecesFromFirstInputWord(gamePresenter.piecesRemovalFirstWordCursorPosition);
            } else if (gamePresenter.piecesRemovalSecondWordCursorPosition !== -1) {
                gamePresenter.removeWordPiecesFromSecondInputWord(gamePresenter.piecesRemovalSecondWordCursorPosition);
            }
        }

        Keys.onReturnPressed: {
            removeWordPieces();
        }

        Keys.onEnterPressed: {
            removeWordPieces();
        }
    }

    // shortcuts shared among multiple panes

    Shortcut {
        sequence: GameStrings.dataEntryButtonShortcut
        enabled: gamePresenter.dataEntryEnabled && (gamePresenter.introPaneVisible || gamePresenter.mainPaneVisible)

        onActivated: gamePresenter.switchToPane(GamePresenter.DATA_ENTRY)
    }

    Shortcut {
        sequence: GameStrings.helpButtonShortcut
        enabled: gamePresenter.introPaneVisible || gamePresenter.mainPaneVisible || gamePresenter.dataEntryPaneVisible

        onActivated: gamePresenter.switchToPane(GamePresenter.HELP)
    }

    Shortcut {
        sequence: GameStrings.quitButtonShortcut
        enabled: gamePresenter.introPaneVisible || gamePresenter.helpPaneVisible || gamePresenter.mainPaneVisible || gamePresenter.dataEntryPaneVisible

        onActivated: {
            if (gamePresenter.dataEntryPaneVisible && gamePresenter.saveAddedWordPairsEnabled) {
                gamePresenter.quitGameDeferred = true;
                gamePresenter.promptForSavingAddedWordPairs();
            } else {
                gamePresenter.quit();
            }
        }
    }
}
