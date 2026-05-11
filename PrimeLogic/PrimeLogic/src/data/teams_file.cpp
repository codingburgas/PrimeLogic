#include "data/teams_file.h"

#include "common/constants.h"
#include "data/csv_split.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>

bool loadTeams(const char* filePath, Team teams[], int* teamCount)
{
    FILE* file = nullptr;
    *teamCount = 0;

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

        if (*teamCount >= MAX_TEAMS)
        {
            break;
        }

        char* tokens[2] = { nullptr, nullptr };
        int tokenCount = splitCsvLine(line, tokens, 2);

        if (tokenCount < 2 || tokens[0][0] == '\0' || tokens[1][0] == '\0')
        {
            continue;
        }

        teams[*teamCount].id = atoi(tokens[0]);
        strcpy_s(teams[*teamCount].name, MAX_NAME_LENGTH, tokens[1]);
        (*teamCount)++;
    }

    fclose(file);
    return true;
}

bool saveTeams(const char* filePath, const Team teams[], int teamCount)
{
    FILE* file = nullptr;

    if (fopen_s(&file, filePath, "w") != 0 || file == nullptr)
    {
        return false;
    }

    fprintf(file, "id,name\n");

    for (int index = 0; index < teamCount; index++)
    {
        fprintf(file, "%d,%s\n", teams[index].id, teams[index].name);
    }

    fclose(file);
    return true;
}

int getNextTeamId(const Team teams[], int teamCount)
{
    int maxId = 0;

    for (int index = 0; index < teamCount; index++)
    {
        if (teams[index].id > maxId)
        {
            maxId = teams[index].id;
        }
    }

    return maxId + 1;
}