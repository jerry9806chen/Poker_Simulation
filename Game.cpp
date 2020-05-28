//
//  Game.cpp
//  assignment2
//
//  Created by rick gessner on 10/6/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#include "Game.hpp"
#include "Hand.hpp"
#include "Card.hpp"

namespace ECE17 {

  Game::Game(float aStartingBalance, size_t aCardCount, float aMinimumBet) {}

  bool Game::addPlayer(IPlayer &aPlayer) {
    return true;
  }


  bool Game::willRun() {
    return true;  //you must have at least 2 players to continue...
  }

  bool Game::run(float anAnte, std::ostream &anOutput) {
          
  /*
    while(multiple players have $ to pay ante...) {
      //Shuffle a full deck for each "round"..
      //Notify the players that new round (N) is starting...
      //Collect ante from players (put in pot). If they can't afford, exclude them from the round
      //(If only 1 player can pay ante, game is over!)
      //Ask each player if they want fold, and remove them from the round (not the game)
      //Ask each player place first bet (min < bet < playerbalance)...
      //Give each player a chance to draw new cards
      //Ask each player if they want fold. Is so, remove them from round.
      //Ask each player place final bet (min < bet < playerbalance)...
      //Determine the winner of the round, and award the winner the pot ($)
    }

  */
    
    return true;
  }
  
  bool Game::didRun(bool aRunStatus, std::ostream &anOutput) {
    //Determine if the game ran and finished, and then announce the winning player here and how much they won
    return true;
  }


}




