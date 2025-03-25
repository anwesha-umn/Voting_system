//
// Created by ASUS on 3/15/2025.
//
#pragma once

#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>

class Candidate {
private:
    std::string name;       // Name of the candidate
    int candidateID;        // Unique ID for the candidate (index in vector)
    int votes;              // Number of votes the candidate has received
    bool winner;            // Indicates if the candidate is a winner
    bool loser;             // Indicates if the candidate is a loser

public:
    // Constructor with default values
    Candidate(std::string name = "", int candidateID = 0, int votes = 0, bool winner = false, bool loser = false);

    // Getters
    std::string getName() const;       // Returns the candidate's name
    int getCandidateID() const;        // Returns the candidate's ID
    int getNumVotes() const;           // Returns the number of votes
    bool isWinner() const;             // Returns true if the candidate is a winner
    bool isLoser() const;              // Returns true if the candidate is a loser

    // Setters
    void updateVotes(int increment);   // Updates the candidate's vote count
    void setWinner(bool isWinner);     // Sets the candidate's winner status
    void setLoser(bool isLoser);       // Sets the candidate's loser status

};

#endif // CANDIDATE_H