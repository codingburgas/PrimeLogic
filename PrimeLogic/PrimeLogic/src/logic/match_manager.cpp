#include "logic/match_manager.h"

#include "common/constants.h"
#include "data/matches_file.h"

#include <cctype>
#include <cstring>

static int findTeamIndexById(const Team teams[], int teamCount, int teamId)
{
    for (int index = 0; index < teamCount; index++)
    {
        if (teams[index].id == teamId)
        {
            return index;
        }
    }

    return -1;
}

static bool isDateBlank(const char* date)
{
    for (int index = 0; date[index] != '\0'; index++)
    {
        if (!isspace((unsigned char)date[index]))
        {
            return false;
        }
    }

    return true;
}

bool validateMatch(
    int homeTeamId,
    int awayTeamId,
    int homeGoals,
    int awayGoals,
    const char* date,
    const Team teams[],
    int teamCount,
    const Match matches[],
    int matchCount,
    char message[])
{
    (void)matches;
    (void)matchCount;

    if (teamCount < 2)
    {
        strcpy_s(message, MAX_STATUS_LENGTH, "Add at least two teams first.");
        return false;
    }

    if (homeTeamId == awayTeamId)
    {
        strcpy_s(message, MAX_STATUS_LENGTH, "A team cannot play against itself.");
        return false;
    }

    if (findTeamIndexById(teams, teamCount, homeTeamId) == -1 ||
        findTeamIndexById(teams, teamCount, awayTeamId) == -1)
    {
        strcpy_s(message, MAX_STATUS_LENGTH, "Enter valid team IDs.");
        return false;
    }

    if (homeGoals < 0 || awayGoals < 0)
    {
        strcpy_s(message, MAX_STATUS_LENGTH, "Goals cannot be negative.");
        return false;
    }

    if (date == nullptr || isDateBlank(date))
    {
        strcpy_s(message, MAX_STATUS_LENGTH, "Date cannot be empty.");
        return false;
    }

    message[0] = '\0';
    return true;
}

bool addMatch(
    Match matches[],
    int* matchCount,
    int homeTeamId,
    int awayTeamId,
    int homeGoals,
    int awayGoals,
    const char* date,
    const Team teams[],
    int teamCount,
    char message[])
{
    if (*matchCount >= MAX_MATCHES)
    {
        strcpy_s(message, MAX_STATUS_LENGTH, "Maximum match count reached.");
        return false;
    }

    if (!validateMatch(
        homeTeamId,
        awayTeamId,
        homeGoals,
        awayGoals,
        date,
        teams,
        teamCount,
        matches,
        *matchCount,
        message))
    {
        return false;
    }

    matches[*matchCount].id = getNextMatchId(matches, *matchCount);
    matches[*matchCount].homeTeamId = homeTeamId;
    matches[*matchCount].awayTeamId = awayTeamId;
    matches[*matchCount].homeGoals = homeGoals;
    matches[*matchCount].awayGoals = awayGoals;
    strcpy_s(matches[*matchCount].date, MAX_DATE_LENGTH, date);
    (*matchCount)++;

    strcpy_s(message, MAX_STATUS_LENGTH, "Match added successfully.");
    return true;
}