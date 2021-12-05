import QtQuick 2.15

Item {
    id: eyes

    property real viewAngle: 0
    property real radius: 10

    rotation: viewAngle * 24

    Row {
        spacing: radius/4

        Repeater {
            model: 2

            Rectangle {
                rotation: viewAngle
                color: "white"
                radius: eyes.radius
                width: radius * 2
                height: radius * 2

                Rectangle {
                    color: "black"
                    radius: eyes.radius/2
                    width: radius * 2
                    height: radius * 2
                    x: radius + radius * Math.cos(viewAngle)
                    y: radius + radius * Math.sin(viewAngle)
                }
            }
        }
    }
}