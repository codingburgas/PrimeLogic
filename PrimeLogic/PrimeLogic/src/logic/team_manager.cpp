#include "logic/team_manager.h"

#include "common/constants.h"
#include "data/teams_file.h"

#include <cctype>
#include <cstring>

static void trimText(const char* source, char destination[])
{
    int start = 0;
    int end = (int)strlen(source) - 1;
    int writeIndex = 0;

    while (source[start] != '\0' && isspace((unsigned char)source[start]))
    {
        start++;
    }

    while (end >= start && isspace((unsigned char)source[end]))
    {
        end--;
    }

    for (int index = start; index <= end && writeIndex < MAX_NAME_LENGTH - 1; index++)
    {
        destination[writeIndex] = source[index];
        writeIndex++;
    }

    destination[writeIndex] = '\0';
}

static bool areEqualIgnoreCase(const char* left, const char* right)
{
    int index = 0;

    while (left[index] != '\0' && right[index] != '\0')
    {
        if (tolower((unsigned char)left[index]) != tolower((unsigned char)right[index]))
        {
            return false;
        }

        index++;
    }

    return left[index] == '\0' && right[index] == '\0';
}

bool validateTeamName(const char* teamName, const Team teams[], int teamCount, char message[])
{
    char trimmedName[MAX_NAME_LENGTH];
    trimText(teamName, trimmedName);

    if (trimmedName[0] == '\0')
    {
        strcpy_s(message, MAX_STATUS_LENGTH, "Team name cannot be empty.");
        return false;
    }

    for (int index = 0; index < teamCount; index++)
    {
        if (areEqualIgnoreCase(trimmedName, teams[index].name))
        {
            strcpy_s(message, MAX_STATUS_LENGTH, "Team name already exists.");
            return false;
        }
    }

    message[0] = '\0';
    return true;
}

bool addTeam(Team teams[], int* teamCount, const char* teamName, char message[])
{
    char trimmedName[MAX_NAME_LENGTH];

    if (*teamCount >= MAX_TEAMS)
    {
        strcpy_s(message, MAX_STATUS_LENGTH, "Maximum team count reached.");
        return false;
    }

    if (!validateTeamName(teamName, teams, *teamCount, message))
    {
        return false;
    }

    trimText(teamName, trimmedName);
    teams[*teamCount].id = getNextTeamId(teams, *teamCount);
    strcpy_s(teams[*teamCount].name, MAX_NAME_LENGTH, trimmedName);
    (*teamCount)++;

    strcpy_s(message, MAX_STATUS_LENGTH, "Team added successfully.");
    return true;
}