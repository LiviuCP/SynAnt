import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    id: helpPane

    property QtObject presenter;

    readonly property double helpPaneRectHeight: height * 0.9
    readonly property double bottomBtnsLayoutHeight: height * 0.1
    readonly property double bottomBtnsMinWidth: (helpPaneRect.width - bottomBtnsLayout.spacing) * 0.5

    readonly property color paneColor: "grey"
    readonly property color borderColor: "white"
    readonly property color textColor: "white"

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
            text: gamePresenter.okButtonLabel
            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: presenter.playButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.visible: hovered

            onClicked: {
                presenter.switchToMainPane();
            }
        }

        Button {
            id: quitBtn
            text: gamePresenter.quitButtonLabel
            Layout.minimumWidth: bottomBtnsMinWidth

            ToolTip.text: presenter.quitButtonToolTip
            ToolTip.delay: presenter.toolTipDelay
            ToolTip.visible: hovered

            onClicked: Qt.quit()
        }
    }
}
