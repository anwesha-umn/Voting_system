/*
 * File: pluralityballot.h
 * Description: Defines the PluralityBallot class, representing a plurality-style ballot in a voting system.
 *              Each ballot contains a list of votes, a unique ballot ID, and a preference.
 * Author: Anwesha Samaddar
 */
#ifndef PLURALITYBALLOT_H
#define PLURALITYBALLOT_H

#include "ballot.h"

/**
 * @class PluralityBallot
 * @brief represents a plurality ballot in an election
 */

class PluralityBallot : public Ballot {
private:
    int preference; // Single candidate preference

public:
    // Constructor
    /**
     * @brief Constructor for PluralityBallot 
     */
    PluralityBallot(std::vector<int> votes, int ballotID);

    // Getter
    /** 
     * @brief gets a voter's preference 
     * @return int representing the index of their preference on the ballot 
     */
    int getPreference() const;
};

#endif // PLURALITYBALLOT_H