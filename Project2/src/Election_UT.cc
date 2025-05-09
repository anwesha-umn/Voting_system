/*
 * File: Election_UT.cc
 * Description: Comprehensive unit tests for Election class functionality
 * including:
 *              - Constructor validation
 *              - Getter methods
 *              - Ballot processing
 *              - Candidate handling
 * Author: Zoe, Anwesha
 */

#include "Election.h"
#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

// Test fixture for Election class unit tests
class electionUnitTests : public ::testing::Test {
protected:
  std::vector<std::string> csvFileNames;
  void SetUp() override {
    // Initialize test CSV files before each test
    csvFileNames = {"plurality_ballots_1.csv", "plurality_ballots_2.csv",
                    "plurality_ballots_3.csv"};
  }
  void TearDown() {}
};

// Constructor validation test
TEST_F(electionUnitTests, electionConstructorTest) {
  // plurality constructor
  EXPECT_NO_THROW(
      Election test = Election(
          std::vector<std::string>{"plurality_ballots.csv"}, "plurality", 3));
}

// Getter method tests
// Verify CSV filenames are concatenated correctly
TEST_F(electionUnitTests, getCSVFileNameTest) {
  Election test = Election(csvFileNames, "plurality", 3);
  std::string nameP = test.getCSVFileName();
  EXPECT_EQ(nameP, "plurality_ballots_1.csv, plurality_ballots_2.csv, "
                   "plurality_ballots_3.csv")
      << "csv file name getter not set up properly";
}

// Test seat count for different election types
TEST_F(electionUnitTests, getNumSeatsTest) {
  Election stvTest =
      Election(std::vector<std::string>{"stv_ballots.csv"}, "stv", 5);
  Election pluralityTest = Election(
      std::vector<std::string>{"plurality_ballots.csv"}, "plurality", 6);

  int seats = stvTest.getNumSeats();
  EXPECT_EQ(seats, 5) << "getNumSeats() not set up correctly";
  int seatP = pluralityTest.getNumSeats();
  EXPECT_EQ(seatP, 6) << "getNumSeats() not set up properly";
}

// Test election algorithm type reporting
TEST_F(electionUnitTests, getAlgorithmTest) {
  Election stvTest = Election(
      std::vector<std::string>{"../../testing/stv_ballots.csv"}, "stv", 5);
  Election pluralityTest =
      Election(std::vector<std::string>{"../../testing/plurality_ballots.csv"},
               "plurality", 6);

  std::string name1 = stvTest.getAlgorithm();
  EXPECT_EQ(name1, "stv") << "getNumSeats() not set up correctly";
  std::string name2 = pluralityTest.getAlgorithm();
  EXPECT_EQ(name2, "plurality") << "getNumSeats() not set up properly";
}

// Test error handling for missing ballot files
TEST_F(electionUnitTests, SetBallotsWithInvalidFile) {
  Election election(std::vector<std::string>{"nonexistent_file.csv"},
                    "plurality", 3);
  EXPECT_THROW(election.setBallots(), std::runtime_error);
}

// Test proper ballot loading and parsing for valid csv file
TEST_F(electionUnitTests, setBallotsTest) {
  Election stvTest = Election(
      std::vector<std::string>{"../../testing/stv_ballots.csv"}, "STV", 3);
  stvTest.setBallots();
  std::vector<Ballot *> ballots = stvTest.getBallots();
  Ballot test_bal({1, 0, 2, 0, 3}, 8);
  EXPECT_EQ(ballots.at(0)->getVotes(), test_bal.getVotes())
      << "set ballots not set up properly";
}

// Test loading ballots from multiple files
TEST_F(electionUnitTests, SetBallotsWithMultipleFiles) {
  Election election(std::vector<std::string>{"../../testing/stv_all_inputs_regular.csv", 
    "../../testing/stv_all_inputs_regular_2.csv", "../../testing/stv_all_inputs_regular_3.csv"}, "stv",3);

  EXPECT_NO_THROW(election.setBallots());
  EXPECT_EQ(election.getNumBallots(), 100);
}

// Candidate handling tests
TEST_F(electionUnitTests, GetCandidatesTest) {
  std::string testFile = "../../testing/plurality_all_inputs_2.csv";

  // Create election and load ballots
  Election election({testFile}, "Plurality", 3);

  // Load the ballots (which will also load candidates)
  ASSERT_NO_THROW(election.setBallots())
      << "Failed to load ballots from test file";

  // Get candidates
  std::vector<Candidate *> candidates = election.getCandidates();

  // Verify the candidates were loaded correctly
  ASSERT_FALSE(candidates.empty()) << "No candidates were loaded";
  EXPECT_EQ(candidates.size(), 6);
  EXPECT_EQ(candidates[0]->getName(), "A");
  EXPECT_EQ(candidates[1]->getName(), "B");
  EXPECT_EQ(candidates[2]->getName(), "C");
}

// Results display tests 
// Segfaults in Linux. Works in Windows, has a path issue in Linux for audit file.
/*
TEST_F(electionUnitTests, DisplayResultsOutput) {
  // Redirect cout to a stringstream buffer
  std::stringstream buffer;
  std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());

  // Create and run election
  Election election(std::vector<std::string>{"../../testing/mv_all_valid_ballots_10.csv"}, "MV", 4, "../audit_test.txt");
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
}*/

// Audit logging tests - Works in Windows, has a path issue in Linux
// However while system testing, we could make it work in Linux and get the audit txt file in src directory. 
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
  //election.displayResults();

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


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
