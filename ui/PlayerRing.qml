import QtQuick 2.7
import QtQuick.Controls 2.0

Rectangle {
  property real hSpacing: 50
  property real vSpacing: 50

  property alias player0: player0
  
  id: playerRingContainer
  color: "transparent"

  width: 440
  height: 350

  Player {
    id: player0
    seat: 0
    anchors.top: parent.top
    anchors.horizontalCenter: parent.horizontalCenter
  }

  Player {
    id: player1
    seat: 1
    anchors.top: player0.top
    anchors.left: player0.right
    anchors.leftMargin: hSpacing
  }

  Player {
    id: player2
    seat: 2
    anchors.top: player1.bottom
    anchors.left: player1.right
    anchors.topMargin: vSpacing
    anchors.leftMargin: hSpacing
  }

  Player {
    id: player3
    seat: 3
    anchors.top: player2.bottom
    anchors.right: player2.left
    anchors.topMargin: vSpacing
    anchors.rightMargin: hSpacing
  }

  Player {
    id: player4
    seat: 4
    anchors.top: player3.top
    anchors.right: player3.left
    anchors.rightMargin: (2 * hSpacing - width) / 3
  }

  Player {
    id: player5
    seat: 5
    anchors.top: player4.top
    anchors.right: player4.left
    anchors.rightMargin: (2 * hSpacing - width) / 3//hSpacing * 2 / 3.0
  }

  Player {
    id: player6
    seat: 6
    anchors.top: player5.top
    anchors.right: player5.left
    anchors.rightMargin: (2 * hSpacing - width) / 3//hSpacing * 2 / 3.0
  }

  Player {
    id: player7
    seat: 7
    anchors.bottom: player6.top
    anchors.right: player6.left
    anchors.bottomMargin: vSpacing
    anchors.rightMargin: hSpacing
  }

  Player {
    id: player8
    seat: 8
    anchors.bottom: player7.top
    //anchors.left: player7.right
    anchors.bottomMargin: vSpacing
    //anchors.leftMargin: hSpacing
    anchors.right: player0.left
    anchors.rightMargin: hSpacing
  }
}
