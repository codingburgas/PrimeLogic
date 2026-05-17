#include "presentation/search_panel.h"
#include "presentation/raylib_ui.h"

#include "logic/searching.h"

#include <cctype>
#include <cstdio>
#include <cstring>

static bool isBlankText(const char* text)
{
    for (int index = 0; text[index] != '\0'; index++)
    {
        if (!isspace((unsigned char)text[index]))
        {
            return false;
        }
    }

    return true;
}

void renderSearchSection(AppState* state, UiLayout* layout)
{
    beginPanel(layout, "Search");
    drawTextInput(layout, "Team Name", state->searchQuery, MAX_NAME_LENGTH, 7);

    if (drawButton(layout, "Search", 140))
    {
        if (isBlankText(state->searchQuery))
        {
            strcpy_s(state->statusMessage, MAX_STATUS_LENGTH, "Enter a team name to search.");
            state->searchResultCount = 0;
            state->searchPerformed = false;
        }
        else
        {
            state->searchResultCount = linearSearchTeamByName(
                state->standings,
                state->standingCount,
                state->searchQuery,
                state->searchResultIndexes,
                MAX_TEAMS);
            state->searchPerformed = true;

            if (state->searchResultCount == 0)
            {
                strcpy_s(state->statusMessage, MAX_STATUS_LENGTH, "No matching teams found.");
            }
            else
            {
                strcpy_s(state->statusMessage, MAX_STATUS_LENGTH, "Search completed.");
            }
        }
    }

    addPanelSpace(layout, 12);

    if (!state->searchPerformed)
    {
        drawUiText(layout->x, layout->y, "Type a team name and press Search.", 18, DARKGRAY);
        return;
    }

    const char* columns[4] = { "Team Name", "Played", "Goal Difference", "Points" };
    const int widths[4] = { 460, 220, 280, 256 };
    drawTableHeader(layout, columns, widths, 4);

    int viewHeight = 460;
    int contentHeight = state->searchResultCount * layout->rowHeight;
    int* scrollOffset = &state->scrollOffsets[3];

    beginScrollableArea(layout->x, layout->y, layout->width, viewHeight, contentHeight, scrollOffset);

    int savedY = layout->y;
    layout->y = savedY - *scrollOffset;

    for (int index = 0; index < state->searchResultCount; index++)
    {
        int standingIndex = state->searchResultIndexes[index];
        char played[16];
        char goalDifference[16];
        char points[16];

        snprintf(played, sizeof(played), "%d", state->standings[standingIndex].played);
        snprintf(goalDifference, sizeof(goalDifference), "%d", state->standings[standingIndex].goalDifference);
        snprintf(points, sizeof(points), "%d", state->standings[standingIndex].points);

        const char* values[4] = {
            state->standings[standingIndex].teamName,
            played,
            goalDifference,
            points
        };

        drawTableRow(layout, values, widths, 4);
    }

    endScrollableArea();

    layout->y = savedY + viewHeight;
}
