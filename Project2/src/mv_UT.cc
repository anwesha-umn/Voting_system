/*
 * File: mv_UT.cc
 * Description: Unit tests for MV (Majority Vote) election logic including:
 *              - Basic election execution
 *              - Winner determination
 *              - Tie handling scenarios
 * Author: Zoe Sepersky
 */

#include "MVlogic.h"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Test fixture for MV election tests
class MVTests : public ::testing::Test {
protected:
  std::vector<Ballot *> ballots;
  std::vector<Ballot *> tieBallots;
  std::vector<Candidate *> candidates;
  std::vector<Candidate *> winners;
  std::vector<Candidate *> tieWinners1;
  std::vector<Candidate *> tieWinners2;
  void setUp() {
    // Create normal ballots 
    Ballot *ballot1 = new Ballot({1, 0, 0}, 1);
    Ballot *ballot2 = new Ballot({1, 1, 0}, 2);
    Ballot *ballot3 = new Ballot({1, 1, 1}, 3);
    ballots = {ballot1, ballot2, ballot3};

    // Create tie ballots 
    Ballot *tballot1 = new Ballot({1, 1, 0}, 1);
    Ballot *tballot2 = new Ballot({1, 1, 0}, 2);
    Ballot *tballot3 = new Ballot({1, 1, 1}, 3);
    tieBallots = {tballot1, tballot2, tballot3};
    
    // Create test candidates
    Candidate *cand1 = new Candidate("A");
    Candidate *cand2 = new Candidate("B");
    Candidate *cand3 = new Candidate("C");
    candidates = {cand1, cand2, cand3};

    // Set expected winner configurations
    tieWinners1 = {cand1};
    tieWinners2 = {cand2};
    winners = {cand1};
  }
  // Clean up allocated memory after each test
  void tearDown() {
    for (int i = 0; i < candidates.size(); i++) {
      delete candidates[i];
    }
    for (int i = 0; i < ballots.size(); i++) {
      delete ballots[i];
    }
    for (int i = 0; i < tieBallots.size(); i++) {
      delete tieBallots[i];
    }
  }
};

// Verifies the election runs without throwing exceptions
TEST_F(MVTests, runElectionTest) {
  MV test(ballots, candidates, 1);
  EXPECT_NO_THROW(test.runElection(ballots, candidates, 1))
      << "Run election throws an exception";
}

// Tests winner determination
TEST_F(MVTests, getWinnersTest) {
  MV test(ballots, candidates, 1);
  test.runElection(ballots, candidates, 1);
  EXPECT_EQ(test.getWinners(), winners) << "Get winners not set up correctly";
}

// Tests tie-breaking logic
TEST_F(MVTests, tieTests) {
  MV test(tieBallots, candidates, 1);
  test.runElection(tieBallots, candidates, 1);
  // winners.push_back(cand2);
  std::vector<Candidate *> testWinners = test.getWinners();
  EXPECT_TRUE(testWinners == tieWinners1 || testWinners == tieWinners2);
}
