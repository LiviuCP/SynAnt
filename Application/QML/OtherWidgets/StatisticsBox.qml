import QtQuick 2.7
import QtQuick.Controls 2.2
import GameUtils 1.0

Item {
    id: statisticsBox

    property string boxLabel
    property string leftValue
    property string rightValue

    property string boxToolTip: ""

    Rectangle {
        id: label

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        color: Styles.statisticsBoxBackgroundColor
        border.color: Styles.borderColor

        width: 1/3 * parent.width
        height: parent.height

        Text {
            anchors {
                fill: parent
                leftMargin: parent.width * 0.02
            }

            text: boxLabel
        }
    }

    Rectangle {
        id: firstValue

        anchors.left: label.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        color: Styles.statisticsBoxValueBackgroundColor
        border.color: Styles.borderColor

        width: 1/3 * parent.width
        height: parent.height

        Text {
            anchors.centerIn: parent
            text: leftValue
        }
    }

    Rectangle {
        id: secondValue

        anchors.left: firstValue.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        color: Styles.statisticsBoxValueBackgroundColor
        border.color: Styles.borderColor

        width: 1/3 * parent.width
        height: parent.height

        Text {
            anchors.centerIn: parent
            text: rightValue
        }
    }

    MouseArea {
        id: statisticsMouseArea

        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        text: boxToolTip
        visible: statisticsMouseArea.containsMouse && boxToolTip !== ""
        delay: 1000
        timeout: 4000
    }
}
