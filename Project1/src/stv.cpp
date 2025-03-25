#include "stv.h"
#include <algorithm>
#include <climits>
#include "Election.h"
#include <iostream>

STV::STV(std::vector<Ballot*> ballots,
         std::vector<Candidate*> candidates,
         int seats)
    : ballots(ballots), candidates(candidates), seats(seats),
      shuffle(false), eliminated(candidates.size(), false),
      candidateBallots(candidates.size()) {}

void STV::setShuffle(bool shuffle) {
    this->shuffle = shuffle;
}

int STV::calculateDroop() const {
    return (ballots.size() / (seats + 1)) + 1;
}


void STV::redistributeSurplus(Candidate* winner, int droop) {
    int surplus = winner->getNumVotes() - droop;
    int count = 0;

    for (auto& ballot : candidateBallots[winner->getCandidateID()]) {
        if (count >= surplus) break;

        STVBallot* stvBallot = static_cast<STVBallot*>(ballot);
        stvBallot->removeTopChoice();

        int newPref = stvBallot->getPreference();
        if (newPref != -1 && !eliminated[newPref]) {
            candidateBallots[newPref].push_back(ballot);
            candidates[newPref]->updateVotes(1);
        }
        count++;
    }
}

Candidate* STV::findLowestCandidate() const {
    int minVotes = INT_MAX;
    Candidate* lowest = nullptr;

    for (auto* c : candidates) {
        if (!eliminated[c->getCandidateID()] &&
            !c->isWinner() &&
            c->getNumVotes() < minVotes) {
            minVotes = c->getNumVotes();
            lowest = c;
            }
    }
    return lowest;
}

void STV::redistributeEliminated(Candidate* eliminatedCandidate) {
    size_t elimID = eliminatedCandidate->getCandidateID();

    for (auto& ballot : candidateBallots[elimID]) {
        STVBallot* stvBallot = static_cast<STVBallot*>(ballot);
        stvBallot->removeTopChoice();

        int newPref = stvBallot->getPreference();
        if (newPref != -1 && !eliminated[newPref]) {
            candidateBallots[newPref].push_back(ballot);
            candidates[newPref]->updateVotes(1);
        }
    }
    candidateBallots[elimID].clear();
}

void STV::runElection(std::vector<Candidate*>& winners, std::vector<Candidate*>& losers) {
    const int droop = calculateDroop();
    if (shuffle) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(ballots.begin(), ballots.end(), g);
    }

    // Initial ballot distribution
    for (auto& ballot : ballots) {
        STVBallot* stvBallot = static_cast<STVBallot*>(ballot);
        int pref = stvBallot->getPreference();
        if (pref != -1 && !eliminated[pref]) {
            candidateBallots[pref].push_back(ballot);
            candidates[pref]->updateVotes(1);
        }
    }

    while (winners.size() < static_cast<size_t>(seats)) {
        // Check if remaining candidates == remaining seats
        int remainingCandidates = 0;
        for (size_t i = 0; i < candidates.size(); ++i) {
            if (!eliminated[i] && !candidates[i]->isWinner()) remainingCandidates++;
        }
        int remainingSeats = seats - winners.size();
        if (remainingCandidates <= remainingSeats) {
            // Elect all remaining candidates
            for (size_t i = 0; i < candidates.size(); ++i) {
                if (!eliminated[i] && !candidates[i]->isWinner()) {
                    winners.push_back(candidates[i]);
                    candidates[i]->setWinner(true);
                }
            }
            break;
        }

        // Existing logic to elect candidates or eliminate the lowest
        bool elected = false;
        for (size_t i = 0; i < candidates.size(); ++i) {
            if (!eliminated[i] && candidates[i]->getNumVotes() >= droop) {
                winners.push_back(candidates[i]);
                eliminated[i] = true;
                elected = true;
                int surplus = candidates[i]->getNumVotes() - droop;
                if (surplus > 0) redistributeSurplus(candidates[i], droop);
            }
        }

        if (winners.size() >= static_cast<size_t>(seats)) break;

        if (!elected) {
            Candidate* lowest = findLowestCandidate();
            if (!lowest) break;
            size_t index = lowest->getCandidateID();
            eliminated[index] = true;
            losers.push_back(lowest);
            redistributeEliminated(lowest);
        }
    }
}
// Helper functions remain similar to previous implementation
// [Include the helper functions from previous STV.cpp here]