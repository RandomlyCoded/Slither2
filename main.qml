import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15

import Slither 1.0

Window {
    id: main

    color: Qt.color("#9e0000")
    property color normalColor: "red"
    property color speedColor: "#ff5555"
    property real fps: 30
    property real zoom: 16
    property bool debugging: false

    width: 640
    height: 480
    visible: true
    visibility: Window.Maximized

    flags: Qt.Sheet
    onVisibleChanged: { playground.initialize(0); playground.load(APP.startmap); }

    Playground {
        id: playground
    }

    Snake {
        id: snake

        destination: Qt.point((mouseArea.mouseX - playgroundView.center.x) / zoom,
                              (mouseArea.mouseY - playgroundView.center.y) / zoom)

        boosting: mouseArea.pressed && snake.canBoost

        skin: ["#ff0000", "#ff7f00", "yellow", "limegreen", "cyan", "blue", "#6a006a"];
        useBot: false
    }

    Rectangle {
        id: playgroundView

        readonly property point center: Qt.point(width/2, height/2)
        property int i: 0

        color: playground.color(i)

        NumberAnimation on i {
            from: 0
            to: 359
            duration: 20000 // 200
            loops: Animation.Infinite
        }

        radius: playground.size * zoom

        x: (main.width - width)/2   - (snake.position.x * zoom)
        y: (main.height - height)/2 - (snake.position.y * zoom)
        width: radius * 2
        height: radius * 2

        border.color: "red"

        MouseArea {
            id: mouseArea

            anchors.fill: parent
            hoverEnabled: true

            onWheel: (wheel) => {
                if (wheel.angleDelta.y < 0 && zoom > 1)
                    zoom -= 1;
                else if (wheel.angleDelta.y > 0 && zoom < 32)
                    zoom += 1;
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
            model: playground.snakes

            SnakeView {
                id: snakeView
                currentSnake: modelData
            }
        }
    }

    Arrow {
        position: Qt.point(playgroundView.x + (playground.size + snake.position.x) * zoom,
                           playgroundView.y + (playground.size + snake.position.y) * zoom)
        direction: snake.direction
    }

    Row {
        spacing: 1
        Button {
            text: "Spawn"
            font.pixelSize: 12

            onClicked: {
                if(snake.isAlive) {
                    playground.killSnake(snake)
                    console.info("snake lived so killed it")
                }
                snake.spawn(Qt.point(10, 10), Qt.point(0, 0));
                playground.addSnake(snake); // FIXME: _maybe_ rewrite into playground.spawnSnake() ?
                snake.setBotType(Snake.FollowMouse)
            }
        }

        Button {
            text: "kill"
            font.pixelSize: 12

            onClicked: {
                playground.killSnake(snake)
            }
        }

        Button {
            text: "save game"
            font.pixelSize: 12

            onClicked: {
                playground.save()
                console.info("saved game")
            }
        }

        Button {
            text: "load game"
            font.pixelSize: 12

            onClicked: {
                playground.load()
                console.info("loaded game")
            }
        }

        CheckBox {
            checked: timer.running
            text: "<- Run\t"

            onToggled: timer.running ^= true
        }

        CheckBox {
            checked: snake.useBot
            text: "<- use bot\t"

            onToggled: snake.changeBotUsing()
        }

        CheckBox {
            checked: playground.masshacks
            text: "<- masshacks active\t"

            onToggled: playground.switch_masshacks()
        }

        CheckBox {
            checked: playground.checkChrash
            text: "<- check chrash\t"
            onToggled: playground.checkChrash ^= 1
        }

        Text {
            text: "your lenght: <b>" + snake.lenght + "</b>"
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
            let dt = (currentTick - lastTick/*get time passes*/) / 1000;
            lastTick = currentTick;

            playground.moveSnakes(dt);
        }
    }

    Item {
        anchors.fill: parent
        focus: true
        onFocusChanged: !focus ? focus = true : 0
        Keys.onPressed: (event) => {
            switch(event.key) {
            case Qt.Key_S: {
                if(snake.isAlive)
                                playground.killSnake(snake)
                snake.spawn(Qt.point(10, 10), Qt.point(0, 0));
                playground.addSnake(snake); // FIXME: _maybe_ rewrite into playground.spawnSnake() ?
                snake.changeBotUsing()
            } break;

            case Qt.Key_K: {
                playground.killSnake(snake)
            } break;
            case Qt.Key_B: {
                snake.changeBotUsing()
            } break;
            case Qt.Key_F3:
            case Qt.Key_D: {
                debugging = !debugging
            } break;
            case Qt.Key_R: {
                timer.running = !timer.running
            } break;
            case Qt.Key_Save: {
                playground.save()
            } break;
            case Qt.Key_F11: {
                console.info("F11")
                if(main.visibility === Window.Maximized) {
                    main.visibility = Window.Windowed
                    console.info("windowing")
                }
                else {
                    main.visibility = Window.Maximized
                    console.info("maximizing")
                }
            } break;
            case Qt.Key_N: {
                console.info("nggyu ._.")
            } break;
            case Qt.Key_M: {
                playground.switch_masshacks();
            } break;
            } // switch
        }
    }

    Minimap {
        x: parent.width - width - 10
        y: parent.height - height - 10
    }

    Rectangle {
        width: playgroundView.width
        height: playgroundView.height
        radius: playgroundView.radius
        x: playgroundView.x
        y: playgroundView.y
        color: "transparent"
        border.color: "#ff0000"
    }

    Column {
        spacing: 1
        Repeater {
            model: Leaderboard.leaderboard
            Row {
                spacing: 5
                Text {
                    text: "#" + (modelData.index + 1) + (modelData.index === 10 ? " " : "  ") + modelData.name
                }
                Text {
                    text: modelData.size
                }
            }
        }
    }

    DebugScreen {
        visible: debugging
    }
}
