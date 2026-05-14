# Блок схема

## Стартиране на приложението

`main()`  
-> `initializeAppState()`  
-> `loadTeams()` и `loadMatches()`  
-> `calculateStandings()`  
-> `InitWindow()` (1280x860)  
-> `initUiFont()` (зарежда Segoe UI TTF)  
-> основен Raylib цикъл  
-> `renderApp()` (заглавие + табове + активен панел + статус)  
-> при затваряне: `unloadUiFont()` -> `CloseWindow()`

## Превключване между табове

Потребителят кликва на таб от лентата  
-> `drawTabBar()` връща новия индекс  
-> `state->activeTab` се обновява  
-> `renderApp()` извиква само панела на активния таб (един от петте)

## Добавяне на отбор

Потребителят въвежда име в секцията `Teams`  
-> натиска `Add Team`  
-> `renderTeamsSection()` извиква `addTeam()`  
-> `validateTeamName()` проверява името  
-> `getNextTeamId()` създава ID  
-> отборът се добавя в масива  
-> `calculateStandings()` обновява класирането  
-> `dataChanged = true`  
-> `saveApplicationData()` записва CSV файловете

## Добавяне на мач

Потребителят отива на таб `Matches`  
-> избира домакин от падащо меню (имена на отбори)  
-> `drawDropdown()` обновява `state->newMatchHomeTeamIndex`  
-> избира гост по същия начин (`newMatchAwayTeamIndex`)  
-> въвежда голове и дата  
-> натиска `Add Match`  
-> `renderMatchesSection()` превръща индексите в team ID и извиква `addMatch()`  
-> `validateMatch()` проверява данните  
-> `getNextMatchId()` създава ID  
-> мачът се добавя в масива  
-> `calculateStandings()` обновява класирането  
-> `dataChanged = true`  
-> `saveApplicationData()` записва CSV файловете

## Сортиране на класиране

Потребителят натиска `Sort by Points`  
-> `renderStandingsSection()` извиква `bubbleSortStandingsByPoints()`  
-> редовете в `standings` се разменят чрез Bubble Sort  
-> таблицата се рисува в новия ред

## Търсене на отбор

Потребителят въвежда текст в секцията `Search`  
-> натиска `Search`  
-> `renderSearchSection()` извиква `linearSearchTeamByName()`  
-> индексите на намерените редове се записват в `searchResultIndexes`  
-> резултатите се показват в таблица

## Рекурсия за голове

Потребителят натиска `Calculate Total Goals`  
-> `renderStatisticsSection()` извиква `sumGoalsRecursive()`  
-> функцията събира головете от текущия мач  
-> извиква себе си за следващия индекс  
-> когато индексът стигне края, връща 0  
-> общият резултат се показва в секцията `Statistics`