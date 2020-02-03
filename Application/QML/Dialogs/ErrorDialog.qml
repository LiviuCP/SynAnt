import QtQuick 2.7
import QtQuick.Dialogs 1.1
import GameUtils 1.0

Item {
    MessageDialog {
        id: errorDialog

        title: GameStrings.fatalErrorWindowTitle
        objectName: GameStrings.fatalErrorWindowObjectName

        onAccepted: Qt.quit()
        Component.onCompleted: visible = true
    }
}
