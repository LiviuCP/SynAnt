import QtQuick 2.7
import QtQuick.Controls 2.2
import GameUtils 1.0
import Animations 1.0

Item {
    id: statusBox

    property string statusMessage

    property string boxToolTip: ""

    property bool isErrorStatus: false
    property bool hasFlickable: false

    function flickUp() {
        if (hasFlickable) {
            statusBoxFlickable.flick(0, Animations.flickUpVelocity);
        }
    }

    function flickDown() {
        if (hasFlickable) {
            statusBoxFlickable.flick(0, Animations.flickDownVelocity);
        }
    }

    MouseArea {
        id: statusBoxMouseArea

        anchors.fill: parent
        hoverEnabled: true
    }

    ToolTip {
        text: boxToolTip
        visible: statusBoxMouseArea.containsMouse && boxToolTip !== ""
        delay: Animations.toolTipDelay
        timeout: Animations.toolTipTimeout
    }

    Rectangle {
        color: Styles.statusBoxBackgroundColor
        border.color: Styles.borderColor

        anchors.fill: parent

        Text {
            visible: !hasFlickable
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

        Flickable {
            id: statusBoxFlickable
            visible: hasFlickable

            clip: true
            contentHeight: flickableText.height

            anchors {
                fill: parent
                leftMargin: parent.width * 0.005
                rightMargin: parent.width * 0.005
            }

            Text {
                id: flickableText
                text: statusMessage
                color: Styles.textColor
                width: parent.width
                wrapMode: Text.WordWrap
            }
        }
    }
}
