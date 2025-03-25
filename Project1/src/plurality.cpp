/*
 * File: plurality.cpp
 * Description: Implements the Plurality voting algorithm.
 * Author: Annabelle
 */

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "plurality.h"
#include "Election.h"

// Constructor
Plurality::Plurality(std::vector<Ballot*> ballots, std::vector<Candidate*> candidates, int seats)
    : Election(ballots, candidates, seats) {}

void Plurality::runElection(std::vector<Ballot*> ballots, std::vector<Candidate*> candidates, int seats) {

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

    // Add top 'seats' candidates as winners
    for (int i = 0; i < seats && i < static_cast<int>(candidateVotes.size()); i++) {
        addWinner(candidateVotes[i].first);
    }

    // Add remaining candidates as losers
    for (size_t i = seats; i < candidateVotes.size(); i++) {
        addLoser(candidateVotes[i].first);
    }

}