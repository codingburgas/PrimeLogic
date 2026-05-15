#include "presentation/standings_panel.h"
#include "presentation/raylib_ui.h"

#include "logic/sorting.h"

#include <cstdio>
#include <cstring>

void renderStandingsSection(AppState* state, UiLayout* layout)
{
    beginPanel(layout, "Standings");

    if (drawButton(layout, "Sort by Points", 180))
    {
        bubbleSortStandingsByPoints(state->standings, state->standingCount);
        state->searchPerformed = false;
        state->searchResultCount = 0;
        strcpy_s(state->statusMessage, MAX_STATUS_LENGTH, "Standings sorted by points.");
    }

    addPanelSpace(layout, 12);

    const char* columns[8] = { "Team", "P", "W", "D", "L", "GF", "GA", "Pts" };
    const int widths[8] = { 480, 100, 100, 100, 100, 100, 100, 136 };
    drawTableHeader(layout, columns, widths, 8);

    int viewHeight = 500;
    int contentHeight = state->standingCount * layout->rowHeight;
    int* scrollOffset = &state->scrollOffsets[2];

    beginScrollableArea(layout->x, layout->y, layout->width, viewHeight, contentHeight, scrollOffset);

    int savedY = layout->y;
    layout->y = savedY - *scrollOffset;

    for (int index = 0; index < state->standingCount; index++)
    {
        char played[16];
        char wins[16];
        char draws[16];
        char losses[16];
        char goalsFor[16];
        char goalsAgainst[16];
        char points[16];

        snprintf(played, sizeof(played), "%d", state->standings[index].played);
        snprintf(wins, sizeof(wins), "%d", state->standings[index].wins);
        snprintf(draws, sizeof(draws), "%d", state->standings[index].draws);
        snprintf(losses, sizeof(losses), "%d", state->standings[index].losses);
        snprintf(goalsFor, sizeof(goalsFor), "%d", state->standings[index].goalsFor);
        snprintf(goalsAgainst, sizeof(goalsAgainst), "%d", state->standings[index].goalsAgainst);
        snprintf(points, sizeof(points), "%d", state->standings[index].points);

        const char* values[8] = {
            state->standings[index].teamName,
            played,
            wins,
            draws,
            losses,
            goalsFor,
            goalsAgainst,
            points
        };

        drawTableRow(layout, values, widths, 8);
    }

    endScrollableArea();

    layout->y = savedY + viewHeight;
}