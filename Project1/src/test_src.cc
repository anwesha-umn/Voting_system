#include "Election.h"
#include "ballot.h"
#include "candidate.h"
#include "pluralityballot.h"
#include "stvballot.h"
#include "userinterface.h"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>

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

TEST_F(candidateUnitTests, CandidateConstructorTests) {
  EXPECT_THROW(Candidate candidate("Name", -1, 0, false, false),
               std::invalid_argument)
      << "Does not catch negative ID";
  EXPECT_THROW(Candidate candidate("name", 0, -5, false, false),
               std::invalid_argument)
      << "Does not catch invalid vote initializer";
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

TEST_F(candidateUnitTests, UpdateVotesTest) {
  test_candidate1.updateVotes(5);
  EXPECT_EQ(test_candidate1.getNumVotes(), 10)
      << "updateVotes() not set up correctly";
  EXPECT_THROW(test_candidate1.updateVotes(-1), std::out_of_range);
}

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

// Test for getPreference() method
TEST_F(STVBallotTest, GetPreferenceTest) {
  votes = {1, 0, 2, 0, 0, 3};
  int preference = 0;
  STVBallot test_ballot(votes, ballotID);
  EXPECT_EQ(test_ballot.getPreference(), preference);
}

// election unit tests

class electionUnitTests : public ::testing::Test {
protected:
  void SetUp() override {
    stvTest = Election("stv_ballots.csv", "STV", 5);
    pluralityTest = Election("plurality_ballots.csv", "Plurality", 6);
    // candidate1 = Candidate("Meredith Grey", 0, 0, false, false);
    // candidate2 = Candidate("Callie Torres", 1, 0, false, false);
    // candidate3 = Candidate("Arizona Robbins", 2, 0, false, false);
    // candidate4 = Candidate("Cristina Yang", 3, 0, false, false);
    // stvTest.candidates.push_back(candidate1);
    // stvTest.candidates.push_back(candidate2);
    // stvTest.candidates.push_back(candidate3);
    // stvTest.candidates.push_back(candidate4);
  }
  void TearDown() {}

  Election stvTest;
  Election pluralityTest;
  // mess with this later :<
  // Candidate candidate1;
  // Candidate candidate2;
  // Candidate candidate3;
  // Candidate candidate4;
};

// TEST_F(electionUnitTests, electionConstructorTest) {
//   EXPECT_NO_THROW(Election test = Election("plurality.csv", "Plurality", 5));
// }

// election getter tests

// TEST_F(electionUnitTests, getCSVFileNameTest) {
//   std::string nameS = stvTest.getCSVFileName();
//   EXPECT_EQ(nameS, "../testing/stv_ballots.csv")
//       << "csv file name getter not set up correctly";
//   std::string nameP = pluralityTest.getCSVFileName();
//   EXPECT_EQ(nameP, "../testing/plurality_ballots.csv")
//       << "csv file name getter not set up properly";
// }

// TEST_F(electionUnitTests, getNumSeatsTest) {
//   int seats = stvTest.getNumSeats();
//   EXPECT_EQ(seats, 5) << "getNumSeats() not set up correctly";
//   int seatP = pluralityTest.getNumSeats();
//   EXPECT_EQ(seatP, 6) << "getNumSeats() not set up properly";
// }




// UserInterface tests
class UserInterfaceTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Initialize default test parameters
    csv_file = "../testing/stv_ballots.csv";       // Default CSV filename
    num_seats = 3;               // Default number of seats
    algorithm = "STV";           // Default algorithm
    audit_file = "audit.txt";    // Default audit filename
    shuffle = true;              // Default shuffle setting
  }

  // Test parameters mirroring actual class members
  std::string csv_file;         // Stores CSV filename for testing
  int num_seats;                // Stores seat count for testing
  std::string algorithm;        // Stores algorithm type for testing
  std::string audit_file;       // Stores audit filename for testing
  bool shuffle;                 // Stores shuffle setting for testing

  // Primary test subject
  UserInterface ui;             // Instance of class under test
};


// Test 1: Verify getter for CSV filename
TEST_F(UserInterfaceTest, GetCsvFileNameTest) {
  EXPECT_EQ(ui.getCsvFileName(), csv_file)
      << "getCsvFileName() should return the correct CSV filename";
}

// Test 2: Verify getter for number of seats
TEST_F(UserInterfaceTest, GetNumSeatsTest) {
  EXPECT_EQ(ui.getNumSeats(), num_seats)
      << "getNumSeats() should return the correct number of seats";
}

// Test 3: Verify getter for algorithm
TEST_F(UserInterfaceTest, GetAlgorithmTest) {
  EXPECT_EQ(ui.getAlgorithm(), algorithm)
      << "getAlgorithm() should return the correct algorithm type";
}

// Test 4: Verify getter for audit filename
TEST_F(UserInterfaceTest, GetAuditFileNameTest) {
  EXPECT_EQ(ui.getAuditFileName(), audit_file)
      << "getAuditFileName() should return the correct audit filename";
}

// Test 5: Verify getter for shuffle setting
TEST_F(UserInterfaceTest, GetShuffleStvTest) {
  EXPECT_TRUE(ui.getShuffleStv())
      << "getShuffleStv() should return the correct shuffle setting";
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}