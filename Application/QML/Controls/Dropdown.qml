import QtQuick 2.7
import QtQuick.Controls 2.2

ComboBox {
    id: dropdown

    property bool disableFirstElementAtIndexChange: false
    property bool dropdownEnabled: false    // use this property and not enabled to ensure the dropdown can be altered

    signal itemChanged

    property var dataModel: null
    property string dropdownToolTip: ""

    enabled: dropdownEnabled && dataModel !== null && count > 0

    delegate: ItemDelegate {
        text: dataModel[index]
        enabled: (index !== 0 || currentIndex == 0 || !disableFirstElementAtIndexChange) && dropdownEnabled
        height: enabled ? implicitHeight : 0
        visible: enabled
        font.bold: index === currentIndex

        onClicked: {
            if (index !== currentIndex) {
                currentIndex = index;
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
