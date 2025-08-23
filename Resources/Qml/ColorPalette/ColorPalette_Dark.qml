import QtQuick

pragma Singleton

QtObject {
    property color background: "#2D2D2D"
    property color border: "#444444"
    property color hover: "#336699CC"
    property color textPrimary: "#FFFFFF"
    property color gradientStart: Qt.rgba(255, 255, 255, 0)
    property color gradientEnd: Qt.rgba(255, 255, 255, 1)
    property color surface: "#3D3D3D"
}
    