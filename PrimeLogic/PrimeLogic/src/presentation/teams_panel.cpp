#include "presentation/teams_panel.h"
#include "presentation/raylib_ui.h"

#include "logic/standings.h"
#include "logic/team_manager.h"

#include <cstdio>

void renderTeamsSection(AppState* state, UiLayout* layout)
{
    /*
    Какво: Рисува полето за добавяне на отбор и таблицата с отбори.
    Как: Използва Raylib полета и бутон, извиква addTeam и пресмята класирането.
    Защо: Потребителите трябва лесно да добавят и виждат отборите.
    */
    beginPanel(layout, "Teams");
    drawTextInput(layout, "Team Name", state->newTeamName, MAX_NAME_LENGTH, 1);

    if (drawButton(layout, "Add Team", 140))
    {
        if (addTeam(state->teams, &state->teamCount, state->newTeamName, state->statusMessage))
        {
            calculateStandings(
                state->teams,
                state->teamCount,
                state->matches,
                state->matchCount,
                state->standings,
                &state->standingCount);

            state->dataChanged = true;
            state->searchPerformed = false;
            state->searchResultCount = 0;
            state->newTeamName[0] = '\0';
        }
    }

    addPanelSpace(layout, 12);

    const char* columns[2] = { "Team ID", "Team Name" };
    const int widths[2] = { 120, 1096 };
    drawTableHeader(layout, columns, widths, 2);

    int viewHeight = 480;
    int contentHeight = state->teamCount * layout->rowHeight;
    int* scrollOffset = &state->scrollOffsets[0];

    beginScrollableArea(layout->x, layout->y, layout->width, viewHeight, contentHeight, scrollOffset);

    int savedY = layout->y;
    layout->y = savedY - *scrollOffset;

    for (int index = 0; index < state->teamCount; index++)
    {
        char idText[16];
        snprintf(idText, sizeof(idText), "%d", state->teams[index].id);

        const char* values[2] = { idText, state->teams[index].name };
        drawTableRow(layout, values, widths, 2);
    }

    endScrollableArea();

    layout->y = savedY + viewHeight;
}
