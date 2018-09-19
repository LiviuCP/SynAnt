import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0

Item {
    id: helpPane

    property QtObject presenter

    readonly property double helpPaneRectHeight: height * 0.9
    readonly property double bottomBtnsLayoutHeight: height * 0.1
    readonly property double bottomBtnsMinWidth: (helpPaneRect.width - bottomBtnsLayout.spacing) * 0.5

    readonly property double pressedButtonOpacity: 0.5
    readonly property double releasedButtonOpacity: 1.0

    readonly property color paneColor: presenter.backgroundColor
    readonly property color buttonColor: presenter.pushButtonColor
    readonly property color borderColor: presenter.borderColor
    readonly property color textColor: presenter.textColor

    MouseArea {
        id: helpPaneMouseArea
        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        id: helpPaneToolTip
        delay: presenter.toolTipDelay
        text: presenter.helpPaneToolTip
        visible: helpPaneMouseArea.containsMouse
    }

    Rectangle {
        id: helpPaneRect
        anchors.top: parent.top
        width: parent.width
        height: helpPaneRectHeight
        color: paneColor

        border {
            color: borderColor
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
                color: textColor
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
                text: presenter.okButtonLabel
                color: textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: buttonColor
                border.color: borderColor
                border.width: width * 0.002
                radius: width * 0.01
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: presenter.okButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.visible: hovered

            Shortcut {
                sequence: presenter.okButtonShortcut
                onActivated: {
                    presenter.switchToPane(GamePresenter.MAIN);
                }
            }

            onClicked: presenter.switchToPane(GamePresenter.MAIN)
            onPressed: opacity = pressedButtonOpacity
            onReleased: opacity = releasedButtonOpacity
            onCanceled: opacity = releasedButtonOpacity
        }

        Button {
            id: quitBtn

            contentItem: Text {
                text: presenter.quitButtonLabel
                color: textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: buttonColor
                border.color: borderColor
                border.width: width * 0.002
                radius: width * 0.01
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: presenter.quitButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.visible: hovered

            onClicked: Qt.quit()
            onPressed: opacity = pressedButtonOpacity
            onReleased: opacity = releasedButtonOpacity
            onCanceled: opacity = releasedButtonOpacity
        }
    }
}
