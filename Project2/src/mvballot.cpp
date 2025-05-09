/*
 * File: mvballot.cpp
 * Description: Implements the MVBallot class for Municipal Voting ballots.
 * Author: Anwesha Samaddar, Hilton Nguyen
 */
 #include "mvballot.h"
 #include <stdexcept>
 #include <algorithm>

 
 // Constructor for MV Ballot
 MVBallot::MVBallot(std::vector<int> votes, int ballotID) : Ballot(votes, ballotID) {
    // Throws invalid_argument if ballot contains invalid votes
     if (!isValid()) {
         throw std::invalid_argument("\nInvalid MV ballot "+std::to_string(ballotID)+": Must have only 1's or 0's.");
     }
     
     // Store indices of voted candidates (where vote = 1)
     for (int i = 0; i < votes.size(); i++) {
         if (votes[i] == 1) {
             preferences.push_back(i);    // Store candidate indices that got votes
         }
     }
 }
 
 // Gets the preferred candidate indices
 std::vector<int> MVBallot::getPreferences() const {
     return preferences;
 }
 
 // Validates the ballot format
 // returns true if ballot only contains 0s and 1s, false otherwise
 bool MVBallot::isValid() const {
    // Check each vote is either 0 (no) or 1 (yes)
    for (int v : votes) {
        if (v != 0 && v != 1) {  
            return false;
        }
    }
    return true;  
}