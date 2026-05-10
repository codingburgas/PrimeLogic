# Raylib

Този проект използва Raylib за графичния интерфейс.

## Текущо състояние

Файловете на Raylib **вече са включени** в репозиторията:

```text
third_party/raylib/
├── include/
│   ├── raylib.h
│   ├── raymath.h
│   └── rlgl.h
└── lib/
    ├── raylib.lib   (статична библиотека за x64, ~5 MB)
    └── raylib.dll   (резерва за динамично свързване)
```

Версия: **Raylib 5.5 (win64_msvc16)**, свалена от
<https://github.com/raysan5/raylib/releases>.

Visual Studio проектът:

- търси header файлове в `third_party/raylib/include`
- търси library файлове в `third_party/raylib/lib`
- дефинира `RAYLIB_STATIC`, така че се свързва статично с `raylib.lib`
  (`raylib.dll` не е нужен по време на работа в текущата конфигурация)

## Ако искате да обновите Raylib

1. Свалете нова версия от <https://github.com/raysan5/raylib/releases>.
2. Изберете подходящата за вашата платформа:
   - **x64 build** &rarr; `raylib-X.Y_win64_msvc16.zip`
   - **Win32 build** &rarr; `raylib-X.Y_win32_msvc16.zip`
3. Разопаковайте и заменете файловете в `include/` и `lib/`.

## Защо Raylib

- Лесна за инсталиране (един header и един lib).
- Работи на Windows, macOS, Linux и web (нашата версия е Windows).
- Има прости функции като `DrawText`, `DrawRectangle`, `IsMouseButtonReleased`.
- Подходяща за ученически проекти.