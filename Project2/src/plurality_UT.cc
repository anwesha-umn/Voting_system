/*
* File: plurality_UT.cc
* Description: Unit Test file for Plurality election system
* Author: Annabelle
*/

#include "Election.h"
#include "plurality.h"
#include "ballot.h"
#include "candidate.h"
#include "pluralityballot.h"
#include <gtest/gtest.h>
#include <vector>

// Test fixture for Plurality election tests
class PluralityTest : public ::testing::Test {
    protected:
        std::vector<Ballot*> ballots;        // Test ballots
        std::vector<Ballot*> tieBallots;    // Ballots for tie scenario
        std::vector<Candidate*> candidates;  // Test candidates
        int seats;                          // Regular test seats
        int tieSeats;                       // Seats for tie test
        std::vector<Candidate*> regularWinners;  // Expected regular winners
        std::vector<Candidate*> regularOnlyWinners; // winners vector
        std::vector<Candidate*> regularOnlyLosers; // losers vector 
        std::vector<Candidate*> tieOnlyWinners1; // winners vector possibility
        std::vector<Candidate*> tieOnlyWinners2; // winners vector possibility 
        std::vector<Candidate*> tieOnlyLosers1; // losers vector possibility
        std::vector<Candidate*> tieOnlyLosers2; // losers vector possibility
        std::vector<Candidate*> tieWinners1;     // Possible tie outcome 1
        std::vector<Candidate*> tieWinners2;     // Possible tie outcome 2
    
        // Setup test data before each test
        void SetUp() override {
            // Create test ballots
            Ballot* ballot1 = new Ballot({1,0,0}, 1);  // Votes for candidate A
            Ballot* ballot2 = new Ballot({0,1,0}, 2);  // Votes for candidate B
            Ballot* ballot3 = new Ballot({0,1,0}, 3);  // Votes for candidate B
    
            // Create test candidates
            Candidate* cand1 = new Candidate("A");
            Candidate* cand2 = new Candidate("B");
            Candidate* cand3 = new Candidate("C");
    
            // Initialize test data
            ballots = {ballot1, ballot2, ballot3};
            candidates = {cand1, cand2, cand3};
            regularWinners = {cand2, cand1, cand3};  // B should win, then A, then C
            regularOnlyWinners = {cand2, cand1};  // winners vector
            regularOnlyLosers = {cand3}; // losers vector
            seats = 2;  // Test with 2 seats
    
            // Setup tie scenario
            Ballot* tieBallot1 = new Ballot({1, 0, 0}, 1);  // Vote for A
            Ballot* tieBallot2 = new Ballot({0, 1, 0}, 2);  // Vote for B
            tieBallots = {tieBallot1, tieBallot2};
            tieWinners1 = {cand1, cand2, cand3};  // Possible tie outcome
            tieOnlyWinners1 = {cand1}; //winners vector possibility
            tieOnlyWinners2 = {cand2}; // alternate winners vector possibility
            tieWinners2 = {cand2, cand1, cand3};  // Alternate tie outcome
            tieOnlyLosers1 = {cand2, cand3}; // losers vector possibility 
            tieOnlyLosers2 = {cand1, cand3}; // losers vector possibility
            tieSeats = 1;  // Test tie with 1 seat
        }
    
        // Clean up test data after each test
        void TearDown() override {
            for (auto ballot : ballots) delete ballot;
            for (auto ballot : tieBallots) delete ballot;
            for (auto candidate : candidates) delete candidate;
        }
    };
    
    // Test Plurality constructor
    TEST_F(PluralityTest, PluralityConstructor) {
        Plurality test_plurality(ballots, candidates, seats);
        EXPECT_EQ(test_plurality.getNumSeats(), seats);
        EXPECT_EQ(test_plurality.getBallots(), ballots);
        EXPECT_EQ(test_plurality.getCandidates(), candidates);
    }
    
    // Test regular election scenario
    TEST_F(PluralityTest, RegularPluralityRunElection) {
        Plurality test_plurality(ballots, candidates, seats);
        test_plurality.runElection(ballots, candidates, seats);
        EXPECT_EQ(test_plurality.getPluralityResults(), regularWinners);
        EXPECT_EQ(test_plurality.getWinners(), regularOnlyWinners); 
        EXPECT_EQ(test_plurality.getLosers(), regularOnlyLosers);
    }
    
    // Test tie scenario (either A or B could win)
    TEST_F(PluralityTest, TiePluralityRunElection) {
        Plurality test_plurality(tieBallots, candidates, tieSeats);
        test_plurality.runElection(tieBallots, candidates, tieSeats);
        EXPECT_TRUE(test_plurality.getPluralityResults() == tieWinners1 ||
                    test_plurality.getPluralityResults() == tieWinners2);
        EXPECT_TRUE(test_plurality.getWinners() == tieOnlyWinners1 || 
                    test_plurality.getWinners() == tieOnlyWinners2); 
        EXPECT_TRUE(test_plurality.getLosers() == tieOnlyLosers1 || 
                    test_plurality.getLosers() == tieOnlyLosers2); 
    }  

// Run all tests
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
