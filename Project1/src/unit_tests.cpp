
#include <gtest/gtest.h>
#include "pluralityballot.h"
#include "stvballot.h"

//Test fixture for Ballot tests
class BallotTest : public ::testing::Test {
protected:
    std::vector<int> votes; 
    int ballotID; 

    void SetUp() override {
        votes = {1, 0, 0, 0, 0, 0}; 
        ballotID = 1; 
    }
}

//Test fixture for PluralityBallot tests 
class PluralityBallotTest : public ::testing::Test {
protected:
    std::vector<int> votes; 
    int ballotID;

    void SetUp() override {
        ballotID = 1; 
    }
}

//Test Fixture for STVBallot Tests 
class STVBallotTest : public ::testing::Test {
protected: 
    std::vector<int> votes; 
    int ballotID;

    void SetUp() override {
        ballotID = 1; 
    }
}

//Unit tests for Ballot 

//Test for ballot constructor
TEST_F(BallotTest, BallotConstructor) {
    Ballot test_ballot(votes, ballotID); 
    EXPECT_EQ(test_ballot.getVotes(), votes); 
    EXPECT_EQ(test_ballot.getID(), ballotID); 
}

//Test for getVotes() method
TEST_F(BallotTest, GetVotes) {
    Ballot test_ballot(votes, ballotID); 
    EXPECT_EQ(test_ballot.getVotes(), votes); 
}

//Test for getID() method
TEST_F(BallotTest, GetBallotID) {
    Ballot test_ballot(votes, ballotID); 
    EXPECT_EQ(test_ballot.getID(), ballotID); 
}

//Unit Tests for Plurality Ballot

//Test should pass, this is a normal plurality ballot 
TEST_F(PluralityBallotTest, NormalPBallotConstructor) {
    votes = {1, 0, 0, 0, 0, 0}; 
    int preference = 0; 
    PluralityBallot test_ballot(votes, ballotID); 
    EXPECT_EQ(test_ballot.getVotes(), votes); 
    EXPECT_EQ(test_ballot.getID(), ballotID); 
    EXPECT_EQ(test_ballot.getPreference(), preference);
}

//Test should throw an error on a ballot with no votes  
TEST_F(PluralityBallotTest, AllZeroesPBallotConstructor) {
    votes = {0, 0, 0, 0, 0, 0};  
    EXPECT_THROW(PluralityBallot test_ballot(votes, ballotID), std::invalid_argument); 
}

//Test should throw an error on a ballot with multiple votes
TEST_F(PluralityBallotTest, MultipleVotesPBallotConstructor) {
    votes = {1, 0, 1, 0, 1, 0}; 
    EXPECT_THROW(PluralityBallot test_ballot(votes, ballotID), std::invalid_argument); 
}

//Test should throw an error on a ballot with multiple votes
TEST_F(PluralityBallotTest, MultipleVotesPBallotConstructor2) {
    votes = {0, 0, 3, 0, 0, 0};  
    EXPECT_THROW(PluralityBallot test_ballot(votes, ballotID), std::invalid_argument); 
}

//Test for getPreference() method
TEST_F(PluralityBallotTest, GetPreferenceTest) {
    votes = {1, 0, 0, 0, 0, 0}; 
    int preference = 0; 
    PluralityBallot test_ballot(votes, ballotID); 
    EXPECT_EQ(test_ballot.getPreference(), preference); 
}

//Unit Tests for STV Ballot 

//Test should pass, this is a normal STV ballot
TEST_F(STVBallotTest, NormalSTVBallotConstructor) {
    votes = {1, 0, 2, 0, 0, 3}; 
    int preference = 0; 
    STVBallot test_ballot(votes, ballotID); 
    EXPECT_EQ(test_ballot.getVotes(), votes); 
    EXPECT_EQ(test_ballot.getID(), ballotID); 
    EXPECT_EQ(test_ballot.getPreference(), preference);
}

//Test for getPreference() method
TEST_F(STVBallotTest, GetPreferenceTest) {
    votes = {1, 0, 2, 0, 0, 3}; 
    int preference = 0; 
    STVBallot test_ballot(votes, ballotID); 
    EXPECT_EQ(test_ballot.getPreference(), preference); 
}