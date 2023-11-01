import QtQuick 2.15

Item {
    id: eyes

    property real radius: 10

    Repeater {
        model: 2

        Rectangle {
            color: "white"
            radius: eyes.radius
            x: (modelData - 1) * radius * 2.5 + radius/2.5
            y: -radius * 1.5
            width: radius * 2
            height: radius * 2

            Rectangle {
                color: "black"
                radius: eyes.radius/2
                width: radius * 2
                height: radius * 2
                x: radius
                y: radius/2
            }
        }
    }
}
