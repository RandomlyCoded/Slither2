import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

import Slither 1.0

Window {
    id: main

    property color normalColor: "red"
    property color speedColor: "#ff5555"
    property real fps: 30
    property real zoom: 16

    width: 640
    height: 480
    visible: true
    visibility: Window.Maximized

    Playground {
        id: playground
    }

    Snake {
        id: snake

        destination: Qt.point((mouseArea.mouseX - playgroundView.center.x) / zoom,
                              (mouseArea.mouseY - playgroundView.center.y) / zoom)

        speed: mouseArea.pressed && snake.canBoost ? 10 : 5
    }

    Rectangle {
        id: playgroundView

        readonly property point center: Qt.point(width/2, height/2)

        color: "gray"
        radius: playground.size * zoom

        x: (main.width - width)/2
        y: (main.height - height)/2
        width: radius * 2
        height: radius * 2

        MouseArea {
            id: mouseArea

            anchors.fill: parent
            hoverEnabled: true

            onWheel: {
                if (wheel.angleDelta.y < 0 && zoom > 1)
                    zoom /= 2;
                else if (wheel.angleDelta.y > 0 && zoom < 32)
                    zoom *= 2;
            }
        }

        Repeater {
            id: pearlView

            model: playground.energyPearls

            Rectangle {
                readonly property var pearl: model.data

                color: pearl.color
                radius: zoom * pearl.amount
                width: radius * 2
                height: radius * 2

                x: playgroundView.center.x + zoom * pearl.position.x - width/2
                y: playgroundView.center.y + zoom * pearl.position.y - height/2
            }
        }
    }

    Item {
        anchors.fill: playgroundView

        Repeater {
            id: snakeView

            model: snake.segments

            Rectangle {
                id: segmentView

                readonly property int index: model.index
                readonly property point segment: modelData

                x: playgroundView.center.x + segment.x * zoom - width/2
                y: playgroundView.center.y + segment.y * zoom - height/2
                z: snakeView.count - model.index

                border { width: 1; color: "black" }

                property int speeding: mouseArea.pressed
                gradient: Gradient {
                    GradientStop { position: 0; color: [["red", "#ff5555"], ["#ff7f00", "orange"], ["yellow", "#ffff66"], ["limegreen", "#55ff55"], ["cyan", "#00ffff"], ["blue", "#5555ff"], ["#6a006a", "#7f007f"]][model.index % 7][speeding]; }
                    GradientStop { position: 0.9; color: [["red", "#ff5555"], ["#ff7f00", "orange"], ["yellow", "#ffff66"], ["limegreen", "#55ff55"], ["cyan", "#00ffff"], ["blue", "#5555ff"], ["#6a006a", "#7f007f"]][(model.index + 1) % 7][speeding]; }
                }

                color: [normalColor, speedColor][speeding]

                property bool isLast: segmentView.index === snakeView.count - 1
                Behavior on radius {
                    animation: NumberAnimation {
                        duration: 1000
                        from: 0
                        to: 0
                    }
                }
//                onRadiusChanged: if(radius !== 16) console.info(index, radius)

                radius: {
                    let r = zoom;

                    if (isLast) {
                        r *= Math.min(1 + snake.load, 1);
                    }


                    return r;
                }

                width: radius * 2
                height: radius * 2

                Eyes {
//                    rotation: Math.atan2(snake.direction.y, snake.direction.x)
                    anchors.horizontalCenter: parent.horizontalCenter
                    viewAngle: Math.atan2(snake.direction.y, snake.direction.x)
                    radius: zoom/2
                    visible: segmentView.index === 0
                }
            }
        }
    }

    Row {
        Button {
            text: "Spawn"

            onClicked: {
                playground.initialize(0);
                snake.spawn(Qt.point(10, 10), Qt.point(0, 0));
                playground.addSnake(snake); // FIXME: _maybe_ rewrite into playground.spawnSnake() ?
            }
        }

        CheckBox {
            checked: timer.running
            text: "Run"

            onToggled: timer.running = !timer.running
        }

        Text {
            text: "your lenght: " + snakeView.count
        }
    }

    Timer {
        id: timer

        repeat: true
        interval: 1000/fps
        running: true

        property real lastTick: now()

        function now() {
            return new Date().getTime();
        }

        onTriggered: {
            let currentTick = now();
            let dt = (currentTick - lastTick) / 1000;
            lastTick = currentTick;

            snake.move(dt);
        }
    }
}
