#include "data/csv_split.h"

int splitCsvLine(char* line, char* tokens[], int maxTokens)
{
    int count = 0;
    int fieldStart = 0;
    int index = 0;

    while (line[index] != '\0' && count < maxTokens)
    {
        if (line[index] == ','  line[index] == '\r'
            line[index] == '\n')
        {
            line[index] = '\0';
            tokens[count] = &line[fieldStart];
            count++;
            fieldStart = index + 1;
        }
        index++;
    }

    if (count < maxTokens && fieldStart <= index)
    {
        tokens[count] = &line[fieldStart];
        count++;
    }

    return count;
}
