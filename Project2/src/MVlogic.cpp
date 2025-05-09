/*
 * File: MVlogic.cpp
 * Description: Implements the MV class for Municipal Voting algorithm.
 * Author: Hilton Nguyen, Anwesha Samaddar
 */
#include "MVlogic.h"
#include "Election.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

// MV Constructor 
// Initializes base Election class with ballots, candidates and seats
MV::MV(std::vector<Ballot *> ballots, std::vector<Candidate *> candidates,
       int seats)
    : Election(ballots, candidates, seats) {}

// Runs the Municipal Voting election
void MV::runElection(std::vector<Ballot *> ballots,
                     std::vector<Candidate *> candidates, int seats) {
  // Initialize all candidate vote counts to 0 
  std::vector<int> voteCounts(candidates.size(), 0);

  // Count votes from all valid ballots
  for (auto *ballot : ballots) {
    std::vector<int> votes = ballot->getVotes();
    for (size_t i = 0; i < votes.size(); i++) {
      if (votes[i] == 1) {
        candidates[i]->updateVotes(1);
      }
    }
  }

  // Sort candidates by votes (descending)
  std::sort(candidates.begin(), candidates.end(),
            [](const Candidate *a, const Candidate *b) {
              return a->getNumVotes() > b->getNumVotes();
            });

  // Clear previous results
  winners.clear();
  losers.clear();

  // Handle case where seats >= candidates
  if (seats >= static_cast<int>(candidates.size())) {
    for (auto *c : candidates) {
      winners.push_back(c);
    }
    return;
  }

  // Determine winners and handle ties
  int cutoffVotes = candidates[seats - 1]->getNumVotes();
  std::vector<Candidate *> potentialWinners;
  std::vector<Candidate *> definiteLosers;

  // Collect candidates at the cutoff
  for (size_t i = 0; i < candidates.size(); i++) {
    if (candidates[i]->getNumVotes() > cutoffVotes) {
      winners.push_back(candidates[i]);
    } else if (candidates[i]->getNumVotes() == cutoffVotes) {
      potentialWinners.push_back(candidates[i]);  // Tie candidates
    } else {
      losers.push_back(candidates[i]);
    }
  }

  // Resolve ties if needed
  int remainingSeats = seats - winners.size();
  if (remainingSeats > 0 && !potentialWinners.empty()) {
    // Random shuffle for tie-breaking
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(potentialWinners.begin(), potentialWinners.end(), g);

    // Add winners from the shuffled list
    for (int i = 0; i < remainingSeats; i++) {
      winners.push_back(potentialWinners[i]);
    }
    // Remaining become losers
    for (size_t i = remainingSeats; i < potentialWinners.size(); i++) {
      losers.push_back(potentialWinners[i]);
    }
  }
}
