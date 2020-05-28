# SP20-Assignment7
### Let's build a game engine, for Five-Card Poker

## Introduction

You've come a long way towards being able to program with objects in C++. Now it's time to put these techniques to work. In this assignment, you are going to build a working game of 5-card poker. The players in the game will be `Player` objects you build, that can respond to the rules of the game, and driven by the process defined in your `Game` object (described below). 

The `Game` object drives the process of game-play.  During each round (hand), your `Player` class will analyze the cards it has received, and choose whether to fold, stand pat, or bet. Try to have fun with this part! 


--- 

## Rules of the Game

Two (or more) players join the game. Each game is made up of a sequence of rounds (or hands). Rounds continue until all but one player has withdrawn from the game (usually by running out of money/chips).

### 1. The Ante
At the start of each round, every player pays an "ante" -- a small fee to participate in the round. The proceeds from player anties are kept in a central "pot" by the game, and these funds get disbursed at the end of the round to the winner of the round.

### 2. Dealing the Cards
The game shuffles a full deck of 52 cards. Each player is dealt a "hand" of five cards - by the game.  In your code, a `Hand` object will hold the five `Cards` that each `Player` receives.  Players are free to examine their own cards, which are kept out of the view of other players. 

### 3. First Bet 

After receiving their starting "hand", each `Player` has three options:

