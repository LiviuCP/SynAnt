import QtQuick 2.7
import QtQuick.Controls 2.2
import GameUtils 1.0

RadioButton {
    id: radioButton

    signal buttonToggled

    property string buttonLabel : ""
    property string shortcutSequence: ""

    property bool buttonChecked: false
    property bool dedicatedShortcutEnabled: true
    property bool buttonEnabled: true

    property alias isActiveFocusOnTabEnabled: radioButton.activeFocusOnTab

    enabled: buttonLabel !== "" && buttonEnabled
    checked: buttonChecked

    text: buttonLabel

    Shortcut {
        sequence: shortcutSequence
        enabled: shortcutSequence !== "" && dedicatedShortcutEnabled

        onActivated: {
            if (!checked) {
                checked = true;
                buttonToggled();
            }
        }
    }

    onToggled: {
        checked = true;
        buttonToggled();
    }
}
