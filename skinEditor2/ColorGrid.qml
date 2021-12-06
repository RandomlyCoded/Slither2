import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Shapes 1.15

import Editor 1.0

Rectangle {
    width: grid.width
    height: grid.height

    color: "transparent"

    Grid {
        id: grid
        rows: 5
        columns: 20

        spacing: 2

        Repeater {
            model: parent.rows * parent.columns

            Circle {
                property string clr: modelData < myData.colorAmount ? myData.qmlColors[modelData] : "transparent"

                property bool isUnknown: clr === "transparent"

                rad: 25

                gradient: Gradient {
                    GradientStop {
                        position: 0.000
                        color: isUnknown ? Qt.rgba(1, 0, 0, 1) : clr
                    }
                    GradientStop {
                        position: 0.167
                        color: isUnknown ? Qt.rgba(1, 1, 0, 1) : clr
                    }
                    GradientStop {
                        position: 0.333
                        color: isUnknown ? Qt.rgba(0, 1, 0, 1) : clr
                    }
                    GradientStop {
                        position: 0.500
                        color: isUnknown ? Qt.rgba(0, 1, 1, 1) : clr
                    }
                    GradientStop {
                        position: 0.667
                        color: isUnknown ? Qt.rgba(0, 0, 1, 1) : clr
                    }
                    GradientStop {
                        position: 0.833
                        color: isUnknown ? Qt.rgba(1, 0, 1, 1) : clr
                    }
                    GradientStop {
                        position: 1.000
                        color: isUnknown ? Qt.rgba(1, 0, 0, 1) : clr
                    }
                }

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        if(!isUnknown) {
                            myData.setColor(i, clr)
                            i++;
                        }
                    }
                }

                Text {
                    anchors.centerIn: parent
                    visible: isUnknown
                    text: " set\nown"
                }
            }
        }
    }
}
