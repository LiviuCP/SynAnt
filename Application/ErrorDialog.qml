import QtQuick 2.7
import QtQuick.Dialogs 1.1

Item {
    readonly property string windowTitle: "Fatal Error!"
    readonly property string windowName: "errorDialog"

    MessageDialog {
        id: errorDialog

        objectName: windowName
        title: windowTitle

        onAccepted: {
            Qt.quit()
        }

        Component.onCompleted: visible = true
    }
}
