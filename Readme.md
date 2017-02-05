# Holdup - No Limit Texas Hold'em [![Build Status](https://travis-ci.org/Brunope/Holdup.svg?branch=master)](https://travis-ci.org/Brunope/Holdup)
A performant no-limit Texas Hold'em poker simulator in C++11, with a framework for writing intelligent agents.

## Usage
To compile the core game engine, its static library, and test suite, `cd core && make`

To run the test suite, `make test`

From the `core/` directory, run the minimalistic command line interface with `./bin/main`


From `core/src/Main.cc`:


```cpp
// Create a no limit hold'em game with static blinds at 10/20
Game game(10, 20);

// Add a listener that logs game events to stdout
LoggerEventListener listener;
game.addEventListener(&listener);

// Add some players, human vs computers.
// Each starts with 1000 chips.
HumanAgent human;
RandomAgent bot0, bot1, bot2;
game.addPlayer(&human, "human", 1000);
game.addPlayer(&bot0, "bot0", 1000);
game.addPlayer(&bot1, "bot1", 1000);
game.addPlayer(&bot2, "bot2", 1000);

// play until one player wins all the chips
game.play();
```

To build the GUI in `ui/`, you will need Qt5.

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
