# Holdup - No Limit Texas Hold'em [![Build Status](https://travis-ci.org/Brunope/Holdup.svg?branch=master)](https://travis-ci.org/Brunope/Holdup)
A performant no-limit Texas Hold'em poker simulator in C++11, with a framework for writing intelligent agents.

## Usage
To compile the core game engine and its static library, and test suite, `make core`.
To compile agents, `make agents`
To compile and run the test suite, `make test`
Or just run `make`

From the root directory, run a minimalistic command line interface with `./bin/console_main`


From `core/src/ConsoleMain.cc`:


```cpp
// Create a no limit game with static blinds at 10/20
Game game(10, 20);

// Add a listener that logs game events to stdout
std::shared_ptr<LoggerEventListener> listener(new LoggerEventListener());
game.addEventListener(listener);

// Add some players to the game, human vs computer.
std::shared_ptr<HumanAgent> human(new HumanAgent());
std::shared_ptr<RandomAgent> bot0(new RandomAgent());
std::shared_ptr<RandomAgent> bot1(new RandomAgent());
std::shared_ptr<RandomAgent> bot2(new RandomAgent());

// Each starts with 1000 chips.
game.addPlayer(human, "human", 1000);
game.addPlayer(bot0, "bot0", 1000);
game.addPlayer(bot1, "bot1", 1000);
game.addPlayer(bot2, "bot2", 1000);

// play until one player wins all the chips
game.play();
```

To build the GUI in `ui/`, you will need Qt5 and either OS X or linux.

## Designing Agents
Just derive from `core/inc/Agent.h` and implement its virtual methods
```cpp
// Called once at the beginning of each Hand in which this Agent is
// participating.
virtual void receiveHoleCards(const std::pair<Card, Card> hc) = 0;

// Called every time it's this Agent's turn to act. Return an Action,
// which contains the type of action and an amount.
virtual Action act(const GameView &view) = 0;

// Called at the end of every hand. history contains the sequence of
// Actions that made up the hand, sorted in chronological order, ie
// most recent Action at the back. 
virtual void receiveHandHistory(const HandHistory &history) = 0;
```
