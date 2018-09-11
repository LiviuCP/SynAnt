import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import GameManagers 1.0

Item {
    id: introPane

    property QtObject presenter

    readonly property double introPaneRectHeight: height * 0.9
    readonly property double bottomBtnsLayoutHeight: height * 0.1

    readonly property double bottomBtnsMinWidth: (introPaneRect.width - 2 * bottomBtnsLayout.spacing) / 3

    readonly property double pressedButtonOpacity: 0.5
    readonly property double releasedButtonOpacity: 1.0

    readonly property color paneColor: presenter.backgroundColor
    readonly property color buttonColor: presenter.pushButtonColor
    readonly property color borderColor: presenter.borderColor
    readonly property color textColor: presenter.textColor

    MouseArea {
        id: introPaneMouseArea
        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        id: introPaneToolTip
        delay: presenter.toolTipDelay
        text: presenter.introPaneToolTip
        visible: introPaneMouseArea.containsMouse
    }

    Rectangle {
        id: introPaneRect
        anchors.top: parent.top
        width: parent.width
        height: introPaneRectHeight
        color: paneColor

        border {
            color: borderColor
        }

        Text {
            id: introPaneText
            anchors.fill: parent
            anchors.leftMargin: parent.width * 0.005
            text: presenter.introPaneMessage
            color: textColor
        }
    }

    RowLayout {
        id: bottomBtnsLayout

        anchors.top: introPaneRect.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: parent.height * 0.01
        anchors.left: introPaneRect.left
        anchors.right: introPaneRect.right

        height: bottomBtnsLayoutHeight

        Button {
            id: playBtn

            contentItem: Text {
                text: presenter.playButtonLabel
                color: textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: buttonColor
                border.color: borderColor
                border.width: width * 0.005
                radius: width * 0.01
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: presenter.playButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.visible: hovered

            onClicked: presenter.switchToPane(GamePresenter.MAIN)
            onPressed: opacity = pressedButtonOpacity
            onReleased: opacity = releasedButtonOpacity
            onCanceled: opacity = releasedButtonOpacity
        }

        Button {
            id: helpBtn

            contentItem: Text {
                text: presenter.helpButtonLabel
                color: textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: buttonColor
                border.color: borderColor
                border.width: width * 0.005
                radius: width * 0.01
            }

            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: presenter.helpButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.visible: hovered

            onClicked: presenter.switchToPane(GamePresenter.HELP)
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
                border.width: width * 0.005
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
