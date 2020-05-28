//
//  main.cpp
//  assignment7
//
//  Created by rick gessner on 11/23/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#include <iostream>
#include "Game.hpp"
#include "Testing.hpp"

std::ostream& getOutput() {return std::cout;}

//----------------------------------------------

int main(int argc, const char * argv[]) {
                
  std::ostream &theOutput=getOutput();

  validateHandRanking(theOutput); //found in test.hpp
    
  ECE17::Game theGame(1000.00, 5, 5.00);

  //STUDENT: Add 2 or more players before running game...
  
  if(theGame.willRun()) {
    theGame.didRun(theGame.run(1.0, theOutput), theOutput);
  }
      
  return 0;
}
