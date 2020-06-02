//
//  Player.hpp
//  assignment2
//
//  Created by rick gessner on 10/6/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "Card.hpp"
#include "Hand.hpp"
#include <stdlib.h>
#include <time.h>

//STUDENT: This class is abstract, and serves as an interface

namespace ECE17 {

  class IPlayer {
  public:

    virtual std::string& getName()=0; //get the player name
    virtual void         willStartRound(size_t aRoundNum)=0; //tell player that round N is starting...`
    virtual bool         isFolding(const ECE17::Hand &aHand, size_t aPlayerCount, float aBalance)=0;
    virtual float        placeBet(const ECE17::Hand &aHand, size_t aPlayerCount, float aMaxBet)=0;
    virtual size_t       discardUnwanted(ECE17::Hand &aHand)=0;
  };

  class Player: public IPlayer {
  public:
	  // Default constructor
	  Player(std::string aName = "noName") : name(aName) {
		  firstBetPlayed = false;
		  folded = false;
		  srand(time(NULL));
	  }

	  // Copy constructor
	  Player(const Player& aPlayer) {
		  name = aPlayer.name;
		  firstBetPlayed = false;
		  folded = false;
		  srand(time(NULL));
	  }

	  // Copy constructor
	  /*Player& operator=(const Player& aPlayer) {
		  name = aPlayer.name;
		  firstBetPlayed = false;
		  folded = false;
		  std::srand(static_cast<unsigned int>(time(nullptr)));
	  }*/

	  /*// Destructor
	  ~Player() {}*/

	  // Return the player's name.
	  std::string& getName() {
		  return name;
	  }

	  // Reset player state at the start of a new round.
	  void IPlayer::willStartRound(size_t aRoundNum) {
		  roundnum = aRoundNum;
		  folded = false;
		  firstBetPlayed = false;
	  }

	  // Return if the player will fold.
	  bool IPlayer::isFolding(const ECE17::Hand& aHand, size_t aPlayerCount, float aBalance) {
		  // STUDENT: Implement the method!!!
		  enum HandTypes handQual = (const_cast<Hand&>(aHand)).determineRank();
		  float randnum = (float)rand() / RAND_MAX;
		  // No folding if only one player in the game.
		  if (aPlayerCount < 2)
			  return folded;
		  // Probability of straights and better hands is less than 1%, so folding is bad idea.
		  if (handQual >= HandTypes::straight)
			  return folded;
		  // Fold 9% of the time with a three of a kind hand.
		  if (handQual == HandTypes::three_kind) {
			  if (randnum < 0.09)
				  folded = true;
			  return folded;
		  }
		  // Fold 15% of the time with a two pair hand on the final bet.
		  if (handQual == HandTypes::two_pair && firstBetPlayed) {
			  if (randnum < 0.15)
				  folded = true;
			  return folded;
		  }
		  // Fold 9% of the time with a two pair hand on the first bet.
		  if (handQual == HandTypes::two_pair) {
			  if (randnum < 0.09)
				  folded = true;
			  return folded;
		  }
		  // Fold 35% of the time with a one pair hand on the final bet if there are more than 3 players.
		  if (handQual == HandTypes::one_pair && firstBetPlayed && aPlayerCount > 3) {
			  if (randnum < 0.35)
				  folded = true;
			  return folded;
		  }
		  // Fold 20% of the time with a one pair hand on the first bet if the first bet is being played.
		  // If it is the final bet, but there are three or fewer players, also fold 20% of the time.
		  if (handQual == HandTypes::one_pair && (!firstBetPlayed || aPlayerCount <= 3)) {
			  if (randnum < 0.2)
				  folded = true;
			  return folded;
		  }
		  // High card is a poor hand. If high card is the hand and the first bet has been played,
		  // consider folding.
		  if (handQual == HandTypes::high_card && aPlayerCount > 3 && firstBetPlayed) {
			  // 75% of the time, fold a high_card hand on final bet. On the other 25%, try bluffing.
			  if (randnum < 0.75)
				  folded = true;
			  return folded;
		  }
		  // If a high card is made on the first bet, consider folding as well.
		  if (handQual == HandTypes::high_card && aPlayerCount > 3) {
			  // 40% of the time, fold a high_card hand on the first bet. On the other 60%, try bluffing.
			  if (randnum < 0.4)
				  folded = true;
			  return folded;
		  }
		  // Under unusual circumstances, fold.
		  folded = true;
		  return folded;
	  }

