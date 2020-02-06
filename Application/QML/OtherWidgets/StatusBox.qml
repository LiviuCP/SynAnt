import QtQuick 2.7
import QtQuick.Controls 2.2
import GameUtils 1.0

Item {
    id: statusBox

    property string statusMessage
    property string boxToolTip
    property bool isErrorStatus: false

    Rectangle {
        color: Styles.backgroundColor
        border.color: Styles.borderColor

        anchors.fill: parent

        MouseArea {
            id: statusBoxMouseArea

            anchors.fill: parent
            hoverEnabled: true
        }

        ToolTip {
            text: boxToolTip
            visible: statusBoxMouseArea.containsMouse && boxToolTip !== ""
            delay: 1000
            timeout: 4000
        }

        Text {
            text: statusMessage
            color: isErrorStatus ? Styles.fatalErrorTextColor : Styles.textColor

            width: parent.width
            wrapMode: Text.WordWrap

            font.pointSize: 12
            minimumPointSize: 8
            fontSizeMode: Text.Fit

            anchors {
                fill: parent
                leftMargin: parent.width * 0.02
                rightMargin: parent.width * 0.02
            }
        }
    }
}
