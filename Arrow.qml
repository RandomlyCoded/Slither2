import QtQuick 2.15
import Slither 1.0

Image {
    visible: true
    width: 50
    height: 50
    smooth: true
    property point position
    property vector2d direction
    source: "qrc:/assets/assets/arrow.svg"
    readonly property int factor: 360
    readonly property real angle: Math.atan2(direction.y, direction.x)
    position: position + Qt.point(Math.cos(angle), Math.sin(angle))
    x: position.x
    y: position.y
//    rotation: Math.atan2(direction.y, direction.x)
//    onRotationChanged: console.info("ATAN2:", Math.floor(Math.atan2(direction.x, direction.y) * 100)/100,          "\n    ",
//                                    "ANGLE:", Math.floor(factor * Math.atan2(direction.x, direction.y) * 100)/100, "\n    ",
//                                    "DIREC:", direction)
}
