/*
 * File: mvballot.h
 * Description: Defines the MVBallot class for Municipal Voting ballots.
 *              Each ballot contains multiple votes (1 to N candidates).
 * Author: Hilton Nguyen, Anwesha Samaddar, Zoe Sepersky
 */
#ifndef MVBALLOT_H
#define MVBALLOT_H

#include "ballot.h"

/**
 * @class MVBallot
 * @brief Represents an MV Ballot
 */
class MVBallot : public Ballot {
private:
  std::vector<int> preferences; // Multiple candidate preferences

public:
  /**
   * @brief Constructor for MVBallots
   * @param votes Votes for this ballot
   * @param ballotID The unique ballotID number
   */
  MVBallot(std::vector<int> votes, int ballotID);
  /**
   * @brief Getter for preferences of a Ballot
   * @return A integer vector of preferences
   */
  std::vector<int> getPreferences() const;

  /**
   * @brief Helper function to check if a ballot is valid
   * @return True if valid, false if not
   */
  bool isValid() const;
};

#endif // MVBALLOT_H