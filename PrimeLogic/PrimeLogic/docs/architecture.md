# Архитектура на проекта

## Общ модел

Проектът използва трислойна архитектура:

```text
Presentation -> Logic -> Data
```

Целта е GUI кодът да не съдържа правилата на приложението, а файловият код да не бъде смесен с визуализацията.

## Presentation слой

Presentation слоят е графичният интерфейс. В текущата версия той е направен с Raylib и
използва интерфейс с пет таба. Един таб е активен в даден момент, останалите са скрити.

Файлове:

- `src/main.cpp` - стартира Raylib прозореца, зарежда TTF шрифт, зарежда данните и извиква `renderApp`
- `include/presentation/main_window.h`
- `src/presentation/main_window.cpp` - заглавие, лента с табове, активен панел, статус лента
- `include/presentation/raylib_ui.h`
- `src/presentation/raylib_ui.cpp` - UI помощници: бутони, полета, **падащи менюта**, **табове**, **скролваеми зони**, TTF шрифт
- `include/presentation/teams_panel.h`
- `src/presentation/teams_panel.cpp` - таб **Teams**: добавяне на отбор и таблица
- `include/presentation/matches_panel.h`
- `src/presentation/matches_panel.cpp` - таб **Matches**: дропдауни за избор на отбори по име, добавяне на мач, таблица
- `include/presentation/standings_panel.h`
- `src/presentation/standings_panel.cpp` - таб **Standings**: бутон за сортиране и таблица с цели имена на колони
- `include/presentation/search_panel.h`
- `src/presentation/search_panel.cpp` - таб **Search**: търсене по име
- `include/presentation/statistics_panel.h`
- `src/presentation/statistics_panel.cpp` - таб **Statistics**: рекурсивна сума на голове + бройки отбори/мачове
- `include/presentation/status_bar.h`
- `src/presentation/status_bar.cpp` - фиксирана статус лента в долната част на прозореца

Отговорности:

- създава и рисува потребителския интерфейс
- управлява навигацията между петте таба чрез лента в горната част
- рисува бутони, текстови полета, падащи менюта и таблици чрез Raylib
- използва TTF шрифт (Segoe UI) за по-четим текст
- скролва дълги таблици с колелцето на мишката (`BeginScissorMode`)
- приема вход от потребителя
- извиква функции от logic слоя
- показва статус съобщения

## Logic слой

Logic слоят съдържа правилата и алгоритмите на приложението.

Файлове:

- `include/logic/team_manager.h`
- `src/logic/team_manager.cpp`
- `include/logic/match_manager.h`
- `src/logic/match_manager.cpp`
- `include/logic/standings.h`
- `src/logic/standings.cpp`
- `include/logic/sorting.h`
- `src/logic/sorting.cpp`
- `include/logic/searching.h`
- `src/logic/searching.cpp`
- `include/logic/goals_recursion.h`
- `src/logic/goals_recursion.cpp`

Отговорности:

- валидира имена на отбори
- валидира мачове
- добавя отбори и мачове в масивите
- пресмята класиране
- сортира класирането чрез Bubble Sort
- търси отбори чрез Linear Search
- изчислява общия брой голове чрез рекурсия

## Data слой

Data слоят отговаря за четене и записване на CSV файловете.

Файлове:

- `include/data/csv_split.h`
- `src/data/csv_split.cpp` - споделена функция `splitCsvLine` за разделяне на CSV ред (без `strtok_s`)
- `include/data/teams_file.h`
- `src/data/teams_file.cpp`
- `include/data/matches_file.h`
- `src/data/matches_file.cpp`

Отговорности:

- зарежда `data/teams.csv`
- записва `data/teams.csv`
- зарежда `data/matches.csv`
- записва `data/matches.csv`
- намира следващо свободно ID за отбор или мач
- не дублира CSV парсинг кода - и двата файла извикват `splitCsvLine` от `csv_split.cpp`

## Общи структури

Файлове:

- `include/common/constants.h`
- `include/common/models.h`
- `include/common/app_state.h`

Основни структури:

- `Team` - ID и име на отбор
- `Match` - ID, домакин, гост, голове и дата
- `StandingRow` - ред от класирането
- `AppState` - всички данни, които GUI-то използва

## Поток на данните

1. `main.cpp` създава `AppState`.
2. Data слоят зарежда CSV файловете в масивите.
3. Logic слоят пресмята началното класиране.
4. `main.cpp` отваря Raylib прозореца и зарежда TTF шрифта.
5. На всеки кадър `renderApp` рисува заглавие, лентата с табове и активния панел.
6. Активният таб се определя от `state->activeTab`; кликане на таб обновява тази стойност.
7. Presentation панелите извикват logic функции при натискане на бутони (`addTeam`, `addMatch`, `bubbleSortStandingsByPoints`, `linearSearchTeamByName`, `sumGoalsRecursive`).
8. При успешна промяна `dataChanged` става `true`.
9. `main.cpp` записва CSV файловете чрез data слоя.
10. При затваряне `unloadUiFont` и `CloseWindow` освобождават ресурсите.