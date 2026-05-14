#include "presentation/raylib_ui.h"

#include <cstdio>
#include <cstring>

static Font g_uiFont;
static bool g_fontLoaded = false;
static int  g_activeFieldId = 0;
static int  g_openDropdownId = 0;

static bool g_overlayActive = false;
static Rectangle g_overlayRect;
static const char* g_overlayOptions[64];
static int g_overlayOptionCount = 0;
static int* g_overlaySelected = nullptr;
static int g_overlayDropdownId = 0;
static int g_overlayRowHeight = 0;

static const int TITLE_FONT_SIZE = 24;
static const int LABEL_FONT_SIZE = 18;
static const int TABLE_FONT_SIZE = 16;
static const int FONT_SPACING = 1;

void initUiFont()
{
    g_uiFont = LoadFontEx("C:\\Windows\\Fonts\\segoeui.ttf", 32, nullptr, 0);

    if (g_uiFont.texture.id == 0)
    {
        g_uiFont = LoadFontEx("C:\\Windows\\Fonts\\arial.ttf", 32, nullptr, 0);
    }

    if (g_uiFont.texture.id == 0)
    {
        g_uiFont = GetFontDefault();
        g_fontLoaded = false;
    }
    else
    {
        SetTextureFilter(g_uiFont.texture, TEXTURE_FILTER_BILINEAR);
        g_fontLoaded = true;
    }
}

void unloadUiFont()
{
    if (g_fontLoaded)
    {
        UnloadFont(g_uiFont);
        g_fontLoaded = false;
    }
}

Font getUiFont()
{
    return g_uiFont;
}

void drawUiText(int x, int y, const char* text, int fontSize, Color color)
{
    DrawTextEx(g_uiFont, text, Vector2{ (float)x, (float)y }, (float)fontSize, (float)FONT_SPACING, color);
}

static int textWidth(const char* text, int fontSize)
{
    Vector2 size = MeasureTextEx(g_uiFont, text, (float)fontSize, (float)FONT_SPACING);
    return (int)size.x;
}

static Rectangle makeRowRectangle(const UiLayout* layout, int width)
{
    Rectangle rectangle;
    rectangle.x = (float)layout->x;
    rectangle.y = (float)layout->y;
    rectangle.width = (float)width;
    rectangle.height = (float)layout->rowHeight;
    return rectangle;
}

static bool isMouseReleasedInside(Rectangle rectangle)
{
    return CheckCollisionPointRec(GetMousePosition(), rectangle) &&
        IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
}

static void drawFieldBox(Rectangle rectangle, bool active)
{
    DrawRectangleRec(rectangle, active ? Color{ 235, 246, 255, 255 } : RAYWHITE);
    DrawRectangleLinesEx(rectangle, 1.0f, active ? BLUE : GRAY);
}

static void updateTextValue(char text[], int maxLength)
{
    int key = GetCharPressed();

    while (key > 0)
    {
        int length = (int)strlen(text);

        if (key >= 32 && key <= 126 && length < maxLength - 1)
        {
            text[length] = (char)key;
            text[length + 1] = '\0';
        }

        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        int length = (int)strlen(text);

        if (length > 0)
        {
            text[length - 1] = '\0';
        }
    }
}

static void updateIntValue(int* value)
{
    int digit = -1;

    for (int key = KEY_ZERO; key <= KEY_NINE; key++)
    {
        if (IsKeyPressed(key))
        {
            digit = key - KEY_ZERO;
        }
    }

    if (digit != -1)
    {
        *value = (*value * 10) + digit;
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        *value = *value / 10;
    }

    if (IsKeyPressed(KEY_MINUS))
    {
        *value = -*value;
    }
}

void beginPanel(UiLayout* layout, const char* title)
{
    drawUiText(layout->x, layout->y, title, TITLE_FONT_SIZE, DARKBLUE);
    layout->y += TITLE_FONT_SIZE + 12;
}

void addPanelSpace(UiLayout* layout, int pixels)
{
    layout->y += pixels;
}

bool drawButton(UiLayout* layout, const char* text, int width)
{
    Rectangle rectangle = makeRowRectangle(layout, width);
    bool hovered = CheckCollisionPointRec(GetMousePosition(), rectangle);

    DrawRectangleRec(rectangle, hovered ? Color{ 210, 230, 250, 255 } : Color{ 230, 235, 240, 255 });
    DrawRectangleLinesEx(rectangle, 1.0f, GRAY);

    int textWidthPx = textWidth(text, LABEL_FONT_SIZE);
    int textX = layout->x + (width - textWidthPx) / 2;
    int textY = layout->y + (layout->rowHeight - LABEL_FONT_SIZE) / 2;
    drawUiText(textX, textY, text, LABEL_FONT_SIZE, BLACK);

    layout->y += layout->rowHeight + 8;
    return isMouseReleasedInside(rectangle);
}

