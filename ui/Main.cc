#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QObject>
#include "HumanAgent.h"
#include "RandomAgent.h"
#include "QGameDriver.h"
#include "QEventListener.h"
#include "QGameView.h"
#include "HumanAgent.h"
#include "RandomAgent.h"
#include "LoggerEventListener.h"
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

  // logs game events to stdout, since the ui doesn't quite show
  // everything yet
  LoggerEventListener logListener;
  game.addEventListener(&logListener);

  // manipulates the game from qml signals
  QGameDriver driver(&game);
  
  // contains a snapshot of the game's state, lives in this main thread,
  // still owned by driver. QML binds properties to this
  QGameView *view = driver.getView();

  // emits signals from the game on state changes
  // and manipulates the QGameView.
  QEventListener *QListener = driver.getListener();
  
  QQmlApplicationEngine engine;

  qmlRegisterType<QPlayer>("poker", 1, 0, "QPlayer");
  engine.rootContext()->setContextProperty("view", view);
  engine.rootContext()->setContextProperty("listener", QListener);

  // engine.load(QUrl::fromLocalFile("Gui.qml"));
  // Instead of loading from local file, compile the qml into the qrc before
  // loading. Now you must recompile after qml changes, but the app can be
  // run from any directory.
  engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

  // connect the startGame signal from qml to the QtGameDriver
  QObject *root = engine.rootObjects()[0];
  QObject::connect(root, SIGNAL(startGame(QString)),
                   &driver, SLOT(startGame(QString)));
  
  return app.exec();
}
