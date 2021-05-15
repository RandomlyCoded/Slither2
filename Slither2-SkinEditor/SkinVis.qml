import QtQuick 2.15

Rectangle {
    property bool animate: false
    property int myRad: 30
    property int step: 0
    property int scaling: myRad/5
    property int animationInterval: 50

    onAnimateChanged: !animateTimer.running ? animateTimer.start() : animateTimer.stop()

    width: 100 * scaling + myRad - scaling
    height: myRad
    color: "transparent"

    Repeater {
        anchors.centerIn: parent
        model: myData.skin

        Circle {
            x: model.index * scaling
            y: Math.sin((model.index + step) * scaling) * scaling
            rad: myRad

            color: modelData

            MouseArea {
                anchors.fill: parent

                onClicked: console.info(myData.getSkin)
            }
        }
    }

    Timer {
        id: animateTimer
        interval: animationInterval // LOL AUF 1 SETZEN!!!
        repeat: true
        onTriggered: step ++
    }
}
