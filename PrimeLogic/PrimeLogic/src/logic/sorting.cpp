#include "logic/sorting.h"

#include <cctype>

static int compareNames(const char* left, const char* right)
{
    int index = 0;

    while (left[index] != '\0' && right[index] != '\0')
    {
        char leftLower = (char)tolower((unsigned char)left[index]);
        char rightLower = (char)tolower((unsigned char)right[index]);

        if (leftLower < rightLower)
        {
            return -1;
        }

        if (leftLower > rightLower)
        {
            return 1;
        }

        index++;
    }

    if (left[index] == '\0' && right[index] == '\0')
    {
        return 0;
    }

    return left[index] == '\0' ? -1 : 1;
}

static bool shouldSwap(const StandingRow* current, const StandingRow* next)
{
    if (current->points != next->points)
    {
        return current->points < next->points;
    }

    if (current->goalDifference != next->goalDifference)
    {
        return current->goalDifference < next->goalDifference;
    }

    if (current->goalsFor != next->goalsFor)
    {
        return current->goalsFor < next->goalsFor;
    }

    return compareNames(current->teamName, next->teamName) > 0;
}

void bubbleSortStandingsByPoints(StandingRow standings[], int standingCount)
{
    for (int pass = 0; pass < standingCount - 1; pass++)
    {
        for (int index = 0; index < standingCount - pass - 1; index++)
        {
            if (shouldSwap(&standings[index], &standings[index + 1]))
            {
                StandingRow temporary = standings[index];
                standings[index] = standings[index + 1];
                standings[index + 1] = temporary;
            }
        }
    }
}