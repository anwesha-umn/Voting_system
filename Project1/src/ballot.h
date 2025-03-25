/*
 * File: ballot.h
 * Description: Defines the Ballot class, which represents a ballot in a voting system.
 *              Each ballot contains a list of votes and a unique ballot ID.
 * Author: Anwesha Samaddar
 */

#ifndef BALLOT_H
#define BALLOT_H

#include <vector>

/**
 * @class Ballot
 * @brief Parent class to pluralityballot and stvballot classes.
 */

class Ballot {
protected:
  std::vector<int> votes; // Vector of ranked candidate preferences
  int ballotID;           // Unique ID for the ballot

public:
  // Constructor
  /**
   * @brief Constructor for Ballot
   */
  Ballot(std::vector<int> votes, int ballotID);

  // Getters
  /**
   * @brief returns the votes of a ballot
   * @return votes as a vector of integers
   */
  std::vector<int> getVotes() const;
  /**
   * @brief returns the ID number of a specific ballot
   * @return Ballot ID as an integer
   */
  int getID() const;
};

#endif // BALLOT_H