	  // Place a bet.
	  float IPlayer::placeBet(const ECE17::Hand& aHand, size_t aPlayerCount, float aMaxBet) {
		  enum HandTypes handQual = (const_cast<Hand&>(aHand)).determineRank();
		  double randnum = (double)rand() / RAND_MAX;
		  double bluffcalc = (double)rand() / RAND_MAX;
		  double bluffthresh = 0.2;

		  // If the hand is a straight or better, bet between 25% and 50% of the max bet on the first bet.
		  if (handQual >= HandTypes::straight && !firstBetPlayed) {
			  firstBetPlayed = true;
			  return (float)(aMaxBet * (0.25 + 0.25 * randnum));
		  }

		  // If the hand is a straight or better, bet between 25% and 100% of the max bet on the final bet.
		  if (handQual >= HandTypes::straight)
			  return (float)(aMaxBet * (0.25 + 0.75 * randnum));

		  // If the hand is a high card, reduce the odds of bluffing.
		  if (handQual == HandTypes::high_card)
			  bluffthresh = 0.15;

		  // Perform a bluff.
		  if (bluffcalc < bluffthresh)
			  return (float)(aMaxBet * randnum);

		  // If the hand is a three-of-a-kind or less, but greater than a high card, bet between 25% and 40% of the max bet on the first bet.
		  if (handQual <= HandTypes::three_kind && !firstBetPlayed && handQual > HandTypes::high_card)
			  return (float)(aMaxBet * (0.25 + 0.15 * randnum));

		  // If the hand is a three-of-a-kind or less, but greater than a high card, bet up to 25% of the max bet on the final bet.
		  if (handQual <= HandTypes::three_kind && handQual > HandTypes::high_card)
			  return (float)(aMaxBet * (0.25 * randnum));

		  // If the hand is a high card, bet conservatively.
		  if (handQual == HandTypes::high_card)
			  return (float)(aMaxBet * (0.25 + 0.10 * randnum));

		  // Don't bet if the hand is unknown.
		  if (handQual == HandTypes::unknown)
			  return 0;

		  return 0;
	  }

