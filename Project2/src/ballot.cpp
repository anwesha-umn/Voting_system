/*
 * File: ballot.cpp
 * Description: Implements the Ballot class, representing a ballot in a voting system.
 *              Each ballot contains a list of votes and a unique ballot ID.
 * Author: Anwesha Samaddar
 */


#include "ballot.h"

// Constructor
Ballot::Ballot(std::vector<int> votes, int ballotID)
    : votes(votes), ballotID(ballotID) {}

// Getter implementations
std::vector<int> Ballot::getVotes() const {
    return votes;
}

// return Ballot IDs
int Ballot::getID() const {
    return ballotID;
}
