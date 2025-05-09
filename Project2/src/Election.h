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

/**
 * @class Election
 * @brief represents all Election styles, parent class to STV, PV, and MV algorithms.
 */

class Election {
protected:
    int numSeats;
    std::vector<Ballot*> ballots;
    //std::string csvFileName;
    std::string algorithm;
    mutable std::string auditFileName; // No longer const
    std::vector<Candidate*> candidates;
    std::vector<Candidate*> winners;
    std::vector<Candidate*> losers;
    std::vector<std::pair<int, std::vector<int>>> invalidBallots; // Stores (ballotID, votes)
    std::vector<std::string> csvFileNames;  // Replace csvFileName with this

    std::vector<std::string> errorLogs; 
    
    // Helper to generate results text
    /**
     * @brief Helper function for generating election results 
     */
    std::string generateResultsText() const;

public:
    /**
     * @brief Constructor for Election
     */
    //Election(std::string csvFileName, std::string algorithm, int numSeats, std::string auditFileName = "");
    // Update constructor
    Election(std::vector<std::string> csvFileNames, std::string algorithm, int numSeats, std::string auditFileName = "");

    /**
     * @brief Constructor for Election's child classes 
     */
    Election(std::vector<Ballot*> ballots, std::vector<Candidate*> candidates, int numSeats);

    //Election(std::vector<Ballot *> ballots, std::vector<Candidate *> candidates, int numSeats, bool shuffle);
    

    // Getters
    /**
     * @brief returns the number of seats up for election 
     * @return numSeats as an int
     */
    int getNumSeats() const;
    /**
     * @brief returns the name of the audit file 
     * @return auditFileName as a string 
     */
    std::string getAuditFileName() const;
    /**
     * @brief returns the name of the input CSV file(s) 
     * @return name(s) as a string
     */
    std::string getCSVFileName() const;
    /**
     * @brief returns the number of ballots 
     * @return numBallots as an int
     */
    int getNumBallots() const;
    /**
     * @brief returns the names of all candidates 
     * @return names in a vector of strings 
     */
    std::vector<std::string> getNames() const;
    /**
     * @brief returns the algorithm selection 
     * @return algorithm as a string
     */
    std::string getAlgorithm() const;
    /**
     * @brief returns all ballots 
     * @return ballots as a vector of Ballot objects 
     */
    std::vector<Ballot*> getBallots() const;
    /**
     * @brief returns all candidates 
     * @return candidates as a vector of Candidate objects 
     */
    std::vector<Candidate*> getCandidates() const;
    /**
     * @brief returns all winners 
     * @return winners as a vector of Candidate objects 
     */
    const std::vector<Candidate*>& getWinners() const { return winners; }
    /**
     * @brief returns all losers 
     * @return losers as a vector of Candidate objects 
     */
    const std::vector<Candidate*>& getLosers() const { return losers; }

    // Core functionality
    /**
     * @brief sets all ballots 
     * @return all ballots as a vector of Ballot objects 
     */
    std::vector<Ballot*> setBallots();
    /**
     * @brief adds a candidate to the winner list 
     * @param candidate to be added to the list 
     */
    void addWinner(Candidate* candidate);
    /**
     * @brief adds a candidate to the loser list 
     * @param candidate to be added to the list 
     */
    void addLoser(Candidate* candidate);
    /**
     * @brief displays election results to the screen
     */
    void displayResults() const;
    /**
     * @brief displays vote allocation per ballot 
     */
    void displayBallotAllocation() const;
    /**
     * @brief prints election results to audit file 
     */
    void printToAudit() const;
    /**
     * @brief returns plurality election results - candidates ordered from highest to lowest vote count 
     * @return results as a vector of Candidate objects
     */
    std::vector<Candidate*> getPluralityResults() const;
    /**
     * @brief returns STV election results - candidates ordered from highest to lowest vote count 
     * @return results as a vector of Candidate objects
     */
    std::vector<std::vector<Candidate*>> getSTVResults() const;
    
};

#endif // ELECTION_H
