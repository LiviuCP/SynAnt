pragma Singleton

import QtQuick 2.7

QtObject {
    id: styles

    // colors
    readonly property string backgroundColor: "lightblue"
    readonly property string markedForDeletionColor: "brown"
    readonly property string pushButtonColor: "lightgrey"
    readonly property string borderColor: "black"
    readonly property string selectedBorderColor: "white"
    readonly property string firstWordInputBorderColor: "green"
    readonly property string secondWordInputBorderColor: "darkorange"
    readonly property string textColor: "black"
    readonly property string fatalErrorTextColor: "red"
    readonly property string wordPieceSelectedColor: "grey"

    // opacities
    readonly property double pressedButtonOpacity: 0.5
    readonly property double releasedButtonOpacity: 1.0
    readonly property double disabledButtonOpacity: 0.3
    readonly property double hoverOpacity: 0.8
    readonly property double pressedOpacity: 0.5
    readonly property double defaultOpacity: 1.0

    // widths
    readonly property int borderWidth: 1
    readonly property int selectedBorderWidth: 2

    // button behavior when using shortcuts
    property QtObject button

    property Timer pressOpacityTimer: Timer {
        interval: 50
        onTriggered: revertButtonOpacityAfterShortcutActivation(button);
    }

    function updateButtonOpacityAtShortcutActivation(btn) {
        btn.buttonOpacity = pressedButtonOpacity;
        button = btn;
        pressOpacityTimer.start();
    }

    function revertButtonOpacityAfterShortcutActivation(btn) {
        btn.buttonOpacity = releasedButtonOpacity;
    }
}
