#pragma once

#include "common/models.h"

void calculateStandings(
    const Team teams[],
    int teamCount,
    const Match matches[],
    int matchCount,
    StandingRow standings[],
    int* standingCount);