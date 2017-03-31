import QtQuick.Controls 2.0
import QtQuick 2.7

Text {
  property int amount: 0
  property string textColor: "white"
  property int targetSeat
  property int targetX: 0
  property int targetY: 0
  
  id: sidePot
  visible: amount > 0
  text: amount
  color: textColor

  onAmountChanged: {
    if (amount > 0) {
      console.log('pot ' + targetSeat + ' transition to ' + targetX + ',' + targetY);
      state = "won"
    } else {
      console.log('pot ' + targetSeat + ' transition to default');
      state = ""
    }
  }

  states: State {
    name: "won"
    PropertyChanges {
      target: sidePot
      x: targetX
      y: targetY
    }
  }
  
  transitions: Transition {
    from: ""
    to: "won"
    NumberAnimation {
      properties: "x,y"
      easing.type: Easing.InOutQuad
      duration: 2000
    }
    onRunningChanged: {
      console.log('specialpot ' + targetSeat + ' running: ' + running)
      root.animationsRunning = running
    }
  }

  onTargetXChanged: console.log('pot ' + targetSeat + ': ' + targetX + ',' + targetY);
}

/*  copy pasted from Pot.qml to save

  SidePot {
    id: pot0
    amount: p0winnings
    targetSeat: 0
  }
  SidePot {
    id: pot1
    amount: p1winnings
    targetSeat: 1
  }
  SidePot {
    id: pot2
    amount: p2winnings
    targetSeat: 2
  }
  SidePot {
    id: pot3
    amount: p3winnings
    targetSeat: 3
  }
  SidePot {
    id: pot4
    amount: p4winnings
    targetSeat: 4
  }

  onP0winningsChanged: {
    console.log('new p0 winnings: ' + p0winnings);
    console.log(pot0.mapFromItem(playerRing, playerRing.player0.x, playerRing.player0.y));
  }
  onP1winningsChanged: {
    console.log('new p1 winnings: ' + p1winnings);
    console.log(pot1.mapFromItem(playerRing, playerRing.player1.x, playerRing.player1.y));
  }
  onP2winningsChanged: {
    console.log('new p2 winnings: ' + p2winnings);
  }
  onP3winningsChanged: {
    console.log('new p3 winnings: ' + p3winnings);
  }
  onP4winningsChanged: {
    console.log('new p4 winnings: ' + p4winnings);
  }
*/

