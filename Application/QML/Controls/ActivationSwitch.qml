import QtQuick 2.7
import QtQuick.Controls 2.2
import GameUtils 1.0

Item {
    id: activationSwitchContainer

    property string leftSwitchLabel: ""
    property string rightSwitchLabel: ""

    property string shortcutSequence: ""

    property string switchToolTip: ""

    readonly property int toolTipDelay: 1000
    readonly property int toolTipTimeout: 4000

    property double customHeight: 28    // use this to determine the actual height of the widget (also add same preferredHeight in Column/Grid layouts)
    readonly property double margin: 7

    property alias switchChecked: activationSwitch.checked
    property alias switchEnabled: activationSwitch.enabled
    property bool dedicatedShortcutEnabled: true

    signal switchClicked

    opacity: activationSwitch.enabled ? Styles.releasedButtonOpacity : Styles.disabledButtonOpacity

    Rectangle {
        id: switchBackground

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        color: Styles.statusBoxBackgroundColor

        width: parent.width
        height: activationSwitch.implicitHeight
    }

    MouseArea {
        id: toolTipMouseArea
        anchors.fill: switchBackground
        hoverEnabled: true
    }

    Text {
        id: leftText

        anchors.left: parent.left
        anchors.leftMargin: margin
        anchors.verticalCenter: parent.verticalCenter

        text: leftSwitchLabel

        color: Styles.switchTextNoHighlight

        visible: leftSwitchLabel !== ""
    }

    Switch {
        id: activationSwitch

        readonly property double switchHeight: customHeight * 0.7

        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        indicator: Rectangle {
                      x: activationSwitch.width - width - activationSwitch.rightPadding
                      y: parent.height / 2 - height / 2
                      implicitWidth: 2 * activationSwitch.switchHeight
                      implicitHeight: activationSwitch.switchHeight
                      radius: 0.5 * activationSwitch.switchHeight

                      color: activationSwitch.checked && (!leftText.visible || !rightText.visible) ? Styles.switchOnColor : Styles.switchOffColor
                      border.color: Styles.switchBorderColor

                      Rectangle {
                          x: activationSwitch.checked ? parent.width - width : 0
                          width: activationSwitch.switchHeight
                          height: activationSwitch.switchHeight
                          radius: 0.5 * activationSwitch.switchHeight

                          border.color: Styles.switchBorderColor
                      }
                  }

        text: undefined

        Shortcut {
            sequence: shortcutSequence
            enabled: shortcutSequence !== "" && dedicatedShortcutEnabled

            onActivated: activationSwitchContainer.switchClicked()
        }

        onClicked: activationSwitchContainer.switchClicked()
    }

    Text {
        id: rightText

        anchors.right: parent.right
        anchors.rightMargin: margin
        anchors.verticalCenter: parent.verticalCenter

        text: rightSwitchLabel

        color: Styles.switchTextNoHighlight

        visible: rightSwitchLabel !== ""
    }

    ToolTip {
        text: switchToolTip
        enabled: text !== ""
        visible: toolTipMouseArea.containsMouse || activationSwitch.hovered
        delay: toolTipDelay
        timeout: toolTipTimeout
    }

    states: [
        State {
            name: "noTextVisible"
            when: !leftText.visible && !rightText.visible

            PropertyChanges {
                target: leftText
                anchors.leftMargin: undefined
            }

            AnchorChanges {
                target: leftText

                anchors.left: undefined
                anchors.verticalCenter: undefined
            }

            AnchorChanges {
                target: activationSwitch

                anchors.right: undefined
                anchors.horizontalCenter: parent.horizontalCenter
            }
        },

        // default state
        State {
            name: "leftTextVisible"
            when: leftText.visible && !rightText.visible
        },

        State {
            name: "rightTextVisible"
            when: !leftText.visible && rightText.visible

            PropertyChanges {
                target: activationSwitch

                anchors.leftMargin: -margin
            }

            AnchorChanges {
                target: activationSwitch

                anchors.right: undefined
                anchors.left: parent.left
            }
        },

        State {
            name: "bothTextsVisible"
            when: leftText.visible && rightText.visible

            AnchorChanges {
                target: activationSwitch

                anchors.right: undefined
                anchors.horizontalCenter: parent.horizontalCenter
            }

            PropertyChanges {
                target: leftText

                anchors.leftMargin: undefined
                anchors.rightMargin: margin

                color : activationSwitch.checked ? Styles.switchTextNoHighlight : Styles.switchHighlightedText
            }

            AnchorChanges {
                target: leftText

                anchors.left: undefined
                anchors.right: activationSwitch.left
            }

            PropertyChanges {
                target: rightText

                anchors.rightMargin: undefined
                anchors.leftMargin: margin

                color : activationSwitch.checked ? Styles.switchHighlightedText : Styles.switchTextNoHighlight
            }

            AnchorChanges {
                target: rightText

                anchors.right: undefined
                anchors.left: activationSwitch.right
            }
        }
    ]
}
