#pragma once

static const int    DISPLAY_WIDTH = 320,
                    DISPLAY_HEIGHT = 240,
                    DISPLAY_WIDTH_GRID = 301,
                    DISPLAY_HEIGHT_GRID = 201,
                    DISPLAY_Y_GRID = 10,
                    DISPLAY_X_GRID = (DISPLAY_WIDTH - (DISPLAY_WIDTH_GRID - 1)) / 2,
                    DISPLAY_TOP_GRID = DISPLAY_Y_GRID,
                    DISPLAY_BOTTOM_GRID = DISPLAY_Y_GRID + DISPLAY_HEIGHT_GRID,
                    DISPLAY_RIGHT_GRID = DISPLAY_X_GRID + DISPLAY_WIDTH_GRID,
                    DISPLAY_WIDTH_TITLE_MENU_PAGE = (DISPLAY_WIDTH_GRID - 1) / 12 * 3,
                    DISPLAY_X_MENU_PAGE = DISPLAY_WIDTH_GRID - DISPLAY_WIDTH_TITLE_MENU_PAGE * DISPLAY_X_GRID - 1,
                    DISPLAY_Y_MENU_PAGE = 10,
                    DISPLAY_X_MENU_ITEM = DISPLAY_X_MENU_PAGE;
                    

void Display_Init(void);
void Display_SetDrawStyle(int _colorFill, int _colorDraw);
void Display_DrawRect(int _x, int _y, int _width, int _height);
void Display_DrawText(char *_text, int _x, int _y);
void Display_Clear(void);
void Display_DrawCursors(void);
void Display_DrawGrid(void);
