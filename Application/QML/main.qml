import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import AppCore 1.0
import AppDialogs 1.0
import GameUtils 1.0
import GameManagers 1.0

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
        visible: gamePresenter.currentPane === GamePresenter.INTRO

        anchors {
            fill: parent
            margins: windowMargins
        }
    }

    MainPane {
        id: mainPane

        presenter: gamePresenter
        visible: gamePresenter.currentPane === GamePresenter.MAIN

        anchors {
            fill: parent
            margins: windowMargins
        }
    }

    DataEntryPane {
        id: dataEntryPane

        presenter: gamePresenter
        visible: gamePresenter.currentPane === GamePresenter.DATA_ENTRY

        anchors {
            fill: parent
            margins: windowMargins
        }
    }

    PromptSaveExitPane {
        id: promptSaveExitPane

        presenter: gamePresenter
        visible: gamePresenter.currentPane === GamePresenter.PROMPT_SAVE_EXIT

        anchors {
            fill: parent
            margins: windowMargins
        }
    }

    PromptDiscardPane {
        id: promptDiscardPane

        presenter: gamePresenter
        visible: gamePresenter.currentPane === GamePresenter.PROMPT_DISCARD

        anchors {
            fill: parent
            margins: windowMargins
        }
    }

    HelpPane {
        id: helpPane

        presenter: gamePresenter
        visible: gamePresenter.currentPane === GamePresenter.HELP

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

        property bool languageDropdownOpened: mainPane.languageDropdownOpened

        anchors.fill: parent
        focus: mainPane.visible && !languageDropdownOpened

        onFocusChanged: {
            if (mainPane.visible && !mainPane.languageDropdownInFocus) {
                forceActiveFocus();
            }
        }

        Keys.onPressed: {
            if (event.key === Qt.Key_C) {
                gamePresenter.enableCursor();
                event.accepted = true;
            }
        }

        Keys.onEscapePressed: {
            gamePresenter.disableCursor();
        }

        Keys.onTabPressed: {
            gamePresenter.moveCursorToNextContainer();
        }

        Keys.onLeftPressed: {
            gamePresenter.moveCursorLeft();
        }

        Keys.onRightPressed: {
            gamePresenter.moveCursorRight();
        }

        Keys.onDownPressed: {
            mainPane.downArrowPressed = true;
            downPressedTimer.start();
        }

        Keys.onUpPressed: {
            mainPane.upArrowPressed = true;
            upPressedTimer.start();
        }

        Timer {
            id: upPressedTimer
            interval: 125
            onTriggered: gamePresenter.executeFirstCursorAction();
        }

        Timer {
            id: downPressedTimer
            interval: 125
            onTriggered: gamePresenter.executeSecondCursorAction();
        }
    }

    // shortcuts shared among multiple panes

    Shortcut {
        sequence: GameStrings.languageSelectionDropdownShortcut
        enabled: gamePresenter.languageSelectionEnabled && !gamePresenter.dataEntry.dataSavingInProgress && !gamePresenter.dataEntry.dataFetchingInProgress

        onActivated: {
            if (gamePresenter.currentPane === GamePresenter.INTRO) {
                introPane.languageDropdownShortcutActivated = true;
            } else if (gamePresenter.currentPane === GamePresenter.MAIN) {
                mainPane.languageDropdownShortcutActivated = true;
            } else if (gamePresenter.currentPane === GamePresenter.DATA_ENTRY) {
                dataEntryPane.languageDropdownShortcutActivated = true;
            }
            else {
                console.log("Error, language dropdown shortcut should not be enabled for current pane");
            }
        }
    }

    Shortcut {
        sequence: GameStrings.timeLimitButtonShortcut
        enabled: !gamePresenter.dataFetchingInProgress

        onActivated: gamePresenter.setTimeLimitEnabled(!gamePresenter.timeLimitEnabled)
    }

    Shortcut {
        sequence: GameStrings.dataEntryButtonShortcut
        enabled: gamePresenter.dataEntry.dataEntryEnabled && (gamePresenter.currentPane === GamePresenter.INTRO || gamePresenter.currentPane === GamePresenter.MAIN) && !gamePresenter.dataEntry.dataFetchingInProgress

        property Timer dataEntryTimer: Timer {
            interval: 100
            onTriggered: {
                gamePresenter.switchToPane(GamePresenter.DATA_ENTRY);
            }
        }

        onActivated: {
            if (gamePresenter.currentPane === GamePresenter.INTRO) {
                introPane.dataEntryButtonShortcutActivated = true;
            } else if (gamePresenter.currentPane === GamePresenter.MAIN) {
                mainPane.dataEntryButtonShortcutActivated = true;
            }
            else {
                console.log("Error, Data entry button shortcut should not be enabled for current pane");
            }

            dataEntryTimer.start();
        }
    }

    Shortcut {
        sequence: GameStrings.helpButtonShortcut
        enabled: gamePresenter.currentPane === GamePresenter.INTRO || gamePresenter.currentPane === GamePresenter.MAIN || (gamePresenter.currentPane === GamePresenter.DATA_ENTRY && !gamePresenter.dataEntry.dataFetchingInProgress)

        property Timer helpTimer: Timer {
            interval: 100
            onTriggered: {
                gamePresenter.switchToPane(GamePresenter.HELP);
            }
        }

        onActivated: {
            if (gamePresenter.currentPane === GamePresenter.INTRO) {
                introPane.helpButtonShortcutActivated = true;
            } else if (gamePresenter.currentPane === GamePresenter.MAIN) {
                mainPane.helpButtonShortcutActivated = true;
            } else if (gamePresenter.currentPane === GamePresenter.DATA_ENTRY) {
                dataEntryPane.helpButtonShortcutActivated = true;
            }
            else {
                console.log("Error, Help button shortcut should not be enabled for current pane");
            }

            helpTimer.start();
        }
    }

    Shortcut {
        sequence: GameStrings.quitButtonShortcut
        enabled: gamePresenter.currentPane === GamePresenter.INTRO || gamePresenter.currentPane === GamePresenter.HELP || gamePresenter.currentPane === GamePresenter.MAIN || gamePresenter.currentPane === GamePresenter.DATA_ENTRY

        property Timer quitTimer: Timer {
            interval: 100
            onTriggered: {
                if (gamePresenter.currentPane === GamePresenter.DATA_ENTRY && gamePresenter.dataEntry.saveAddedWordPairsEnabled) {
                    gamePresenter.quitGameDeferred = true;
                    gamePresenter.promptForSavingAddedWordPairs();
                } else {
                    gamePresenter.quit();
                }
            }
        }

        onActivated: {
            if (gamePresenter.currentPane === GamePresenter.INTRO) {
                introPane.quitButtonShortcutActivated = true;
            } else if (gamePresenter.currentPane === GamePresenter.MAIN) {
                mainPane.quitButtonShortcutActivated = true;
            } else if (gamePresenter.currentPane === GamePresenter.HELP) {
                helpPane.quitButtonShortcutActivated = true;
            } else if (gamePresenter.currentPane === GamePresenter.DATA_ENTRY) {
                dataEntryPane.quitButtonShortcutActivated = true;
            }
            else {
                console.log("Error, Quit button shortcut should not be enabled for current pane");
            }

            quitTimer.start();
        }
    }
}
