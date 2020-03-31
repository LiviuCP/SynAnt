import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0
import Controls 1.0
import StatusBoxes 1.0
import Animations 1.0

Item {
    id: helpPane

    property QtObject presenter

    property bool quitButtonShortcutActivated: false

    readonly property double helpPaneStatusBoxHeight: height * 0.9
    readonly property double bottomBtnsLayoutHeight: height * 0.1
    readonly property double bottomBtnsMinWidth: (helpPaneStatusBox.width - bottomBtnsLayout.spacing) * 0.5

    // couldn't use just the down arrow key due to a focus conflict with the language dropdown (probably will be fixed in a future version)
    Shortcut {
        id: flickUpShortcut

        enabled: helpPane.visible

        sequence: GameStrings.helpMenuFlickUpShortcut
        onActivated: helpPaneStatusBox.flickUp();
    }

    // couldn't use just the up arrow key due to a focus conflict with the language dropdown (probably will be fixed in a future version)
    Shortcut {
        id: flickDownShortcut

        enabled: helpPane.visible

        sequence: GameStrings.helpMenuFlickDownShortcut
        onActivated: helpPaneStatusBox.flickDown();
    }

    StatusBox {
        id: helpPaneStatusBox

        width: parent.width
        height: helpPaneStatusBoxHeight

        anchors.top: parent.top

        statusMessage: presenter.helpPaneMessage
        boxToolTip: presenter.helpPaneToolTip
        hasFlickable: true
    }

    RowLayout {
        id: bottomBtnsLayout

        anchors {
            top: helpPaneStatusBox.bottom
            topMargin: parent.height * 0.01
            bottom: parent.bottom
            left: helpPaneStatusBox.left
            right: helpPaneStatusBox.right
        }

        height: bottomBtnsLayoutHeight

        AppButton {
            id: okBtn

            dedicatedShortcutEnabled: presenter.currentPane === GamePresenter.HELP_PANE

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.okButtonLabel
            buttonToolTip: GameStrings.okButtonToolTip
            shortcutSequence: GameStrings.okButtonShortcut

            onButtonClicked: presenter.goBack()
        }

        AppButton {
            id: quitBtn

            dedicatedShortcutEnabled: false

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.quitButtonLabel
            buttonToolTip: GameStrings.quitButtonToolTip

            onButtonClicked: {
                if (presenter.dataEntry.saveAddedWordPairsEnabled) {
                    presenter.quitGameDeferred = true;
                    presenter.promptForSavingAddedWordPairs();
                } else {
                    presenter.quit();
                }
            }
        }
    }

    onQuitButtonShortcutActivatedChanged: {
        Animations.updateButtonOpacityAtShortcutActivation(quitBtn);
        quitButtonShortcutActivated = false;
    }
}
