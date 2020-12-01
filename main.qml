import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    width: 1024
    height: 1024
    visible: true
    title: qsTr("Game of Life")

    header: ToolBar {
        Row {
            anchors { fill: parent; leftMargin: 16 }
            spacing: 16
            Label {
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Game Of Life")
            }
            ToolButton {
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Start")
                icon.source: checked ? "qrc:/icons/stop-24px.svg" : "qrc:/icons/play_arrow-24px.svg"
                checkable: true
                onClicked: {
                    if(controller.running)
                        controller.stop()
                    else
                        controller.start()
                }
            }
            ToolButton {
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Reset")
                icon.source: "qrc:/icons/settings_backup_restore-24px.svg"
                onClicked: tableModel.reset()
                enabled: !controller.running
            }
            ToolButton {
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Clear")
                icon.source: "qrc:/icons/clear-24px.svg"
                onClicked: tableModel.clear()
                enabled: !controller.running
            }
        }
    }

    TableView {
        id: table
        anchors.fill: parent
        anchors.margins: 16
        model: tableModel
        interactive: false

        onWidthChanged: forceLayout()
        onHeightChanged: forceLayout()

        delegate: Item {
            implicitWidth: table.width / controller.columnCount
            implicitHeight: table.height / controller.rowCount

            Rectangle {
                anchors.fill: parent
                color: model.alive ? "darkgrey" : "white"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: tableModel.select(model.index, true)
//                onClicked: console.log(model.index, styleData.row, styleData.column)
            }
        }
    }
}
