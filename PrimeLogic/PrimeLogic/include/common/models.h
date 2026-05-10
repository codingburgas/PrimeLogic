#pragma once

#include "common/constants.h"

struct Team
{
    int id;
    char name[MAX_NAME_LENGTH];
};

struct Match
{
    int id;
    int homeTeamId;
    int awayTeamId;
    int homeGoals;
    int awayGoals;
    char date[MAX_DATE_LENGTH];
};

struct StandingRow
{
    int teamId;
    char teamName[MAX_NAME_LENGTH];
    int played;
    int wins;
    int draws;
    int losses;
    int goalsFor;
    int goalsAgainst;
    int goalDifference;
    int points;
};