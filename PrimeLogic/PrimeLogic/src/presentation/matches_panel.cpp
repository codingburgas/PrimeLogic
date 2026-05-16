#include "presentation/matches_panel.h"
#include "presentation/raylib_ui.h"

#include "logic/match_manager.h"
#include "logic/standings.h"

#include <cstdio>
#include <cstring>

static const char* getTeamNameById(const Team teams[], int teamCount, int teamId)
{
    /*
    Какво: Намира име на отбор по неговото ID за показване.
    Как: Търси в масива с отбори и връща намереното име или "Unknown".
    Защо: Таблицата с мачове е по-четима с имена, а не само с ID-та.
    */
    for (int index = 0; index < teamCount; index++)
    {
        if (teams[index].id == teamId)
        {
            return teams[index].name;
        }
    }

    return "Unknown";
}

void renderMatchesSection(AppState* state, UiLayout* layout)
{
    /*
    Какво: Рисува полетата за въвеждане на мач и таблицата с мачове.
    Как: Падащи менюта за избор на отбори по име, цифрови полета за голове, текстово за дата.
    Защо: Изборът на отбор по име е по-удобен от въвеждане на ID.
    */
    beginPanel(layout, "Matches");

    if (state->teamCount < 2)
    {
        drawUiText(layout->x, layout->y, "Add at least two teams in the Teams tab first.", 18, MAROON);
        addPanelSpace(layout, 30);
        return;
    }

    const char* teamNames[MAX_TEAMS];
    for (int index = 0; index < state->teamCount; index++)
    {
        teamNames[index] = state->teams[index].name;
    }

    drawDropdown(layout, "Home Team", teamNames, state->teamCount,
        &state->newMatchHomeTeamIndex, 101);
    drawDropdown(layout, "Away Team", teamNames, state->teamCount,
        &state->newMatchAwayTeamIndex, 102);

    drawIntInput(layout, "Home Goals", &state->newMatchHomeGoals, 4);
    drawIntInput(layout, "Away Goals", &state->newMatchAwayGoals, 5);
    drawTextInput(layout, "Date", state->newMatchDate, MAX_DATE_LENGTH, 6);

    if (drawButton(layout, "Add Match", 160))
    {
        int homeId = -1;
        int awayId = -1;

        if (state->newMatchHomeTeamIndex >= 0 && state->newMatchHomeTeamIndex < state->teamCount)
        {
            homeId = state->teams[state->newMatchHomeTeamIndex].id;
        }
        if (state->newMatchAwayTeamIndex >= 0 && state->newMatchAwayTeamIndex < state->teamCount)
        {
            awayId = state->teams[state->newMatchAwayTeamIndex].id;
        }

        if (homeId == -1 || awayId == -1)
        {
            strcpy_s(state->statusMessage, MAX_STATUS_LENGTH, "Please select both home and away teams.");
        }
        else if (addMatch(
                state->matches,
                &state->matchCount,
                homeId,
                awayId,
                state->newMatchHomeGoals,
                state->newMatchAwayGoals,
                state->newMatchDate,
                state->teams,
                state->teamCount,
                state->statusMessage))
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
            state->totalGoalsCalculated = false;
            state->newMatchHomeGoals = 0;
            state->newMatchAwayGoals = 0;
            strcpy_s(state->newMatchDate, MAX_DATE_LENGTH, "2026-04-22");
        }
    }

    addPanelSpace(layout, 12);

    const char* columns[5] = { "Date", "Home Team", "Score", "Away Team", "Match ID" };
    const int widths[5] = { 200, 380, 140, 380, 116 };
    drawTableHeader(layout, columns, widths, 5);

    int viewHeight = 320;
    int contentHeight = state->matchCount * layout->rowHeight;
    int* scrollOffset = &state->scrollOffsets[1];

    beginScrollableArea(layout->x, layout->y, layout->width, viewHeight, contentHeight, scrollOffset);

    int savedY = layout->y;
    layout->y = savedY - *scrollOffset;

    for (int index = 0; index < state->matchCount; index++)
    {
        char scoreText[32];
        char idText[16];
        snprintf(scoreText, sizeof(scoreText), "%d - %d", state->matches[index].homeGoals, state->matches[index].awayGoals);
        snprintf(idText, sizeof(idText), "%d", state->matches[index].id);

        const char* values[5] = {
            state->matches[index].date,
            getTeamNameById(state->teams, state->teamCount, state->matches[index].homeTeamId),
            scoreText,
            getTeamNameById(state->teams, state->teamCount, state->matches[index].awayTeamId),
            idText
        };

        drawTableRow(layout, values, widths, 5);
    }

    endScrollableArea();

    layout->y = savedY + viewHeight;
}
