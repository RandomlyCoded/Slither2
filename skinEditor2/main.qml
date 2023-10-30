import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Shapes 1.15

import Editor 1.0

Window {
    id: mainWnd
    flags: Qt.Sheet
    width: 750
    height: 600
    visible: true
    color: "#1f1f1f"
    title: "offline Slither Skin Editor"
    property int i: 0

    Data {
        id: myData
    }

    Column {
        anchors.centerIn: parent
        spacing: 10

        Rectangle {
            id: skinField
            width: editSkin.width
            height: editSkin.height
            color: "transparent"

            SkinVis {
                id: editSkin
                anchors.centerIn: parent
            }
        }

        Rectangle {
            id: __nameInput_rect
            width: Math.max(nameInput.width, __nameInput_text.width) + 4
            height: __nameInput_text.height + 4
            color: "transparent"
            border { color: "black"; width: 1 }

            x: parent.width/2 - width/2

            TextInput {
                id: nameInput
                //                font.family: "qtds_propertyIconFont.ttf" mal Font angucken
                font.pixelSize: 20
                maximumLength: 14
                anchors.centerIn: parent
                width: Math.max(__TEXT.width, __nameInput_text.width)
                height: __nameInput_text.height
                color: "white"

                x: 2
                y: 2

                onEditingFinished: { myData.getPlayerName(text); }
                Keys.onEscapePressed: nameInput.clear()

                Text {
                    id: __nameInput_text
                    font.pixelSize: 20
                    anchors.centerIn: parent
                    visible: !parent.text
                    color: "white"
                    text: "enter playername\t\t"
                }

                Text {
                    id: __TEXT
                    visible: false
                    text: parent.text
                }
            }
        }

        ColorGrid {
            x: mainWnd.width/2 - width/1.5
//            Text {
//                rotation: 22.5
//                anchors.centerIn: parent

//                text: "hier kommen später NOCH MEHR(alle freuen sich!!!)🥳 bunte Kugeln hin!!!🥳🎆"
//                font.pixelSize: 17
//            }
        }

        Row {
            x: 5
            spacing: 5
            Button {
                label: "create own\n    color"
                textSize: 18
                onActivated: myData.createColor()
            }

            Row {
                id: animationSettings
                spacing: 5
                Button {
                    id: animationStartStop
                    label: "animate"
                    textSize: 20

                    onActivated: editSkin.animate = !editSkin.animate
                }

                Row {
                    id: animationData
                    spacing: 2

                    Button {
                        id: animationIncreement
                        textSize: 20
                        label: "+"

                        onActivated: myData.animationInterval < 1000 ? myData.setAnimationInterval(myData.animationInterval + 1) : 0
                    }

                    Text {
                        id: animationSpeedInfo
                        anchors.verticalCenter: animationIncreement.verticalCenter
                        font.pixelSize: 15
                        color: "white"
                        text: "animation interval: "
                    }

                    TextInput {
                        id: animationIntervalInput
                        text: myData.animationInterval
                        color: "lightgray"
                        anchors.verticalCenter: animationIncreement.verticalCenter
                        onTextEdited:
                            if(text && // is there input?
                                    (text !== "0" && text !== "00" && text !== "000")) // is the input unequal to 0?
                                myData.setAnimationInterval(myData.getInt(text))
                        font.pixelSize: 15
                        maximumLength: 3
                    }
                    Text {
                        anchors.verticalCenter: animationIncreement.verticalCenter
                        color: "white"
                        text: " ms"
                        font.pixelSize: 15
                    }

                    Button {
                        id: animationDecreement
                        label: "-"
                        textSize: 20

                        onActivated: myData.animationInterval > 1 ? myData.setAnimationInterval(myData.animationInterval - 1) : 0
                    }
                }
            }
        }

        Row {
            spacing: 5
            x: parent.width/2 - width/2

            Column {
                id: skinButtons
                spacing: 5
                Button {
                    id: saveButton
                    label: "Save Skin"
                    textSize: 20
                    onActivated: myData.save()
                }

                Button {
                    id: loadButton
                    label: "Load Skin"
                    textSize: 20
                    onActivated: myData.loadSkin()
                }
            }

            Button {
                id: gameStartButton
                textSize: 19
                label: "start Game"

                onActivated: myData.startGame()
            }

            Column {
                id: pendantButtons

                spacing: 5

                Button {
                    id: pendantCreateButton
                    textSize: 18

                    label: " create\npendant"

                    onActivated: console.info(pendantCreateButton, "was clicked: create pendant")
                }

                Button {
                    id: pendantLoadButtons
                    textSize: 18
                    label: " load\npendant"
                    onActivated: console.info("want to call\"void myData::loadPendant()\"")
                }
            }
        }

        Button {
            x: 5
            id: exitEditor
            textSize: 20
            label: "exit Editor"
            onActivated: Qt.quit()
        }
    }
}
