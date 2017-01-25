#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "HumanAgent.h"
#include "RandomAgent.h"
#include "QtGameDriver.h"
#include "HumanAgent.h"
#include "RandomAgent.h"
#include "Game.h"

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);
  
  // set up the game
  HumanAgent human;
  RandomAgent bot;
  Game game(5, 10);
  game.addPlayer(&human, "human", 1000);
  game.addPlayer(&bot, "bot", 1000);

  // manipulates the game from qml signals
  QtGameDriver driver(&game);

  // emits signals from the game on state changes
  QtEventListener *listener = driver.getListener();

  QQmlApplicationEngine engine;

  // let qml reference the event listener to received its signals
  engine.rootContext()->setContextProperty("listener", listener);
  
  // engine.load(QUrl::fromLocalFile("Gui.qml"));
  // Instead of loading from local file, compile the qml into the qrc before
  // loading. Now you must recompile after qml changes, but the app can be
  // run from any directory.
  engine.load(QUrl(QStringLiteral("qrc:/Gui.qml")));

  // connect the startGame signal from qml to the QtGameDriver
  QObject *root = engine.rootObjects()[0];
  QObject::connect(root, SIGNAL(startGame(QString)),
                   &driver, SLOT(startGame(QString)));
  
  return app.exec();
}
