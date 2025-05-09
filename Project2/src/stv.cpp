/*
 * File: stv.cpp
 * Description: STV election system implementation 
 * Implements ballot shuffle, droop quota calculation, handles vote transfers, candidate elimination, votes redistribution and seat allocation.
 * Authors: Anwesha Samaddar
 */

 #include "stv.h"
 #include <algorithm>
 #include <climits>
 #include "Election.h"
 #include <iostream>
 #include <random>
 #include <unordered_map>
 #include <numeric>
 #include <iostream>

 
using namespace std;

 // Constructor - initializes election parameters
 STV::STV(std::vector<Ballot*> ballots, std::vector<Candidate*> candidates, int seats)
     : Election(ballots, candidates, seats),
       eliminated(candidates.size(), false),
       candidateBallots(candidates.size()) {}
 

// Enable or disable ballot shuffling for randomizing tiebreaks
void STV::setShuffle(bool shuffle) {
    this->shuffle = shuffle;
    if (shuffle) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(ballots.begin(), ballots.end(), g);
    }
    // Debug statements
    
    // std::cout << "shuffle set to: "<<shuffle;
    // std::cout << "Shuffle is " << (shuffle ? "ENABLED" : "DISABLED") << std::endl;
    // std::cout << shuffle;
    
}

// Calculate the Droop quota - minimum votes needed to win a seat
 int STV::calculateDroop() const {
     return (ballots.size() / (getNumSeats() + 1)) + 1;
 }


// Redistribute surplus votes from an elected candidate
 void STV::redistributeSurplus(Candidate* winner, int droop) {
    // Calculate surplus votes
     int surplus = winner->getNumVotes() - droop;
     int count = 0;
 
     for (auto& ballot : candidateBallots[winner->getCandidateID()]) {
        // Stop after redistributing surplus
         if (count >= surplus) break; 
 
         STVBallot* stvBallot = static_cast<STVBallot*>(ballot);

        // Move to next preference
         stvBallot->removeTopChoice();
         int newPref = stvBallot->getPreference();
        
        // Add to new candidate if preference is valid 
         if (newPref != -1 && !eliminated[newPref]) {
             candidateBallots[newPref].push_back(ballot);
             candidates[newPref]->updateVotes(1);
             
             // Check for immediate election
             if (candidates[newPref]->getNumVotes() >= droop) {
                 winners.push_back(candidates[newPref]);
                 eliminated[newPref] = true;
                 candidateBallots[newPref].clear();
             }
         }
         count++;
     }
 }



