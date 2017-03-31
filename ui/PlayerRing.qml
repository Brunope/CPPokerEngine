import QtQuick 2.7
import QtQuick.Controls 2.0

Rectangle {
  property real hSpacing: width / 9
  property real vSpacing: height / 11

  property alias player0: player0
  property alias player1: player1
  property alias player2: player2
  property alias player3: player3
  property alias player4: player4
  property alias player5: player5
  property alias player6: player6
  property alias player7: player7
  property alias player8: player8
  
  id: playerRingContainer
  color: "transparent"

  width: 440
  height: 350

  Player {
    id: player0
    seat: 0
    facing: 'bottom'
    anchors.top: parent.top
    anchors.horizontalCenter: parent.horizontalCenter
  }

  Player {
    id: player1
    seat: 1
    facing: 'bottom'
    anchors.top: player0.top
    anchors.left: player0.right
    anchors.leftMargin: hSpacing
  }

  Player {
    id: player2
    seat: 2
    facing: 'left'
    anchors.verticalCenter: parent.verticalCenter
    anchors.left: player1.right
    anchors.topMargin: vSpacing
    anchors.leftMargin: hSpacing / 2
  }

  Player {
    id: player3
    seat: 3
    facing: 'top'
    anchors.bottom: parent.bottom
    anchors.right: player2.left
    anchors.topMargin: vSpacing
    anchors.rightMargin: hSpacing / 2
  }

  Player {
    id: player4
    seat: 4
    facing: 'top'
    anchors.top: player3.top
    anchors.right: player3.left
    anchors.rightMargin: (2 * hSpacing - width) / 3
  }

  Player {
    id: player5
    seat: 5
    facing: 'top'
    anchors.top: player4.top
    anchors.right: player4.left
    anchors.rightMargin: (2 * hSpacing - width) / 3//hSpacing * 2 / 3.0
  }

  Player {
    id: player6
    seat: 6
    facing: 'top'
    anchors.top: player5.top
    anchors.right: player5.left
    anchors.rightMargin: (2 * hSpacing - width) / 3//hSpacing * 2 / 3.0
  }

  Player {
    id: player7
    seat: 7
    facing: 'right'
    anchors.verticalCenter: parent.verticalCenter
    anchors.right: player6.left
    anchors.bottomMargin: vSpacing
    anchors.rightMargin: hSpacing / 2
  }

  Player {
    id: player8
    seat: 8
    facing: 'bottom'
    anchors.top: parent.top
    anchors.bottomMargin: vSpacing
    anchors.right: player0.left
    anchors.rightMargin: hSpacing
  }
}
