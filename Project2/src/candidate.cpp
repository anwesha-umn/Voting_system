/*
 * File: candidate.cpp
 * Description: Implements the Candidate class, representing a candidate in the
 * voting system. Each candidate has a name, ID, number of votes, and boolean
 * representing their win/lose status. 
 * Author: Anwesha Samaddar
 */

#include "candidate.h"
#include <iostream>
#include <stdexcept>

// Constructor with default values and validation
Candidate::Candidate(std::string name, int candidateID, int votes, bool winner,
                     bool loser)
    : name(name), candidateID(candidateID), votes(votes), winner(winner),
      loser(loser) {
  // Validate that candidate ID is not negative
  if (candidateID < 0) {
    throw std::invalid_argument("Candidate ID cannot be negative");
  }
  // Validate that vote count is not negative
  if (votes < 0) {
    throw std::invalid_argument("Votes cannot be negative");
  }
}

// Getter implementations
// Returns the candidate's name
std::string Candidate::getName() const { return name; }

// Returns the candidate's unique ID
int Candidate::getCandidateID() const { return candidateID; }

// Returns the number of votes the candidate has received
int Candidate::getNumVotes() const { return votes; }

// Returns whether the candidate is a winner
bool Candidate::isWinner() const { return winner; }

// Returns whether the candidate is a loser
bool Candidate::isLoser() const { return loser; }



// Setter implementations
// Updates the candidate's vote count by adding the increment value
void Candidate::updateVotes(int increment) {
  if (increment < 0) {
    throw std::out_of_range("A candidate's votes cannot be decremented");
  }
  votes += increment; // Increment the candidate's vote count
}

// Sets the candidate's winner status
void Candidate::setWinner(bool isWinner) {
  // Check if candidate is already marked as loser
  if (isLoser() && isWinner) {
    throw std::invalid_argument(
        "A candidate cannot be a loser and a winner at the same time!");
  }
  winner = isWinner; 
}

// Sets the candidate's loser status
void Candidate::setLoser(bool isLoser) {
  // Check if candidate is already marked as winner
  if (isWinner() && isLoser) {
    throw std::invalid_argument(
        "A candidate cannot be a loser and a winner at the same time!");
  }
  loser = isLoser; 
}
