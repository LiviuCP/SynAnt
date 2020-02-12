import QtQuick 2.7
import QtQuick.Controls 2.2

ComboBox {
    id: dropdown

    property var dataModel: null

    property string dropdownToolTip: ""
    property string noItemSelectedText: ""

    property int selectedIndex: -1 // don't modify this property manually, it is only meant to be used by the onItemChanged() slot

    property bool dropdownEnabled: false    // use this property and not enabled to ensure the dropdown can be altered

    signal itemChanged

    enabled: dropdownEnabled && dataModel !== null && count > 0

    Text {
        z:1
        text: noItemSelectedText

        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.verticalCenter: parent.verticalCenter
        visible: currentIndex === -1
    }

    delegate: ItemDelegate {
        id: dropdownDelegate

        width: parent.width
        height: enabled ? implicitHeight : 0

        text: dataModel[index]
        font.bold: index === currentIndex

        onClicked: {
            if (index !== currentIndex) {
                selectedIndex = index;
                itemChanged();
            }
        }
    }

    ToolTip.visible: hovered && dropdownToolTip !== ""
    ToolTip.text: dropdownToolTip
    ToolTip.delay: 1000
    ToolTip.timeout: 4000

    editable: false
    model: dataModel
}
