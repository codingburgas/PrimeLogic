#include "presentation/statistics_panel.h"
#include "presentation/raylib_ui.h"

#include "logic/goals_recursion.h"

#include <cstdio>
#include <cstring>

void renderStatisticsSection(AppState* state, UiLayout* layout)
{
    beginPanel(layout, "Statistics");

    if (drawButton(layout, "Calculate Total Goals", 240))
    {
        state->totalGoals = sumGoalsRecursive(state->matches, state->matchCount, 0);
        state->totalGoalsCalculated = true;
        strcpy_s(state->statusMessage, MAX_STATUS_LENGTH, "Total goals calculated.");
    }

    addPanelSpace(layout, 12);

    char totalGoalsText[80];
    if (state->totalGoalsCalculated)
    {
        snprintf(totalGoalsText, sizeof(totalGoalsText), "Total goals in all matches: %d", state->totalGoals);
    }
    else
    {
        strcpy_s(totalGoalsText, sizeof(totalGoalsText), "Total goals in all matches: not calculated yet.");
    }

    drawUiText(layout->x, layout->y, totalGoalsText, 22, BLACK);
    addPanelSpace(layout, 30);

    char matchesText[80];
    snprintf(matchesText, sizeof(matchesText), "Number of matches recorded: %d", state->matchCount);
    drawUiText(layout->x, layout->y, matchesText, 18, DARKGRAY);
    addPanelSpace(layout, 24);

    char teamsText[80];
    snprintf(teamsText, sizeof(teamsText), "Number of teams: %d", state->teamCount);
    drawUiText(layout->x, layout->y, teamsText, 18, DARKGRAY);
}
