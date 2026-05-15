#include "presentation/main_window.h"

#include "common/app_state.h"
#include "common/constants.h"
#include "data/matches_file.h"
#include "data/teams_file.h"
#include "logic/standings.h"

#include "raylib.h"

#include <cstring>

static void initializeAppState(AppState* state)
{
    memset(state, 0, sizeof(AppState));
    strcpy_s(state->newMatchDate, MAX_DATE_LENGTH, "2026-04-22");
    strcpy_s(state->statusMessage, MAX_STATUS_LENGTH, "Ready.");
}

static void loadApplicationData(AppState* state)
{
    loadTeams("data/teams.csv", state->teams, &state->teamCount);
    loadMatches("data/matches.csv", state->matches, &state->matchCount);
    calculateStandings(
        state->teams,
        state->teamCount,
        state->matches,
        state->matchCount,
        state->standings,
        &state->standingCount);
}

static void saveApplicationData(AppState* state)
{
    if (!state->dataChanged)
    {
        return;
    }

    if (saveTeams("data/teams.csv", state->teams, state->teamCount) &&
        saveMatches("data/matches.csv", state->matches, state->matchCount))
    {
        strcpy_s(state->statusMessage, MAX_STATUS_LENGTH, "Data saved successfully.");
        state->dataChanged = false;
    }
    else
    {
        strcpy_s(state->statusMessage, MAX_STATUS_LENGTH, "Unable to save data files.");
    }
}

int main()
{
    AppState appState;
    initializeAppState(&appState);
    loadApplicationData(&appState);

    InitWindow(1280, 860, "PrimeLogic - Sports Score Manager");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        renderApp(&appState);
        EndDrawing();

        saveApplicationData(&appState);
    }

    CloseWindow();
    return 0;
}