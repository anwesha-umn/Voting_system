/*
 * File: Election.cpp
 * Description: Implements the Election class, which handles the loading of
 * ballots, processing of election results, and generation of output for both
 * console display and audit logs. Supports Plurality and STV algorithms.
 * Author: Anwesha Samaddar & Zoe Sepersky (some debugging)
 */

#include "Election.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include "ballot.h"
#include "mvballot.h"
#include "pluralityballot.h"
#include "stvballot.h"

// Constructors
Election::Election(std::vector<std::string> csvFileNames, std::string algorithm,
                   int numSeats, std::string auditFileName)
    : csvFileNames(csvFileNames), algorithm(algorithm), numSeats(numSeats),
      auditFileName(auditFileName) {}

Election::Election(std::vector<Ballot *> ballots,
                   std::vector<Candidate *> candidates, int numSeats)
    : ballots(ballots), candidates(candidates), numSeats(numSeats) {}

// Getters
/*
std::string Election::getCSVFileName() const {
 return csvFileName;
} // Returns the CSV file name
*/

// Update getCSVFileName() to return first file or concatenated names
std::string Election::getCSVFileName() const {
  if (csvFileNames.empty())
    return "";
  if (csvFileNames.size() == 1)
    return csvFileNames[0];

  std::string result;
  for (const auto &name : csvFileNames) {
    if (!result.empty())
      result += ", ";
    result += name;
  }
  return result;
}

// Returns the audit file name
std::string Election::getAuditFileName() const { return auditFileName; }
// Returns the number of seats
int Election::getNumSeats() const { return numSeats; }
// Returns the number of ballots
int Election::getNumBallots() const { return ballots.size(); }
// Returns the election algorithm type
std::string Election::getAlgorithm() const { return algorithm; }

// returns the vector of ballots
std::vector<Ballot *> Election::getBallots() const { return ballots; }
// returns the vector of candidates
std::vector<Candidate *> Election::getCandidates() const { return candidates; }

std::vector<std::string> Election::getNames() const {
  // Returns a vector of candidate names
  std::vector<std::string> names;
  for (const Candidate *c : candidates)
    names.push_back(c->getName());
  return names;
}

// Updated setBallots() to handle multiple files
std::vector<Ballot *> Election::setBallots() {
  ballots.clear();
  invalidBallots.clear();
  candidates.clear(); // Clear any existing candidates

  if (csvFileNames.empty()) {
    throw std::runtime_error("No CSV files specified");
  }

  // Process first file to get candidates
  bool firstFile = true;
  int ballotID = 1;

  for (const auto &fileName : csvFileNames) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
      throw std::runtime_error("Failed to open CSV file: " + fileName);
    }

    std::string line;

    // Read header info (same for all files)
    std::getline(file, line); // algorithm
    std::getline(file, line); // num seats
    std::getline(file, line); // num candidates
    std::getline(file, line); // num ballots

    // Only read candidates from first file
    if (firstFile) {
      std::getline(file, line); // candidate names
      std::istringstream iss(line);
      std::string name;
      int candidateID = 0;

      while (std::getline(iss, name, ',')) {
        candidates.push_back(new Candidate(name, candidateID++));
      }
      firstFile = false;
    } else {
      // Skip candidate names line for subsequent files
      std::getline(file, line);

      // Verify candidate count matches with candidates vector size
      std::istringstream iss(line);
      int candidateCount = 0;
      while (std::getline(iss, line, ','))
        candidateCount++;

      if (candidateCount != candidates.size()) {
        throw std::runtime_error("Candidate count mismatch in file: " +
                                 fileName);
      }
    }

    // Process ballots from this file
    while (std::getline(file, line)) {
      std::vector<int> votes(candidates.size(), 0);
      std::istringstream ballotStream(line);
      std::string cell;
      size_t column = 0;
      
      // Parse each vote value
      while (std::getline(ballotStream, cell, ',')) {
        try {
          if (!cell.empty())
            votes[column] = std::stoi(cell); // Convert to integer
        } catch (...) {
          votes[column] = 0;  // Default to 0 for empty cell
        }
        column++;
      }

      if (column == 0)
        continue;
      // create ballots as per election algorithm
      try {
        Ballot *ballot = nullptr;
        if (algorithm == "STV" || algorithm == "stv") {
          ballot = new STVBallot(votes, ballotID);
        } 
        else if (algorithm == "PV" || algorithm == "pv") {
          ballot = new PluralityBallot(votes, ballotID);
        } 
        else if (algorithm == "MV" || algorithm == "mv") {
          ballot = new MVBallot(votes, ballotID);
        }
        // Add valid ballot to vector of ballots
        if (ballot)
          ballots.push_back(ballot);
      } catch (const std::invalid_argument &e) {
        // Store invalid ballots separately
        invalidBallots.emplace_back(ballotID, votes);
        std::cerr << e.what() << '\n';
      }
      ballotID++;
    }
    file.close();
  }
  return ballots;
}

