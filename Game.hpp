//
//  Game.hpp
//  assignment2
//
//  Created by rick gessner on 10/6/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include <map>
#include "Player.hpp"
#include "Deck.hpp"
#include "Hand.hpp"

namespace ECE17 {
    
  //student -- declare the Game class here...
  class Game {
  public:
    Game(float aStartingBalance, size_t aCardCount=5, float aMinimumBet=5.0);

    bool    addPlayer(IPlayer &aPlayer);

    bool    willRun();  // Return true if the game is properly configured and is ready to run
    bool    run(float anAnte, std::ostream &anOutput); // run the game, returns true if it was run
    bool    didRun(bool aRunStatus, std::ostream &anOutput); // if aRunStatus is true, prints winner information
        
  };

}

#endif /* Game_hpp */
