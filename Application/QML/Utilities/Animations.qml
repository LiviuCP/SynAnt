pragma Singleton

import QtQuick 2.7
import GameUtils 1.0

QtObject {
    id: animations

    // delays
    readonly property int toolTipDelay: 1000
    readonly property int toolTipTimeout: 4000

    // flick velocities
    readonly property int flickDownVelocity: 500
    readonly property int flickUpVelocity: -500

    // button behavior when using shortcuts
    property QtObject button

    property Timer pressOpacityTimer: Timer {
        interval: 50
        onTriggered: revertButtonOpacityAfterShortcutActivation(button);
    }

    function updateButtonOpacityAtShortcutActivation(btn) {
        btn.buttonOpacity = Styles.pressedButtonOpacity;
        button = btn;
        pressOpacityTimer.start();
    }

    function revertButtonOpacityAfterShortcutActivation(btn) {
        btn.buttonOpacity = Styles.releasedButtonOpacity;
    }
}
