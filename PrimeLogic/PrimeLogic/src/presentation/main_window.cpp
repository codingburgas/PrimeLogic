#include "presentation/main_window.h"

#include "presentation/matches_panel.h"
#include "presentation/search_panel.h"
#include "presentation/statistics_panel.h"
#include "presentation/standings_panel.h"
#include "presentation/status_bar.h"
#include "presentation/teams_panel.h"
#include "presentation/raylib_ui.h"

#include "raylib.h"

void renderApp(AppState* state)
{
    ClearBackground(Color{ 245, 247, 250, 255 });

    drawUiText(24, 18, "PrimeLogic - Sports Score Manager", 32, DARKBLUE);

    const char* tabLabels[5] = { "Teams", "Matches", "Standings", "Search", "Statistics" };
    state->activeTab = drawTabBar(24, 64, 1232, 44, tabLabels, 5, state->activeTab);

    UiLayout layout;
    layout.x = 32;
    layout.y = 124;
    layout.width = 1216;
    layout.rowHeight = 36;

    if (state->activeTab == 0)
    {
        renderTeamsSection(state, &layout);
    }
    else if (state->activeTab == 1)
    {
        renderMatchesSection(state, &layout);
    }
    else if (state->activeTab == 2)
    {
        renderStandingsSection(state, &layout);
    }
    else if (state->activeTab == 3)
    {
        renderSearchSection(state, &layout);
    }
    else if (state->activeTab == 4)
    {
        renderStatisticsSection(state, &layout);
    }

    renderStatusSection(state, 24, 818, 1232);

    flushOverlays();
}
