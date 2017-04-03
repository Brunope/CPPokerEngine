#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QObject>
#include <memory>
#include "QGameDriver.h"
#include "QEventListener.h"
#include "QGameView.h"
#include "QHumanAgent.h"
#include "SlowTightAgent.h"
#include "SlowRandomAgent.h"
#include "LoggerEventListener.h"
#include "Game.h"

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);
  
  // set up the game
  std::shared_ptr<Game> game = std::make_shared<Game>(5, 10);
  
  auto human = std::make_shared<QHumanAgent>();
  auto bot1 = std::make_shared<SlowRandomAgent>();
  auto bot2 = std::make_shared<SlowTightAgent>();
  auto bot3 = std::make_shared<SlowTightAgent>();
  auto bot4 = std::make_shared<SlowTightAgent>();
  auto bot5 = std::make_shared<SlowRandomAgent>();
  auto bot6 = std::make_shared<SlowTightAgent>();
  auto bot7 = std::make_shared<SlowRandomAgent>();
  auto bot8 = std::make_shared<SlowTightAgent>();

  game->addPlayer(human, "You", 1000);
  game->addPlayer(bot1, "Phil Hellmuth", 1000);
  game->addPlayer(bot2, "Phil Ivey", 1000);
  game->addPlayer(bot3, "Phil Galfond", 1000);
  game->addPlayer(bot4, "Doyle Brunson", 1000);
  game->addPlayer(bot5, "JCarver", 1000);
  game->addPlayer(bot6, "Ali", 1000);
  game->addPlayer(bot7, "Kanye West", 1000);
  game->addPlayer(bot8, "John Nash", 1000);

  // manipulates the game from qml signals
  QGameDriver driver(game);

  // contains a snapshot of the game's state, lives in this main thread,
  // still owned by driver. QML binds properties to this
  QGameView *view = driver.getView();

  // emits signals from the game on state changes
  // and manipulates the QGameView.
  std::shared_ptr<QEventListener> listener = driver.getListener();

  QQmlApplicationEngine engine;

  qmlRegisterUncreatableType<QPlayer>("poker", 1, 0, "QPlayer", "");
  qmlRegisterUncreatableType<QCard>("poker", 1, 0, "QCard", "");
  
  engine.rootContext()->setContextProperty("view", view);
  engine.rootContext()->setContextProperty("listener", listener.get());
  engine.rootContext()->setContextProperty("human", human.get());

  // engine.load(QUrl::fromLocalFile("Gui.qml"));
  // Instead of loading from local file, compile the qml into the qrc before
  // loading. Now you must recompile after qml changes, but the app can be
  // run from any directory.
  engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

  // connect the playHand signal from qml to the QGameDriver
  QObject *root = engine.rootObjects()[0];
  QObject::connect(root, SIGNAL(playHand()),
                   &driver, SLOT(playOneHand()));

  // connect action signal to the QHumanAgent
  QObject::connect(root, SIGNAL(sendAction(int, int)),
                   human.get(), SLOT(doAction(int, int)));


  // tell qml all the signal are connected, loading done
  //QMetaObject::invokeMethod(root, "loadingDone");
  
  
  return app.exec();
}
