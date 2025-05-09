/*
* File: Election_UT.cc
* Description: Includes all unit tests for STV election logic (stv.cpp)
* Tests: Constructor tests, droop quota calculation, ballot shuffling, single candidate case, 
*        candidate elimination, vote redistribution, edge cases
* Author: Zoe Sepersky, Anwesha Samaddar
*/

#include "Election.h"
#include "ballot.h"
#include "candidate.h"
#include "stv.h"
#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <gtest/gtest_prod.h>  
#include "stvballot.h"

// Test fixture for STV election tests
class STVTests : public ::testing::Test {
protected:
  std::vector<Ballot *> ballots;
  std::vector<Ballot *> ballots_single_candidate;
  std::vector<Ballot *> tieBallots;
  std::vector<Ballot *> shuffle_ballots;
  std::vector<Candidate *> candidates;
  std::vector<Candidate *> single_candidate;
  int seats;
  std::vector<Candidate *> regularWinners;
  std::vector<Candidate *> shuffle_cand;
  std::vector<Candidate *> single_winner;
  std::vector<Candidate *> regularLosers;
  

  void SetUp() {
    // Create test ballots with different preference orders
    Ballot *ballot1 = new Ballot(
        {
            0,1,2,
        },
        1);
    Ballot *ballot2 = new Ballot(
        {
            0,1,2,
        },
        2);
    Ballot *ballot3 = new Ballot(
        {
            2,1,0,
        },
        3);
    
    // Create test candidates
    Candidate *cand1 = new Candidate("A");
    Candidate *cand2 = new Candidate("B");
    Candidate *cand3 = new Candidate("C");
    Candidate *cand4 = new Candidate("D");
    
    // Initialize test data
    ballots = {ballot1, ballot2, ballot3};
    candidates = {cand1, cand2, cand3};
    shuffle_cand = {cand1, cand2, cand3, cand4};
    regularWinners = {cand2, cand3};
    single_winner = {cand4};
    regularLosers = {cand1};
    seats = 2;

  }
  void TearDown() {
    // Clean up allocated memory
    for (int i = 0; i < candidates.size(); i++) {
      delete candidates[i];
    }
    for (int i = 0; i < single_candidate.size(); i++) {
      delete single_candidate[i];
    }
    for (int i = 0; i < ballots.size(); i++) {
      delete ballots[i];
    }
    for (int i = 0; i < ballots_single_candidate.size(); i++) {
      delete ballots_single_candidate[i];
    }
  }
};


// Tests basic STV object construction and getters
TEST_F(STVTests, STVConstructorTests) {
  STV test(ballots, candidates, seats);
  EXPECT_NO_THROW(STV check(ballots, candidates, seats));
  EXPECT_EQ(test.getNumSeats(), seats);
  EXPECT_EQ(test.getBallots(), ballots);
  EXPECT_EQ(test.getCandidates(), candidates);
}


// Tests candidate name retrieval functionality 
TEST_F(STVTests, CandidateNameTest) {
  STV test(ballots, candidates, seats);
  std::vector<std::string> names = test.getNames();
  EXPECT_EQ(names.at(0), "A") << "election::getNames() not set up correctly";
  EXPECT_EQ(names.at(1), "B") << "election::getNames() not set up correctly";
  EXPECT_EQ(names.at(2), "C") << "election::getNames() not set up correctly";
}


// Tests Droop quota calculation in various cases
TEST_F(STVTests, DroopQuotaTest) {
  // 3 ballots, 2 seats -> quota should be floor(3/(2+1)) + 1 = 2
  STV test(ballots, candidates, seats);
  EXPECT_EQ(test.calculateDroop(), 2) << "Droop quota calculation incorrect";
  
  // Test with different numbers
  STV test2(ballots, candidates, 1); // 3 ballots, 1 seat -> quota = floor(3/2)+1 = 2
  EXPECT_EQ(test2.calculateDroop(), 2);
  
  STV test3({}, candidates, 1); // 0 ballots
  EXPECT_EQ(test3.calculateDroop(), 1);
}


// Tests ballot shuffling functionality
TEST_F(STVTests, BallotShufflingTest) {
  Ballot *ballot1 = new Ballot(
    {
        1,0,2,4,
    },
    1);
  Ballot *ballot2 = new Ballot(
    {
        0,1,2,0,
    },
    2);
  Ballot *ballot3 = new Ballot(
    {
        1,4,0,0,
    },
    3);
  Ballot *ballot4 = new Ballot(
      {
          1,2,0,3, 
      },
      4);
  Ballot *ballot5 = new Ballot(
      {
          0,3,1,0
      },
      5);
  Ballot *ballot6 = new Ballot(
      {
          2,3,0,1
      },
      6);
  Ballot *ballot7 = new Ballot(
          {
              1,0,2,0  
          },
          7);
  Ballot *ballot8 = new Ballot(
          {
              0,1,0,2
          },
          8);
  Ballot *ballot9 = new Ballot(
          {
              1,2,3,0
          },
          9);
  shuffle_ballots = {ballot1, ballot2, ballot3, ballot4, ballot5, ballot6, ballot7, ballot8, ballot9};
  STV test(shuffle_ballots, shuffle_cand, 1);
  std::vector<Ballot *> originalOrder = test.getBallots();
  
  // Enable shuffling and verify ballots order changed
  test.setShuffle(true);
  EXPECT_NE(test.getBallots(),originalOrder) << "Shuffling did not affect election outcome";
}


