import QtQuick.Controls 2.0
import QtQuick 2.7

Text {
  property int p0winnings: 0
  property int p1winnings: 0
  property int p2winnings: 0
  property int p3winnings: 0
  property int p4winnings: 0
  property int p5winnings: 0
  property int p6winnings: 0
  property int p7winnings: 0
  property int p8winnings: 0

  property string textColor: "white"
  
  id: mainPot
  visible: view.pot > 0
  font.pointSize: 25
  text: view.pot
  color: textColor
  state: "default"
  
  
  states: State {
    name: "won"
    PropertyChanges {
      target: mainPot
      x: 0
      y: 0
    }
  }
  transitions: Transition {
    from: "default"
    to: "won"
    NumberAnimation {
      properties: "x,y"
      easing.type: Easing.InOutQuad
      duration: animationWinDuration
    }
    onRunningChanged: {
      console.log('duration: ' + animationWinDuration)
      console.log('pot running: ' + running)
      root.animationWinRunning = running
    }
  }

  onStateChanged: console.log(state + '\n' + x + ', ' + y)

  function resetWinnings() {
    for (var i = 0; i < 9; i++) {
      eval("p" + i + "winnings = 0");
    }
  }

}