	  // Discard unwanted cards from the hand.
	  size_t IPlayer::discardUnwanted(ECE17::Hand& aHand) {
		  enum HandTypes handrank = aHand.determineRank();
		  float randnum = (float)rand() / RAND_MAX;
		  // There is nothing to discard if there are no cards.
		  // Also straights and anything better are all good hands dependent on all its cards,
		  // so discarding them and ruining the hand would be a bad idea.
		  if (aHand.isEmpty() || handrank >= HandTypes::straight)
			  return 0;
		  else {
			  // If the hand is lesser, discard a card and to try to get a better hand.
			  const int faces = 13 + 2;
			  const int suits = 4 + 1;
			  int facecount[faces];
			  int suitcount[suits];
			  double probsecdiscard[faces] = { 1, 0, 1, 0.8, 0.8, 0.7, 0.7, 0.6, 0.5, 0.2, 0, 0, 0, 0, 1 };
			  std::vector<Card> cards;
			  Card currentCard;

			  for (int facenum = 0; facenum < faces; facenum++)
				  facecount[facenum] = 0;
			  for (int suitnum = 0; suitnum < suits; suitnum++)
				  suitcount[suitnum] = 0;

			  while (!aHand.isEmpty()) {
				  currentCard = aHand.getCard();
				  facecount[(static_cast<int>(currentCard.GetFace()))]++;
				  suitcount[(static_cast<int>(currentCard.GetSuit()))]++;
				  cards.push_back(currentCard);
			  }

			  // Discard the lowest card.
			  for (enum FaceEnum facenum = FaceEnum::ACE; facenum <= FaceEnum::KING; facenum = static_cast<enum FaceEnum>(facenum + 1)) {
				  // Determine the first card to discard's face value.
				  if (facecount[(static_cast<int>(facenum))] != 1) {
					  continue;
				  }
				  // Then find it in cards and remove it. Update the facecount and suitcount.
				  for (int cardnum = 0; cardnum < cards.size(); cardnum++) {
					  if (cards[cardnum].GetFace() == facenum) {
						  currentCard = cards[cardnum];
						  cards.erase(cardnum + cards.begin());
						  facecount[(static_cast<int>(facenum))] = 0;
						  suitcount[(static_cast<int>(currentCard.GetSuit()))]--;
						  break;
					  }
				  }
				  break;
			  }

			  // Consider discarding a second non-three-of-kind cards for a three of a kind hand.
			  if (handrank == HandTypes::three_kind) {
				  for (enum FaceEnum facenum = FaceEnum::ACE; facenum <= FaceEnum::KING; facenum = static_cast<enum FaceEnum>(facenum + 1)) {
					  // Determine the second card to discard's face value.
					  if (facecount[(static_cast<int>(facenum))] != 1) {
						  continue;
					  }

					  randnum = (float)rand() / RAND_MAX;

					  // Then find it in cards and consider removing it. Update the facecount and suitcount.
					  for (int cardnum = 0; cardnum < cards.size(); cardnum++) {
						  // If the card in cards is found, and probability dictates to remove it, remove it.
						  if (cards[cardnum].GetFace() == facenum && randnum < probsecdiscard[(static_cast<int>(facenum))]) {
							  currentCard = cards[cardnum];
							  cards.erase(cardnum + cards.begin());
							  facecount[(static_cast<int>(facenum))] = 0;
							  suitcount[(static_cast<int>(currentCard.GetSuit()))]--;
							  for (int cardnum = 0; cardnum < cards.size(); cardnum++)
								  aHand.addCard(cards[cardnum]);
							  return 2;
						  }
						  // If probability dictates not to remove it, don't, and update the hand.
						  if (cards[cardnum].GetFace() == facenum) {
							  for (int cardnum = 0; cardnum < cards.size(); cardnum++)
								  aHand.addCard(cards[cardnum]);
							  return 1;
						  }
					  }
					  break;
				  }
				  for (int cardnum = 0; cardnum < cards.size(); cardnum++)
					  aHand.addCard(cards[cardnum]);
				  return 5 - cards.size();
			  }

			  // Consider discarding two or three non-pair cards total for a single pair hand.
			  if (handrank == HandTypes::one_pair) {
				  // Consider discarding the second card.
				  for (enum FaceEnum facenum = FaceEnum::ACE; facenum <= FaceEnum::KING; facenum = static_cast<enum FaceEnum>(facenum + 1)) {
					  // Determine the second card to discard's face value.
					  if (facecount[(static_cast<int>(facenum))] != 1) {
						  continue;
					  }
					  // Then find it in cards and consider removing it. Update the facecount and suitcount.
					  for (int cardnum = 0; cardnum < cards.size(); cardnum++) {
						  // If the card in cards is found, and probability dictates to remove it, remove it.
						  if (cards[cardnum].GetFace() == facenum && randnum < probsecdiscard[(static_cast<int>(facenum))]) {
							  currentCard = cards[cardnum];
							  cards.erase(cardnum + cards.begin());
							  facecount[(static_cast<int>(facenum))] = 0;
							  suitcount[(static_cast<int>(currentCard.GetSuit()))]--;
							  break;
						  }
						  // If probability dictates not to remove it, don't, and update the hand.
						  if (cards[cardnum].GetFace() == facenum) {
							  for (int cardnum = 0; cardnum < cards.size(); cardnum++)
								  aHand.addCard(cards[cardnum]);
							  return 1;
						  }
					  }
					  break;
				  }

				  // Consider discarding the third card.
				  for (enum FaceEnum facenum = FaceEnum::ACE; facenum <= FaceEnum::KING; facenum = static_cast<enum FaceEnum>(facenum + 1)) {
					  // Determine the second card to discard's face value.
					  if (facecount[(static_cast<int>(facenum))] != 1) {
						  continue;
					  }
					  // Then find it in cards and consider removing it. Update the facecount and suitcount.
					  for (int cardnum = 0; cardnum < cards.size(); cardnum++) {
						  // If the card in cards is found, and probability dictates to remove it, remove it.
						  if (cards[cardnum].GetFace() == facenum && randnum < probsecdiscard[(static_cast<int>(facenum))]) {
							  currentCard = cards[cardnum];
							  cards.erase(cardnum + cards.begin());
							  facecount[(static_cast<int>(facenum))] = 0;
							  suitcount[(static_cast<int>(currentCard.GetSuit()))]--;
							  for (int cardnum = 0; cardnum < cards.size(); cardnum++)
								  aHand.addCard(cards[cardnum]);
							  return 3;
						  }
						  // If probability dictates not to remove it, don't, and update the hand.
						  if (cards[cardnum].GetFace() == facenum) {
							  for (int cardnum = 0; cardnum < cards.size(); cardnum++)
								  aHand.addCard(cards[cardnum]);
							  return 2;
						  }
					  }
					  break;
				  }

				  // Consider discarding two to three non-pair cards total for a single pair hand.
				  if (handrank == HandTypes::one_pair) {
					  // Consider discarding the second card.
					  for (enum FaceEnum facenum = FaceEnum::ACE; facenum <= FaceEnum::KING; facenum = static_cast<enum FaceEnum>(facenum + 1)) {
						  // Determine the second card to discard's face value.
						  if (facecount[(static_cast<int>(facenum))] != 1) {
							  continue;
						  }

						  // Then find it in cards and consider removing it. Update the facecount and suitcount.
						  for (int cardnum = 0; cardnum < cards.size(); cardnum++) {
							  // If the card in cards is found, and probability dictates to remove it, remove it.
							  if (cards[cardnum].GetFace() == facenum && randnum < probsecdiscard[(static_cast<int>(facenum))]) {
								  currentCard = cards[cardnum];
								  cards.erase(cardnum + cards.begin());
								  facecount[(static_cast<int>(facenum))] = 0;
								  suitcount[(static_cast<int>(currentCard.GetSuit()))]--;
								  break;
							  }
							  // If probability dictates not to remove it, don't, and update the hand.
							  if (cards[cardnum].GetFace() == facenum) {
								  for (int cardnum = 0; cardnum < cards.size(); cardnum++)
									  aHand.addCard(cards[cardnum]);
								  return 1;
							  }
						  }
						  break;
					  }

					  // Consider discarding the third card.
					  for (enum FaceEnum facenum = FaceEnum::ACE; facenum <= FaceEnum::KING; facenum = static_cast<enum FaceEnum>(facenum + 1)) {
						  // Determine the third card to discard's face value.
						  if (facecount[(static_cast<int>(facenum))] != 1) {
							  continue;
						  }
						  // Then find it in cards and consider removing it. Update the facecount and suitcount.
						  for (int cardnum = 0; cardnum < cards.size(); cardnum++) {
							  // If the card in cards is found, and probability dictates to remove it, remove it.
							  if (cards[cardnum].GetFace() == facenum && randnum < probsecdiscard[(static_cast<int>(facenum))]) {
								  currentCard = cards[cardnum];
								  cards.erase(cardnum + cards.begin());
								  facecount[(static_cast<int>(facenum))] = 0;
								  suitcount[(static_cast<int>(currentCard.GetSuit()))]--;
								  for (int cardnum = 0; cardnum < cards.size(); cardnum++)
									  aHand.addCard(cards[cardnum]);
								  return 3;
							  }
							  // If probability dictates not to remove it, don't, and update the hand.
							  if (cards[cardnum].GetFace() == facenum) {
								  for (int cardnum = 0; cardnum < cards.size(); cardnum++)
									  aHand.addCard(cards[cardnum]);
								  return 2;
							  }
						  }
						  break;
					  }
				  }
			  }

			  // Consider discarding two to four non-pair cards total for a high card hand.
			  if (handrank == HandTypes::high_card) {
				  // Consider discarding the second card.
				  for (enum FaceEnum facenum = FaceEnum::ACE; facenum <= FaceEnum::KING; facenum = static_cast<enum FaceEnum>(facenum + 1)) {
					  // Determine the second card to discard's face value.
					  if (facecount[(static_cast<int>(facenum))] != 1) {
						  continue;
					  }
					  // Then find it in cards and consider removing it. Update the facecount and suitcount.
					  for (int cardnum = 0; cardnum < cards.size(); cardnum++) {
						  // If the card in cards is found, and probability dictates to remove it, remove it.
						  if (cards[cardnum].GetFace() == facenum && *probsecdiscard < facecount[(static_cast<int>(facenum))]) {
							  currentCard = cards[cardnum];
							  cards.erase(cardnum + cards.begin());
							  facecount[(static_cast<int>(facenum))] = 0;
							  suitcount[(static_cast<int>(currentCard.GetSuit()))]--;
							  break;
						  }
						  // If probability dictates not to remove it, don't, and update the hand.
						  if (cards[cardnum].GetFace() == facenum) {
							  for (int cardnum = 0; cardnum < cards.size(); cardnum++)
								  aHand.addCard(cards[cardnum]);
							  return 1;
						  }
					  }
					  break;
				  }

				  // Consider discarding the third card.
				  for (enum FaceEnum facenum = FaceEnum::ACE; facenum <= FaceEnum::KING; facenum = static_cast<enum FaceEnum>(facenum + 1)) {
					  // Determine the second card to discard's face value.
					  if (facecount[(static_cast<int>(facenum))] != 1) {
						  continue;
					  }
					  // Then find it in cards and consider removing it. Update the facecount and suitcount.
					  for (int cardnum = 0; cardnum < cards.size(); cardnum++) {
						  // If the card in cards is found, and probability dictates to remove it, remove it.
						  if (cards[cardnum].GetFace() == facenum && *probsecdiscard < facecount[(static_cast<int>(facenum))]) {
							  currentCard = cards[cardnum];
							  cards.erase(cardnum + cards.begin());
							  facecount[(static_cast<int>(facenum))] = 0;
							  suitcount[(static_cast<int>(currentCard.GetSuit()))]--;
							  break;
						  }
						  // If probability dictates not to remove it, don't, and update the hand.
						  if (cards[cardnum].GetFace() == facenum) {
							  for (int cardnum = 0; cardnum < cards.size(); cardnum++)
								  aHand.addCard(cards[cardnum]);
							  return 2;
						  }
					  }
					  break;
				  }

				  // Consider discarding the fourth card.
				  for (enum FaceEnum facenum = FaceEnum::ACE; facenum <= FaceEnum::KING; facenum = static_cast<enum FaceEnum>(facenum + 1)) {
					  // Determine the second card to discard's face value.
					  if (facecount[(static_cast<int>(facenum))] != 1) {
						  continue;
					  }
					  // Then find it in cards and consider removing it. Update the facecount and suitcount.
					  for (int cardnum = 0; cardnum < cards.size(); cardnum++) {
						  // If the card in cards is found, and probability dictates to remove it, remove it.
						  if (cards[cardnum].GetFace() == facenum && *probsecdiscard < facecount[(static_cast<int>(facenum))]) {
							  currentCard = cards[cardnum];
							  cards.erase(cardnum + cards.begin());
							  facecount[(static_cast<int>(facenum))] = 0;
							  suitcount[(static_cast<int>(currentCard.GetSuit()))]--;
							  for (int cardnum = 0; cardnum < cards.size(); cardnum++)
								  aHand.addCard(cards[cardnum]);
							  return 4;
						  }
						  // If probability dictates not to remove it, don't, and update the hand.
						  if (cards[cardnum].GetFace() == facenum) {
							  for (int cardnum = 0; cardnum < cards.size(); cardnum++)
								  aHand.addCard(cards[cardnum]);
							  return 3;
						  }
					  }
					  break;
				  }
			  }
			  /*if (aHand.size() < 5) {

			  }*/
			  return 1;
		  }
	  }

  private:
	  std::string name;
	  size_t roundnum;
	  bool firstBetPlayed, folded;
  };
}

#endif /* Player_hpp */