// Tests ballot counting functionality
TEST_F(STVTests, GetNumBallotsTest) {
  STV test(ballots, candidates, seats);
  EXPECT_EQ(test.getNumBallots(), 3) << "getNumBallots not set up correctly";
}


// Tests finding of lowest scoring candidate
TEST_F(STVTests, FindLowestCandidateTest) {
  // Setup specific vote counts
  std::vector<Ballot*> test_ballots = {
      new STVBallot({0,1,2}, 1),  // A
      new STVBallot({1,0,2}, 2),  // B
      new STVBallot({1,0,2}, 3)   // B
  };
  
  STV test(test_ballots, candidates, 1);
  test.setShuffle(false);
  
  // Manually set up candidate votes
  candidates[0]->updateVotes(1);  // A:1
  candidates[1]->updateVotes(2);  // B:2
  candidates[2]->updateVotes(0);  // C:0
  
  Candidate* lowest = test.findLowestCandidate();
  ASSERT_NE(lowest, nullptr);
  EXPECT_EQ(lowest->getName(), "C");
  
  // Clean up
  for (auto* b : test_ballots) delete b;
}


// Tests single candidate election case
TEST_F(STVTests, SingleCandidateTest) {

  Ballot* ballot = new Ballot({1}, 1);
  Candidate* solo = new Candidate("D");
  single_candidate = {solo};
  ballots_single_candidate = {ballot};

  STV test(ballots_single_candidate, single_candidate, 1);

  test.setShuffle(false);

  std::vector<Candidate*> winners, losers;
  test.runElection(winners, losers);
  
  // Verify results
  ASSERT_EQ(winners.size(), 1) << "Should have exactly one winner";
  EXPECT_EQ(winners[0]->getName(), "D") << "Single candidate should win";
  EXPECT_TRUE(losers.empty()) << "No losers expected in single-candidate election";
}


// Tests surplus vote redistribution logic
TEST_F(STVTests, SurplusRedistributionTest) {
  // Create ballots where one candidate clearly exceeds quota
  std::vector<Ballot*> surplus_ballots = {
    new STVBallot({0,1,2}, 1),
    new STVBallot({0,1,2}, 2),
    new STVBallot({2,1,3}, 3),
    new STVBallot({0,1,2}, 4),
    new STVBallot({2,1,0}, 5)
  };
  
  STV test(surplus_ballots, candidates, 1); // 1 seat, quota = 3
  test.setShuffle(false);
  
  std::vector<Candidate*> winners;
  std::vector<Candidate*> losers;
  test.runElection(winners, losers);
  
  // Clean up
  for (auto* b : surplus_ballots) delete b;
  
  // Verify candidate A (index 0) wins with surplus redistribution
  ASSERT_EQ(winners.size(), 1);
  EXPECT_EQ(winners[0]->getName(), "B");
  
  // Verify surplus votes were redistributed properly
  // Candidate A should end with exactly quota votes (3)
  EXPECT_EQ(winners[0]->getNumVotes(), 3);
}


// Tests complete STV election simulation
TEST_F(STVTests, runElectionTest) {
  // Convert existing ballots to STVBallot format
  std::vector<Ballot*> stv_ballots = {
    new STVBallot({0,1,2}, 1),  // A(1), B(2), C(3)
    new STVBallot({2,0,1}, 2),  // A(1), B(2)
    new STVBallot({0,1,2}, 3)   // C(3), A(1), B(2)
  };
  
  STV test(stv_ballots, candidates, seats);
  test.setShuffle(false);
  
  std::vector<Candidate*> winners;
  std::vector<Candidate*> losers;
  test.runElection(winners, losers);
  
  // Clean up test ballots
  for (auto* b : stv_ballots) delete b;
  
  // Verify results
  ASSERT_EQ(winners.size(), seats) << "Should have exactly 2 winners";
  ASSERT_EQ(losers.size(), 1) << "Should have exactly 1 loser";
  
  // Check that B and C are the winners (based on vote distribution)
  bool hasB = false, hasC = false;
  for (auto* winner : winners) {
    if (winner->getName() == "B") hasB = true;
    if (winner->getName() == "C") hasC = true;
  }
  EXPECT_TRUE(hasB && hasC) << "B and C should be winners";
  
  // Check that A is the loser
  EXPECT_EQ(losers[0]->getName(), "A") << "A should be the loser";
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}