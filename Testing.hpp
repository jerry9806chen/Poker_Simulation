//
//  Testing.hpp
//  assignment7
//
//  Created by rick gessner on 11/24/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#ifndef Testing_h
#define Testing_h


bool validateHandRanking(std::ostream &anOutput) {

	//STUDENT:  Write tests to confirm that your Hand::determineRank() method is correct
	//          for each of the 10 types of hands you can have in 5-card poker.
	//          (See enum at top of hands.hpp for the full list)

	std::vector<ECE17::Card> cardList;

	// Test for a royal flush:
	enum ECE17::FaceEnum face = ECE17::FaceEnum::ACE;
	enum ECE17::SuitEnum suit = ECE17::SuitEnum::SPADE;
	cardList.push_back(ECE17::Card(face, suit));
	for (face = ECE17::FaceEnum::KING; face >= ECE17::FaceEnum::TEN; face = static_cast<ECE17::FaceEnum>(face - 1)) {
		cardList.push_back(ECE17::Card(face, suit));
	}
	ECE17::Hand royal_flush(cardList);
	anOutput << "Royal Flush test: ";
	if (royal_flush.determineRank() != ECE17::HandTypes::royal_flush) {
		anOutput << "Failed!" << std::endl;
		return false;
	}
	else {
		anOutput << "Passed!" << std::endl;
	}

	// Test for a straight flush: 
	cardList.clear();
	suit = ECE17::SuitEnum::HEART;
	for (face = ECE17::FaceEnum::FOUR; face <= ECE17::FaceEnum::EIGHT; face = static_cast<ECE17::FaceEnum>(face + 1)) {
		cardList.push_back(ECE17::Card(face, suit));
	}
	ECE17::Hand straight_flush(cardList);
	anOutput << "Straight Flush test: ";
	if (straight_flush.determineRank() != ECE17::HandTypes::straight_flush) {
		anOutput << "Failed!" << std::endl;
		return false;
	}
	else {
		anOutput << "Passed!" << std::endl;
	}

	// Test for a four of a kind: 
	cardList.clear();
	face = ECE17::FaceEnum::ACE;
	for (suit = ECE17::SuitEnum::SPADE; suit < ECE17::SuitEnum::INVALID_SUIT; suit = static_cast<ECE17::SuitEnum>(suit + 1)) {
		cardList.push_back(ECE17::Card(face, suit));
	}
	cardList.push_back(ECE17::Card(ECE17::FaceEnum::FOUR, ECE17::SuitEnum::DIAMOND));
	ECE17::Hand four_kind(cardList);
	anOutput << "Four of a kind test: ";
	if (four_kind.determineRank() != ECE17::HandTypes::four_kind) {
		anOutput << "Failed!" << std::endl;
		return false;
	}
	else {
		anOutput << "Passed!" << std::endl;
	}

	// Test for full house: 
	cardList.clear();
	face = ECE17::FaceEnum::EIGHT;
	for (suit = ECE17::SuitEnum::DIAMOND; suit < ECE17::SuitEnum::INVALID_SUIT; suit = static_cast<ECE17::SuitEnum>(suit + 1)) {
		cardList.push_back(ECE17::Card(face, suit));
	}
	face = ECE17::FaceEnum::KING;
	for (suit = ECE17::SuitEnum::SPADE; suit < ECE17::SuitEnum::INVALID_SUIT; suit = static_cast<ECE17::SuitEnum>(suit + 2)) {
		cardList.push_back(ECE17::Card(face, suit));
	}
	ECE17::Hand full_house(cardList);
	anOutput << "Full house test: ";
	if (full_house.determineRank() != ECE17::HandTypes::full_house) {
		anOutput << "Failed!" << std::endl;
		return false;
	}
	else {
		anOutput << "Passed!" << std::endl;
	}
  
	// Test for flush: 
	cardList.clear();
	face = ECE17::FaceEnum::TEN;
	suit = ECE17::SuitEnum::CLUB;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::FOUR;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::QUEEN;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::SEVEN;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::TWO;
	cardList.push_back(ECE17::Card(face, suit));
	ECE17::Hand flush(cardList);
	anOutput << "Flush test: ";
	if (flush.determineRank() != ECE17::HandTypes::flush) {
		anOutput << "Failed!" << std::endl;
		return false;
	}
	else {
		anOutput << "Passed!" << std::endl;
	}

	// Test for straight: 
	cardList.clear();
	face = ECE17::FaceEnum::SEVEN;
	suit = ECE17::SuitEnum::CLUB;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::EIGHT;
	suit = ECE17::SuitEnum::HEART;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::NINE;
	suit = ECE17::SuitEnum::DIAMOND;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::TEN;
	suit = ECE17::SuitEnum::HEART;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::JACK;
	suit = ECE17::SuitEnum::SPADE;
	cardList.push_back(ECE17::Card(face, suit));
	ECE17::Hand straight(cardList);
	anOutput << "Straight test: ";
	if (straight.determineRank() != ECE17::HandTypes::straight) {
		anOutput << "Failed!" << std::endl;
		return false;
	}
	else {
		anOutput << "Passed!" << std::endl;
	}

	// Test for three of a kind: 
	cardList.clear();
	face = ECE17::FaceEnum::QUEEN;
	suit = ECE17::SuitEnum::CLUB;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::QUEEN;
	suit = ECE17::SuitEnum::HEART;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::QUEEN;
	suit = ECE17::SuitEnum::DIAMOND;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::FIVE;
	suit = ECE17::SuitEnum::SPADE;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::ACE;
	suit = ECE17::SuitEnum::DIAMOND;
	cardList.push_back(ECE17::Card(face, suit));
	ECE17::Hand three_kind(cardList);
	anOutput << "Three of a kind test: ";
	if (three_kind.determineRank() != ECE17::HandTypes::three_kind) {
		anOutput << "Failed!" << std::endl;
		return false;
	}
	else {
		anOutput << "Passed!" << std::endl;
	}

	// Test for two pairs of a kind: 
	cardList.clear();
	face = ECE17::FaceEnum::THREE;
	suit = ECE17::SuitEnum::HEART;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::THREE;
	suit = ECE17::SuitEnum::DIAMOND;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::SIX;
	suit = ECE17::SuitEnum::CLUB;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::SIX;
	suit = ECE17::SuitEnum::HEART;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::KING;
	suit = ECE17::SuitEnum::SPADE;
	cardList.push_back(ECE17::Card(face, suit));
	ECE17::Hand two_pair(cardList);
	anOutput << "Three of a kind test: ";
	if (two_pair.determineRank() != ECE17::HandTypes::two_pair) {
		anOutput << "Failed!" << std::endl;
		return false;
	}
	else {
		anOutput << "Passed!" << std::endl;
	}

	// Test for one pair of a kind: 
	cardList.clear();
	face = ECE17::FaceEnum::FIVE;
	suit = ECE17::SuitEnum::HEART;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::FIVE;
	suit = ECE17::SuitEnum::SPADE;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::TWO;
	suit = ECE17::SuitEnum::CLUB;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::JACK;
	suit = ECE17::SuitEnum::CLUB;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::ACE;
	suit = ECE17::SuitEnum::DIAMOND;
	cardList.push_back(ECE17::Card(face, suit));
	ECE17::Hand one_pair(cardList);
	anOutput << "One pair test: ";
	if (one_pair.determineRank() != ECE17::HandTypes::one_pair) {
		anOutput << "Failed!" << std::endl;
		return false;
	}
	else {
		anOutput << "Passed!" << std::endl;
	}

	// Test for high card: 
	cardList.clear();
	face = ECE17::FaceEnum::TWO;
	suit = ECE17::SuitEnum::DIAMOND;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::FIVE;
	suit = ECE17::SuitEnum::SPADE;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::SIX;
	suit = ECE17::SuitEnum::SPADE;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::JACK;
	suit = ECE17::SuitEnum::HEART;
	cardList.push_back(ECE17::Card(face, suit));
	face = ECE17::FaceEnum::ACE;
	suit = ECE17::SuitEnum::CLUB;
	cardList.push_back(ECE17::Card(face, suit));
	ECE17::Hand high_card(cardList);
	anOutput << "High card test: ";
	if (high_card.determineRank() != ECE17::HandTypes::high_card) {
		anOutput << "Failed!" << std::endl;
		return false;
	}
	else {
		anOutput << "Passed!" << std::endl;
	}

	return true;
}


#endif /* Testing_h */
