#pragma once

#include "common/models.h"

bool loadMatches(const char* filePath, Match matches[], int* matchCount);
bool saveMatches(const char* filePath, const Match matches[], int matchCount);
int getNextMatchId(const Match matches[], int matchCount);
