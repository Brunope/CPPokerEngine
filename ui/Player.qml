import QtQuick 2.7
import QtQuick.Controls 2.0


Rectangle {
  property int seat
  property string name: view.players[seat].name
  property int chips: view.players[seat].chips
  property int chipsInPlay: view.players[seat].chipsInPlay
  property int winnings: eval('pot.p' + seat + 'winnings')
  property bool live: view.players[seat].live
  property bool exists: view.players[seat].exists
  property bool showing: false
  property bool winning: false
  property bool acting: view.actingPlayerSeat == seat
  property string textColor: getColor()
  property string card0str: "back"
  property string card1str: "back"
  property string facing: "top"
  property bool winRunning

  id: playerContainer

  width: 75
  height: 100

  visible: exists || showing
  color: "transparent"

  border.width: 0
  border.color: "yellow"

  Text {
    id: playerName
    anchors.top: cardContainer.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    text: name
    color: textColor
  }

  Rectangle {
    id: cardContainer
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.top: parent.top
    anchors.topMargin: 20

    width: c0.width + c1.width
    height: c0.height
    color: "transparent"

    Card {
      id: c0
      anchors.left: parent.left
      str: card0str
      scale: 0.5
      visible: live || showing
    }
    Card {
      id: c1
      anchors.left: c0.right
      str: card1str
      scale: 0.5
      visible: live || showing
    }
  }

  Text {
    id: playerChips
    text: chips
    color: textColor
    anchors.top: playerName.bottom
    anchors.horizontalCenter: parent.horizontalCenter
  }
  
  Text {
    id: playerChipsInPlay
    text: chipsInPlay
    color: textColor
    visible: chipsInPlay != 0
    font.pointSize: 14
    
    // has to be a better way to do this
    anchors.right: facing == 'left' ? cardContainer.left : undefined
    anchors.left: facing == 'right' ? cardContainer.right : undefined
    anchors.top: facing == 'bottom' ? playerChips.bottom : undefined
    anchors.bottom: facing == 'top' ? cardContainer.top : undefined
    anchors.horizontalCenter: getHorizontalCenterFromFacing(facing)
    anchors.verticalCenter: getVerticalCenterFromFacing(facing)
    anchors.margins: 8
  }

  Text {
    id: winningChips
    text: '+' + winnings
    color: textColor
    visible: winning
    font.pointSize: 25
    anchors.top: cardContainer.bottom
    anchors.horizontalCenter: parent.horizontalCenter
  }

  states: State {
    name: 'winning'
    PropertyChanges {
      target: playerChips
      visible: false
    }
    PropertyChanges {
      target: playerName
      visible: false
    }
    PropertyChanges {
      target: winningChips
      visible: true
    }
  }
  transitions: Transition {
    from: 'default'
    to: 'winning'
    NumberAnimation {
      duration: animationShowDuration
    }

    onRunningChanged: {
      console.log('duration: ' + animationShowDuration)
      console.log('pwin running: ' + running)
    }
  }

  function getColor() {
    if (winning) {
      return "lime"
    }
    if (live) {
      if (acting) {
        return "red"
      } else {
        return "white"
      }
    }
    return "gray"
  }

  function getAnchorsFromFacing(facing) {
    var newAnchors = {};
    newAnchors[facing] = playerContainer.facing;
    if (facing == 'bottom' || facing == 'top') {
      newAnchors[horizontalCenter] = playerContainer.horizontalCenter;
    } else {
      newAnchors[verticalCenter] = playerContainer.verticalCenter;
    }
    console.log(newAnchors)
    return newAnchors
  }

  function getHorizontalCenterFromFacing(facing) {
    if (facing == 'top' || facing == 'bottom') {
      return playerContainer.horizontalCenter;
    } else {
      return undefined;
    }
  }

    function getVerticalCenterFromFacing(facing) {
    if (facing == 'left' || facing == 'right') {
      return playerContainer.verticalCenter;
    } else {
      return undefined;
    }
  }
}
