import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import QtCore 
import ColorPalette

Item{
    id: control
    property var value
    implicitHeight: dirBox.implicitHeight
    signal asValueChanged(text: var)

    function setValue(newValue: var){
        if(newValue !== value){
            value = newValue
            dirBox.value = value
            asValueChanged(value)
        }
    }

    LineTextBox {
        id: dirBox
        value: control.value
        anchors.left: parent.left
        anchors.right: button.left
        anchors.verticalCenter: parent.verticalCenter
        onValueChanged: {
            control.setValue(value)
        }
    }
    
    Button {
        id: button
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        width: 30 
        height: 25  
        text: "..."
        palette.buttonText: ColorPalette.theme.textPrimary
        background: Rectangle {
            color:  ColorPalette.theme.buttonBackground
        }

        onClicked: {
            folderDialog.open()
        }
    }
    FolderDialog {
        id: folderDialog
        title: "选择目录"
        onAccepted: {
            var filePath = currentFolder.toString();
            
            if (filePath.startsWith("file:///")) {
                filePath = filePath.substring(8);
            }
        
            control.setValue(filePath);
        }
    }
}

