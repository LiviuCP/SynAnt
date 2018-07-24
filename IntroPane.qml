import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    id: introPane

    property QtObject presenter;

    readonly property double introPaneRectHeight: height * 0.9
    readonly property double bottomBtnsLayoutHeight: height * 0.1

    readonly property double bottomBtnsMinWidth: (introPaneRect.width - 2 * bottomBtnsLayout.spacing) / 3

    readonly property color paneColor: "grey"
    readonly property color borderColor: "white"
    readonly property color textColor: "white"

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
            text: presenter.playButtonLabel
            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: presenter.playButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.visible: hovered

            onClicked: {
                presenter.switchToMainPane();
            }


        }

        Button {
            id: helpBtn
            text: presenter.helpButtonLabel
            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: presenter.helpButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.visible: hovered

            onClicked: {
                presenter.switchToHelpPane();
            }
        }

        Button {
            id: quitBtn
            text: presenter.quitButtonLabel
            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: presenter.quitButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.visible: hovered

            onClicked: Qt.quit()
        }
    }
}
