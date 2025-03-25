/*
* File: Election.cpp
 * Description: Implements the Election class, which handles the loading of ballots,
 *              processing of election results, and generation of output for both
 *              console display and audit logs. Supports Plurality and STV algorithms.
 * Author: Anwesha Samaddar
 */

#include "Election.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <filesystem>

#include "pluralityballot.h"
#include "stvballot.h"
#include "ballot.h"

namespace fs = std::filesystem;

// Constructors
Election::Election(std::string csvFileName, std::string algorithm, int numSeats, std::string auditFileName)
    : csvFileName(csvFileName), algorithm(algorithm), numSeats(numSeats), auditFileName(auditFileName) {}

Election::Election(std::vector<Ballot*> ballots, std::vector<Candidate*> candidates, int numSeats)
    : ballots(ballots), candidates(candidates), numSeats(numSeats) {}

// Getters
std::string Election::getCSVFileName() const { return csvFileName; } // Returns the CSV file name
std::string Election::getAuditFileName() const { return auditFileName; } // Returns the audit file name
int Election::getNumSeats() const { return numSeats; } // Returns the number of seats
int Election::getNumBallots() const { return ballots.size(); } // Returns the number of ballots
std::string Election::getAlgorithm() const { return algorithm; } // Returns the election algorithm type
// returns the vector of ballots
std::vector<Ballot*> Election::getBallots() const {
    return ballots;
}
//returns the vector of candidates
std::vector<Candidate*> Election::getCandidates() const {
    return candidates;
}

std::vector<std::string> Election::getNames() const {
    // Returns a vector of candidate names
    std::vector<std::string> names;
    for (const Candidate* c : candidates)
        names.push_back(c->getName());
    return names;
}

// Ballot processing
std::vector<Ballot*> Election::setBallots() {
    // Loads ballots from the CSV file and initializes candidates
    std::ifstream file(csvFileName);
    if (!file.is_open())
        throw std::runtime_error("Failed to open CSV file: " + csvFileName);

    // Read candidates from the first line of the CSV
    std::string line;
    std::getline(file, line);
    std::istringstream iss(line);
    std::string name;
    int candidateID = 0;
    while (std::getline(iss, name, ','))
        candidates.push_back(new Candidate(name, candidateID++));

    // Read ballots from the remaining lines of the CSV
    int ballotID = 1;
    while (std::getline(file, line)) {
        std::vector<int> votes(candidates.size(), 0);
        std::istringstream ballotStream(line);
        std::string cell;
        size_t column = 0;

        while (std::getline(ballotStream, cell, ',')) {
            try { if (!cell.empty()) votes[column] = std::stoi(cell); }
            catch (...) { votes[column] = 0; }
            column++;
        }

        if (column == 0) continue;

        Ballot* ballot = nullptr;
        try {
            if (algorithm == "STV") ballot = new STVBallot(votes, ballotID);
            else if (algorithm == "Plurality") ballot = new PluralityBallot(votes, ballotID);
        } catch (const std::exception& e) {
            std::cerr << "Error creating ballot " << ballotID << ": " << e.what() << '\n';
        }

        if (ballot) {
            ballots.push_back(ballot);
            ballotID++;
        }
    }
    file.close();
    return ballots;
}

