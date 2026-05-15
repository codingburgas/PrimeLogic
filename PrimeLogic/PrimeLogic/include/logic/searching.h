#pragma once

#include "common/models.h"

int linearSearchTeamByName(
    const StandingRow standings[],
    int standingCount,
    const char* query,
    int resultIndexes[],
    int maxResults);