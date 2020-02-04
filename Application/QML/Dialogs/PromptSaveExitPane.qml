import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0
import Controls 1.0

Item {
    id: promptSaveExitPane

    property QtObject presenter

    readonly property double promptPaneRectHeight: height * 0.89
    readonly property double bottomBtnsLayoutHeight: height * 0.1
    readonly property double bottomBtnsMinWidth: (promptStatusBox.width - 2 * bottomBtnsLayout.spacing) / 3
    readonly property double buttonRadiusRatio: 0.025

    function quitOrExitDataEntry() {
        if (presenter.quitGameDeferred) {
            presenter.quit();
        } else {
            // user should return to the page that was visited before arriving to the data entry page (from which the prompt was issued)
            presenter.goBack();
            presenter.goBack();
        }
    }

    MouseArea {
        id: promptPaneMouseArea

        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        text: GameStrings.promptPaneToolTip
        visible: promptPaneMouseArea.containsMouse
        delay: presenter.toolTipDelay
        timeout: presenter.toolTipTimeout
    }

    Rectangle {
        id: promptStatusBox

        width: parent.width
        height: promptPaneRectHeight

        anchors.top: parent.top

        color: Styles.backgroundColor
        border.color: Styles.borderColor

        Text {
            text: GameStrings.promptSaveBeforeExitMessage;
            wrapMode: Text.WordWrap
            anchors.fill: parent
        }
    }

    RowLayout {
        id: bottomBtnsLayout

        height: bottomBtnsLayoutHeight

        anchors {
            top: promptStatusBox.bottom
            topMargin: parent.height * 0.01
            bottom: parent.bottom
            left: promptStatusBox.left
            right: promptStatusBox.right
        }

        AppButton {
            id: yesBtn

            dedicatedShortcutEnabled: presenter.promptSaveExitPaneVisible

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.promptSaveYesButtonLabel
            buttonToolTip: GameStrings.promptSaveYesButtonToolTip
            shortcutSequence: GameStrings.promptSaveYesButtonShortcut

            onButtonClicked: {
                presenter.dataEntry.handleSaveAddedWordPairsRequest();
                promptSaveExitPane.quitOrExitDataEntry();
            }
        }

        AppButton {
            id: noBtn

            dedicatedShortcutEnabled: presenter.promptSaveExitPaneVisible

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.promptSaveNoButtonLabel
            buttonToolTip: GameStrings.promptSaveNoButtonToolTip
            shortcutSequence: GameStrings.promptSaveNoButtonShortcut

            onButtonClicked: {
                presenter.dataEntry.handleClearAddedWordPairsRequest();
                promptSaveExitPane.quitOrExitDataEntry();
            }
        }

        AppButton {
            id: cancelBtn

            dedicatedShortcutEnabled: presenter.promptSaveExitPaneVisible

            Layout.minimumWidth: bottomBtnsMinWidth

            buttonLabel: GameStrings.promptSaveCancelButtonLabel
            buttonToolTip: GameStrings.promptSaveCancelButtonToolTip
            shortcutSequence: GameStrings.promptSaveCancelButtonShortcut

            onButtonClicked: presenter.goBack()
        }
    }
}
