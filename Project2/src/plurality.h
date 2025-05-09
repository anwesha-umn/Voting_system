/*
 * File: plurality.h
 * Description: Defines the Plurality voting algorithm.
 * Author: Annabelle
 */

#ifndef PLURALITY_H
#define PLURALITY_H

#include "Election.h"

/**
 * @class Plurality
 * @brief Represents a plurality election algorithm, inherits from Election
 */

class Plurality : public Election {
public:
    /**
     * @brief Constructor for Plurality
     */
    // Constructor
    Plurality(std::vector<Ballot*> ballots, std::vector<Candidate*> candidates, int seats);

    /**
     * @brief runs the plurality election algorithm
     * @param ballots is a vector of all ballots
     * @param candidates is a vector of all candidates
     * @param seats is the number of seats up for election
     */
    void runElection(std::vector<Ballot*> ballots, std::vector<Candidate*> candidates, int seats);

};

#endif // PLURALITY_H