//
//  main.cpp
//  assignment7
//
//  Created by rick gessner on 11/23/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#include <iostream>
#include <vector>
#include "Game.hpp"
#include "Testing.hpp"
#include "Player.hpp"

std::ostream& getOutput() {return std::cout;}

//----------------------------------------------

int main(int argc, const char * argv[]) {
                
  std::ostream &theOutput=getOutput();

  validateHandRanking(theOutput); //found in test.hpp
    
  ECE17::Game theGame(1000.00, 5, 5.00);

  srand(time(NULL));

  //STUDENT: Add 2 or more players before running game...
  std::vector<ECE17::Player> players;

  players.push_back(ECE17::Player("Alice"));
  players.push_back(ECE17::Player("Bob"));
  players.push_back(ECE17::Player("Jack"));
  players.push_back(ECE17::Player("Joe"));
  players.push_back(ECE17::Player("Stephanie"));
  players.push_back(ECE17::Player("William"));

  const int min_players = 3;
  int num_of_players = min_players + static_cast<int>((players.size()-min_players) * (float)rand() / RAND_MAX);
  
  for (int playernum = 0; playernum < num_of_players; playernum++) {
      int playerIndex = static_cast<int>(players.size() * (float)rand() / RAND_MAX);
      theGame.addPlayer(players[playerIndex]);
      players.erase(players.begin() + playerIndex);
  }
  /*theGame.addPlayer(ECE17::Player("Alice"));
  theGame.addPlayer(ECE17::Player("Bob"));
  theGame.addPlayer(ECE17::Player("Jack"));
  theGame.addPlayer(ECE17::Player("Joe"));
  theGame.addPlayer(ECE17::Player("Stephanie"));
  theGame.addPlayer(ECE17::Player("William"));*/

  if(theGame.willRun()) {
    theGame.didRun(theGame.run(1.0, theOutput), theOutput);
    std::cout << "The game ran." << std::endl;
  }
      
  return 0;
}
