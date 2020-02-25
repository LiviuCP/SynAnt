pragma Singleton

import QtQuick 2.7

QtObject {
    id: styles

    // colors
    readonly property color backgroundColor: "#add8e6"
    readonly property color statusBoxBackgroundColor: "#dddddd"
    readonly property color statisticsBoxBackgroundColor: "#bebebe"
    readonly property color statisticsBoxValueBackgroundColor: "#eeeeee"
    readonly property color mixedPiecesBackgroundColor: "#a0a0a0"
    readonly property color firstWordInputBackgroundColor: "#cccccc"
    readonly property color secondWordInputBackgroundColor: "#dedede"
    readonly property color markedForDeletionColor: "#9f4513"
    readonly property color pushButtonColor: "#d3d3d3"
    readonly property color switchOnColor: "#00aaff"
    readonly property color switchOffColor: "#999999"
    readonly property color switchBorderColor: "#333333"
    readonly property color switchTextNoHighlight: "#000000"
    readonly property color switchHighlightedText: "#ffffff"
    readonly property color borderColor: "#000000"
    readonly property color mixedPiecesBorderColor: "#444444"
    readonly property color selectedBorderColor: "#ffffff"
    readonly property color firstWordInputBorderColor: "#008000"
    readonly property color secondWordInputBorderColor: "#ff8c00"
    readonly property color textColor: "#000000"
    readonly property color fatalErrorTextColor: "#ff0000"

    // opacities
    readonly property double pressedButtonOpacity: 0.5
    readonly property double releasedButtonOpacity: 1.0
    readonly property double disabledButtonOpacity: 0.3
    readonly property double hoverOpacity: 0.7
    readonly property double pressedOpacity: 0.5
    readonly property double defaultOpacity: 1.0

    // dimensions
    readonly property int borderWidth: 1
    readonly property int selectedBorderWidth: 2
    readonly property int pushButtonRadius: 5

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
