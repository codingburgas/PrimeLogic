#pragma once

#include "raylib.h"

struct UiLayout
{
    int x;
    int y;
    int width;
    int rowHeight;
};

void initUiFont();
void unloadUiFont();
Font getUiFont();
void drawUiText(int x, int y, const char* text, int fontSize, Color color);

void beginPanel(UiLayout* layout, const char* title);
void addPanelSpace(UiLayout* layout, int pixels);
bool drawButton(UiLayout* layout, const char* text, int width);
void drawTextInput(UiLayout* layout, const char* label, char text[], int maxLength, int fieldId);
void drawIntInput(UiLayout* layout, const char* label, int* value, int fieldId);
void drawDropdown(UiLayout* layout, const char* label, const char* options[], int optionCount, int* selectedIndex, int dropdownId);
void flushOverlays();
void drawTableHeader(UiLayout* layout, const char* columns[], const int widths[], int columnCount);
void drawTableRow(UiLayout* layout, const char* values[], const int widths[], int columnCount);

int  drawTabBar(int x, int y, int width, int height, const char* labels[], int tabCount, int activeIndex);

void beginScrollableArea(int x, int y, int width, int viewHeight, int contentHeight, int* scrollOffset);
void endScrollableArea();