#include "stvballot.h"
#include <climits>

STVBallot::STVBallot(const std::vector<int>& votes, int id)
    : Ballot(votes, id), preference(-1) {
    updatePreference();
}

void STVBallot::removeTopChoice() {
    if (preference != -1) {
        votes[preference] = 0;
        updatePreference();
    }
}

void STVBallot::updatePreference() {
    preference = -1;
    int minRank = INT_MAX;

    for (size_t i = 0; i < votes.size(); ++i) {
        if (votes[i] > 0 && votes[i] < minRank) {
            minRank = votes[i];
            preference = i;
        }
    }
}

int STVBallot::getPreference() const {
    return preference;
}
