/*
 * File: mv.h
 * Description: Defines the MV (Municipal Voting) election algorithm.
 * Author: Hilton Nguyen, Anwesha Samaddar
 */
#ifndef MV_H
#define MV_H

#include "Election.h"
/**
 * @class MV
 * @brief Implements Municipal Voting algorithm logic
 */
class MV : public Election {
public:
  /**
   * @brief Constructor for MV election
   * @param ballots vector of ballots to be tallied
   * @param candidates vector of candidates up for election
   * @param seats number of seats up for election
   */
  MV(std::vector<Ballot *> ballots, std::vector<Candidate *> candidates,
     int seats);

  /**
   * @brief Runs the MV election algorithm
   * @param ballots vector of ballots to be tallied
   * @param candidates vector of candidates up for election
   * @param seats number of seats up for election
   */
  void runElection(std::vector<Ballot *> ballots,
                   std::vector<Candidate *> candidates, int seats);
};

#endif // MV_H