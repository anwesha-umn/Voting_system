/*
 * File: plurality.cpp
 * Description: Implements the Plurality voting algorithm.
 * Authors: Annabelle & Anwesha
 */


#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "plurality.h"
#include "Election.h"
#include <vector>

//Constructor
Plurality::Plurality(std::vector<Ballot*> ballots, std::vector<Candidate*> candidates, int seats)
    : Election(ballots, candidates, seats) {}

void Plurality::runElection(std::vector<Ballot*> ballots, std::vector<Candidate*> candidates, int seats) {
    // Initialize random seed
    std::srand(std::time(0));

    // Initialize vote counts for each candidate
    std::vector<int> voteCounts(candidates.size(), 0);

    // Count votes for each candidate
    for (auto ballot : ballots) {
        std::vector<int> votes = ballot->getVotes();
        for (size_t i = 0; i < votes.size(); i++) {
            if (votes[i] == 1) {
                voteCounts[i]++;
            }
        }
    }

    // Update each candidate's vote count
    for (size_t i = 0; i < candidates.size(); i++) {
        candidates[i]->updateVotes(voteCounts[i] - candidates[i]->getNumVotes());
    }

    // Associate candidates with their vote counts
    std::vector<std::pair<Candidate*, int>> candidateVotes;
    for (size_t i = 0; i < candidates.size(); i++) {
        candidateVotes.push_back(std::make_pair(candidates[i], voteCounts[i]));
    }

    // Sort candidates by votes in descending order
    std::sort(candidateVotes.begin(), candidateVotes.end(),
              [](const auto& a, const auto& b) { return b.second < a.second; });

    // Clear previous results
    winners.clear();
    losers.clear();

    // Handle case where we have more seats than candidates
    if (seats >= static_cast<int>(candidateVotes.size())) {
        for (auto& cv : candidateVotes) {
            addWinner(cv.first);
        }
        return;
    }

    // Add winners that are clearly above the tie threshold
    int lastWinnerIndex = seats - 1;
    for (int i = 0; i < lastWinnerIndex; i++) {
        if (candidateVotes[i].second > candidateVotes[i+1].second) {
            addWinner(candidateVotes[i].first);
        } else {
            // We've reached a tie situation
            lastWinnerIndex = i;
            break;
        }
    }

    // Find all candidates tied at the cutoff point
    std::vector<Candidate*> tiedCandidates;
    int cutoffVotes = candidateVotes[lastWinnerIndex].second;

    // First add any candidates we might have missed in the initial pass
    for (int i = winners.size(); i <= lastWinnerIndex; i++) {
        tiedCandidates.push_back(candidateVotes[i].first);
    }

    // Then add any other candidates with the same vote count
    for (size_t i = lastWinnerIndex + 1; i < candidateVotes.size(); i++) {
        if (candidateVotes[i].second == cutoffVotes) {
            tiedCandidates.push_back(candidateVotes[i].first);
        } else {
            break;  
        }
    }

    // Determine how many seats remain to be filled
    int remainingSeats = seats - winners.size();

    // If we have more tied candidates than remaining seats, randomly select winners
    if (remainingSeats > 0 && !tiedCandidates.empty()) {
        // Randomly shuffle the tied candidates
        std::random_shuffle(tiedCandidates.begin(), tiedCandidates.end());

        // Add the needed number of winners from the shuffled list
        for (int i = 0; i < remainingSeats && i < static_cast<int>(tiedCandidates.size()); i++) {
            addWinner(tiedCandidates[i]);
        }

        // Add the remaining tied candidates as losers
        for (size_t i = remainingSeats; i < tiedCandidates.size(); i++) {
            addLoser(tiedCandidates[i]);
        }
    }

    // Add remaining candidates as losers (those not already processed)
    for (auto& cv : candidateVotes) {
        if (cv.second < cutoffVotes &&
            std::find(winners.begin(), winners.end(), cv.first) == winners.end() &&
            std::find(losers.begin(), losers.end(), cv.first) == losers.end()) {
            addLoser(cv.first);
        }
    }
}
