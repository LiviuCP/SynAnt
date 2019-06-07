import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0
import GameUtils 1.0

Item {
    id: helpPane

    property QtObject presenter

    readonly property double helpPaneRectHeight: height * 0.9
    readonly property double bottomBtnsLayoutHeight: height * 0.1
    readonly property double bottomBtnsMinWidth: (helpPaneRect.width - bottomBtnsLayout.spacing) * 0.5

    MouseArea {
        id: helpPaneMouseArea
        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        id: helpPaneToolTip
        delay: presenter.toolTipDelay
        timeout: presenter.toolTipTimeout
        text: GameStrings.helpPaneToolTip
        visible: helpPaneMouseArea.containsMouse
    }

    Rectangle {
        id: helpPaneRect
        anchors.top: parent.top
        width: parent.width
        height: helpPaneRectHeight
        color: Styles.backgroundColor

        border {
            color: Styles.borderColor
        }

        Flickable {
            id: helpPaneFlickable

            anchors.fill: parent
            anchors.leftMargin: parent.width * 0.005
            anchors.rightMargin: parent.width * 0.005
            contentHeight: helpPaneText.height

            clip: true

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

        anchors.top: helpPaneRect.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: parent.height * 0.01
        anchors.left: helpPaneRect.left
        anchors.right: helpPaneRect.right

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
                border.color: Styles.borderColor
                border.width: width * 0.002
                radius: width * 0.01
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: GameStrings.okButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

            Shortcut {
                sequence: GameStrings.okButtonShortcut
                onActivated: {
                    presenter.switchToPane(presenter.previousPane);
                }
            }

            onClicked: presenter.switchToPane(presenter.previousPane)
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }

        Button {
            id: quitBtn

            contentItem: Text {
                text: GameStrings.quitButtonLabel
                color: Styles.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: Styles.pushButtonColor
                border.color: Styles.borderColor
                border.width: width * 0.002
                radius: width * 0.01
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: GameStrings.quitButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.timeout: presenter.toolTipTimeout
            ToolTip.visible: hovered

            onClicked: presenter.quit()
            onPressed: opacity = Styles.pressedButtonOpacity
            onReleased: opacity = Styles.releasedButtonOpacity
            onCanceled: opacity = Styles.releasedButtonOpacity
        }
    }
}