void drawTextInput(UiLayout* layout, const char* label, char text[], int maxLength, int fieldId)
{
    int textY = layout->y + (layout->rowHeight - LABEL_FONT_SIZE) / 2;
    drawUiText(layout->x, textY, label, LABEL_FONT_SIZE, DARKGRAY);

    Rectangle rectangle = {
        (float)(layout->x + 170),
        (float)layout->y,
        (float)(layout->width - 170),
        (float)layout->rowHeight
    };

    if (isMouseReleasedInside(rectangle))
    {
        g_activeFieldId = fieldId;
    }

    bool active = g_activeFieldId == fieldId;
    drawFieldBox(rectangle, active);
    drawUiText((int)rectangle.x + 10, textY, text, LABEL_FONT_SIZE, BLACK);

    if (active)
    {
        updateTextValue(text, maxLength);
    }

    layout->y += layout->rowHeight + 8;
}

void drawIntInput(UiLayout* layout, const char* label, int* value, int fieldId)
{
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%d", *value);

    int textY = layout->y + (layout->rowHeight - LABEL_FONT_SIZE) / 2;
    drawUiText(layout->x, textY, label, LABEL_FONT_SIZE, DARKGRAY);

    Rectangle rectangle = {
        (float)(layout->x + 170),
        (float)layout->y,
        (float)(layout->width - 170),
        (float)layout->rowHeight
    };

    if (isMouseReleasedInside(rectangle))
    {
        g_activeFieldId = fieldId;
    }

    bool active = g_activeFieldId == fieldId;
    drawFieldBox(rectangle, active);
    drawUiText((int)rectangle.x + 10, textY, buffer, LABEL_FONT_SIZE, BLACK);

    if (active)
    {
        updateIntValue(value);
    }

    layout->y += layout->rowHeight + 8;
}

static bool isClickOnOpenDropdownOptions()
{
    if (!g_overlayActive)
    {
        return false;
    }

    Rectangle optionsArea = {
        g_overlayRect.x,
        g_overlayRect.y + g_overlayRect.height,
        g_overlayRect.width,
        g_overlayRect.height * g_overlayOptionCount
    };

    return CheckCollisionPointRec(GetMousePosition(), optionsArea);
}
void drawDropdown(UiLayout* layout, const char* label, const char* options[], int optionCount, int* selectedIndex, int dropdownId)
{
    int textY = layout->y + (layout->rowHeight - LABEL_FONT_SIZE) / 2;
    drawUiText(layout->x, textY, label, LABEL_FONT_SIZE, DARKGRAY);

    Rectangle rectangle = {
        (float)(layout->x + 170),
        (float)layout->y,
        (float)(layout->width - 170),
        (float)layout->rowHeight
    };

    bool isOpen = (g_openDropdownId == dropdownId);
    bool clickIsOnOpenOptions = isClickOnOpenDropdownOptions();
    bool boxClicked = isMouseReleasedInside(rectangle) && !clickIsOnOpenOptions;

    if (boxClicked)
    {
        g_openDropdownId = isOpen ? 0 : dropdownId;
        isOpen = !isOpen;
    }

    DrawRectangleRec(rectangle, isOpen ? Color{ 235, 246, 255, 255 } : RAYWHITE);
    DrawRectangleLinesEx(rectangle, 1.0f, isOpen ? BLUE : GRAY);

    const char* shown = (*selectedIndex >= 0 && *selectedIndex < optionCount)
        ? options[*selectedIndex]
        : "(select)";
    drawUiText((int)rectangle.x + 10, textY, shown, LABEL_FONT_SIZE, BLACK);

    int triX = (int)(rectangle.x + rectangle.width) - 18;
    int triY = (int)(rectangle.y + rectangle.height / 2.0f) - 3;
    DrawTriangle(
        Vector2{ (float)triX, (float)triY },
        Vector2{ (float)(triX + 6), (float)(triY + 8) },
        Vector2{ (float)(triX + 12), (float)triY },
        DARKGRAY);

    layout->y += layout->rowHeight + 8;

    if (isOpen)
    {
        int storedCount = optionCount;
        if (storedCount > 64)
        {
            storedCount = 64;
        }

        for (int index = 0; index < storedCount; index++)
        {
            g_overlayOptions[index] = options[index];
        }

        g_overlayActive = true;
        g_overlayRect = rectangle;
        g_overlayOptionCount = storedCount;
        g_overlaySelected = selectedIndex;
        g_overlayDropdownId = dropdownId;
        g_overlayRowHeight = layout->rowHeight;
    }
}

