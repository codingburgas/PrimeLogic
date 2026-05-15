#pragma once

#include "common/models.h"

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
    char message[]);

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
    char message[]);
