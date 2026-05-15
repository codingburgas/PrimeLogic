#include "data/matches_file.h"

#include "common/constants.h"
#include "data/csv_split.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>

bool loadMatches(const char* filePath, Match matches[], int* matchCount)
{
    FILE* file = nullptr;
    *matchCount = 0;

    if (fopen_s(&file, filePath, "r") != 0 || file == nullptr)
    {
        return true;
    }

    char line[256];
    bool isFirstLine = true;

    while (fgets(line, sizeof(line), file) != nullptr)
    {
        if (isFirstLine)
        {
            isFirstLine = false;
            continue;
        }

        if (*matchCount >= MAX_MATCHES)
        {
            break;
        }

        char* tokens[6] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
        int tokenCount = splitCsvLine(line, tokens, 6);

        if (tokenCount < 6)
        {
            continue;
        }

        bool hasEmpty = false;
        for (int t = 0; t < 6; t++)
        {
            if (tokens[t] == nullptr || tokens[t][0] == '\0')
            {
                hasEmpty = true;
                break;
            }
        }
        if (hasEmpty)
        {
            continue;
        }

        matches[*matchCount].id = atoi(tokens[0]);
        matches[*matchCount].homeTeamId = atoi(tokens[1]);
        matches[*matchCount].awayTeamId = atoi(tokens[2]);
        matches[*matchCount].homeGoals = atoi(tokens[3]);
        matches[*matchCount].awayGoals = atoi(tokens[4]);
        strcpy_s(matches[*matchCount].date, MAX_DATE_LENGTH, tokens[5]);
        (*matchCount)++;
    }

    fclose(file);
    return true;
}

bool saveMatches(const char* filePath, const Match matches[], int matchCount)
{
    FILE* file = nullptr;

    if (fopen_s(&file, filePath, "w") != 0 || file == nullptr)
    {
        return false;
    }

    fprintf(file, "id,homeTeamId,awayTeamId,homeGoals,awayGoals,date\n");

    for (int index = 0; index < matchCount; index++)
    {
        fprintf(
            file,
            "%d,%d,%d,%d,%d,%s\n",
            matches[index].id,
            matches[index].homeTeamId,
            matches[index].awayTeamId,
            matches[index].homeGoals,
            matches[index].awayGoals,
            matches[index].date);
    }

    fclose(file);
    return true;
}

int getNextMatchId(const Match matches[], int matchCount)
{
    int maxId = 0;

    for (int index = 0; index < matchCount; index++)
    {
        if (matches[index].id > maxId)
        {
            maxId = matches[index].id;
        }
    }

    return maxId + 1;
}