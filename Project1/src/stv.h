#ifndef STV_H
#define STV_H

#include "Election.h"
#include "stvballot.h"
#include <vector>
#include <random>

class STV {
private:
    std::vector<Ballot*> ballots;
    std::vector<Candidate*> candidates;
    int seats;
    bool shuffle;

    // STV-specific data structures
    std::vector<bool> eliminated;
    std::vector<std::vector<Ballot*>> candidateBallots;

    // Core logic helpers
    int calculateDroop() const;
    void redistributeSurplus(Candidate* winner, int droop);
    Candidate* findLowestCandidate() const;
    void redistributeEliminated(Candidate* eliminated);

public:
    STV(std::vector<Ballot*> ballots,
        std::vector<Candidate*> candidates,
        int seats);

    void setShuffle(bool shuffle);
    void runElection(std::vector<Candidate*>& winners,
                    std::vector<Candidate*>& losers);

};

#endif // STV_H