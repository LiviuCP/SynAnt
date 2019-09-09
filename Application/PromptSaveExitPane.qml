import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0

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

        Button {
            id: yesBtn

            property double buttonOpacity: Styles.releasedButtonOpacity

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.promptSaveYesButtonLabel
                color: Styles.textColor
                opacity: yesBtn.buttonOpacity
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                opacity: yesBtn.buttonOpacity
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            ToolTip {
                text: GameStrings.promptSaveYesButtonToolTip
                visible: yesBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            // we will not use the Styles functions here as we need the prompt page to be activated when the timer triggers
            property Timer shortcutActivationTimer: Timer {
                interval: 50
                onTriggered: {
                    yesBtn.opacity = Styles.releasedButtonOpacity;
                    presenter.dataEntry.handleSaveAddedWordPairsRequest();
                    promptSaveExitPane.quitOrExitDataEntry();
                }
            }

            Shortcut {
                sequence: GameStrings.promptSaveYesButtonShortcut
                enabled: presenter.promptSaveExitPaneVisible

                onActivated: {
                    yesBtn.opacity = Styles.pressedButtonOpacity;
                    yesBtn.shortcutActivationTimer.start();
                }
            }

            onClicked: {
                presenter.dataEntry.handleSaveAddedWordPairsRequest();
                promptSaveExitPane.quitOrExitDataEntry();
            }

            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: noBtn

            property double buttonOpacity: Styles.releasedButtonOpacity

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.promptSaveNoButtonLabel
                color: Styles.textColor
                opacity: noBtn.buttonOpacity
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                opacity: noBtn.buttonOpacity
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            ToolTip {
                text: GameStrings.promptSaveNoButtonToolTip
                visible: noBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            // we will not use the Styles functions here as we need the prompt page to be activated when the timer triggers
            property Timer shortcutActivationTimer: Timer {
                interval: 50
                onTriggered: {
                    noBtn.opacity = Styles.releasedButtonOpacity;
                    presenter.dataEntry.handleClearAddedWordPairsRequest();
                    promptSaveExitPane.quitOrExitDataEntry();
                }
            }

            Shortcut {
                sequence: GameStrings.promptSaveNoButtonShortcut
                enabled: presenter.promptSaveExitPaneVisible

                onActivated: {
                    noBtn.opacity = Styles.pressedButtonOpacity;
                    noBtn.shortcutActivationTimer.start();
                }
            }

            onClicked: {
                presenter.dataEntry.handleClearAddedWordPairsRequest();
                promptSaveExitPane.quitOrExitDataEntry();
            }

            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: cancelBtn

            property double buttonOpacity: Styles.releasedButtonOpacity

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.promptSaveCancelButtonLabel
                color: Styles.textColor
                opacity: cancelBtn.buttonOpacity
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                opacity: cancelBtn.buttonOpacity
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            ToolTip {
                text: GameStrings.promptSaveCancelButtonToolTip
                visible: cancelBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            // we will not use the Styles functions here as we need the prompt page to be activated when the timer triggers
            property Timer shortcutActivationTimer: Timer {
                interval: 50
                onTriggered: {
                    cancelBtn.opacity = Styles.releasedButtonOpacity;
                    presenter.goBack();
                }
            }

            Shortcut {
                sequence: GameStrings.promptSaveCancelButtonShortcut
                enabled: presenter.promptSaveExitPaneVisible

                onActivated: {
                    cancelBtn.opacity = Styles.pressedButtonOpacity;
                    cancelBtn.shortcutActivationTimer.start();
                }
            }

            onClicked: presenter.goBack()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }
    }
}
