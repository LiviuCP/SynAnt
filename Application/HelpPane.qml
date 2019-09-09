import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0

Item {
    id: helpPane

    property QtObject presenter

    property bool quitButtonShortcutActivated: false

    readonly property double helpPaneRectHeight: height * 0.9
    readonly property double bottomBtnsLayoutHeight: height * 0.1
    readonly property double bottomBtnsMinWidth: (helpPaneRect.width - bottomBtnsLayout.spacing) * 0.5
    readonly property double buttonRadiusRatio: 0.01

    MouseArea {
        id: helpPaneMouseArea

        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        delay: presenter.toolTipDelay
        timeout: presenter.toolTipTimeout
        text: GameStrings.helpPaneToolTip
        visible: helpPaneMouseArea.containsMouse
    }

    Rectangle {
        id: helpPaneRect

        width: parent.width
        height: helpPaneRectHeight

        anchors.top: parent.top

        color: Styles.backgroundColor
        border.color: Styles.borderColor

        Flickable {
            id: helpPaneFlickable

            clip: true
            contentHeight: helpPaneText.height

            anchors {
                fill: parent
                leftMargin: parent.width * 0.005
                rightMargin: parent.width * 0.005
            }

            Text {
                id: helpPaneText
                text: presenter.helpPaneMessage
                color: Styles.textColor
                width: parent.width
                wrapMode: Text.WordWrap
            }
        }
    }

    RowLayout {
        id: bottomBtnsLayout

        anchors {
            top: helpPaneRect.bottom
            topMargin: parent.height * 0.01
            bottom: parent.bottom
            left: helpPaneRect.left
            right: helpPaneRect.right
        }

        height: bottomBtnsLayoutHeight

        Button {
            id: okBtn

            contentItem: Text {
                text: GameStrings.okButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.002
                }
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip {
                text: GameStrings.okButtonToolTip
                visible: okBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            // we will not use the Styles functions here as we need the presenter invokable to be called after timer expires
            property Timer shortcutActivationTimer: Timer {
                interval: 50
                onTriggered: {
                    okBtn.opacity = Styles.releasedButtonOpacity;
                    presenter.goBack();
                }
            }

            Shortcut {
                sequence: GameStrings.okButtonShortcut
                enabled: presenter.helpPaneVisible

                onActivated: {
                    okBtn.opacity = Styles.pressedButtonOpacity;
                    okBtn.shortcutActivationTimer.start();
                }
            }

            onClicked: presenter.goBack()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: quitBtn

            property double buttonOpacity: Styles.releasedButtonOpacity

            Layout.minimumWidth: bottomBtnsMinWidth

            contentItem: Text {
                text: GameStrings.quitButtonLabel
                color: Styles.textColor
                opacity: quitBtn.buttonOpacity
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                opacity: quitBtn.buttonOpacity
                radius: width * buttonRadiusRatio

                border {
                    color: Styles.borderColor
                    width: width * 0.002
                }
            }

            ToolTip {
                text: GameStrings.quitButtonToolTip
                visible: quitBtn.hovered
                delay: presenter.toolTipDelay
                timeout: presenter.toolTipTimeout
            }

            onClicked: presenter.quit()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }
    }

    onQuitButtonShortcutActivatedChanged: {
        Styles.updateButtonOpacityAtShortcutActivation(quitBtn);
        quitButtonShortcutActivated = false;
    }
}
