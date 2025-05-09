/*
 * File: candidate.h
 * Description: Defines the Candidate class, which represents a candidate in a voting system.
 *              Each candidate has a name, ID, number of votes, and boolean representing their win/lose status.
 * Author: Anwesha Samaddar
 */
#pragma once

#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>

/**
 * @class Candidate
 * @brief represents a single candidate in an election.
 */

class Candidate {
private:
    std::string name;       // Name of the candidate
    int candidateID;        // Unique ID for the candidate (index in vector)
    int votes;              // Number of votes the candidate has received
    bool winner;            // Indicates if the candidate is a winner
    bool loser;             // Indicates if the candidate is a loser

public:
    // Constructor with default values
    /**
     * @brief Constructor for Candidate
     */
    Candidate(std::string name = "", int candidateID = 0, int votes = 0, bool winner = false, bool loser = false);

    // Getters
    /**
     * @brief returns the name of a candidate
     * @return name as a string
     */
    std::string getName() const;       // Returns the candidate's name
    /**
     * @brief returns a candidate's ID
     * @return ID as an int
     */
    int getCandidateID() const;        // Returns the candidate's ID
    /**
     * @brief returns the number of votes a candidate recieved
     * @return numVotes as an int
     */
    int getNumVotes() const;           // Returns the number of votes
    /**
     * @brief reports whether candidate is a winner or loser 
     * @return bool true if candidate is a winner
     */
    bool isWinner() const;             // Returns true if the candidate is a winner
    /**
     * @brief reports whether candidate is a winner or loser 
     * @return bool true if candidate is a loser
     */
    bool isLoser() const;              // Returns true if the candidate is a loser

    // Setters
    /**
     * @brief updates a candidate's vote count
     * @param increment to add to vote count 
     */
    void updateVotes(int increment);   // Updates the candidate's vote count
    /**
     * @brief sets a candidate as a winner
     * @param isWinner has a value of true 
     */
    void setWinner(bool isWinner);     // Sets the candidate's winner status
    /**
     * @brief sets a candidate as a loser
     * @param isLoser has a value of true 
     */
    void setLoser(bool isLoser);       // Sets the candidate's loser status
};

#endif // CANDIDATE_H