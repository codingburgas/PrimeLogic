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
    /*
    Какво: Зарежда системния шрифт Segoe UI за по-четим текст.
    Как: Опитва да зареди TTF файла от Windows; ако не успее, ползва шрифта на Raylib.
    Защо: Стандартният Raylib шрифт е малък и пикселизиран, неудобен за ученически проект.
    */
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
    /*
    Какво: Проверява дали мишката е върху списъка на отворено падащо меню.
    Как: Ако има активен overlay, прави правоъгълник под кутията и проверява за съвпадение.
    Защо: Когато меню е отворено, кликове върху неговите опции не трябва да се възприемат
         от долните полета (които биха получили клика през "наслагването").
    */
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