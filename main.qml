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
    property bool debugging: false

    width: 640
    height: 480
    visible: true
    visibility: Window.Maximized

    flags: Qt.Sheet
    onVisibleChanged: playground.initialize(0);

    Playground {
        id: playground
    }

    // FIXME: es gibt jetzt auch m_snakes(von playground), das heißt, dass ich als nächstes gerne einen Repeater einbauen würde, der ALLE der Schlangen anzeigst

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
        position: playgroundView.center
        direction: Qt.vector2d(mouseArea.mouseX, mouseArea.mouseY)
    }

    Row {
        spacing: 1
        Button {
            text: "Spawn"
            font.pixelSize: 12

            onClicked: {
                if(snake.isAlive) {
                    playground.killSnake(snake)
                    console.info("snake lived so kill it")
                }
                snake.spawn(Qt.point(10, 10), Qt.point(0, 0));
                playground.addSnake(snake); // FIXME: _maybe_ rewrite into playground.spawnSnake() ?
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
            text: "use/unuse bot"
            font.pixelSize: 12

            onClicked: {
                snake.changeBotUsing()
                console.info("using bot:", snake.useBot)
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
            text: "Run"

            onToggled: timer.running ^= true
        }

        Text {
            text: "your lenght: " + snake.lenght
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
            if(event.key === Qt.Key_S) {
                if(snake.isAlive)
                                playground.killSnake(snake)
                snake.spawn(Qt.point(10, 10), Qt.point(0, 0));
                playground.addSnake(snake); // FIXME: _maybe_ rewrite into playground.spawnSnake() ?
                snake.changeBotUsing()
            }
            if(event.key === Qt.Key_K)
                playground.killSnake(snake)
            if(event.key === Qt.Key_B)
                snake.changeBotUsing()
            if(event.key === Qt.Key_F3 || event.key === Qt.Key_D)
                debugging = !debugging
            if(event.key === Qt.Key_R)
                timer.running = !timer.running
            if(event.key === Qt.Key_Save)
                playground.save()
        }
    }

    Minimap {
        x: parent.width - width - 10
        y: parent.height - height - 10
    }

    DebugScreen {
        visible: debugging
    }
}
