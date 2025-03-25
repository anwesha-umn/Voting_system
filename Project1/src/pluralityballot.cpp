//
// Created by ASUS on 3/16/2025.
//

#include "pluralityballot.h"

#include <stdexcept>

// Constructor
PluralityBallot::PluralityBallot(std::vector<int> votes, int ballotID) : Ballot(votes, ballotID) {
    int voteCount = 0;
    preference = -1;

    for (int i = 0; i < votes.size(); i++) {
        if (votes[i] == 1) {
            preference = i;
            voteCount++;
        } else if (votes[i] != 0) {
            throw std::invalid_argument("Invalid Plurality ballot: Non-zero/non-one value found.");
        }
    }

    // A valid Plurality ballot must have exactly one '1'
    if (voteCount != 1) {
        throw std::invalid_argument("Invalid Plurality ballot: Exactly one '1' is required.");
    }
}


// Getter implementation
int PluralityBallot::getPreference() const {
    return preference;
}