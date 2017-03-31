import QtQuick 2.7
import QtQuick.Controls 2.0
import poker 1.0

Rectangle {
  id: actionInterfaceContainer
  property int fontSize: 15
  property int fontPixSize: 16
  property int buttonWidth: 100
  property int buttonHeight: 50

  property int chips: view.players[human.seat].chips
  property int chipsInPlay: view.players[human.seat].chipsInPlay
  property bool acting: view.actingPlayerSeat == human.seat
  property bool canCheck: view.currentBet == chipsInPlay
  property bool canRaise: chips > view.currentBet - chipsInPlay
  property int raiseAmount: raiseSlider.value
  property bool raiseInputUpdating: false
  
  color: "transparent"
  visible: acting

  Rectangle {
    id: raiseInputContainer
    visible: canRaise
    anchors.bottom: raiseButton.top
    anchors.right: raiseButton.right
    anchors.bottomMargin: 5
    width: buttonWidth
    height: 25
    border.color: "white"
    color: "transparent"
    
    TextInput {
      id: raiseInput
      anchors.fill: parent
      text: raiseAmount
      color: "white"
      readOnly: false
      cursorVisible: true
      selectByMouse: true
      maximumLength: 10
      verticalAlignment: TextInput.AlignVCenter
      font.pixelSize: fontPixSize
      font.family: root.fontFamily
      leftPadding: 5
      selectedTextColor: "black"
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
    value: Math.round(raiseInput.text)
    snapMode: Slider.SnapAlways
  }
  
  Button {
    id: foldButton
    anchors.left: parent.left
    anchors.bottom: parent.bottom
    anchors.margins: 10
    width: buttonWidth
    height: buttonHeight
    Text {
      text: "Fold"
      anchors.centerIn: parent
      font.pixelSize: fontPixSize
      font.family: root.fontFamily
      horizontalAlignment: Text.AlignHCenter
      wrapMode: Text.Wrap
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
    width: buttonWidth
    height: buttonHeight
    Text {
      text: "Call " + (view.currentBet - view.players[human.seat].chipsInPlay)
      anchors.centerIn: parent
      font.family: root.fontFamily
      font.pixelSize: fontPixSize
      horizontalAlignment: Text.AlignHCenter
      wrapMode: Text.Wrap
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
    width: buttonWidth
    height: buttonHeight
    Text {
      text: "Check"
      anchors.centerIn: parent
      font.family: root.fontFamily
      font.pixelSize: fontPixSize
      horizontalAlignment: Text.AlignHCenter
      wrapMode: Text.Wrap
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
    width: buttonWidth
    height: buttonHeight
    Text {
      text: "Raise " + raiseAmount
      anchors.centerIn: parent
      font.family: root.fontFamily
      font.pixelSize: fontPixSize
      horizontalAlignment: Text.AlignHCenter
      wrapMode: Text.Wrap
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
    return chips + chipsInPlay
  }
}
