import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import poker 1.0

Window {
  property bool animationsRunning: false
  property bool handRunning: false
  property bool gameRunning: view.players.length > 1
  property int winningPlayerSeat
  property int animationDuration: 3000
  
  id: root
  visible: true
  width: 640
  height: 480
  Image {
    id: bgImage
    anchors.fill: parent
    source: "background.png"
    fillMode: Image.Tile
  }

  signal playHand()
  signal sendAction(int type, int amount)


  Button {
    id: startGameButton
    visible: true
    text: "start game"
    anchors.verticalCenter: parent.verticalCenter
    anchors.horizontalCenter: parent.horizontalCenter

    onClicked: {
      visible = false
      resetPlayHand()
    }
  }

  Board {
    id: board
    anchors.verticalCenter: playerRing.verticalCenter
    anchors.horizontalCenter: playerRing.horizontalCenter
    anchors.verticalCenterOffset: 15
  }

  Text {
    id: pot
    anchors.horizontalCenter: board.horizontalCenter
    anchors.bottom: board.top
    anchors.margins: 5
    visible: view.pot > 0
    font.pointSize: 16
    text: view.pot
    color: "white"
    state: "default"

    states: State {
      name: "won"
      PropertyChanges {
        target: pot
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
        duration: animationDuration
      }
      onRunningChanged: {
        console.log('pot running: ' + running)
        root.animationsRunning = running
      }
    }

    onStateChanged: console.log(state + '\n' + x + ', ' + y)

  }
  
  PlayerRing {
    id: playerRing
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.margins: 10
    //border.color: "white"
  }

  ChatBox {
    id: chat
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.top: playerRing.bottom
    anchors.topMargin: 2
  }
  
  Rectangle {
    id: numHandsContainer
    anchors.left: parent.left
    anchors.bottom: chat.top
    height: 20
    Text {
      text: "hand # " + view.numHands
      color: "white"
      leftPadding: 10
    }
  }

  ActionInterface {
    id: actionInterface
    anchors.bottom: parent.bottom
    anchors.top: playerRing.bottom
    anchors.left: chat.right
    anchors.right: parent.right
  }
  
  Connections {
    id: humanConnection
    target: human
    ignoreUnknownSignals: true
    onHoleCardsChanged: {
      // gross
      eval("playerRing.player" + human.seat).card0str = human.card0.str
      eval("playerRing.player" + human.seat).card1str = human.card1.str
    }
  }

  Connections {
    id: cppConnection
    target: listener
    ignoreUnknownSignals: true
    onGameStart: {
      console.log(sig_text);
      appendChatText(sig_text);
    }
    onGameEnd: {
    }
    onPlayerJoin: {
      console.log(sig_text);
      appendChatText(sig_text + " joined")
    }
    onPlayerLeave: {
      console.log(sig_text);
      appendChatText(sig_text)
    }
    onHandStart: {
      console.log(sig_text);
      handRunning = true
      appendChatText(sig_text)
      pot.state = "default"
    }
    onDeal: {
      // appendChatText(sig_text)
    }
    onPlayerAction: {
      appendChatText(sig_text);
    }
    onShowCards: {
      appendChatText(sig_text);
      console.log(player.name + " shows " + card0.str + card1.str);
      eval("playerRing.player" + player.seat).card0str = card0.str;
      eval("playerRing.player" + player.seat).card1str = card1.str;
      eval("playerRing.player" + player.seat).showing = true
    }
    onShowdown: {
      appendChatText(sig_text)
    }
    onPotWin: {
      console.log(sig_text)
      appendChatText(sig_text)
      pot.state = "won"
      //rootState.state = "p0win"
      eval("playerRing.player" + player.seat).winning = true
      handRunning = false
    }
  }

  function appendChatText(text) {
    if (chat.textItem.lineCount > chat.maxLines) {
      chat.text = chat.text.substring(chat.text.indexOf("\n") + 1) + text + "\n"
    }
    else {
      chat.text = chat.text + text + "\n"
    }
  }

  function resetPlayHand() {
    console.log("reset play hand");
    for (var i = 0; i < 9; i++) {
      eval("playerRing.player" + i).card0str = "back";
      eval("playerRing.player" + i).card1str = "back";
      eval("playerRing.player" + i).showing = false;
      eval("playerRing.player" + i).winning = false;
    }
    pot.state = "default";
    rootState.state = "default";
    root.playHand()
  }

  onAnimationsRunningChanged: {
    console.log("ar: " + animationsRunning);
    console.log("hr: " + handRunning);
    if (!animationsRunning && !handRunning && gameRunning) {
      resetPlayHand()
    }
  }

  Item {
    id: rootState
    state: "default"
    states: [
      State {
        name: "default"
        AnchorChanges {
          target: pot
          anchors.horizontalCenter: board.horizontalCenter
          anchors.bottom: board.top
        }
      },
      State {
        name: "p0win"
        PropertyChanges {
          target: playerRing.player0
          winning: true
        }
      },
      State {
        name: "p1win"
        AnchorChanges {
          target: pot
          anchors.top: playerRing.player1.bottom
          anchors.horizontalCenter: playerRing.player1.horizontalCenter
        }
      }
    ]
  }

}

