import QtQuick 2.7
import QtQuick.Controls 2.0
import poker 1.0

Rectangle {
  id: actionInterfaceContainer

  property bool acting: view.actingPlayerSeat == human.seat
  property bool canCheck: view.currentBet == view.players[human.seat].chipsInPlay
  property bool canRaise: getMinRaise() < getMaxRaise()
  property real raiseAmount
  property real fontSize: 15
  
  color: "transparent"
  visible: acting

  Rectangle {
    id: raiseInputContainer
    visible: canRaise
    anchors.bottom: raiseButton.top
    anchors.right: raiseButton.right
    anchors.bottomMargin: 5
    width: 100
    height: 40
    border.color: "white"
    color: "transparent"
    
    TextInput {
      id: raiseInput
      anchors.fill: parent
      text: raiseSlider.value
      color: "white"
      readOnly: false
      cursorVisible: true
      selectByMouse: true
      maximumLength: 10
      verticalAlignment: TextInput.AlignVCenter
      font.pointSize: fontSize
      leftPadding: 5
      selectedTextColor: "white"
      selectionColor: "red"
    }
  }
    
  Slider {
    id: raiseSlider
    visible: canRaise
    anchors.bottom: raiseButton.top
    anchors.right: raiseInputContainer.left
    anchors.left: foldButton.left
    from: getMinRaise()
    to: getMaxRaise()
    stepSize: 1
    value: raiseInput.text
    snapMode: Slider.SnapAlways
  }
  
  Button {
    id: foldButton
    anchors.left: parent.left
    anchors.bottom: parent.bottom
    anchors.margins: 10
    width: 100
    height: 60
    Text {
      text: "Fold"
      anchors.centerIn: parent
      font.pointSize: fontSize
    }
    onClicked: {
      console.log("fold clicked");
      root.sendAction(2, 0);
    }
  }

  Button {
    id: callButton
    anchors.left: foldButton.right
    anchors.bottom: parent.bottom
    anchors.margins: 10
    visible: !canCheck
    width: 100
    height: 60
    Text {
      text: "Call\n" + (view.currentBet - view.players[human.seat].chipsInPlay)
      anchors.centerIn: parent
      font.pointSize: fontSize
    }
    onClicked: {
      console.log("call clicked");
      root.sendAction(1, 0);
    }
  }

  Button {
    id: checkButton
    anchors.left: foldButton.right
    anchors.bottom: parent.bottom
    anchors.margins: 10
    visible: canCheck
    width: 100
    height: 60
    Text {
      text: "Check"
      anchors.centerIn: parent
      font.pointSize: fontSize
    }
    onClicked: {
      console.log("check clicked");
      root.sendAction(3, 0);
    }
  }

  Button {
    id: raiseButton
    visible: canRaise
    anchors.left: callButton.right
    anchors.bottom: parent.bottom
    anchors.margins: 10
    width: 100
    height: 60
    Text {
      text: "Raise\n" + raiseSlider.value
      anchors.centerIn: parent
      font.pointSize: fontSize
    }
    onClicked: {
      console.log("raise clicked");
      root.sendAction(0, raiseSlider.value);
    }
  }

  function getMinRaise() {
    var mr = view.currentBet + view.currentRaiseBy
    var chips = view.players[human.seat].chips
    if (mr < chips) {
      return mr;
    }
    return chips;
  }

  function getMaxRaise() {
    return view.players[human.seat].chips
  }
}
