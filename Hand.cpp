//
//  Hand.cpp
//  assignment2
//
//  Created by rick gessner on 10/6/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#include "Hand.hpp"

//student: define the Hand class methods below...
namespace ECE17 {
	// Default constructor.
	Hand::Hand(const CardList &aCards) {
		// std::string faceSym, suitSym;
		for (int cardnum = 0; cardnum < aCards.size(); cardnum++)
			this->addCard(aCards[cardnum]);
	}
	
	// Copy constructor
	/*Hand::Hand(const Hand &aHand) {
		this->cards = aHand.cards;
	}*/

	// Return if the hand is empty.
	bool Hand::empty() {
		if (this->isEmpty())
			return false;
		while (!(this->isEmpty()))
			this->cards.clear();
		return true;
	}

	// Remove a card from the hand.
	bool Hand::discard(const Card& aCard) {
		std::list<Card>::iterator it = cards.begin();
		while (it != cards.end()) {
			if (*it == aCard) {
				cards.erase(it);
				return true;
			}
		}
		return false;
	}

	// Determine the rank of the hand.
	HandTypes Hand::determineRank() {
		const int faces = 13 + 2;
		const int suits = 4 + 1;
		int facecount[faces];
		int suitcount[suits];
		//int cardcount = 0;
		bool isFlush, isStraight;
		std::list<Card>::iterator it = cards.begin();
		std::string face, suit;

		for (int facenum = 0; facenum < faces; facenum++) {
			facecount[facenum] = 0;
		}

		for (int suitnum = 0; suitnum < suits; suitnum++) {
			suitcount[suitnum] = 0;
		}

		// Count the number of cards total and number of cards for each face and suit.
		while (it != cards.end()) {
			facecount[(static_cast<int>((*it).GetFace()))]++;
			suitcount[(static_cast<int>((*it).GetSuit()))]++;
			//cardcount++;
			it++;
		}

		// Make sure the hand is valid. If it is not valid, return the hand type as unknown.
		if (count() != 5 || suitcount[4] > 0 || facecount[0] > 0 || facecount[14] > 0)
			return HandTypes::unknown;

		// Check if the hand is a flush.
		isFlush = false;
		for (int suitnum = 0; suitnum < suits; suitnum++) {
			if (suitcount[suitnum] == 5) {
				isFlush = true;
				break;
			}
		}

		// Check if the hand is a royal flush. If so return it.
		if (isFlush && facecount[1] == 1 && facecount[13] == 1 && facecount[12] == 1 && facecount[11] == 1 && facecount[10] == 1)
			return HandTypes::royal_flush;

		// Check if the hand is a straight.
		isStraight = false;
		for (int firstFace = 1; firstFace <= 13 - 4; firstFace++) {
			if (facecount[firstFace] == 1 && facecount[firstFace + 1] == 1 && facecount[firstFace + 2] == 1 && facecount[firstFace + 3] == 1 && facecount[firstFace + 4] == 1) {
				isStraight = true;
				break;
			}
		}

		// Check if the hand is a straight flush. If so, return it.
		if (isStraight && isFlush)
			return HandTypes::straight_flush;

		// Check if the hand is four of a kind. If so, return it.
		for (int facenum = 1; facenum < faces; facenum++) {
			if (facecount[facenum] == 4)
				return HandTypes::four_kind;
		}

		// Check if the hand is a full house. If so, return it.
		for (int firstFace = 1; firstFace < faces - 2; firstFace++) {
			for (int secondFace = firstFace + 1; secondFace < faces - 1; secondFace++) {
				if ((facecount[firstFace] == 2 && facecount[secondFace] == 3) || (facecount[firstFace] == 3 && facecount[secondFace] == 2))
					return HandTypes::full_house;
			}
		}

		// If the hand is a flush, return it.
		if (isFlush)
			return HandTypes::flush;

		// If the hand is a straight, return it.
		if (isStraight)
			return HandTypes::straight;

		// If hand is three of a kind, return it.
		for (int facenum = 1; facenum < faces - 1; facenum++)
			if (facecount[facenum] == 3)
				return HandTypes::three_kind;

		// If hand is two pairs, return it.
		for (int firstFace = 1; firstFace < faces - 2; firstFace++) {
			for (int secondFace = firstFace + 1; secondFace < faces - 1; secondFace++) {
				if (facecount[firstFace] == 2 && facecount[secondFace] == 2)
					return HandTypes::two_pair;
			}
		}

		// If hand is one pair, return it.
		for (int facenum = 1; facenum < faces - 1; facenum++)
			if (facecount[facenum] == 2)
				return HandTypes::one_pair;

		// With none of the other options being valid, the only possible play of the hand is the high card.
		return HandTypes::high_card;
	}

	// Return the number of cards in the hand.
	size_t Hand::count() {
		return cards.size();
	}
}