/*
//Ballots processing method - for single csv file input
std::vector<Ballot *> Election::setBallots() {
  // Loads ballots from the CSV file and initializes candidates
  std::ifstream file(csvFileName);
  if (!file.is_open())
    throw std::runtime_error("Failed to open CSV file: " + csvFileName);

  // Read candidates from the first line of the CSV
  std::string line;
  std::getline(file, line); // algorithm
  std::getline(file, line); // num seats
  std::getline(file, line); // num candidates
  std::getline(file, line); // num ballots
  std::getline(file, line); // candidate names
  std::istringstream iss(line);
  std::string name;
  int candidateID = 0;

  // Create Candidate objects from header
  while (std::getline(iss, name, ','))
    candidates.push_back(new Candidate(name, candidateID++));

  // Read ballots from the remaining lines of the CSV
  int ballotID = 1;
  while (std::getline(file, line)) {
    std::vector<int> votes(candidates.size(), 0);
    std::istringstream ballotStream(line);
    std::string cell;
    size_t column = 0;

    // Parse each vote in the ballot
    while (std::getline(ballotStream, cell, ',')) {
      try {
        if (!cell.empty())
          votes[column] = std::stoi(cell);
      } catch (...) {
        votes[column] = 0; // Default to 0 if conversion fails
      }
      column++;
    }

    if (column == 0) // Skip empty lines
      continue;

    // Create appropriate ballot type based on algorithm
    try {
      Ballot *ballot = nullptr;
      if (algorithm == "STV" or algorithm=="stv") {
        ballot = new STVBallot(votes, ballotID); // Validates in constructor
      } else if (algorithm == "PV" or algorithm == "pv") {
        ballot = new PluralityBallot(votes, ballotID);
      } else if (algorithm == "MV" or algorithm == "mv") {
        ballot = new MVBallot(votes, ballotID);
      }
      if (ballot)
        ballots.push_back(ballot);
    } catch (const std::invalid_argument &e) {
      invalidBallots.emplace_back(ballotID, votes); // Record invalid ballot
      std::cerr << e.what() << '\n';
    }
    ballotID++;
  }
  file.close();
  return ballots;
}
*/


