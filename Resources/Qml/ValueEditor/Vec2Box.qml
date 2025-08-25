import QtQuick;
import QtQuick.Controls;
import QtQuick.Layouts;

Item{
    id: control
    property vector2d value
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
                    control.setValue(Qt.vector2d(number, control.value.y))
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
                    control.setValue(Qt.vector2d(control.value.x, number))
                }
            }
        }
        Item {
               Layout.fillWidth: true
        }
    }
}
