#pragma once

#include "common/models.h"

bool validateTeamName(const char* teamName, const Team teams[], int teamCount, char message[]);
bool addTeam(Team teams[], int* teamCount, const char* teamName, char message[]);