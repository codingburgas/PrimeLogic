#pragma once

#include "common/constants.h"
#include "common/models.h"

struct AppState
{
    Team teams[MAX_TEAMS];
    int teamCount;

    Match matches[MAX_MATCHES];
    int matchCount;

    StandingRow standings[MAX_TEAMS];
    int standingCount;

    char newTeamName[MAX_NAME_LENGTH];

    int newMatchHomeTeamIndex;
    int newMatchAwayTeamIndex;
    int newMatchHomeGoals;
    int newMatchAwayGoals;
    char newMatchDate[MAX_DATE_LENGTH];

    int activeTab;
    int scrollOffsets[5];

    char searchQuery[MAX_NAME_LENGTH];
    int searchResultIndexes[MAX_TEAMS];
    int searchResultCount;
    bool searchPerformed;

    int totalGoals;
    bool totalGoalsCalculated;

    char statusMessage[MAX_STATUS_LENGTH];
    bool dataChanged;
};