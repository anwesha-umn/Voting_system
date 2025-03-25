/*
* File: Election.h
 * Description: Defines the Election class, which handles the counting of votes
 *              and determining winners for elections using Plurality or STV algorithms.
 *              Manages ballots, candidates, and election results.
 * Author: Anwesha Samaddar
 */

#ifndef ELECTION_H
#define ELECTION_H

#include "ballot.h"
#include "candidate.h"
#include <string>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

class Election {
protected:
    int numSeats;
    std::vector<Ballot*> ballots;
    std::string csvFileName;
    std::string algorithm;
    mutable std::string auditFileName; // No longer const
    std::vector<Candidate*> candidates;
    std::vector<Candidate*> winners;
    std::vector<Candidate*> losers;

    // Helper to generate results text
    std::string generateResultsText() const;

public:
    Election(std::string csvFileName, std::string algorithm, int numSeats, std::string auditFileName = "");
    Election(std::vector<Ballot*> ballots, std::vector<Candidate*> candidates, int numSeats);

    // Getters
    int getNumSeats() const;
    std::string getAuditFileName() const;
    std::string getCSVFileName() const;
    int getNumBallots() const;
    std::vector<std::string> getNames() const;
    std::string getAlgorithm() const;
    std::vector<Ballot*> getBallots() const;
    std::vector<Candidate*> getCandidates() const;
    const std::vector<Candidate*>& getWinners() const { return winners; }
    const std::vector<Candidate*>& getLosers() const { return losers; }

    // Core functionality
    std::vector<Ballot*> setBallots();
    void addWinner(Candidate* candidate);
    void addLoser(Candidate* candidate);
    void displayResults() const;
    void displayBallotAllocation() const;
    void printToAudit() const;
    std::vector<Candidate*> getPluralityResults() const;
    std::vector<std::vector<Candidate*>> getSTVResults() const;
};

#endif // ELECTION_H