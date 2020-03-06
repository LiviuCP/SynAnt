import QtQuick 2.7
import QtQuick.Controls 2.2
import GameUtils 1.0

Button {
    id: button

    signal buttonClicked

    property string buttonLabel : ""
    property string buttonToolTip: ""
    property string shortcutSequence: ""

    property double buttonOpacity: Styles.releasedButtonOpacity

    property bool dedicatedShortcutEnabled: true
    property bool buttonEnabled: true

    property alias isActiveFocusOnTabEnabled: button.activeFocusOnTab

    readonly property int shortcutActivationDelay: 50

    property Timer shortcutActivationTimer: Timer {
        interval: shortcutActivationDelay
        onTriggered: {
            buttonOpacity = Styles.releasedButtonOpacity;
            buttonClicked();
        }
    }

    enabled: buttonLabel !== "" && buttonEnabled

    contentItem: Text {
        text: buttonLabel
        color: Styles.textColor
        opacity: enabled ? buttonOpacity : Styles.disabledButtonOpacity
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        color: Styles.pushButtonColor
        opacity: enabled ? buttonOpacity : Styles.disabledButtonOpacity
        radius: Styles.pushButtonRadius

        border {
            color: Styles.borderColor
            width: Styles.borderWidth
        }
    }

    ToolTip {
        text: buttonToolTip
        enabled: text !== ""
        visible: button.hovered
        delay: Animations.toolTipDelay
        timeout: Animations.toolTipTimeout
    }

    Shortcut {
        sequence: shortcutSequence
        enabled: shortcutSequence !== "" && dedicatedShortcutEnabled

        onActivated: {
            buttonOpacity = Styles.pressedButtonOpacity;
            shortcutActivationTimer.start();
        }
    }

    onClicked: buttonClicked()
    onPressed: opacity = Styles.pressedButtonOpacity
    onReleased: opacity = Styles.releasedButtonOpacity
    onCanceled: opacity = Styles.releasedButtonOpacity
}
