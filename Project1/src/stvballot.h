#ifndef STVBALLOT_H
#define STVBALLOT_H

#include "ballot.h"

class STVBallot : public Ballot {
private:
    int preference;

public:
    STVBallot(const std::vector<int>& votes, int ballotID);
    int getPreference() const;
    void removeTopChoice();
    void updatePreference();
};

#endif // STVBALLOT_H