// Results handling
std::string Election::generateResultsText() const {
  std::stringstream ss;

  ss << "\n=== Election Results ===" << std::endl;
  ss << "Election Type: " << algorithm << std::endl;

  if (algorithm == "PV") {
    ss << "\n####### PV Results #######" << std::endl;
    ss << "Number of Invalid Ballots: " << invalidBallots.size() << std::endl;

    // display the invalid ballots
    if (!invalidBallots.empty()) {
      ss << "\n\n=== List of Removed Ballots ===";
      for (const auto &[id, votes] : invalidBallots) {
        ss << "\nID " << id << ": ";
        for (int v : votes)
          ss << v << " ";
      }
      ss << std::endl;
      ss << std::endl;
    }

    ss << "Number of Valid Ballots: " << ballots.size() << std::endl;
    ss << "Number of Seats: " << numSeats << std::endl;
    ss << "Number of Candidates: " << candidates.size() << std::endl;

    // if there are no valid ballots, abort election
    if (ballots.empty()) {
      ss << std::endl
         << "ERROR: Election aborted. No valid ballots to process."
         << std::endl;
      return ss.str();
    }

    // Display winners for PV
    ss << "\nWinners:" << std::endl;
    for (const Candidate *winner : winners) {
      double percentage =
          (static_cast<double>(winner->getNumVotes()) / ballots.size()) * 100;
      ss << "  " << winner->getName() << " | Votes: " << winner->getNumVotes()
         << " | Percentage: " << std::fixed << std::setprecision(2)
         << percentage << "%" << std::endl;
    }

    // Display losers for PV
    ss << "\nLosers:" << std::endl;
    for (const Candidate *loser : losers) {
      double percentage =
          (static_cast<double>(loser->getNumVotes()) / ballots.size()) * 100;
      ss << "  " << loser->getName() << " | Votes: " << loser->getNumVotes()
         << " | Percentage: " << std::fixed << std::setprecision(2)
         << percentage << "%" << std::endl;
    }
  } else if (algorithm == "STV") {

    // Calculate Droop quota for STV
    int droopQuota = (ballots.size() / (numSeats + 1)) + 1;

    ss << "\n####### STV Results #######" << std::endl;
    ss << "Number of Invalid Ballots: " << invalidBallots.size() << std::endl;

    // display the invalid ballots
    if (!invalidBallots.empty()) {
      ss << "\n\n=== List of Removed Ballots ===";
      for (const auto &[id, votes] : invalidBallots) {
        ss << "\nID " << id << ": ";
        for (int v : votes)
          ss << v << " ";
      }
      ss << std::endl;
      ss << std::endl;
    }

    ss << "Number of Valid Ballots: " << ballots.size() << std::endl;
    ss << "Number of Seats: " << numSeats << std::endl;
    ss << "Number of Candidates: " << candidates.size() << std::endl;

    // if there are no valid ballots, abort election
    if (ballots.empty()) {
      ss << std::endl
         << "ERROR: Election aborted. No valid ballots to process."
         << std::endl;
      return ss.str();
    }

    ss << "Droop Quota: " << droopQuota << " votes" << std::endl;

    // Display winners and whether they met the quota
    ss << "\nWinners:" << std::endl;
    for (const Candidate *winner : winners) {
      double percentage =
          (static_cast<double>(winner->getNumVotes()) / ballots.size()) * 100;
      bool metQuota = (winner->getNumVotes() >= droopQuota);

      ss << "  " << winner->getName() << " | Votes: " << winner->getNumVotes()
         << " | Percentage: " << std::fixed << std::setprecision(2)
         << percentage << "%" << " | Met Quota: " << (metQuota ? "Yes" : "No")
         << std::endl;
    }

    // Display losers
    ss << "\nLosers:" << std::endl;
    for (const Candidate *loser : losers) {
      double percentage =
          (static_cast<double>(loser->getNumVotes()) / ballots.size()) * 100;
      ss << "  " << loser->getName() << " | Votes: " << loser->getNumVotes()
         << " | Percentage: " << std::fixed << std::setprecision(2)
         << percentage << "%" << std::endl;
    }
  } else if (algorithm == "MV") {
    ss << "\n####### MV Results #######" << std::endl;
    ss << "Number of Invalid Ballots: " << invalidBallots.size() << std::endl;

    // display the invalid ballots
    if (!invalidBallots.empty()) {
      ss << "\n\n=== List of Removed Ballots ===";
      for (const auto &[id, votes] : invalidBallots) {
        ss << "\nID " << id << ": ";
        for (int v : votes)
          ss << v << " ";
      }
      ss << std::endl;
      ss << std::endl;
    }

    ss << "Number of Valid Ballots: " << ballots.size() << std::endl;
    ss << "Number of Seats: " << numSeats << std::endl;
    ss << "Number of Candidates: " << candidates.size() << std::endl;

    // if there are no valid ballots, abort election
    if (ballots.empty()) {
      ss << std::endl
         << "ERROR: Election aborted. No valid ballots to process."
         << std::endl;
      return ss.str();
    }

    // Display winners for Plurality
    ss << "\nWinners:" << std::endl;
    for (const Candidate *winner : winners) {
      double percentage =
          (static_cast<double>(winner->getNumVotes()) / ballots.size()) * 100;
      ss << "  " << winner->getName() << " | Votes: " << winner->getNumVotes()
         << " | Percentage: " << std::fixed << std::setprecision(2)
         << percentage << "%" << std::endl;
    }

    // Display losers for Plurality
    ss << "\nLosers:" << std::endl;
    for (const Candidate *loser : losers) {
      double percentage =
          (static_cast<double>(loser->getNumVotes()) / ballots.size()) * 100;
      ss << "  " << loser->getName() << " | Votes: " << loser->getNumVotes()
         << " | Percentage: " << std::fixed << std::setprecision(2)
         << percentage << "%" << std::endl;
    }
  }

  ss << "\n=== End of Results ===" << std::endl;
  return ss.str();
}

