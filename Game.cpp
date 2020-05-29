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
#include "Player.hpp"

namespace ECE17 {

    // Default constructor for Game.
    Game::Game(float aStartingBalance, size_t aCardCount, float aMinimumBet) {
        startingBalance = aStartingBalance;
        cardCount = aCardCount;
        minBet = aMinimumBet;
        played = false;
    }

    // Add player aPlayer to the game.
    bool Game::addPlayer(ECE17::Player &aPlayer) {
        players.push_back(aPlayer);
        player_balances.push_back(startingBalance);
        return true;
    }

    // Returns if the game will run. It will only run if it has not been run and there are more than one
    // players.
    bool Game::willRun() {
        if (players.size() > 1 && !played)
            return true;
        return false;
    }

    bool Game::run(float anAnte, std::ostream &anOutput) {
        // Game will not run if it already has been played.
        if (played)
            return false;
        played = true;
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

        // The top vectors contains the indices of the participating players in the players vector
        // at the current point in the game and the round respectively.
        // The bottom vector contains the respective hands of each participating player.
        std::vector<int> game_participant_nums, round_participant_nums;
        std::vector<Hand> round_hands;

        int anteCount = 0;
        int roundNum = 1;
        float pot = 0;
        int idx = 0;

        // At the start of the game, all players are participants in the game.
        for (auto& x : players) {
            game_participant_nums.push_back(idx);
            idx++;
        }

        // Until only one player can pay up the ante, the game is not over.
        while (game_participant_nums.size() > 1) {
            // If any players are unable to continue playing, due to not being able to pay the ante,
            // remove them from the game.
            std::vector<int>::iterator it1 = game_participant_nums.begin();
            int playernum = 0;
            while (it1 != game_participant_nums.end()) {
                if (player_balances[*it1] < minBet) {
                    game_participant_nums.erase(it1);
                    it1 = game_participant_nums.begin() + playernum;
                }
                else {
                    playernum++;
                    it1++;
                }
            }

            std::vector<Hand>::iterator it2;

            // The deck for starting the game. It is shuffled at the start.
            Deck startingDeck;
            startingDeck.Shuffle();

            // Play it safe by clearing round_participant_nums at the start of each round.
            // We don't need or want last round's info!
            round_participant_nums.clear();

            // For anyone who can still take part in the game, they are now participating
            // this round. Each player is verified as capable of continual play at the
            // start of the round as seen above, so round_participant_nums is the same 
            // as game_participant_nums at the start of the round.
            anOutput << "\nPlayers this round are: " << std::endl;
            for (auto& x : game_participant_nums) {
                round_participant_nums.push_back(x);
                anOutput << players[x].getName() << std::endl;
            }
            anOutput << std::endl;

            // Check the values at each position when it is needed.
            it1 = round_participant_nums.begin();

            // For each participating player this round, each player is given a hand
            // and has the ante deducted from their balance.
            while (it1 != round_participant_nums.end()) {
                //anOutput << "dork1" << std::endl;
                players[*it1].willStartRound(roundNum);
                std::vector<Card> cardList;
                cardList.clear();

                for (int cardnum = 0; cardnum < cardCount; cardnum++) {
                    cardList.push_back(startingDeck.getCard());
                }
                
                Hand newHand(cardList);
                round_hands.push_back(newHand);
                player_balances[*it1] -= minBet;

                anOutput << players[*it1].getName() << "'s balance is " << player_balances[*it1] << std::endl;

                pot += minBet;
                anteCount++;
                it1++;
            }
            anOutput << "Pot is: " << pot << std::endl;

            // If by any faulty chance, only one player can meet the ante, the game is over.
            // and the money is returned to the sole player.
            if (anteCount < 2) {
                for (auto& x : round_participant_nums) {
                    player_balances[x] += minBet;
                }
                pot = 0;
                break;
            }

            // Check which players wish to fold before the first bet.
            //int aCount = game_participants.size();
            it1 = round_participant_nums.begin();
            it2 = round_hands.begin();
            playernum = 0;
            while (it1 != round_participant_nums.end() && round_participant_nums.size() > 1) {
                //anOutput << "dork2" << std::endl;
                anOutput << "\n" << players[*it1].getName() << std::endl;
                (*it2).printPile();
                anOutput << handName((*it2).determineRank()) << std::endl;
                if (players[*it1].isFolding(*it2, round_participant_nums.size(), player_balances[*it1])) {
                    //std::cout << "dork21" << std::endl;
                    anOutput << players[*it1].getName() << " folds." << std::endl;

                    round_participant_nums.erase(it1);
                    round_hands.erase(it2);
                    //anOutput << "dork22" << std::endl;
                    it1 = round_participant_nums.begin() + playernum;
                    it2 = round_hands.begin() + playernum;
                }
                else {
                    anOutput << players[*it1].getName() << " stays." << std::endl;

                    it1++;
                    it2++;
                    playernum++;
                }
                /*if (it1 != round_participant_nums.end()) {
                    anOutput << players[*it1].getName() << std::endl;
                    (*it2).printPile();
                }
                anOutput << "dork23" << std::endl;*/
            }

            // If only one player has withheld from folding, he/she wins the pot by default.
            if (round_participant_nums.size() < 2) {
                it1 = round_participant_nums.begin();
                anOutput << players[*it1].getName() << " wins " << pot << " by default this round." << std::endl;
                player_balances[*it1] += pot;
                pot = 0;
                continue;
            }

            // Check the first bets the remaining players wish to make.
            it1 = round_participant_nums.begin();
            it2 = round_hands.begin();
            while (it1 != round_participant_nums.end()) {
                // anOutput << "dork3" << std::endl;
                float bet = players[*it1].placeBet(*it2, round_participant_nums.size(), player_balances[*it1]);
                anOutput << "\n" << players[*it1].getName() << " bets " << bet << std::endl;
                pot += bet;
                anOutput << "Pot is now " << pot << std::endl;
                player_balances[*it1] -= bet;
                anOutput << players[*it1].getName() << "'s balance is now " << player_balances[*it1] << std::endl;
                it1++;
                it2++;
            }
            anOutput << std::endl;

            // Have players discard cards they wish to discard and replace the discarded cards from the deck.
            it1 = round_participant_nums.begin();
            it2 = round_hands.begin();
            while (it1 != round_participant_nums.end()) {
                //anOutput << "dork4" << std::endl;
                players[*it1].discardUnwanted(*it2);
                while ((*it2).count() < cardCount) {
                    (*it2).addCard(startingDeck.getCard());
                }
                anOutput << players[*it1].getName() << "'s hand is now ";
                (*it2).printPile();
                it1++;
                it2++;
            }
            anOutput << std::endl;

            // Check which players wish to fold before the final bet.
            it1 = round_participant_nums.begin();
            it2 = round_hands.begin();
            playernum = 0;
            while (it1 != round_participant_nums.end() && round_participant_nums.size() > 1) {
                //anOutput << "dork5" << std::endl;
                anOutput << players[*it1].getName() << std::endl;
                (*it2).printPile();
                anOutput << handName((*it2).determineRank()) << std::endl;
                if (players[*it1].isFolding(*it2, round_participant_nums.size(), player_balances[*it1])) {
                    //anOutput << "dork51" << std::endl;
                    anOutput << players[*it1].getName() << " folds." << std::endl;

                    round_participant_nums.erase(it1);
                    round_hands.erase(it2);
                    //anOutput << "dork52" << std::endl;
                    it1 = round_participant_nums.begin() + playernum;
                    it2 = round_hands.begin() + playernum;
                }
                else {
                    anOutput << players[*it1].getName() << " stays." << std::endl;

                    it1++;
                    it2++;
                    playernum++;
                }
                /*if (it1 != round_participant_nums.end()) {
                    anOutput << players[*it1].getName() << std::endl;
                    (*it2).printPile();
                }
                anOutput << "dork53" << std::endl;*/
            }
            anOutput << std::endl;

            // If only one player has withheld from folding by this point, he/she wins the pot by default.
            if (round_participant_nums.size() < 2) {
                it1 = round_participant_nums.begin();
                anOutput << players[*it1].getName() << " wins " << pot << " by default this round." << std::endl;
                player_balances[*it1] += pot;
                pot = 0;
                continue;
            }

            // Check the bets the remaining players wish to make.
            it1 = round_participant_nums.begin();
            it2 = round_hands.begin();
            while (it1 != round_participant_nums.end()) {
                // anOutput << "dork6" << std::endl;
                float bet = players[*it1].placeBet(*it2, round_participant_nums.size(), player_balances[*it1]);
                anOutput << players[*it1].getName() << " bets " << bet << std::endl;
                pot += bet;
                anOutput << "Pot is now " << pot << std::endl;
                player_balances[*it1] -= bet;
                anOutput << players[*it1].getName() << "'s balance is now " << player_balances[*it1] << std::endl;
                it1++;
                it2++;
            }
            anOutput << std::endl;

            // Determine the winner of the round and hand him the pot.
            enum HandTypes winningHand = HandTypes::unknown;
            int winningIndex = 0;
            for (int participant = 0; participant < round_participant_nums.size(); participant++) {
                anOutput << players[round_participant_nums[participant]].getName() << "'s hand is a " << handName(round_hands[participant].determineRank()) << std::endl;
                if (round_hands[participant].determineRank() > winningHand) {
                    winningIndex = round_participant_nums[participant];
                    winningHand = round_hands[participant].determineRank();
                }
            }
            anOutput << players[winningIndex].getName() << " wins the pot with a " << handName(winningHand) << std::endl;

            player_balances[winningIndex] += pot;
            pot = 0;

            anOutput << players[winningIndex].getName() << "'s balance is now " << player_balances[winningIndex] << std::endl;

            round_participant_nums.clear();
            round_hands.clear();

            roundNum++;
        }
        winnerIndex = game_participant_nums[0];
    
        return true;
    }
  
    bool Game::didRun(bool aRunStatus, std::ostream &anOutput) {
        //Determine if the game ran and finished, and then announce the winning player here and how much they won
        if (played) {
            anOutput << "Game has been won! Player number " << winnerIndex + 1 << " named ";
            anOutput << players[winnerIndex].getName() << " has won! Congratulations!" << std::endl;
        }
        else {
            anOutput << "Game has not been played." << std::endl;
        }
        return played;
    }

    std::string Game::handName(enum HandTypes type) {
        if (type == HandTypes::royal_flush)
            return "royal flush";
        if (type == HandTypes::straight_flush)
            return "straight flush";
        if (type == HandTypes::straight)
            return "straight";
        if (type == HandTypes::flush)
            return "flush";
        if (type == HandTypes::full_house)
            return "full house";
        if (type == HandTypes::four_kind)
            return "four of a kind";
        if (type == HandTypes::three_kind)
            return "three of a kind";
        if (type == HandTypes::two_pair)
            return "two pairs";
        if (type == HandTypes::one_pair)
            return "one pair";
        if (type == HandTypes::high_card)
            return "high card";

        return "unknown";
    }
}




