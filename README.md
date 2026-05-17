<p align="center">
  <img src="https://github.com/codingburgas/PrimeLogic/blob/main/PrimeLogic/PrimeLogic/docs/PrimeLogic_Logo.png?raw=true%22 alt="PrimeLogic Logo" width="500" />
</p>
 
# PrimeLogic — Football Score Manager
 
<p align="center">
  <a href="#-about-the-project"><img src="https://img.shields.io/badge/ABOUT%20THE%20PROJECT-2196F3?style=for-the-badge" /></a>
  <a href="#-how-to-build-and-run"><img src="https://img.shields.io/badge/HOW%20TO%20BUILD%20AND%20RUN-4CAF50?style=for-the-badge" /></a>
  <a href="#️-technologies"><img src="https://img.shields.io/badge/TECHNOLOGIES-FF9800?style=for-the-badge" /></a>
  <a href="#-our-team"><img src="https://img.shields.io/badge/OUR%20TEAM-9C27B0?style=for-the-badge" /></a>
</p>
 
---
 
## 🧠 About the Project
 
PrimeLogic is a **graphical desktop application** built in C++ for managing football teams and match results. It features a clean, tab-based GUI powered by [Raylib](https://www.raylib.com/) and stores all data persistently in CSV files.
 
The project follows a strict **three-layer architecture** — Presentation → Logic → Data — keeping the GUI, business rules, and file I/O fully separated. It demonstrates core C++ concepts including Bubble Sort, Linear Search, and recursion.
 
### Key Features
 
- **Teams tab** — Add new football teams; view the full team list in a scrollable table.
- **Matches tab** — Select home and away teams from dropdowns, enter goals and date, record the match.
- **Standings tab** — Full league table with Played, W/D/L, Goals For/Against, Goal Difference, and Points. Sort by points with one click (Bubble Sort).
- **Search tab** — Linear search by team name; results shown instantly.
- **Statistics tab** — Total goals across all matches calculated recursively; team and match counts.
- **Status bar** — Live feedback messages at the bottom of every screen.
- **CSV persistence** — All teams and matches are saved to `data/teams.csv` and `data/matches.csv` on every change.
 
### Application Structure (5 Tabs)
 
| Tab | Description |
|---|---|
| 🏟️ Teams | Register teams; scrollable team list |
| ⚽ Matches | Pick teams via dropdown; log match result and date |
| 📊 Standings | Full league table sorted by points |
| 🔍 Search | Find a team by name using Linear Search |
| 📈 Statistics | Recursive total goals; team & match counters |
 
### Points System
 
| Result | Points |
|---|---|
| Win | 3 |
| Draw | 1 |
| Loss | 0 |
 
---
 
## 📁 Project Structure
 
```
PrimeLogic/
├── src/
│   ├── main.cpp                        — entry point: opens Raylib window, loads data, runs render loop
│   ├── presentation/
│   │   ├── main_window.cpp             — title bar, tab strip, active panel, status bar
│   │   ├── raylib_ui.cpp               — shared UI helpers: buttons, text fields, dropdowns, scrollable tables, TTF font
│   │   ├── teams_panel.cpp             — Teams tab: add team form + team table
│   │   ├── matches_panel.cpp           — Matches tab: dropdowns, add match form + match table
│   │   ├── standings_panel.cpp         — Standings tab: sort button + full standings table
│   │   ├── search_panel.cpp            — Search tab: name input + search results
│   │   ├── statistics_panel.cpp        — Statistics tab: recursive goal sum + counters
│   │   └── status_bar.cpp              — fixed status bar at the bottom of the window
│   ├── logic/
│   │   ├── team_manager.cpp            — add/validate teams
│   │   ├── match_manager.cpp           — add/validate matches, compute standings row
│   │   ├── standings.cpp               — build standings from match history
│   │   ├── sorting.cpp                 — Bubble Sort on StandingRow[] by points
│   │   ├── searching.cpp               — Linear Search: find teams by name
│   │   └── goals_recursion.cpp         — recursive sum of all goals across all matches
│   └── data/
│       ├── csv_split.cpp               — shared CSV line parser (splitCsvLine)
│       ├── teams_file.cpp              — read/write data/teams.csv
│       └── matches_file.cpp            — read/write data/matches.csv
├── include/
│   ├── common/
│   │   ├── constants.h                 — MAX_TEAMS, MAX_MATCHES, MAX_NAME_LENGTH, etc.
│   │   ├── models.h                    — Team, Match, StandingRow structs
│   │   └── app_state.h                 — AppState: all runtime data used by the GUI
│   ├── presentation/                   — headers for every presentation source file
│   ├── logic/                          — headers for every logic source file
│   └── data/                           — headers for every data source file
├── data/
│   ├── teams.csv                       — persistent team storage (id, name)
│   └── matches.csv                     — persistent match storage (id, homeTeamId, awayTeamId, homeGoals, awayGoals, date)
├── tests/
│   ├── checks_main.cpp                 — unit checks for logic functions
│   ├── sample_teams.csv                — test fixture: teams
│   └── sample_matches.csv              — test fixture: matches
├── third_party/
│   └── raylib/                         — bundled Raylib headers and .lib/.dll
└── PrimeLogic.slnx                     — Visual Studio solution
```
 
---
 
## 🚀 How to Build and Run
 
### Prerequisites
 
- [Visual Studio 2022](https://visualstudio.microsoft.com/) with the **Desktop development with C++** workload, **or**
- A C++17 compiler (GCC / MinGW) on Linux or macOS
 
Raylib is already bundled under `third_party/raylib/` — no separate install needed.
 
### Visual Studio (recommended on Windows)
 
1. Open `PrimeLogic.slnx` in Visual Studio 2022.
2. Set the configuration to **Debug | x64** (or Release | x64).
3. Press **Ctrl+Shift+B** to build, then **F5** to run.
 
### MinGW (Windows)
 
```cmd
g++ -std=c++17 src/main.cpp src/presentation/*.cpp src/logic/*.cpp src/data/*.cpp ^
    -Iinclude -Ithird_party/raylib/include ^
    -Lthird_party/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm ^
    -o PrimeLogic.exe
PrimeLogic.exe
```
 
### g++ (Linux / macOS)
 
Install Raylib system-wide (`sudo apt install libraylib-dev` or `brew install raylib`), then:
 
```bash
g++ -std=c++17 src/main.cpp src/presentation/*.cpp src/logic/*.cpp src/data/*.cpp \
    -Iinclude -lraylib -lGL -lm -lpthread -ldl \
    -o PrimeLogic
./PrimeLogic
```
 
> **Note:** On Windows the compiled binary expects `raylib.dll` to be in the same directory as the `.exe`. It is copied there automatically when building via Visual Studio.
 
---
 
## 🛠️ Technologies
 
<p align="center">
  <a href="https://isocpp.org/" title="C++17"><img src="https://skillicons.dev/icons?i=cpp" alt="C++" width="50" /></a>&nbsp;&nbsp;
  <a href="https://visualstudio.microsoft.com/" title="Visual Studio 2022"><img src="https://skillicons.dev/icons?i=visualstudio" alt="Visual Studio" width="50" /></a>&nbsp;&nbsp;
  <a href="https://git-scm.com/" title="Git"><img src="https://skillicons.dev/icons?i=git" alt="Git" width="50" /></a>&nbsp;&nbsp;
  <a href="https://github.com/" title="GitHub"><img src="https://skillicons.dev/icons?i=github" alt="GitHub" width="50" /></a>&nbsp;&nbsp;
  <a href="https://www.raylib.com/" title="Raylib — GUI & rendering"><img src="https://assets.streamlinehq.com/image/private/w_300,h_300,ar_1/f_auto/v1/icons/logos/raylib-gu84w0m6k8djljr8ek4sq9.png/raylib-j326e49obzmg0j0h2zyt.png?_a=DATAiZAAZAA0%22" alt="Raylib" width="50" /></a>&nbsp;&nbsp;
  <a href="https://www.microsoft.com/en-us/microsoft-365/powerpoint" title="PowerPoint"><img src="https://upload.wikimedia.org/wikipedia/commons/thumb/d/df/Microsoft_Office_PowerPoint_%282025%E2%80%93present%29.svg/960px-Microsoft_Office_PowerPoint_%282025%E2%80%93present%29.svg.png%22" alt="PowerPoint" width="50" /></a>&nbsp;&nbsp;
  <a href="https://www.microsoft.com/en-us/microsoft-365/word" title="Word"><img src="https://upload.wikimedia.org/wikipedia/commons/thumb/e/e8/Microsoft_Office_Word_%282025%E2%80%93present%29.svg/960px-Microsoft_Office_Word_%282025%E2%80%93present%29.svg.png%22" alt="Word" width="50" /></a>&nbsp;&nbsp;
  <a href="https://www.microsoft.com/en-us/microsoft-teams/group-chat-software" title="Microsoft Teams"><img src="https://upload.wikimedia.org/wikipedia/commons/thumb/0/07/Microsoft_Office_Teams_%282025%E2%80%93present%29.svg/1280px-Microsoft_Office_Teams_%282025%E2%80%93present%29.svg.png%22" alt="Teams" width="50" /></a>
</p>
 
| Tool / Library | Purpose |
|---|---|
| [C++17](https://isocpp.org/) | Core language standard |
| [Raylib 5.x](https://www.raylib.com/) | GUI rendering, input handling, TTF font loading |
| [Visual Studio 2022](https://visualstudio.microsoft.com/) | IDE and build system (`.slnx`) |
| [Git](https://git-scm.com/) & [GitHub](https://github.com/) | Version control and collaboration |
| CSV (custom parser) | Persistent data storage via `splitCsvLine` |
 
---
 
## 👥 Our Team
 
| Name | Role |
|---|---|
| [Tatiana Kovacheva](https://github.com/TatianaKovacheva) | Scrum Trainer |
| [Plamen Rusev](https://github.com/PNRusev24) | Back-End Developer |
| [Mustafa Husein](https://github.com/MustafaMustafa25) | Front-End Developer |
| [Selin Vatansever](https://github.com/SAVatansever24) | QA Engineer |
