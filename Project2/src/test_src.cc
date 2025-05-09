/*
 * File: test_src.cc
 * Description: Contains all unit tests for the voting system for Project 1
 * Authors: Anwesha Samaddar, Annabelle Coler, Zoe Sepersky
 * NOTE: NOT updated with mv ballot and logic unit tests
 */

#include "Election.h"
#include "ballot.h"
#include "candidate.h"
#include "plurality.h"
#include "pluralityballot.h"
#include "stv.h"
#include "stvballot.h"
#include "userinterface.h"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>  
#include <cstdio>   
#include <gtest/gtest_prod.h> 

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
  public:
    std::vector<int> votes;
    int ballotID;
  
    void SetUp() override { ballotID = 1; }
    void TearDown() {}};

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

// election unit tests
/*

IMPORTANT!!!!!!!!!!!!!

*/
// had to change STVBallot's constructor from const vector<int>& back to
// vector<int>
// Test fixture for Election class unit tests
class electionUnitTests : public ::testing::Test {
  protected:
    std::vector<std::string> csvFileNames;
    void SetUp() override {
      // Initialize test CSV files before each test
      csvFileNames = {"plurality_ballots_1.csv", "plurality_ballots_2.csv", "plurality_ballots_3.csv"}; 
    }
    void TearDown() {}
  };
  
  // Constructor validation test
  TEST_F(electionUnitTests, electionConstructorTest) {
    // plurality constructor
    EXPECT_NO_THROW(Election test =
                        Election(std::vector<std::string>{"plurality_ballots.csv"}, "PV", 3));
  }
  
  
  // Getter method tests
  // Verify CSV filenames are concatenated correctly
  TEST_F(electionUnitTests, getCSVFileNameTest) {
    Election test = Election(csvFileNames, "plurality", 3);
    std::string nameP = test.getCSVFileName();
    EXPECT_EQ(nameP, "plurality_ballots_1.csv, plurality_ballots_2.csv, plurality_ballots_3.csv")
        << "csv file name getter not set up properly";
  }
  
  // Test seat count for different election types
  TEST_F(electionUnitTests, getNumSeatsTest) {
    Election stvTest = Election(std::vector<std::string>{"stv_ballots.csv"}, "stv", 5);
    Election pluralityTest = Election(std::vector<std::string>{"plurality_ballots.csv"}, "PV", 6);
  
    int seats = stvTest.getNumSeats();
    EXPECT_EQ(seats, 5) << "getNumSeats() not set up correctly";
    int seatP = pluralityTest.getNumSeats();
    EXPECT_EQ(seatP, 6) << "getNumSeats() not set up properly";
  }
  
  // Test election algorithm type reporting
  TEST_F(electionUnitTests, getAlgorithmTest) {
    Election stvTest = Election(std::vector<std::string>{"../../testing/stv_ballots.csv"}, "stv", 5);
    Election pluralityTest = Election(std::vector<std::string>{"../../testing/plurality_ballots.csv"}, "PV", 6);
  
    std::string name1 = stvTest.getAlgorithm();
    EXPECT_EQ(name1, "stv") << "getNumSeats() not set up correctly";
    std::string name2 = pluralityTest.getAlgorithm();
    EXPECT_EQ(name2, "PV") << "getNumSeats() not set up properly";
  }
  
  // Test error handling for missing ballot files
  TEST_F(electionUnitTests, SetBallotsWithInvalidFile) {
    Election election(std::vector<std::string>{"nonexistent_file.csv"}, "PV", 3);
    EXPECT_THROW(election.setBallots(), std::runtime_error);
  }
  
  // Test proper ballot loading and parsing for valid csv file
  TEST_F(electionUnitTests, setBallotsTest) {
    Election stvTest = Election(std::vector<std::string>{"../../testing/stv_ballots.csv"}, "STV", 3);
    stvTest.setBallots();
    std::vector<Ballot *> ballots = stvTest.getBallots();
    Ballot test_bal({1, 0, 2, 0, 3}, 8);
    EXPECT_EQ(ballots.at(0)->getVotes(), test_bal.getVotes())
        << "set ballots not set up properly";
  }
  
  // Test loading ballots from multiple files
  TEST_F(electionUnitTests, SetBallotsWithMultipleFiles) {
    Election election(std::vector<std::string>{"../../testing/stv_all_inputs_regular.csv", "../../testing/stv_all_inputs_regular_2.csv"}, "stv",3);
    EXPECT_NO_THROW(election.setBallots());
    EXPECT_EQ(election.getNumBallots(), 50);
  }
  
  // Candidate handling tests
  TEST_F(electionUnitTests, GetCandidatesTest) {
    std::string testFile = "../../testing/plurality_all_inputs_2.csv";  
    
    // Create election and load ballots
    Election election({testFile}, "PV", 3);
  
    // Load the ballots (which will also load candidates)
    ASSERT_NO_THROW(election.setBallots()) << "Failed to load ballots from test file";
  
    // Get candidates
    std::vector<Candidate*> candidates = election.getCandidates();
  
    // Verify the candidates were loaded correctly
    ASSERT_FALSE(candidates.empty()) << "No candidates were loaded";
    EXPECT_EQ(candidates.size(), 6);
    EXPECT_EQ(candidates[0]->getName(), "A");
    EXPECT_EQ(candidates[1]->getName(), "B"); 
    EXPECT_EQ(candidates[2]->getName(), "C");
  }
  
  // Results display tests
  TEST_F(electionUnitTests, DisplayResultsOutput) {
    // Redirect cout to a stringstream buffer
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
  
    // Create and run election
    Election election(std::vector<std::string>{"../../testing/mv_all_valid_ballots.csv"}, "MV", 4, "../audit_test.txt");
    election.setBallots();
    election.displayResults();
  
    // Restore cout
    std::cout.rdbuf(old);
  
    // Get the output and verify contents
    std::string output = buffer.str();
    EXPECT_NE(output.find("=== Election Results ==="), std::string::npos);
    EXPECT_NE(output.find("Election Type: MV"), std::string::npos);
    // EXPECT_NE(output.find("=== End of Results ==="), std::string::npos);
    
    // For more specific testing based on your implementation:
    if (output.find("Number of Valid Ballots") != std::string::npos) {
        // Verify ballot count format if shown
        EXPECT_NE(output.find("Number of Valid Ballots: "), std::string::npos);
    }
  }
  
  // Audit logging tests
  /*
  TEST_F(electionUnitTests, AuditFileCreationTest) {
    // create unique test filename
    std::string testAuditPath = "../test_audit_log.txt";
    
    // Remove file if it exists from previous test runs
    std::remove(testAuditPath.c_str());
  
    // Create election with test audit path
    Election election(std::vector<std::string>{"../../testing/stv_ballots.csv"}, 
                     "STV", 3, testAuditPath);
    election.setBallots();
    
    // This should call printToAudit()
    election.displayResults();
  
    // Verify file was created
    std::ifstream auditFile(testAuditPath);
    bool fileExists = auditFile.is_open();
    
    EXPECT_TRUE(fileExists) << "Audit file was not created at: " << testAuditPath;
  
    // Verify basic content if file exists
    if (fileExists) {
        std::string content((std::istreambuf_iterator<char>(auditFile)),
                          std::istreambuf_iterator<char>());
        auditFile.close();
        
        // Clean up
        std::remove(testAuditPath.c_str());
    }
  }*/
  
