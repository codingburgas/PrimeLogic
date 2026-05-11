#pragma once

#include "common/models.h"

bool loadTeams(const char* filePath, Team teams[], int* teamCount);
bool saveTeams(const char* filePath, const Team teams[], int teamCount);
int getNextTeamId(const Team teams[], int teamCount);