#include "logic/standings.h"

#include "common/constants.h"

#include <cstring>

static int findStandingIndexByTeamId(const StandingRow standings[], int standingCount, int teamId)
{
    for (int index = 0; index < standingCount; index++)
    {
        if (standings[index].teamId == teamId)
        {
            return index;
        }
    }

    return -1;
}

static void applyMatchResult(StandingRow* homeRow, StandingRow* awayRow, const Match* match)
{
    homeRow->played++;
    awayRow->played++;

    homeRow->goalsFor += match->homeGoals;
    homeRow->goalsAgainst += match->awayGoals;
    awayRow->goalsFor += match->awayGoals;
    awayRow->goalsAgainst += match->homeGoals;

    if (match->homeGoals > match->awayGoals)
    {
        homeRow->wins++;
        awayRow->losses++;
        homeRow->points += WIN_POINTS;
    }
    else if (match->homeGoals < match->awayGoals)
    {
        awayRow->wins++;
        homeRow->losses++;
        awayRow->points += WIN_POINTS;
    }
    else
    {
        homeRow->draws++;
        awayRow->draws++;
        homeRow->points += DRAW_POINTS;
        awayRow->points += DRAW_POINTS;
    }
}

void calculateStandings(
    const Team teams[],
    int teamCount,
    const Match matches[],
    int matchCount,
    StandingRow standings[],
    int* standingCount)
{
    *standingCount = teamCount;

    for (int index = 0; index < teamCount; index++)
    {
        standings[index].teamId = teams[index].id;
        strcpy_s(standings[index].teamName, MAX_NAME_LENGTH, teams[index].name);
        standings[index].played = 0;
        standings[index].wins = 0;
        standings[index].draws = 0;
        standings[index].losses = 0;
        standings[index].goalsFor = 0;
        standings[index].goalsAgainst = 0;
        standings[index].goalDifference = 0;
        standings[index].points = 0;
    }

    for (int matchIndex = 0; matchIndex < matchCount; matchIndex++)
    {
        int homeIndex = findStandingIndexByTeamId(standings, *standingCount, matches[matchIndex].homeTeamId);
        int awayIndex = findStandingIndexByTeamId(standings, *standingCount, matches[matchIndex].awayTeamId);

        if (homeIndex == -1 || awayIndex == -1)
        {
            continue;
        }

        applyMatchResult(&standings[homeIndex], &standings[awayIndex], &matches[matchIndex]);
    }

    for (int index = 0; index < *standingCount; index++)
    {
        standings[index].goalDifference = standings[index].goalsFor - standings[index].goalsAgainst;
    }
}