/*
* File: pluralityballot_UT.cc
* Description: Unit Test file for Plurality ballot
* Author: Annabelle
*/

#include "ballot.h"
#include "pluralityballot.h"
#include <gtest/gtest.h>
#include <sstream>
#include <vector>


// Test fixture for PluralityBallot tests
class PluralityBallotTest : public ::testing::Test {
protected:
    std::vector<int> votes;
    int ballotID;

    void SetUp() override { ballotID = 1; }
};

// Unit Tests for Plurality Ballot

// Test should pass, this is a normal plurality ballot
TEST_F(PluralityBallotTest, NormalPBallotConstructor) {
    votes = {1, 0, 0, 0, 0, 0};
    int preference = 0;
    PluralityBallot test_ballot(votes, ballotID);
    EXPECT_EQ(test_ballot.getVotes(), votes);
    EXPECT_EQ(test_ballot.getID(), ballotID);
    EXPECT_EQ(test_ballot.getPreference(), preference);
}

// Test should throw an error on a ballot with no votes
TEST_F(PluralityBallotTest, AllZeroesPBallotConstructor) {
    votes = {0, 0, 0, 0, 0, 0};
    EXPECT_THROW(PluralityBallot test_ballot(votes, ballotID),
                 std::invalid_argument);
}

// Test should throw an error on a ballot with multiple votes
TEST_F(PluralityBallotTest, MultipleVotesPBallotConstructor) {
    votes = {1, 0, 1, 0, 1, 0};
    EXPECT_THROW(PluralityBallot test_ballot(votes, ballotID),
                 std::invalid_argument);
}

// Test should throw an error on a ballot with multiple votes
TEST_F(PluralityBallotTest, MultipleVotesPBallotConstructor2) {
    votes = {0, 0, 3, 0, 0, 0};
    EXPECT_THROW(PluralityBallot test_ballot(votes, ballotID),
                 std::invalid_argument);
}

// Test for getPreference() method
TEST_F(PluralityBallotTest, GetPreferenceTest) {
    votes = {1, 0, 0, 0, 0, 0};
    int preference = 0;
    PluralityBallot test_ballot(votes, ballotID);
    EXPECT_EQ(test_ballot.getPreference(), preference);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
