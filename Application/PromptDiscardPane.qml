import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0

Item {
    id: promptDiscardPane

    property QtObject presenter

    readonly property double promptPaneRectHeight: height * 0.89
    readonly property double bottomBtnsLayoutHeight: height * 0.1
    readonly property double bottomBtnsMinWidth: (promptMessageBox.width - bottomBtnsLayout.spacing) / 2
    readonly property double buttonRadiusRatio: 0.02

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
        id: promptMessageBox

        width: parent.width
        height: promptPaneRectHeight

        anchors.top: parent.top

        color: Styles.backgroundColor
        border.color: Styles.borderColor

        Text {
            text: GameStrings.promptDiscardMessage;
            wrapMode: Text.WordWrap
            anchors.fill: parent
        }
    }

    RowLayout {
        id: bottomBtnsLayout

        height: bottomBtnsLayoutHeight

        anchors {
            top: promptMessageBox.bottom
            topMargin: parent.height * 0.01
            bottom: parent.bottom
            left: promptMessageBox.left
            right: promptMessageBox.right
        }

        Button {
            id: yesBtn

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.promptDiscardYesButtonLabel
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
                    width: width * 0.003
                }
            }

            ToolTip {
                text: GameStrings.promptDiscardYesButtonToolTip
                visible: yesBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Shortcut {
                sequence: GameStrings.promptDiscardYesButtonShortcut
                enabled: presenter.promptDiscardPaneVisible

                onActivated: {
                    presenter.dataEntry.handleClearAddedWordPairsRequest();
                    presenter.goBack();
                }
            }

            onClicked: {
                presenter.dataEntry.handleClearAddedWordPairsRequest();
                presenter.goBack();
            }

            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: cancelBtn

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.promptDiscardCancelButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.003
                }
            }

            ToolTip {
                text: GameStrings.promptDiscardCancelButtonToolTip
                visible: cancelBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            Shortcut {
                sequence: GameStrings.promptDiscardCancelButtonShortcut
                enabled: presenter.promptDiscardPaneVisible

                onActivated: presenter.goBack()
            }

            onClicked: presenter.goBack()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }
    }
}
