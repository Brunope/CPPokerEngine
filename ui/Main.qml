import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import poker 1.0

Window {
  // config params
  property int animationWinDuration: 1000
  property int animationShowDuration: 3000
  property string fontFamily: font.name
  property int fontPixSize: 12
  property bool debugBorders: false
  
  // state vars
  property bool handRunning: false
  property bool gameRunning: view.players.length > 1
  property int winningPlayerSeat
  property bool animationsRunning: animationShowRunning || animationWinRunning
  property bool animationShowRunning: false
  property bool animationWinRunning: false
  
  id: root
  visible: true
  minimumWidth: 640
  minimumHeight: 480

  FontLoader {
    id: font
    source: "monaco.ttf"
  }
  
  Image {
    id: bgImage
    anchors.fill: parent
    source: "img/background.png"
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
    anchors.verticalCenterOffset: 30
  }

  Pot {
    id: pot
    anchors.horizontalCenter: board.horizontalCenter
    anchors.bottom: board.top
    anchors.margins: 5
  }
  
  PlayerRing {
    id: playerRing
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    height: root.height * .75
    border.color: "white"
    border.width: debugBorders ? 1 : 0
  }

  ChatBox {
    id: chat
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.top: playerRing.bottom
    anchors.topMargin: 20
  }
  
  Rectangle {
    id: numHandsContainer
    anchors.left: parent.left
    anchors.bottom: chat.top
    height: 20
    Text {
      text: "hand # " + view.numHands
      font.family: fontFamily
      font.pixelSize: fontPixSize
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
    anchors.topMargin: 20
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
    }
    onGameEnd: {
      gameRunning = false
      console.log("game over");
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
      rootState.state = 'showing'
    }
    onShowdown: {
      appendChatText(sig_text)
    }
    onPotWin: {
      console.log(sig_text)
      appendChatText(sig_text)
      pot.state = "won"
      eval("pot.p" + player.seat + "winnings = amount");
      eval("playerRing.player" + player.seat).winning = true
      eval("playerRing.player" + player.seat).state = "winning"
      handRunning = false
    }
  }

  Item {
    id: rootState
    states: State {
      name: 'showing'
    }
    transitions: Transition {
      from: 'default'
      to: 'showing'
      NumberAnimation {
        duration: animationShowDuration
      }
      // why can't I just property bind this
      onRunningChanged: {
        console.log('show running: ' + running);
        animationShowRunning = running
      }
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
      eval("playerRing.player" + i).state = "default";
    }
    rootState.state = "default";
    pot.state = "default";
    pot.resetWinnings();


    if (!handRunning && gameRunning) {
      root.playHand();
    }
  }

  onAnimationsRunningChanged: {
    console.log("ar: " + animationsRunning + ' ' + animationShowRunning + ' ' + animationWinRunning);
    console.log("hr: " + handRunning);
    if (!animationsRunning && !handRunning) {
      resetPlayHand()
    }
  }

  onGameRunningChanged: {
    console.log("game running: " + gameRunning);
  }
}

