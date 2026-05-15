#include "logic/goals_recursion.h"

int sumGoalsRecursive(const Match matches[], int matchCount, int index)
{
    if (index >= matchCount)
    {
        return 0;
    }

    return matches[index].homeGoals + matches[index].awayGoals +
        sumGoalsRecursive(matches, matchCount, index + 1);
}
