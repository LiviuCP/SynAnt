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

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.yesButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                opacity: enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            ToolTip {
                text: GameStrings.yesButtonToolTip
                visible: yesBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Shortcut {
                sequence: GameStrings.yesButtonShortcut
                enabled: presenter.promptSaveExitPaneVisible

                onActivated: {
                    presenter.handleSaveAddedWordPairsRequest();
                    promptSaveExitPane.quitOrExitDataEntry();
                }
            }

            onClicked: {
                presenter.handleSaveAddedWordPairsRequest();
                promptSaveExitPane.quitOrExitDataEntry();
            }

            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: noBtn

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.noButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            ToolTip {
                text: GameStrings.noButtonToolTip
                visible: noBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Shortcut {
                sequence: GameStrings.noButtonShortcut
                enabled: presenter.promptSaveExitPaneVisible

                onActivated: {
                    presenter.handleClearAddedWordPairsRequest();
                    promptSaveExitPane.quitOrExitDataEntry();
                }
            }

            onClicked: {
                presenter.handleClearAddedWordPairsRequest();
                promptSaveExitPane.quitOrExitDataEntry();
            }

            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: cancelBtn

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.cancelButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.005
                }
            }

            ToolTip {
                text: GameStrings.cancelButtonToolTip
                visible: cancelBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Shortcut {
                sequence: GameStrings.cancelButtonShortcut
                enabled: presenter.promptSaveExitPaneVisible

                onActivated: presenter.goBack()
            }

            onClicked: presenter.goBack()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }
    }
}