1. It can fold their hand, and relinquish the money it put in the pot, and await the start of the next round. 
2. It can "stand pat" -- and not make an anticipatory bet (usually because they don't have a very strong hand)
3. It can place a bet (0<bet<balance)

If _any_ player places a bet at this point, then _every_ player must match that bet, or elect to fold. To make our game a little simpler, players are not permitted to raise any bet. A player can either match a bet, or fold. 

If more than one player remains after that first round of betting, there follows a round of "drawing" new cards. If only 1 player is left, they immediately win the pot, and the next round begins. 

> Note: The game engine should _not_ allow the "last" player in a round to fold, even if that `Player` elects to do so. The last player in a round automatically wins the pot.

### 4. Drawing Cards (discard/add)

Each `Player` still in the round is given the opportunity to discard from 0 to 4 cards. After discarding N cards, the game will automatically add N new cards that player's hand, ensuring that each player has 5 cards. 

### 5. Final Bet -- 

As before, each player is again given the option to fold, hold, or bet. As before, if any player choose to bet, then every player who wants to remain in the round must match that bet, or they may fold.

### 6. Winning the Round 
Cards for all players are now revealed (you will show the cards in the hand for each player in the terminal), and the game determines which player has the winning hand, and how much they won in the pot (also shown in the terminal -- see below). The winning player receives all the money from the pot for this round. The game then continues back at step one in the process with a new shuffled `Deck`. 

```
Round 1 --
Player1 : {8H, 7H, 6H, 5H, 4H}
Player2 : {AD, KD, QD, JD, 10D} WINS $20
```

---

## Hands you can achieve in Five-Card Poker

### 1. Royal flush
A sequence of 5 highest cards (all in the same suit).  Example: `{AD, KD, QD, JD, 10D}` (shown in order for clarity)

### 2. Straight flush
Five cards in a sequence, all in the same suit.  Examples : `{8H 7H 6H 5H 4H}`, or `{7D 8D 9D 10D JD}`

### 3. Four of a kind
All four cards of the same face value (suit doesn't matter). Example: `{K-K-K-K 3}`

### 4. Full house
Three cards with the same face value, and two cards with a different (but matching between them) face value. Example: `{10-10-10 9-9}`

### 5. Flush
Any five cards with the same suit, but not in a sequence.  Example:  `{4D JD 8D 2D 9D}`

### 6. Straight
Five cards in a sequence, regardless of suit. Example: `{5 6 7 8 9}`

### 7. Three of a kind
Three cards of the same rank, regardless of suit.  `{Q-Q-Q 2 3}`

### 8. Two pair
Two pairs of two cards with same face value.  Example: `{K-K 7-7 2}`

### 9. One Pair
Two cards of the same face value.  Example: `{A-A 5 2 8}`

### 10. High Card
When you haven't made any of the hands above, your highest card is the only play. 

--- 

## Your Deliverables

### Build a working Poker game

In order to get your game to work, we'll need the 5 classes described below. 

#### 1. The `Card` class

The `Card` class represents a given card (suit, face) in a deck. As usual, there are 4 suits, with 13 faces in each, for a total of 52 cards. We have provided you with an implementation (same as from the midterm).

#### 2. The `Deck` class

The `Deck` class will hold 52 distinct (non-repeating) `Card` objects in an internal data structure. We've provided you with an implementation (from the midterm, note that it inherits from CardPile).

#### 3. The `Hand` class

A `Hand` will represent the 5 cards that each player will have during each round of the game. Generally speaking, your `Hand` class should have a data member that contains up to 5 `Card` objects.  `Card` inherits from `CardPile` and MUST also implement additional methods (see `Hand.hpp`). One of these methods will be more difficult to implement than the others:

```cpp
HandTypes determineRank(); //analyze the cards in the hand, and determine what type of logical hand it is (pair, full-house, etc)
```

It's essential that you implement `determineRank()` correctly. Given a `Hand` object that contains 5 cards, this method will analyze the collection of cards to determine the "best" logical hand the cards represent. (See section on "Testing" below). Your game will rely on this method working in order to choose a winning `Hand` from reach round.

#### 4. The `Player` class

Your `Player` class will act like a virtual player, and respond to calls made by the `Game` object to iterate through rounds until a winner emerges (with all the money!). Your `Player` class is a subclass of the abstract `IPlayer`, shown below. You are required to create a subclass of the `IPlayer` class, and implement the necessary methods.

```cpp
  class IPlayer {
  public:

    virtual std::string& getName()=0; //get the player name
    virtual void         willStartRound(size_t aRoundNum);
    virtual bool         isFolding(const ECE17::Hand &aHand, size_t aPlayerCount, float aBalance)=0;
    virtual float        placeBet(const ECE17::Hand &aHand, size_t aPlayerCount, float aMaxBet)=0;
    virtual size_t       discardUnwanted(ECE17::Hand &aHand)=0;
  };
```

> Note: In both `isFolding()` and `placeBet()`, the game provides an indicator of how many players remain in the current round, as the argument `aPlayerCount`.  Betting strategies are often impacted by the number of remaining players.

In addition to implementing the `IPlayer` methods above, you are free to add any other methods you like to your `Player` class. If you are feeling ambitious, you can implement more than one player type and pit them against each other!

#### 5. The `Game` class

The `Game` class is a "process" object that drives the game forward.  The `Game` class defines the following methods, that you must fully implement, but you are free to add other methods/data members as well:

```cpp
    bool    addPlayer(IPlayer &aPlayer);
    bool    willRun();   
    bool    run(float anAnte, std::ostream &anOutput);
    bool    didRun(bool aRunStatus, std::ostream &anOutput);
```

## Running the Game

In your `main()` function, you will construct a single `Game` object. Next, you must add two (2 or more) of your automated player objects to the `Game` by calling `Game::addPlayer()`.  Somewhere in your game you need to track the current account balance for each player. The starting balance value is given to the game as an argument to the ctor() (I suggest $1000).

Once you've added players, the `main()` function begins the main `Game` process using this (process-oriented) logic:

```cpp
  if(theGame.willRun()) {
    theGame.didRun(theGame.run(1.0, theOutput), theOutput);
  }
```

> Note: In the `Game.willRun()` method, you must validate that 2 (or more) players were added to the game.  If not, `willRun()` should return false, and the game should be terminated without playing any rounds.

### How the `Game` object drives game play for each round

Before starting a round, make sure to call `Player::willStartRound(int aRoundNumber)` for each `Player` object. This gives the `Player` object a chance to reset any state it may keep during a given round.

#### 1. Dealing Cards

All of the players who can afford to pay the ante are given 5 cards. Those that can't pay ante are ejected from the game. Player anties are accumulated in a "pot" (game data member), and the account total for that player is reduced accordingly.

#### 2. Place First Bet (or Fold)

The game then iterates over all the players still in the round. Each player object can review its current hand, when it receives the message: `bool Player::isFolding(Hand &aHand, aPlayerCount, aBalance)`. If `Player` object returns `true`, that player exits the round, and forfeits any money it put in the pot so far. If the `Player` doesn't fold, it is given a chance to place a bet when it receives the message, `float Player::placeBet(Hand &aHand, size_t aPlayerCount, float aMaxBet)`. They can bet as little as 0 (meaning "hold") or up to the maximum amount in their bank (given as aMaxBet).  If the player makes a bet, that amount is reduced from their account total, and added to the pot.

> Note: When `Player::placeBet(aHand, aPlayerCount, aMaxBet)` is called, the `aPlayerCount` argument informs the `Player` how many players remain in the round (which can influence the player betting decision).

#### 3. Drawing Cards

Each player who is still in the round is given a chance to drop unwanted cards, and get replacements (`Player::discardUnwanted(Hand &aHand)`). The `Player` object should call `Hand::dropCard(Card &aCard)` for each card it wants to drop.

#### 4. Place Second Bet (or Fold)

The game then iterates over all the players still in the round. Each player object can review its current hand, when it receives the message: `bool Player::isFolding(Hand &aHand, playerCount, aBalance)`. If `Player` object returns `true`, that player exits the round, and forfeits any money it put in the pot so far. If the `Player` doesn't fold, it is given a chance to place a bet when it receives the message, `float Player::placeBet(Hand &aHand, float aMaxBet)`. They can bet as little as 0 (meaning "hold") or up to the maximum amount in their bank (given as aMaxBet).  If the player makes a bet, that amount is reduced from their account total, and added to the pot.


#### 5. Award money in pot to winner

The game analyzes the quality of the hand of each of the remaining players, by calling, `HandTypes Hand::determineRand()`.  The player who receives the highest mark their hand is determined to be the winner. The winner is awarded all the money (from "ante" and bets) from the pot.

--- 

## Testing

Included in your starter project folder is a file called `Testing.hpp`. This file contains a single method we suggest you implement. 

```cpp
bool validateHandRanking(std::ostream &anOutput);
```

The `validateHandRanking()` method is used to test and validate another method: `HandTypes Hand::determineRank()`. The job that `Hand::determineRank()` performs is to identify the type of logical hand represented by the `Cards` in the given `Hand` object. These were described above in the section called, "Hands you can achieve in Five-Card Poker".  

At the top of `Hand.hpp`, we have provided an enum which lists the types of logical hands a player may have:

```cpp
  enum class HandTypes {
    unknown=0, high_card=1, one_pair=2, two_pair=3, three_kind=4, straight=5,
    flush=6, full_house=7, four_kind=8, straight_flush=9, royal_flush=10
  };
```

Again, `HandTypes Hand::determineRank()` will evalate the `Cards` in the current `Hand` object, and return the enum value that represents the _best_ hand represented by that set of `Cards`. 

> NOTE: To complete this task, you need manually create a `Hand` object that contains `Cards` that represent each type of logical hand (one-pair, two-pair, three-of-a-kind, etc) that can be achieved. Make sure that `Hand::determineRank()` can properly determine each type of logical hand that can occur in the game. If you fail to do this, you'll lose points on the assignment, and more importantly, your game will not operate correctly.





