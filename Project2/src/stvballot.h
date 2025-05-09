/*
 * File: stvballot.h
 * Description: Defines the STVBallot class, representing a stv-style ballot in a voting system.
 *              Each ballot contains a list of votes, a unique ballot ID, and a preference.
 * Author: Anwesha Samaddar, Hilton Nguyen
 */

#ifndef STVBALLOT_H
#define STVBALLOT_H

#include "ballot.h"

/**
 * @class STVBallot
 * @brief represents a STV ballot in an election
 */

class STVBallot : public Ballot {
private:
    int preference;

public:
    /**
     * @brief STVBallot constructor
     */
    STVBallot(std::vector<int> votes, int ballotID);
    /** 
     * @brief gets preference of voter 
     * @return int index of #1 ranking on ballot 
     */
    int getPreference() const;
    /**
     * @brief removes current top choice and updates preference
     */
    void removeTopChoice();
    /**
     * @brief updates current top preference based on vote rankings 
     */
    void updatePreference();
};

#endif // STVBALLOT_H
