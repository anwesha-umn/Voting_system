/*
* File: stvballot_UT.cc
* Description: Unit Test file for stvballot system
* Author: Annabelle
*/



#include "Election.h"
#include "ballot.h"
#include "stvballot.h"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>



// Test Fixture for STVBallot Tests
class STVBallotTest : public ::testing::Test {
public:
  std::vector<int> votes;
  int ballotID;

  void SetUp() override { ballotID = 1; }
  void TearDown() {}};



// Unit Tests for STV Ballot

// Test should pass, this is a normal STV ballot
TEST_F(STVBallotTest, NormalSTVBallotConstructor) {
  votes = {1, 0, 2, 0, 0, 3};
  int preference = 0;
  STVBallot test_ballot(votes, ballotID);
  EXPECT_EQ(test_ballot.getVotes(), votes);
  EXPECT_EQ(test_ballot.getID(), ballotID);
  EXPECT_EQ(test_ballot.getPreference(), preference);
}

// Test should throw an error, this is an invalid STV ballot
TEST_F(STVBallotTest, InvalidSTVBallotConstructor) {
  votes = {1, 0, 0, 0, 0, 2};  // Only 2 candidates ranked, required at least 3 ranked

  try {
    STVBallot test_ballot(votes, ballotID);
    FAIL() << "Expected std::invalid_argument";
  } catch (const std::invalid_argument& err) {
    std::string expectedMessage = 
      "\nInvalid STV Ballot 1: Only 2 candidates ranked (minimum 3 required).";
    EXPECT_EQ(expectedMessage, std::string(err.what()));
  } catch (...) {
    FAIL() << "Expected std::invalid_argument";
  }
}


// Test for getPreference() method
TEST_F(STVBallotTest, GetPreferenceTest) {
  votes = {1, 0, 2, 0, 0, 3};
  int preference = 0;
  STVBallot test_ballot(votes, ballotID);
  EXPECT_EQ(test_ballot.getPreference(), preference);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
