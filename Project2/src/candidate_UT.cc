/*
* File: candidate_UT.cc
* Description: Unit Test file for Candidate.cpp
* Author: Zoe
*/


#include "candidate.h"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// unit tests for candidate class
class candidateUnitTests : public ::testing::Test {
public:
  void SetUp() {
    test_candidate1 = Candidate("Buffy Summers", 1, 5, false, false);
  }
  void TearDown() {}

protected:
  Candidate test_candidate1;
};

// Constructor tests
TEST_F(candidateUnitTests, CandidateConstructorTests) {
  // Test negative ID validation
  EXPECT_THROW(Candidate candidate("Name", -1, 0, false, false),
               std::invalid_argument)
      << "Does not catch negative ID";
  // Test negative votes validation
  EXPECT_THROW(Candidate candidate("name", 0, -5, false, false),
               std::invalid_argument)
      << "Does not catch invalid vote initializer";
  // Test valid construction
  EXPECT_NO_THROW(Candidate candidate("name", 0, 0, false, false))
      << "Constructor not set up properly";
}

// getter tests
TEST_F(candidateUnitTests, DisplayCandidateName) {
  std::string name = test_candidate1.getName();
  EXPECT_EQ(name, "Buffy Summers") << "getName() not set up correctly";
}

TEST_F(candidateUnitTests, GetCandidateIDTest) {
  int id = test_candidate1.getCandidateID();
  EXPECT_EQ(id, 1) << "getCandidateID() not set up correctly";
}

TEST_F(candidateUnitTests, GetWinnerTest) {
  bool win = test_candidate1.isWinner();
  EXPECT_EQ(win, false) << "isWinner() not set up correctly";
}

TEST_F(candidateUnitTests, GetLoserTest) {
  bool lose = test_candidate1.isLoser();
  EXPECT_EQ(lose, false) << "isWinner() not set up correctly";
}

TEST_F(candidateUnitTests, GetVotesTest) {
  int v = test_candidate1.getNumVotes();
  EXPECT_EQ(v, 5) << "getNumVotes() not set up correctly";
}

// setter tests
// Test valid vote increment
TEST_F(candidateUnitTests, UpdateVotesTest) {
  test_candidate1.updateVotes(5);
  EXPECT_EQ(test_candidate1.getNumVotes(), 10)
      << "updateVotes() not set up correctly";
  EXPECT_THROW(test_candidate1.updateVotes(-1), std::out_of_range);
}

// Test setting winner
TEST_F(candidateUnitTests, setWinnerTest) {
  test_candidate1.setWinner(true);
  EXPECT_EQ(test_candidate1.isWinner(), true)
      << "setWinner() not set up correctly";
  test_candidate1.setWinner(false);
  EXPECT_EQ(test_candidate1.isWinner(), false)
      << "setWinner() not set up correctly";
  Candidate loser = Candidate("Name", 0, 0, false, true);
  EXPECT_THROW(loser.setWinner(true), std::invalid_argument)
      << "Candidate cannot be a winner and a loser at the same time";
}

// Test setting loser
TEST_F(candidateUnitTests, setLoserTest) {
  test_candidate1.setLoser(true);   
  EXPECT_EQ(test_candidate1.isLoser(), true)
      << "setWinner() not set up correctly";
  test_candidate1.setLoser(false);  
  EXPECT_EQ(test_candidate1.isLoser(), false)
      << "setWinner() not set up correctly";
  
  Candidate winner = Candidate("Name", 0, 0, true, false);
  EXPECT_THROW(winner.setLoser(true), std::invalid_argument)
      << "Candidate cannot be a winner and a loser at the same time";
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