// Results handling
std::string Election::generateResultsText() const {
    std::stringstream ss;

    ss << "\n=== Election Results ===" << std::endl;
    ss << "Election Type: " << algorithm << std::endl;
    ss << "Number of Ballots: " << ballots.size() << std::endl;
    ss << "Number of Seats: " << numSeats << std::endl;
    ss << "Number of Candidates: " << candidates.size() << std::endl;

    if (algorithm == "Plurality") {
        ss << "\n--- Plurality Results ---" << std::endl;

        // Display winners
        ss << "Winners:" << std::endl;
        for (const Candidate* winner : winners) {
            double percentage = (static_cast<double>(winner->getNumVotes()) / ballots.size()) * 100;
            ss << "  " << winner->getName()
               << " | Votes: " << winner->getNumVotes()
               << " | Percentage: " << std::fixed << std::setprecision(2) << percentage << "%" << std::endl;
        }

        // Display losers
        ss << "\nLosers:" << std::endl;
        for (const Candidate* loser : losers) {
            double percentage = (static_cast<double>(loser->getNumVotes()) / ballots.size()) * 100;
            ss << "  " << loser->getName()
               << " | Votes: " << loser->getNumVotes()
               << " | Percentage: " << std::fixed << std::setprecision(2) << percentage << "%" << std::endl;
        }
    }else if (algorithm == "STV") {
        // Calculate Droop quota for STV
        int droopQuota = (ballots.size() / (numSeats + 1)) + 1;

        ss << "\n--- STV Results ---" << std::endl;
        ss << "Droop Quota: " << droopQuota << " votes" << std::endl;

        // Display winners and whether they met the quota
        ss << "\nWinners:" << std::endl;
        for (const Candidate* winner : winners) {
            double percentage = (static_cast<double>(winner->getNumVotes()) / ballots.size()) * 100;
            bool metQuota = (winner->getNumVotes() >= droopQuota);

            ss << "  " << winner->getName()
               << " | Votes: " << winner->getNumVotes()
               << " | Percentage: " << std::fixed << std::setprecision(2) << percentage << "%"
               << " | Met Quota: " << (metQuota ? "Yes" : "No") << std::endl;
        }

        // Display losers
        ss << "\nLosers:" << std::endl;
        for (const Candidate* loser : losers) {
            double percentage = (static_cast<double>(loser->getNumVotes()) / ballots.size()) * 100;
            ss << "  " << loser->getName()
               << " | Votes: " << loser->getNumVotes()
               << " | Percentage: " << std::fixed << std::setprecision(2) << percentage << "%" << std::endl;
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
    // Writes election results to the audit file in the src folder
    // Get the current working directory
    fs::path currentDir = fs::current_path();

    // Construct the path to the src folder
    fs::path srcDir = currentDir / "../"; // Go up one level and into the src folder

    // Use a default file name if auditFileName is empty
    std::string fileName = auditFileName.empty() ? "audit.txt" : auditFileName;

    // Combine the src directory path with the file name
    fs::path auditFilePath = srcDir / fileName;

    // Normalize the path (resolve ".." and ".")
    auditFilePath = auditFilePath.lexically_normal();

    // Open the file in the src folder
    std::ofstream auditStream(auditFilePath);
    if (!auditStream) {
        throw std::runtime_error("Failed to create audit file: " + auditFilePath.string());
    }

    // Write the results to the file
    auditStream << generateResultsText();
    auditStream.close();

    // Debug output to confirm file creation
    std::cout << "Audit log written to: " << fs::absolute(auditFilePath) << std::endl;
}

// Remaining methods
void Election::displayBallotAllocation() const {
    // Displays the allocation of votes for each ballot (for testing)
    std::cout << "Ballot Allocation:\n";
    for (const Ballot* b : ballots) {
        std::cout << "Ballot ID: " << b->getID() << ", Votes: ";
        for (int v : b->getVotes()) std::cout << v << " ";
        std::cout << "\n";
    }
}

void Election::addWinner(Candidate* candidate) {
    // Adds a candidate to the winners list and marks them as a winner
    winners.push_back(candidate);
    candidate->setWinner(true);
}

void Election::addLoser(Candidate* candidate) {
    // Adds a candidate to the losers list and marks them as a loser
    losers.push_back(candidate);
    candidate->setLoser(true);
}

std::vector<Candidate*> Election::getPluralityResults() const {
    // Returns candidates sorted by votes in descending order (for Plurality elections)
    auto results = candidates;
    std::sort(results.begin(), results.end(), [](auto a, auto b) {
        return a->getNumVotes() > b->getNumVotes();
    });
    return results;
}

std::vector<std::vector<Candidate*>> Election::getSTVResults() const {
    // Returns STV results by rounds (for STV elections)
    std::vector<std::vector<Candidate*>> results;
    auto roundResults = candidates;
    std::sort(roundResults.begin(), roundResults.end(), [](auto a, auto b) {
        return a->getNumVotes() > b->getNumVotes();
    });
    results.push_back(roundResults);
    return results;
}