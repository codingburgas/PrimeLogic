#include <cctype>
#include <cstring>

static bool containsIgnoreCase(const char* text, const char* query)
{
    int queryLength = (int)strlen(query);

    if (queryLength == 0)
    {
        return false;
    }

    for (int start = 0; text[start] != '\0'; start++)
    {
        int queryIndex = 0;

        while (text[start + queryIndex] != '\0' && query[queryIndex] != '\0')
        {
            char left = (char)tolower((unsigned char)text[start + queryIndex]);
            char right = (char)tolower((unsigned char)query[queryIndex]);

            if (left != right)
            {
                break;
            }

            queryIndex++;
        }

        if (queryIndex == queryLength)
        {
            return true;
        }
    }

    return false;
}

int linearSearchTeamByName(
    const StandingRow standings[],
    int standingCount,
    const char* query,
    int resultIndexes[],
    int maxResults)
{
    int resultCount = 0;

    for (int index = 0; index < standingCount; index++)
    {
        if (containsIgnoreCase(standings[index].teamName, query))
        {
            if (resultCount < maxResults)
            {
                resultIndexes[resultCount] = index;
                resultCount++;
            }
        }
    }

    return resultCount;
}