#include <QGuiApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QObject>
#include <memory>
#include "HumanAgent.h"
#include "RandomAgent.h"
#include "QGameDriver.h"
#include "QEventListener.h"
#include "QGameView.h"
#include "QHumanAgent.h"
#include "SlowAgent.h"
#include "LoggerEventListener.h"
#include "Game.h"

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);
  
  // set up the game
  std::shared_ptr<Game> game = std::make_shared<Game>(5, 10);
  
  std::shared_ptr<QHumanAgent> human = std::make_shared<QHumanAgent>();
  std::shared_ptr<SlowAgent> bot1 = std::make_shared<SlowAgent>();
  std::shared_ptr<SlowAgent> bot2 = std::make_shared<SlowAgent>();
  std::shared_ptr<SlowAgent> bot3 = std::make_shared<SlowAgent>();
  std::shared_ptr<SlowAgent> bot4 = std::make_shared<SlowAgent>();
  std::shared_ptr<SlowAgent> bot5 = std::make_shared<SlowAgent>();
  std::shared_ptr<SlowAgent> bot6 = std::make_shared<SlowAgent>();
  std::shared_ptr<SlowAgent> bot7 = std::make_shared<SlowAgent>();
  std::shared_ptr<SlowAgent> bot8 = std::make_shared<SlowAgent>();

  // Game *game = new Game(5, 10);
  game->addPlayer(human, "human", 1000);
  game->addPlayer(bot1, "bot1", 1000);
  game->addPlayer(bot2, "bot2", 1000);
  game->addPlayer(bot3, "bot3", 1000);
  game->addPlayer(bot4, "bot4", 1000);
  game->addPlayer(bot5, "bot5", 1000);
  game->addPlayer(bot6, "bot6", 1000);
  game->addPlayer(bot7, "bot7", 1000);
  game->addPlayer(bot8, "bot8", 1000);

  // manipulates the game from qml signals
  QGameDriver driver(game);

  // contains a snapshot of the game's state, lives in this main thread,
  // still owned by driver. QML binds properties to this
  QGameView *view = driver.getView();

  // emits signals from the game on state changes
  // and manipulates the QGameView.
  std::shared_ptr<QEventListener> listener = driver.getListener()
  
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

  // connect the startGame signal from qml to the QGameDriver
  QObject *root = engine.rootObjects()[0];
  QObject::connect(root, SIGNAL(startGame(QString)),
                   &driver, SLOT(startGame(QString)));
  // connect action signal to the QHumanAgent
  QObject::connect(root, SIGNAL(sendAction(int, int)),
                   human.get(), SLOT(doAction(int, int)));
  
  return app.exec();
}
