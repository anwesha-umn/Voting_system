/*
 * File: stvballot.cpp
 * Description: Implements the STVBallot class, representing a STV-style ballot
 * in a voting system. Each ballot contains a list of votes, a unique ballot ID,
 * and a preference.
 * Author: Anwesha Samaddar, Hilton Nguyen
 */

#include "stvballot.h"
#include <climits>
#include <stdexcept>
#include <vector>

// Constructor - validates ballot and sets initial preference
STVBallot::STVBallot(std::vector<int> votes, int id)
    : Ballot(votes, id), preference(-1) {
    // Count of ranked candidates
    int rankedCount = 0;

    // Count non-zero votes
    for (int vote : votes) {
        if (vote > 0) rankedCount++;
    }
    
    // Require at least half (floor division) of candidates to be ranked
    int minRequired = votes.size() / 2;

    // Validate minimum ranking requirement
    if (rankedCount < minRequired) {
        throw std::invalid_argument(
            "\nInvalid STV Ballot "+std::to_string(id)+": Only " + std::to_string(rankedCount) + 
            " candidates ranked (minimum " + std::to_string(minRequired) + " required)."
        );
    }
    // Set initial top preference
    updatePreference();
}



// Removes current top choice and updates preference
void STVBallot::removeTopChoice() {
    if (preference != -1) {
        votes[preference] = 0;
        updatePreference();
    }
}

// Updates current top preference based on vote rankings
void STVBallot::updatePreference() {
    // Reset to invalid vote
    preference = -1;  

    // Initialize to maximum possible rank
    int minRank = INT_MAX;   

    for (std::size_t i = 0; i < votes.size(); ++i) {
        // Find candidate with lowest rank
        if (votes[i] > 0 && votes[i] < minRank) {
            minRank = votes[i];
            // Update preferred candidate index
            preference = i;     
        }
    }
}

// Returns current top preference candidate index
int STVBallot::getPreference() const {
    return preference;
}