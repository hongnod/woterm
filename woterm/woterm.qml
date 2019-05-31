import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.3
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "layout"
import "control"
import WoItem 1.0
import WoTermItem 1.0


Item {
    visible: true

    Component.onCompleted: {
    }

    WoTheme {
        id:g_theme
    }

    Component {
        id: itemTab
        WoTermItem {
            Rectangle {
                anchors.fill: parent
                border.width: 3
                border.color: "yellow"
                color: "green"
            }
        }
    }


    ColumnLayout {
        spacing: 0
        anchors.fill: parent
        ToolBar {
            Layout.fillWidth: true
            Layout.margins:0
            RowLayout {
                anchors.fill: parent
                ToolButton {
                    text: qsTr("New")
                    onClicked: {
                        var cnt = remoteBar.count;
                        lstModel.insert(0, {"title":"target"+cnt, "server":"target"})
                        remoteBar.currentIndex = cnt
                    }
                }
                ToolButton {
                    text: qsTr("Connect")
                    implicitWidth: 60
                    onClicked: {

                    }
                    Button{
                        width: 20
                        anchors.right: parent.right
                        text : "Drop"
                    }
                }
                Item {
                    Layout.fillWidth: true
                }
            }
        }

        Component {
            id: tabViewStyle
            TabViewStyle {
                tabOverlap: 16
                frameOverlap: 4
                tabsMovable: true

                frame: Rectangle {
                    gradient: Gradient {
                        GradientStop { color: "#e5e5e5" ; position: 0 }
                        GradientStop { color: "#e0e0e0" ; position: 1 }
                    }
                    border.color: "#898989"
                    Rectangle { anchors.fill: parent ; anchors.margins: 1 ; border.color: "white" ; color: "transparent" }
                }
                tab: Item {
                    property int totalOverlap: tabOverlap * (control.count - 1)
                    implicitWidth: Math.min ((styleData.availableWidth + totalOverlap)/control.count - 4, image.sourceSize.width)
                    implicitHeight: image.sourceSize.height
                    BorderImage {
                        id: image
                        anchors.fill: parent
                        source: styleData.selected ? "images/tab_selected.png" : "images/tab.png"
                        border.left: 30
                        smooth: false
                        border.right: 30
                    }
                    Text {
                        text: styleData.title
                        anchors.centerIn: parent
                    }
                }
                leftCorner: Item { implicitWidth: 12 }
            }
        }

        TabView {
            id: remoteBar
            Layout.fillHeight: true
            Layout.fillWidth: true

            style: tabViewStyle

            Repeater {
                model: ListModel{
                    id: lstModel
                }

                delegate:Component {
                    Tab {
                        title: model.title
                        width: implicitWidth
                        WoTermItem {
                            id: m_term
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.margins: 0

                            Component.onCompleted: {
                                m_term.connect(model.server)
                            }

                            Rectangle {
                                anchors.fill: parent
                                border.width: 3
                                border.color: "yellow"
                                color: "green"
                            }
                        }
                    }
                }
            }
        }
    }
}