// UserInterface tests
// Fixture for testing class UserInterface.
 class UserInterfaceTest : public testing::Test {
  protected:
  
   UserInterfaceTest() {
   }
 
   ~UserInterfaceTest() override {
   }
 
 
   void SetUp() override {
      csv_files = {"../../testing/stv_ballots.csv","../../testing/stv_ballots2.csv"}; // Default CSV filenames
      num_seats = 3;
      algorithm = "STV";
      audit_file = "audit.txt";
      shuffle = true;
   }
 
   void TearDown() override {
   }

 
   // Class members for testing
   std::vector<std::string> csv_files;   
   int num_seats;
   std::string algorithm;
   std::string audit_file;
   bool shuffle;
   UserInterface ui;  // Instance under test
 };
 

 // Tests that UserInterface gets the correct CSV filename
 TEST_F(UserInterfaceTest, GetCsvFileNameReturnsCorrectValue) {
   EXPECT_EQ(ui.getCsvFileNames(), csv_files);
 }
 

 // Tests that UserInterface gets the correct number of seats
 TEST_F(UserInterfaceTest, getNumSeatsFromCsv) {
  UserInterface ui;
  
  // Simulate user input
  std::stringstream input_stream;
  input_stream << "1\n";             // Number of files
  input_stream << "../../testing/plurality_all_inputs_3.csv" << "\n";  // CSV file name
  input_stream << "audit.txt\n";     // Audit file name
  
  // Redirect cin
  auto original_cin = std::cin.rdbuf(input_stream.rdbuf());
  
  ui.getInfo();
  
  // Verify the number of seats was read correctly from CSV
  EXPECT_EQ(ui.getNumSeats(), 3);
  
  // Restore cin
  std::cin.rdbuf(original_cin);
 }
 

 // Tests that UserInterface gets the correct algorithm type from CSV
 TEST_F(UserInterfaceTest, getAlgorithmFromCsv) {
  UserInterface ui;
  
  // Simulate user input
  std::stringstream input_stream;
  input_stream << "1\n";             // Number of files
  input_stream << "../../testing/plurality_all_inputs_3.csv" << "\n";  // CSV file name
  input_stream << "audit.txt\n";     // Audit file name
  
  // Redirect cin
  auto original_cin = std::cin.rdbuf(input_stream.rdbuf());
  
  ui.getInfo();
  
  // Verify the algorithm was read correctly from CSV
  EXPECT_EQ(ui.getAlgorithm(), "PV");
  
  // Restore cin
  std::cin.rdbuf(original_cin);
 }

 
 // Tests that UserInterface gets the correct audit filename
 TEST_F(UserInterfaceTest, GetAuditFileNameReturnsCorrectValue) {
   EXPECT_EQ(ui.getAuditFileName(), audit_file);
 }
 

 // Tests that UserInterface gets the correct shuffle setting
 TEST_F(UserInterfaceTest, GetShuffleStvReturnsCorrectValue) {
   EXPECT_TRUE(ui.getShuffleStv());
 }


 // Tests that audit filename is not empty and equal to simulated audit file name
 TEST_F(UserInterfaceTest, AuditFileNameNotEmpty) {
  UserInterface ui;

  // Simulate user input
  std::stringstream input_stream;
  input_stream << "1\n";             // Number of files
  input_stream << "../../testing/plurality_all_inputs_3.csv\n";  // CSV file
  input_stream << "audit_file.txt\n"; // Audit file name

  // Redirect std::cin
  auto original_cin = std::cin.rdbuf(input_stream.rdbuf());

  ui.getInfo();  // This populates the audit file name

  // Assert that the audit file name is not empty
  EXPECT_FALSE(ui.getAuditFileName().empty());
  EXPECT_EQ(ui.getAuditFileName(),"audit_file.txt");

  // Restore std::cin
  std::cin.rdbuf(original_cin);
}


 // Tests that all CSV filenames have .csv extension
 TEST_F(UserInterfaceTest, AllCsvFilesHaveCorrectExtension) {
  for (const auto& file : ui.getCsvFileNames()) {
    EXPECT_EQ(file.substr(file.size() - 4), ".csv");
  }
 }

 // Tests if invalid file number and invalid file names are handled correctly 
 TEST(UserInterfaceSimpleTest, HandlesInvalidInputs) {
  UserInterface ui;
  
  std::stringstream input_stream;
  input_stream << "-10\nabc\n0\n";  // Three invalid inputs
  
  auto original_cin = std::cin.rdbuf(input_stream.rdbuf());
  
  testing::internal::CaptureStdout();
  EXPECT_EXIT(ui.getInfo(), testing::ExitedWithCode(1), "");
  
  std::string output = testing::internal::GetCapturedStdout();
  std::cin.rdbuf(original_cin);
  
  // verify the program exited after invalid inputs
  SUCCEED();  
}

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
  
  
  
  // Tests single candidate election case
  TEST_F(STVTests, SingleCandidateTest) {
    // Create test-specific ballot that meets STVBallot requirements
    Ballot* ballot = new STVBallot({1}, 1);  // Valid single-candidate ballot
    
    std::vector<Ballot*> single_ballots = {ballot};
    std::vector<Candidate*> single_cand = {new Candidate("D", 0)};  // Candidate ID must match ballot index
    
    STV test(single_ballots, single_cand, 1);  // 1 seat for 1 candidate
    test.setShuffle(false);
    
    std::vector<Candidate*> winners;
    std::vector<Candidate*> losers;
    test.runElection(winners, losers);
    
    // Clean up test-specific objects
    delete ballot;
    delete single_cand[0];
    
    // Verify results
    ASSERT_EQ(winners.size(), 1) << "Should have exactly one winner";
    EXPECT_EQ(winners[0]->getName(), "D") << "Single candidate should win";
    EXPECT_TRUE(losers.empty()) << "No losers expected in single-candidate election";
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