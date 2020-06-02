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
    bool Game::addPlayer(ECE17::IPlayer &aPlayer) {
        players.push_back(static_cast<ECE17::Player&>(aPlayer));
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
                    // Player cannot pay the ante, so they will be removed from the game.
                    game_participant_nums.erase(it1);
                    it1 = game_participant_nums.begin() + playernum;
                }
                else {
                    // Player can pay the ante, so they remain in the game.
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
                players[*it1].willStartRound(roundNum);
                std::vector<Card> cardList;
                cardList.clear();

                // Build a hand from cards in the deck.
                for (int cardnum = 0; cardnum < cardCount; cardnum++) {
                    cardList.push_back(startingDeck.getCard());
                }
                
                // Give the player the just-built hand and deduct the ante from the player's balance.
                Hand newHand(cardList);
                round_hands.push_back(newHand);
                player_balances[*it1] -= minBet;

                // State the updated player balance.
                anOutput << players[*it1].getName() << "'s balance is " << player_balances[*it1] << std::endl;

                // Add the ante to the bet and the count the number of players in the round.
                pot += minBet;
                anteCount++;
                it1++;
            }
            anOutput << "Pot is: " << pot << std::endl;

            // If only one player can meet the ante, the game is over.
            // and the money is returned to the sole player.
            if (anteCount < 2) {
                for (auto& x : round_participant_nums) {
                    player_balances[x] += minBet;
                }
                pot = 0;
                break;
            }

            // Check which players wish to fold before the first bet.
            it1 = round_participant_nums.begin();
            it2 = round_hands.begin();
            playernum = 0;
            while (it1 != round_participant_nums.end() && round_participant_nums.size() > 1) {
                // State each player and their hand.
                anOutput << "\n" << players[*it1].getName() << std::endl;
                (*it2).printPile();
                anOutput << handName((*it2).determineRank()) << std::endl;
                
                if (players[*it1].isFolding(*it2, round_participant_nums.size(), player_balances[*it1])) {
                    // If the player folds, state such, remove them from the game, and move onto the next player.
                    anOutput << players[*it1].getName() << " folds." << std::endl;

                    round_participant_nums.erase(it1);
                    round_hands.erase(it2);
                    it1 = round_participant_nums.begin() + playernum;
                    it2 = round_hands.begin() + playernum;
                }
                else {
                    // If the player stays, state such and move onto the next player.
                    anOutput << players[*it1].getName() << " stays." << std::endl;

                    it1++;
                    it2++;
                    playernum++;
                }
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
                // Get the player's bet.
                float bet = players[*it1].placeBet(*it2, round_participant_nums.size(), player_balances[*it1]);
                anOutput << "\n" << players[*it1].getName() << " bets " << bet << std::endl;

                // Add the bet to the pot.
                pot += bet;
                anOutput << "Pot is now " << pot << std::endl;

                // Remove the bet from the player's balance.
                player_balances[*it1] -= bet;
                anOutput << players[*it1].getName() << "'s balance is now " << player_balances[*it1] << std::endl;

                // And move onto the next player.
                it1++;
                it2++;
            }
            anOutput << std::endl;

            // Have players discard cards they wish to discard and replace the discarded cards from the deck.
            it1 = round_participant_nums.begin();
            it2 = round_hands.begin();
            while (it1 != round_participant_nums.end()) {
                // Give the players a chance to discard unwanted cards.
                players[*it1].discardUnwanted(*it2);

                while ((*it2).count() < cardCount) {
                    // If any cards have been discarded, replace them.
                    (*it2).addCard(startingDeck.getCard());
                }

                // State the player's current hand, regardless of discarding.
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
                // State each player and their hand.
                anOutput << "\n" << players[*it1].getName() << std::endl;
                (*it2).printPile();
                anOutput << handName((*it2).determineRank()) << std::endl;

                if (players[*it1].isFolding(*it2, round_participant_nums.size(), player_balances[*it1])) {
                    // If the player folds, state such, remove them from the game, and move onto the next player.
                    anOutput << players[*it1].getName() << " folds." << std::endl;

                    round_participant_nums.erase(it1);
                    round_hands.erase(it2);
                    it1 = round_participant_nums.begin() + playernum;
                    it2 = round_hands.begin() + playernum;
                }
                else {
                    // If the player stays, state such and move onto the next player.
                    anOutput << players[*it1].getName() << " stays." << std::endl;

                    it1++;
                    it2++;
                    playernum++;
                }
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
                // Get the player's bet.
                float bet = players[*it1].placeBet(*it2, round_participant_nums.size(), player_balances[*it1]);
                anOutput << "\n" << players[*it1].getName() << " bets " << bet << std::endl;

                // Add the bet to the pot.
                pot += bet;
                anOutput << "Pot is now " << pot << std::endl;

                // Remove the bet from the player's balance.
                player_balances[*it1] -= bet;
                anOutput << players[*it1].getName() << "'s balance is now " << player_balances[*it1] << std::endl;

                // And move onto the next player.
                it1++;
                it2++;
            }
            anOutput << std::endl;

            // Determine the winner of the round and hand him the pot.
            std::vector<int> winningIndices;
            std::vector<Card> emptyHand;
            emptyHand.clear();
            Hand winningHand = Hand(emptyHand);
            for (int participant = 0; participant < round_participant_nums.size(); participant++) {
                anOutput << players[round_participant_nums[participant]].getName() << "'s hand is a " << handName(round_hands[participant].determineRank()) << std::endl;
                if (round_hands[participant].determineRank() > winningHand.determineRank()) {
                    // If the current player's hand is better than the other hands before it.
                    winningIndices.clear();
                    winningIndices.push_back(round_participant_nums[participant]);
                    winningHand = round_hands[participant];
                }
                else if (round_hands[participant].determineRank() == winningHand.determineRank()) {
                    // If the current player's hand is tied with prior hands, try a tiebreaker.
                    // If their hands cannot be tiebroken, they will split the pot.
                    int winnerNum = betterHand(winningHand, round_hands[participant]);
                    if (winnerNum == 0) {
                        winningIndices.push_back(round_participant_nums[participant]);
                        winningHand = round_hands[participant];
                    }
                    else if (winnerNum > 0) {
                        winningIndices.clear();
                        winningIndices.push_back(round_participant_nums[participant]);
                        winningHand = round_hands[participant];
                    }
                }
                std::cout << "Winning hand is: " << handName(winningHand.determineRank()) << "\n\n";
            }

            // Output the winning hand(s).
            if (winningIndices.size() == 1)
                anOutput << players[winningIndices[0]].getName() << " wins the pot with a " << handName(winningHand.determineRank()) << std::endl;
            else if (winningIndices.size() == 2) {
                anOutput << players[winningIndices[0]].getName() << " and " << players[winningIndices[1]].getName() << " split the pot with " << handName(winningHand.determineRank()) << "'s "<< std::endl;
            }
            else {
                for (int i = 0; i < winningIndices.size(); i++) {
                    if (i < winningIndices.size() - 1)
                        anOutput << players[winningIndices[i]].getName() << ", ";
                    else
                        anOutput << "and " << players[winningIndices[i]].getName() << " split the pot with " << handName(winningHand.determineRank()) << "'s " << std::endl;
                }
            }

            // Set and output the winner(s) new balance(s).
            for (int i = 0; i < winningIndices.size(); i++) {
                player_balances[winningIndices[i]] += pot / winningIndices.size();
                anOutput << players[winningIndices[i]].getName() << "'s balance is now " << player_balances[winningIndices[i]] << std::endl;
            }

            // Empty the pot.
            pot = 0;

            // Clear all vectors for the next round.
            round_participant_nums.clear();
            round_hands.clear();
            winningIndices.clear();

            roundNum++;
        }
        winnerIndex = game_participant_nums[0];
    
        return true;
    }
  
    bool Game::didRun(bool aRunStatus, std::ostream &anOutput) {
        //Determine if the game ran and finished, and then announce the winning player here and how much they won
        if (aRunStatus) {
            if (played) {
                anOutput << "Game has been won! Player number " << winnerIndex + 1 << " named ";
                anOutput << players[winnerIndex].getName() << " has won! Congratulations!" << std::endl;
            }
            else {
                anOutput << "Game has not been played." << std::endl;
            }
        }
        return played;
    }

    // For output purposes, this is to help Game determine each player's hand in string form.
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

    // Determines which of the hands is the better hand. Returns -1 if the first hand is better.
    // Returns 1 if the second hand is better. Returns 0 if the hands are equally good.
    int Game::betterHand(Hand hand1, Hand hand2) {
        if (hand2.determineRank() > hand1.determineRank())
            return 1;
        else if (hand2.determineRank() < hand1.determineRank())
            return -1;
        else {
            const int faces = 13 + 2;
            const int suits = 4 + 1;
            int facecount1[faces];
            int facecount2[faces];
            enum HandTypes hand = hand1.determineRank();
            std::vector<Card> handCopy1, handCopy2;
            std::string face, suit;

            // Setup the facecount and suitcount arrays for tracking each card count.
            for (int facenum = 0; facenum < faces; facenum++) {
                facecount1[facenum] = 0;
                facecount2[facenum] = 0;
            }

            // Record each card.
            while (!hand1.isEmpty()) {
                handCopy1.push_back(hand1.getCard());
                handCopy2.push_back(hand2.getCard());
            }

            std::vector<Card>::iterator it1 = handCopy1.begin();
            std::vector<Card>::iterator it2 = handCopy2.begin();

            // Record instance of each card by its faces and suits.
            while (it1 != handCopy1.end()) {
                facecount1[(static_cast<int>((*it1).GetFace()))]++;
                it1++;
                facecount2[(static_cast<int>((*it2).GetFace()))]++;
                it2++;
            }

            // If the hands are high card, straight, flush, or straight flush, check for the highest card that is not tied. Whichever hand has the highest not-tied card wins.
            if (hand == HandTypes::high_card || hand == HandTypes::straight || hand == HandTypes::flush || hand == HandTypes::straight_flush) {
                for (enum FaceEnum face = FaceEnum::KING; face >= FaceEnum::ACE; face = static_cast<FaceEnum>(face - 1)) {
                    if (facecount1[static_cast<int>(face)] > facecount2[static_cast<int>(face)]) {
                        if (facecount2[static_cast<int>(FaceEnum::ACE)] > facecount1[static_cast<int>(FaceEnum::ACE)])
                            return 1;
                        return -1;
                    }
                    if (facecount1[static_cast<int>(face)] < facecount2[static_cast<int>(face)]) {
                        if (facecount2[static_cast<int>(FaceEnum::ACE)] < facecount1[static_cast<int>(FaceEnum::ACE)])
                            return -1;
                        return 1;
                    }
                }
            }
            else if (hand == HandTypes::one_pair || hand == HandTypes::two_pair) {
                // If the hands have one or two pairs, the winning hand is the one with the highest non-tied pair.
                if (facecount2[static_cast<int>(FaceEnum::ACE)] > facecount1[static_cast<int>(FaceEnum::ACE)] && facecount2[static_cast<int>(FaceEnum::ACE)] == 2)
                    return 1;
                if (facecount2[static_cast<int>(FaceEnum::ACE)] < facecount1[static_cast<int>(FaceEnum::ACE)] && facecount1[static_cast<int>(FaceEnum::ACE)] == 2)
                    return -1;
                for (enum FaceEnum face = FaceEnum::KING; face >= FaceEnum::ACE; face = static_cast<FaceEnum>(face - 1)) {
                    if (facecount1[static_cast<int>(face)] > facecount2[static_cast<int>(face)] && facecount1[static_cast<int>(face)] == 2) {
                        return -1;
                    }
                    if (facecount1[static_cast<int>(face)] < facecount2[static_cast<int>(face)] && facecount2[static_cast<int>(face)] == 2) {
                        return 1;
                    }
                }
                // If the hands' pair(s) is/are tied, look for the highest single card.
                for (enum FaceEnum face = FaceEnum::KING; face >= FaceEnum::ACE; face = static_cast<FaceEnum>(face - 1)) {
                    if (facecount1[static_cast<int>(face)] > facecount2[static_cast<int>(face)]) {
                        if (facecount2[static_cast<int>(FaceEnum::ACE)] > facecount1[static_cast<int>(FaceEnum::ACE)])
                            return 1;
                        return -1;
                    }
                    if (facecount1[static_cast<int>(face)] < facecount2[static_cast<int>(face)]) {
                        if (facecount2[static_cast<int>(FaceEnum::ACE)] < facecount1[static_cast<int>(FaceEnum::ACE)])
                            return -1;
                        return 1;
                    }
                }
            }
            else if (hand == HandTypes::three_kind || hand == HandTypes::full_house) {
                // If the hands are three kind or full house, look for which has the higher three of a kind.
                if (facecount2[static_cast<int>(FaceEnum::ACE)] > facecount1[static_cast<int>(FaceEnum::ACE)] && facecount2[static_cast<int>(FaceEnum::ACE)] == 3)
                    return 1;
                if (facecount2[static_cast<int>(FaceEnum::ACE)] < facecount1[static_cast<int>(FaceEnum::ACE)] && facecount1[static_cast<int>(FaceEnum::ACE)] == 3)
                    return -1;
                for (enum FaceEnum face = FaceEnum::KING; face >= FaceEnum::ACE; face = static_cast<FaceEnum>(face - 1)) {
                    if (facecount1[static_cast<int>(face)] > facecount2[static_cast<int>(face)] && facecount1[static_cast<int>(face)] == 3)
                        return -1;
                    if (facecount1[static_cast<int>(face)] < facecount2[static_cast<int>(face)] && facecount2[static_cast<int>(face)] == 3)
                        return 1;
                }
            }
            else if (hand == HandTypes::four_kind) {
                // If the hands are four kind, look for which has the higher four of a kind.
                if (facecount2[static_cast<int>(FaceEnum::ACE)] > facecount1[static_cast<int>(FaceEnum::ACE)] && facecount2[static_cast<int>(FaceEnum::ACE)] == 4)
                    return 1;
                if (facecount2[static_cast<int>(FaceEnum::ACE)] < facecount1[static_cast<int>(FaceEnum::ACE)] && facecount1[static_cast<int>(FaceEnum::ACE)] == 4)
                    return -1;
                for (enum FaceEnum face = FaceEnum::KING; face >= FaceEnum::ACE; face = static_cast<FaceEnum>(face - 1)) {
                    if (facecount1[static_cast<int>(face)] > facecount2[static_cast<int>(face)] && facecount1[static_cast<int>(face)] == 4)
                        return -1;
                    if (facecount1[static_cast<int>(face)] < facecount2[static_cast<int>(face)] && facecount2[static_cast<int>(face)] == 4)
                        return 1;
                }
            }
        }
        return 0; // By default, return that the hands are tied.
    }
}




