import QtQuick 2.15
import Slither 1.0

Image {
    visible: false
    width: 50
    height: 50
    smooth: true
    property point position
    property vector2d direction
    source: "qrc:/assets/assets/arrow.svg"
    x: position.x - width
    y: position.y - height
    rotation: Math.atan2(direction.y, direction.x) * 180/Math.PI + 90
}
