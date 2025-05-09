/*
 * File: pluralityballot.cpp
 * Description: Implements the PluralityBallot class, representing a plurality-style ballot in a voting system.
 *              Each ballot contains a list of votes and a unique ballot ID.
 * Author: Anwesha Samaddar
 */

#include "pluralityballot.h"

#include <stdexcept>

// Constructor - Initialize base class
PluralityBallot::PluralityBallot(std::vector<int> votes, int ballotID) : Ballot(votes, ballotID) {
    int voteCount = 0;
    preference = -1;

    // Process each vote in the ballot
    for (int i = 0; i < votes.size(); i++) {
        if (votes[i] == 1) {
            preference = i; // Store preferred candidate index
            voteCount++;
        } else if (votes[i] != 0) {
            // Throw invaid argument when values other than 0 or 1 are found in ballots
            throw std::invalid_argument("\nInvalid Plurality ballot "+std::to_string(ballotID)+": Non-zero/non-one value found.");
        }
    }

    // A valid Plurality ballot must have exactly one '1'
    // Throw invaid argument to notify user of invalid ballots
    if (voteCount != 1) {
        throw std::invalid_argument("\nInvalid Plurality ballot "+std::to_string(ballotID)+": Exactly one '1' is required.");
    }
}


// Getter implementation
int PluralityBallot::getPreference() const {
    return preference;
}