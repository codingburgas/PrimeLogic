#include "data/matches_file.h"
#include "data/teams_file.h"
#include "logic/match_manager.h"
#include "logic/goals_recursion.h"
#include "logic/searching.h"
#include "logic/sorting.h"
#include "logic/standings.h"
#include "logic/team_manager.h"

#include "common/constants.h"

#include <cstdio>
#include <cstring>

static int g_totalChecks = 0;
static int g_passedChecks = 0;

static void printCheckResult(const char* name, bool passed)
{
    g_totalChecks++;

    if (passed)
    {
        g_passedChecks++;
        printf("PASS: %s\n", name);
    }
    else
    {
        printf("FAIL: %s\n", name);
    }
}

static bool testTeamSaveLoad()
{
    Team teams[MAX_TEAMS] = {};
    Team loadedTeams[MAX_TEAMS] = {};
    int teamCount = 2;
    int loadedCount = 0;

    teams[0].id = 1;
    strcpy_s(teams[0].name, MAX_NAME_LENGTH, "Lions");
    teams[1].id = 2;
    strcpy_s(teams[1].name, MAX_NAME_LENGTH, "Tigers");

    if (!saveTeams("tests/temp_teams.csv", teams, teamCount))
    {
        return false;
    }

    if (!loadTeams("tests/temp_teams.csv", loadedTeams, &loadedCount))
    {
        return false;
    }

    return loadedCount == 2 &&
        loadedTeams[0].id == 1 &&
        strcmp(loadedTeams[1].name, "Tigers") == 0;
}

static bool testMatchSaveLoad()
{
    Match matches[MAX_MATCHES] = {};
    Match loadedMatches[MAX_MATCHES] = {};
    int matchCount = 1;
    int loadedCount = 0;

    matches[0].id = 1;
    matches[0].homeTeamId = 1;
    matches[0].awayTeamId = 2;
    matches[0].homeGoals = 2;
    matches[0].awayGoals = 1;
    strcpy_s(matches[0].date, MAX_DATE_LENGTH, "2026-04-22");

    if (!saveMatches("tests/temp_matches.csv", matches, matchCount))
    {
        return false;
    }

    if (!loadMatches("tests/temp_matches.csv", loadedMatches, &loadedCount))
    {
        return false;
    }

    return loadedCount == 1 &&
        loadedMatches[0].homeGoals == 2 &&
        strcmp(loadedMatches[0].date, "2026-04-22") == 0;
}

static bool testTeamValidation()
{
    Team teams[MAX_TEAMS] = {};
    int teamCount = 1;
    char message[MAX_STATUS_LENGTH] = {};

    teams[0].id = 1;
    strcpy_s(teams[0].name, MAX_NAME_LENGTH, "Lions");

    return !validateTeamName("", teams, teamCount, message) &&
        !validateTeamName("Lions", teams, teamCount, message) &&
        validateTeamName("Tigers", teams, teamCount, message);
}

static bool testMatchValidation()
{
    Team teams[MAX_TEAMS] = {};
    Match matches[MAX_MATCHES] = {};
    char message[MAX_STATUS_LENGTH] = {};

    teams[0].id = 1;
    strcpy_s(teams[0].name, MAX_NAME_LENGTH, "Lions");
    teams[1].id = 2;
    strcpy_s(teams[1].name, MAX_NAME_LENGTH, "Tigers");

    return !validateMatch(1, 1, 1, 0, "2026-04-22", teams, 2, matches, 0, message) &&
        !validateMatch(1, 2, -1, 0, "2026-04-22", teams, 2, matches, 0, message) &&
        validateMatch(1, 2, 1, 0, "2026-04-22", teams, 2, matches, 0, message);
}

static bool testStandingsSortSearchAndRecursion()
{
    Team teams[MAX_TEAMS] = {};
    Match matches[MAX_MATCHES] = {};
    StandingRow standings[MAX_TEAMS] = {};
    int resultIndexes[MAX_TEAMS] = {};
    int standingCount = 0;

    teams[0].id = 1;
    strcpy_s(teams[0].name, MAX_NAME_LENGTH, "Lions");
    teams[1].id = 2;
    strcpy_s(teams[1].name, MAX_NAME_LENGTH, "Tigers");
    teams[2].id = 3;
    strcpy_s(teams[2].name, MAX_NAME_LENGTH, "Bears");

    matches[0].id = 1;
    matches[0].homeTeamId = 1;
    matches[0].awayTeamId = 2;
    matches[0].homeGoals = 3;
    matches[0].awayGoals = 1;
    strcpy_s(matches[0].date, MAX_DATE_LENGTH, "2026-04-22");

    matches[1].id = 2;
    matches[1].homeTeamId = 3;
    matches[1].awayTeamId = 1;
    matches[1].homeGoals = 0;
    matches[1].awayGoals = 0;
    strcpy_s(matches[1].date, MAX_DATE_LENGTH, "2026-04-23");

    calculateStandings(teams, 3, matches, 2, standings, &standingCount);
    bubbleSortStandingsByPoints(standings, standingCount);

    if (standings[0].teamId != 1 || standings[0].points != 4)
    {
        return false;
    }

    if (linearSearchTeamByName(standings, standingCount, "ion", resultIndexes, MAX_TEAMS) != 1)
    {
        return false;
    }

    return sumGoalsRecursive(matches, 2, 0) == 4;
}

int main()
{
    printCheckResult("Team save and load", testTeamSaveLoad());
    printCheckResult("Match save and load", testMatchSaveLoad());
    printCheckResult("Team validation", testTeamValidation());
    printCheckResult("Match validation", testMatchValidation());
    printCheckResult("Standings, sort, search and recursion", testStandingsSortSearchAndRecursion());

    printf("\nSummary: %d/%d checks passed.\n", g_passedChecks, g_totalChecks);
    return g_passedChecks == g_totalChecks ? 0 : 1;
}