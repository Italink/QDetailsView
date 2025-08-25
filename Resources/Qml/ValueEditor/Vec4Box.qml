import QtQuick;
import QtQuick.Controls;
import QtQuick.Layouts;

Item{
    id: control
    property vector4d value
    implicitHeight: 25
    signal asValueChanged(value:var)
    function setValue(newValue:var){
        if(value !== newValue){
            value = newValue
            asValueChanged(value)
        }
    }
    RowLayout{
        anchors.fill: parent
        NumberBox{
            id: xBox
            width: parent.width/4
            Layout.alignment: Qt.AlignLeft
            number: value.x
            onNumberChanged: {
                if (control.value) {
                    control.setValue(Qt.vector4d(number, control.value.y, control.value.z, control.value.w))
                }
            }
        }
        NumberBox{
            id: yBox
            width: parent.width/4
            Layout.alignment: Qt.AlignLeft
            number: value.y
            onNumberChanged: {
                if (control.value) {
                    control.setValue(Qt.vector4d(control.value.x, number, control.value.z, control.value.w))
                }
            }
        }
        NumberBox{
            id: zBox
            width: parent.width/4
            Layout.alignment: Qt.AlignLeft
            number: value.z
            onNumberChanged: {
                if (control.value) {
                    control.setValue(Qt.vector4d(control.value.x, control.value.y, number, control.value.w))
                }
            }
        }
            NumberBox{
            id: wBox
            width: parent.width/4
            Layout.alignment: Qt.AlignLeft
            number: value.w
            onNumberChanged: {
                if (control.value) {
                    control.setValue(Qt.vector4d(control.value.x, control.value.y, control.value.z, number))
                }
            }
        }
        Item {
               Layout.fillWidth: true
        }
    }
}
