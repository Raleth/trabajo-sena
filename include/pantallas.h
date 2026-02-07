#ifndef PANTALLAS_H
#define PANTALLAS_H

#include <stdint.h>
#include <raylib.h>
#include <clay.h>
#include "estructuras.h"

typedef struct {
    Clay_Color fondo;
    Clay_Color panel;
    Clay_Color input;
    Clay_Color texto;
    Clay_Color botonPrimary;
    Clay_Color botonPrimaryHover;
    Clay_Color botonSecondary;
    Clay_Color botonSecondaryHover;
    Clay_Color botonDanger;
    Clay_Color botonDangerHover;
} UITheme;

typedef struct {
    Clay_ElementId amountInput;
    Clay_ElementId descInput;
    Clay_ElementId btnIncome;
    Clay_ElementId btnExpense;
    Clay_ElementId btnSaveCsv;
    Clay_ElementId btnLoadCsv;
    Clay_ElementId btnExportTxt;
    Clay_ElementId btnConfig;
} UIIds;

UITheme UI_DefaultTheme(void);
void UI_InitFonts(Font *fonts, int fontId);
void UI_AssignElementIds(UIIds *ids);
Clay_RenderCommandArray UI_BuildLayout(const UITheme *t, const UIIds *ids, int fontId);
void UI_HandlePointerFocus(const UIIds *ids);
void UI_HandleTextInput(void);
void UI_HandleButtons(const UIIds *ids);

#endif
