/*
 * File: stv.h
 * Description: Defines the STV (Single Transferable Vote) election system
 * Authors: Anwesha Samaddar
 */
#ifndef STV_H
#define STV_H

#include "Election.h"
#include "stvballot.h"
#include <unordered_map>
#include <vector>
/*
 * IMPORTANT NOTE: Uncomment FRIEND_TESTs and the include below to run stv_UT,
 * otherwise keep FRIEND_TESTs commented for the makefile to run in case any issues arise.
 * These are the 3 FRIEND_TESTs used to test private functions: 
 * 1. FRIEND_TEST(STVTests, DroopQuotaTest)
 * 2. FRIEND_TEST(STVTests, redistributeSurplusTest)
 * 3. FRIEND_TEST(STVTests, FindLowestCandidateTest);
 */

// #include <gtest/gtest_prod.h>  // IMPORTANT: Needed for FRIEND_TEST - Uncomment this for unit testing with stv_UT.cc

/**
 * @class STV
 * @brief STV election algorithm inheriting from election
 */

// STV election class (inherits from Election)
class STV : public Election {
private:
  bool shuffle;                 // Enable/disable ballot shuffling
  std::vector<bool> eliminated; // Track eliminated candidates
  std::vector<std::vector<Ballot *>> candidateBallots; // Ballots per candidate
  std::unordered_map<int, int>
      candidateFirstReceiptOrder; // Track first ballot receipt

public:
  // Constructor
  /**
   * @brief STV constructor
   */
  STV(std::vector<Ballot *> ballots, std::vector<Candidate *> candidates,
      int seats); //, bool shuffle);

  // Set shuffle option
  /**
   * @brief toggles shuffle on and off
   * @param shuffle boolean dictating whether shuffle is on or off
   */
  void setShuffle(bool shuffle);

  // Main election runner
  /**
   * @brief runs the election
   * @param winners is a list of winners
   * @param losers is a list of losers
   */
  void runElection(std::vector<Candidate *> &winners,
                   std::vector<Candidate *> &losers);

private:
  // Calculate winning threshold (Droop quota)
  /**
   * @brief calculates droop quota
   * @return int droop quota
   */
  int calculateDroop() const;
  // IMPORTANT: Uncomment for testing
  //FRIEND_TEST(STVTests, DroopQuotaTest); // adding to run unit test

  // Redistribute excess votes from winning candidate
  /**
   * @brief redistributes excess votes from winning candidate
   * @param winner is the winning candidate
   * @param droop is the droop quota
   */
  void redistributeSurplus(Candidate *winner, int droop);

  // IMPORTANT: Uncomment for testing
  //FRIEND_TEST(STVTests, redistributeSurplusTest);

  // Find candidate with fewest votes
  /**
   * @brief finds candidate with fewest votes
   * @return candidate object with fewest votes
   */
  Candidate *findLowestCandidate() const;
  // IMPORTANT: Uncomment for testing
  //FRIEND_TEST(STVTests, FindLowestCandidateTest);

  // Redistribute votes from eliminated candidate
  /**
   * @brief redistributes votes from eliminated candidate
   * @param eliminatedCandidate is eliminated candidate object
   */
  void redistributeEliminated(Candidate *eliminatedCandidate);

};

#endif // STV_H
