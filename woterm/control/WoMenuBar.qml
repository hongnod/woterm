import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Layouts 1.11
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Imagine 2.4


MenuBar {
    delegate: MenuBarItem {
        id: menuBarItem
        contentItem: Text {
            text: menuBarItem.text
            font: menuBarItem.font
            opacity: enabled ? 1.0 : 0.3
            color: menuBarItem.highlighted ? g_theme.menubarHoverTextColor : g_theme.menubarTextColor
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            implicitWidth: 40
            opacity: enabled ? 1 : 0.3
            color: menuBarItem.highlighted ? g_theme.menubarHoverBackColor : g_theme.menubarBackColor
        }
    }

    background: Item {
        Rectangle {
            color: g_theme.seperatorColor
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
        }
    }
}