void Election::displayResults() const {
  // Displays election results to the console
  std::cout << generateResultsText();
  printToAudit();
}

void Election::printToAudit() const {
  // Get the current working directory
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) == nullptr) {
    throw std::runtime_error("Failed to get current working directory.");
  }

  std::string currentDir = std::string(cwd);
  // FOR WINDOWS - uncomment this
  
  // Go one level up (assuming the audit file should be in the parent directory)
  size_t pos = currentDir.find_last_of("/\\");
  std::string srcDir =
      (pos != std::string::npos) ? currentDir.substr(0, pos) : currentDir;

  // Use default file name if auditFileName is empty
  std::string fileName = auditFileName.empty() ? "audit.txt" : auditFileName;

  // Construct the audit file path manually  // IMPORTANT - note 4/18: updated this for linux pathing
  std::string auditFilePath =
      srcDir + "\\src\\" + fileName; 

  // Open the file in the src folder
  std::ofstream auditStream(auditFilePath);
  if (!auditStream) {
    throw std::runtime_error("Failed to create audit file: " +
    auditFilePath);
  }

  // FOR LINUX
  /*
  // Go one level up (assuming the audit file should be in the parent directory)
  size_t pos = currentDir.find_last_of("/");
  std::string srcDir =
      (pos != std::string::npos) ? currentDir.substr(0, pos) : currentDir;

  // Use default file name if auditFileName is empty
  std::string fileName = auditFileName.empty() ? "audit.txt" : auditFileName;

  // Construct the audit file path manually
  std::string auditFilePath =
      srcDir + "/src/" + fileName; // note 4/18: updated this for linux pathing

  // Open the file in the src folder
  std::ofstream auditStream(auditFilePath);
  if (!auditStream) {
    throw std::runtime_error("Failed to create audit file: " + auditFilePath);
  } */
  
  // Write the results to the file
  auditStream << generateResultsText();
  auditStream.close();

  // Debug output to confirm file creation
  std::cout << "Audit log written to: " << auditFilePath << std::endl;
}

// Remaining methods
void Election::displayBallotAllocation() const {
  // Displays the allocation of votes for each ballot (for testing)
  std::cout << "Ballot Allocation:\n";
  for (const Ballot *b : ballots) {
    std::cout << "Ballot ID: " << b->getID() << ", Votes: ";
    for (int v : b->getVotes())
      std::cout << v << " ";
    std::cout << "\n";
  }
}

void Election::addWinner(Candidate *candidate) {
  // Adds a candidate to the winners list and marks them as a winner
  winners.push_back(candidate);
  candidate->setWinner(true);
}

void Election::addLoser(Candidate *candidate) {
  // Adds a candidate to the losers list and marks them as a loser
  losers.push_back(candidate);
  candidate->setLoser(true);
}

std::vector<Candidate *> Election::getPluralityResults() const {
  // Returns candidates sorted by votes in descending order (for Plurality
  // elections)
  auto results = candidates;
  std::sort(results.begin(), results.end(),
            [](auto a, auto b) { return a->getNumVotes() > b->getNumVotes(); });
  return results;
}

std::vector<std::vector<Candidate *>> Election::getSTVResults() const {
  // Returns STV results by rounds (for STV elections)
  std::vector<std::vector<Candidate *>> results;
  auto roundResults = candidates;
  std::sort(roundResults.begin(), roundResults.end(),
            [](auto a, auto b) { return a->getNumVotes() > b->getNumVotes(); });
  results.push_back(roundResults);
  return results;
}
