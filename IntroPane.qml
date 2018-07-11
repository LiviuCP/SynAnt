import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    id: introPane

    property QtObject presenter;

    readonly property double introPaneRectHeight: height * 0.9;
    readonly property double bottomBtnsLayoutHeight: height * 0.1;

    readonly property double bottomBtnsMinWidth: (introPaneRect.width - 2 * bottomBtnsLayout.spacing) / 3

    Rectangle {
        id: introPaneRect
        anchors.top: parent.top
        width: parent.width
        height: introPaneRectHeight
        color: "grey"
        border {
            color: "white"
        }
        Text {
            id: introPaneText
            anchors.fill: parent
            anchors.leftMargin: parent.width * 0.005
            text: presenter.introPaneMessage
            color: "white"
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
            text: "Play"
            Layout.minimumWidth: bottomBtnsMinWidth
            onClicked: {
                presenter.switchToMainPane();
            }
        }

        Button {
            id: helpBtn
            text: "Help"
            Layout.minimumWidth: bottomBtnsMinWidth
            onClicked: {
                presenter.switchToHelpPane();
            }
        }

        Button {
            id: quitBtn
            text: "Quit"
            Layout.minimumWidth: bottomBtnsMinWidth
            onClicked: Qt.quit()
        }
    }
}