void flushOverlays()
{
    if (!g_overlayActive)
    {
        return;
    }

    for (int index = 0; index < g_overlayOptionCount; index++)
    {
        Rectangle optionRect = {
            g_overlayRect.x,
            g_overlayRect.y + g_overlayRect.height + index * g_overlayRect.height,
            g_overlayRect.width,
            g_overlayRect.height
        };

        bool hovered = CheckCollisionPointRec(GetMousePosition(), optionRect);
        DrawRectangleRec(optionRect, hovered ? Color{ 220, 235, 250, 255 } : RAYWHITE);
        DrawRectangleLinesEx(optionRect, 1.0f, GRAY);

        int optTextY = (int)optionRect.y + (g_overlayRowHeight - LABEL_FONT_SIZE) / 2;
        drawUiText((int)optionRect.x + 10, optTextY, g_overlayOptions[index], LABEL_FONT_SIZE, BLACK);

        if (hovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            *g_overlaySelected = index;
            g_openDropdownId = 0;
        }
    }

    Rectangle entireArea = {
        g_overlayRect.x,
        g_overlayRect.y,
        g_overlayRect.width,
        g_overlayRect.height * (1 + g_overlayOptionCount)
    };

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) &&
        g_openDropdownId == g_overlayDropdownId &&
        !CheckCollisionPointRec(GetMousePosition(), entireArea))
    {
        g_openDropdownId = 0;
    }

    g_overlayActive = false;
}

void drawTableHeader(UiLayout* layout, const char* columns[], const int widths[], int columnCount)
{
    int currentX = layout->x;
    int textY = layout->y + (layout->rowHeight - TABLE_FONT_SIZE) / 2;

    for (int index = 0; index < columnCount; index++)
    {
        DrawRectangle(currentX, layout->y, widths[index], layout->rowHeight, Color{ 220, 225, 230, 255 });
        DrawRectangleLines(currentX, layout->y, widths[index], layout->rowHeight, GRAY);
        drawUiText(currentX + 8, textY, columns[index], TABLE_FONT_SIZE, BLACK);
        currentX += widths[index];
    }

    layout->y += layout->rowHeight;
}

void drawTableRow(UiLayout* layout, const char* values[], const int widths[], int columnCount)
{
    int currentX = layout->x;
    int textY = layout->y + (layout->rowHeight - TABLE_FONT_SIZE) / 2;

    for (int index = 0; index < columnCount; index++)
    {
        DrawRectangle(currentX, layout->y, widths[index], layout->rowHeight, RAYWHITE);
        DrawRectangleLines(currentX, layout->y, widths[index], layout->rowHeight, LIGHTGRAY);
        drawUiText(currentX + 8, textY, values[index], TABLE_FONT_SIZE, BLACK);
        currentX += widths[index];
    }

    layout->y += layout->rowHeight;
}

void beginScrollableArea(int x, int y, int width, int viewHeight, int contentHeight, int* scrollOffset)
{
    Rectangle area = { (float)x, (float)y, (float)width, (float)viewHeight };

    if (CheckCollisionPointRec(GetMousePosition(), area))
    {
        float wheel = GetMouseWheelMove();
        *scrollOffset -= (int)(wheel * 30.0f);
    }

    int maxScroll = contentHeight - viewHeight;
    if (maxScroll < 0)
    {
        maxScroll = 0;
    }

    if (*scrollOffset < 0)
    {
        *scrollOffset = 0;
    }
    if (*scrollOffset > maxScroll)
    {
        *scrollOffset = maxScroll;
    }

    BeginScissorMode(x, y, width, viewHeight);
}

void endScrollableArea()
{
    EndScissorMode();
}

int drawTabBar(int x, int y, int width, int height, const char* labels[], int tabCount, int activeIndex)
{
    int tabWidth = width / tabCount;
    int newActive = activeIndex;

    DrawRectangle(x, y + height - 1, width, 1, GRAY);

    for (int index = 0; index < tabCount; index++)
    {
        int tabX = x + index * tabWidth;
        Rectangle rectangle = { (float)tabX, (float)y, (float)tabWidth, (float)height };

        bool isActive = (index == activeIndex);
        bool hovered = CheckCollisionPointRec(GetMousePosition(), rectangle);

        Color background;
        if (isActive)
        {
            background = RAYWHITE;
        }
        else if (hovered)
        {
            background = Color{ 225, 235, 245, 255 };
        }
        else
        {
            background = Color{ 235, 240, 245, 255 };
        }

        DrawRectangleRec(rectangle, background);
        DrawRectangleLines(tabX, y, tabWidth, height, GRAY);

        if (isActive)
        {
            DrawRectangle(tabX + 1, y + height - 4, tabWidth - 1, 4, BLUE);
        }

        int labelWidth = textWidth(labels[index], LABEL_FONT_SIZE);
        int labelX = tabX + (tabWidth - labelWidth) / 2;
        int labelY = y + (height - LABEL_FONT_SIZE) / 2;
        drawUiText(labelX, labelY, labels[index], LABEL_FONT_SIZE, isActive ? DARKBLUE : DARKGRAY);

        if (hovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            newActive = index;
        }
    }

    return newActive;
}