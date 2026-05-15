#include "presentation/main_window.h"

#include "presentation/matches_panel.h"
#include "presentation/search_panel.h"
#include "presentation/statistics_panel.h"
#include "presentation/standings_panel.h"
#include "presentation/status_bar.h"
#include "presentation/teams_panel.h"

#include "raylib.h"

void renderApp(AppState* state)
{
    ClearBackground(Color{ 245, 247, 250, 255 });
    DrawText("PrimeLogic - Sports Score Manager", 24, 18, 28, DARKBLUE);

    UiLayout leftLayout = { 24, 64, 430, 30 };
    UiLayout rightLayout = { 490, 64, 760, 30 };

    renderTeamsSection(state, &leftLayout);
    addPanelSpace(&leftLayout, 18);
    renderMatchesSection(state, &leftLayout);

    renderSearchSection(state, &rightLayout);
    addPanelSpace(&rightLayout, 18);
    renderStatisticsSection(state, &rightLayout);
    addPanelSpace(&rightLayout, 18);
    renderStandingsSection(state, &rightLayout);
    addPanelSpace(&rightLayout, 18);
    renderStatusSection(state, &rightLayout);
}