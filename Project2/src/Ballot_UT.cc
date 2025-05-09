
/*
* File: Ballot_UT.cc
* Description: Unit Test file for Ballot.cpp
* Author: Annabelle Coler
*/

#include "ballot.h"
#include <gtest/gtest.h>
#include <vector>


class BallotTest : public ::testing::Test {
protected:
    std::vector<int> votes;
    int ballotID;

    void SetUp() override {
        votes = {1, 0, 0, 0, 0, 0};
        ballotID = 1;
    }
};

// Test fixture for PluralityBallot tests
class PluralityBallotTest : public ::testing::Test {
protected:
    std::vector<int> votes;
    int ballotID;

    void SetUp() override { ballotID = 1; }
};

// Test Fixture for STVBallot Tests
class STVBallotTest : public ::testing::Test {
protected:
    std::vector<int> votes;
    int ballotID;

    void SetUp() override { ballotID = 1; }
};

// Unit tests for Ballot

// Test for ballot constructor
TEST_F(BallotTest, BallotConstructor) {
    Ballot test_ballot(votes, ballotID);
    EXPECT_EQ(test_ballot.getVotes(), votes);
    EXPECT_EQ(test_ballot.getID(), ballotID);
}

// Test for getVotes() method
TEST_F(BallotTest, GetVotes) {
    Ballot test_ballot(votes, ballotID);
    EXPECT_EQ(test_ballot.getVotes(), votes);
}

// Test for getID() method
TEST_F(BallotTest, GetBallotID) {
    Ballot test_ballot(votes, ballotID);
    EXPECT_EQ(test_ballot.getID(), ballotID);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}