//
// Created by ASUS on 3/15/2025.
//

#include "candidate.h"
#include <stdexcept>
#include <iostream>

// Constructor with default values and validation
Candidate::Candidate(std::string name, int candidateID, int votes, bool winner, bool loser)
    : name(name), candidateID(candidateID), votes(votes), winner(winner), loser(loser) {
    if (candidateID < 0) {
        throw std::invalid_argument("Candidate ID cannot be negative");
    }
    if (votes < 0) {
        throw std::invalid_argument("Votes cannot be negative");
    }
}

// Getter implementations
std::string Candidate::getName() const {
    return name;
}

int Candidate::getCandidateID() const {
    return candidateID;
}

int Candidate::getNumVotes() const {
    return votes;
}

bool Candidate::isWinner() const {
    return winner;
}

bool Candidate::isLoser() const {
    return loser;
}

// Setter implementations
void Candidate::updateVotes(int increment) {
    votes += increment; // Increment the candidate's vote count
}

void Candidate::setWinner(bool isWinner) {
    winner = isWinner; // Set the candidate's winner status
}

void Candidate::setLoser(bool isLoser) {
    loser = isLoser; // Set the candidate's loser status
}


