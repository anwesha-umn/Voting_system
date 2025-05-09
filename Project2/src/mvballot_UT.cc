/*
 * File: mv_UT.cc
 * Description: Contains all unit tests for MV ballots
 * Author: Zoe Sepersky
 */

#include "mvballot.h"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Test fixture for MV ballot tests
class MVBallotTests : public ::testing::Test {
protected:
  //   MVBallot *bal1;
  //   MVBallot *invalid;
  void setUp() {
    // bal1 = new MVBallot({1, 0, 0, 1, 1, 0}, 1);
    // invalid = new MVBallot({1, 2, 3, 4}, 2);
  }
  void tearDown() {
    // delete bal1;
    // delete invalid;
  }
};

// Tests basic ballot construction
TEST_F(MVBallotTests, ConstructorTest) {
  EXPECT_NO_THROW(MVBallot b({1, 0, 0}, 3));
}

// Tests preference extraction logic
TEST_F(MVBallotTests, GetPreferencesTest) {
  // Test normal case with clear preferences
  std::vector<int> correct = {0, 3, 4};
  MVBallot bal1({1, 0, 0, 1, 1, 0}, 1);
  EXPECT_EQ(bal1.getPreferences(), correct);

  // Test edge case with no preferences
  MVBallot bal2({0}, 2);
  std::vector<int> b2p = {};
  EXPECT_EQ(bal2.getPreferences(), b2p);
}

// Tests ballot validation logic
TEST_F(MVBallotTests, isValidTest) {
  // Test valid ballot case
  MVBallot bal1({1, 0, 0, 1, 1, 0}, 1);
  EXPECT_TRUE(bal1.isValid());
  
  // should throw exception if invalid, checking if valid within constructor
  EXPECT_ANY_THROW(MVBallot bal2({1, 2, 3, 4}, 2));
}