// Find the candidate with the fewest votes (for elimination)
 Candidate* STV::findLowestCandidate() const {
     int minVotes = INT_MAX;
     Candidate* lowest = nullptr;
     int lastReceipt = -1;
     std::unordered_map<int, int> receiptTracker;
 
     // Initialize receipt tracking
     for (auto* c : candidates) {
         receiptTracker[c->getCandidateID()] = -1;
     }
 
     // Find lowest candidate with tiebreaker  
     for (auto* c : candidates) {
         if (!eliminated[c->getCandidateID()] && !c->isWinner()) {
            // Tiebreaker: earlier ballot receipt wins
             if (c->getNumVotes() < minVotes || 
                (c->getNumVotes() == minVotes && 
                 receiptTracker[c->getCandidateID()] > lastReceipt)) {
                 minVotes = c->getNumVotes();
                 lowest = c;
                 lastReceipt = receiptTracker[c->getCandidateID()];
             }
         }
     }
     return lowest;
 }
 

 // Redistribute votes from an eliminated candidate
 void STV::redistributeEliminated(Candidate* eliminatedCandidate) {
     size_t elimID = eliminatedCandidate->getCandidateID();
    
     // Process all ballots for eliminated candidate
     for (auto& ballot : candidateBallots[elimID]) {
         // Process all ballots
         STVBallot* stvBallot = static_cast<STVBallot*>(ballot);
         stvBallot->removeTopChoice();

         // Add to new candidate if preference is valid
         int newPref = stvBallot->getPreference();
 
         if (newPref != -1 && !eliminated[newPref]) {
             candidateBallots[newPref].push_back(ballot);
             candidates[newPref]->updateVotes(1);
         }
     }
     // Clear ballots for eliminated candidate
     candidateBallots[elimID].clear();
 }
 
 
 // Main STV election algorithm
 void STV::runElection(std::vector<Candidate*>& winners, std::vector<Candidate*>& losers) {

     // Calculate winning threshold - droop quota
     const int droop = calculateDroop();
     int ballotOrder = 0;
     std::unordered_map<int, int> firstReceiptOrder;
 
     // Initialize receipt tracking
     for (auto* c : candidates) {
         firstReceiptOrder[c->getCandidateID()] = -1;
     }
 
     // Shuffle ballots if enabled - Randomize ballot order if enabled (for fair tiebreaking)
     if (shuffle) {
         std::random_device rd;
         std::mt19937 g(rd());
         std::shuffle(ballots.begin(), ballots.end(), g);
     }

    //Print ballots AFTER shuffling - for testing
    
     std::cout << "After Shuffle:" << std::endl;
     for (auto* b : ballots) {
        std::cout << "Ballot ID: " << b->getID() << " -> ";
        for (int vote : b->getVotes()) std::cout << vote << " ";
        std::cout << std::endl;
     }
 
     // Initial ballot distribution to first preferences
     for (auto& ballot : ballots) {
         STVBallot* stvBallot = static_cast<STVBallot*>(ballot);
         int pref = stvBallot->getPreference();
         
         if (pref != -1 && !eliminated[pref]) {
             candidateBallots[pref].push_back(ballot);
             candidates[pref]->updateVotes(1);
             
             // Track first ballot receipt
             if (firstReceiptOrder[pref] == -1) {
                 firstReceiptOrder[pref] = ballotOrder++;
             }
             
             // Immediate election check
             if (candidates[pref]->getNumVotes() >= droop) {
                 winners.push_back(candidates[pref]);
                 eliminated[pref] = true;
                 //candidateBallots[pref].clear();
             }
         }
     }
    
 
     // Main election loop - continues until all seats filled
     while (winners.size() < static_cast<size_t>(getNumSeats())) {
         // Check if remaining candidates <= remaining seats
         int remaining = 0;
         for (size_t i = 0; i < candidates.size(); ++i) {
             if (!eliminated[i] && !candidates[i]->isWinner()) remaining++;
         }
         int remainingSeats = getNumSeats() - winners.size();
 
         // If candidates <= seats, elect all remaining
         if (remaining <= remainingSeats) {
             for (size_t i = 0; i < candidates.size(); ++i) {
                 if (!eliminated[i] && !candidates[i]->isWinner()) {
                     winners.push_back(candidates[i]);  // Elect remaining candidates
                     candidates[i]->setWinner(true);
                     //added
                     eliminated[i] = true;  // Mark as elected

                     // Ensure they're not in losers list
                     losers.erase(std::remove(losers.begin(), losers.end(), candidates[i]), losers.end()); ///added
                 }
             }
             break;
         }
 
         // Try to elect candidates who reached quota
         bool elected = false;
         for (size_t i = 0; i < candidates.size(); ++i) {
             if (!eliminated[i] && candidates[i]->getNumVotes() >= droop) {
                 winners.push_back(candidates[i]);
                 eliminated[i] = true;
                 elected = true;
                 redistributeSurplus(candidates[i], droop);
             }
         }
         
         // Stop if all seats filled
         if (winners.size() >= static_cast<size_t>(getNumSeats())) break;
 
         // If no one is elected, eliminate lowest candidate 
         
         if (!elected) {
             Candidate* lowest = findLowestCandidate();
             if (!lowest) break; // Shouldn't happen if seats < candidates
             
             size_t index = lowest->getCandidateID();
             eliminated[index] = true;
            //  losers.insert(losers.begin(), lowest);
            //  redistributeEliminated(lowest);
             losers.push_back(lowest);  // Changed from insert to push_back
             redistributeEliminated(lowest);
         }

     }
    // Debug statements
    /*
    std::cout << "\nDEBUG - Final Classification:\n";
    std::cout << "Winners (" << winners.size() << "):\n";
    for (auto* w : winners) {
        std::cout << "  " << w->getName() << ": " << w->getNumVotes() << " votes\n";
    }
    std::cout << "Losers (" << losers.size() << "):\n";
    for (auto* l : losers) {
        std::cout << "  " << l->getName() << ": " << l->getNumVotes() << " votes\n";
    }
    */ 
}
