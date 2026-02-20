#include "pantallas.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "graficas.h"

// Forward declaration for internal Clay helper (not exposed in headers)
extern Clay_ElementId Clay__HashNumber(uint32_t offset, uint32_t seed);
#include "clay.h"

// Helper function to create a Clay string from a C string
Clay_String Clay_MakeString(const char *str)
{
    return (Clay_String){
        .length = (int)strlen(str),
        .chars = (char *)str};
}

UITheme UI_DefaultTheme(void)
{
    return (UITheme){
        .fondo = {43, 41, 51, 255},
        .panel = {70, 70, 70, 255},
        .input = {55, 55, 65, 255},
        .texto = {240, 240, 240, 255},
        .botonPrimary = {50, 150, 250, 255},
        .botonPrimaryHover = {70, 170, 255, 255},
        .botonSecondary = {65, 105, 225, 255},
        .botonSecondaryHover = {80, 120, 240, 255},
        .botonDanger = {220, 50, 50, 255},
        .botonDangerHover = {240, 65, 65, 255},
    };
}

void UI_InitFonts(Font *fonts, int fontId)
{
    fonts[fontId] = LoadFontEx("fuente/Roboto-Regular.ttf", 48, 0, 250);
    if (fonts[fontId].texture.id == 0)
    {
        fonts[fontId] = GetFontDefault();
    }
    SetTextureFilter(fonts[fontId].texture, TEXTURE_FILTER_BILINEAR);
}

void UI_AssignElementIds(UIIds *ids)
{
    ids->amountInput = CLAY_ID("amount_input");
    ids->descInput = CLAY_ID("desc_input");
    ids->btnIncome = CLAY_ID("btn_add_income");
    ids->btnExpense = CLAY_ID("btn_add_expense");
    ids->btnSaveCsv = CLAY_ID("btn_save_csv");
    ids->btnLoadCsv = CLAY_ID("btn_load_csv");
    ids->btnExportTxt = CLAY_ID("btn_export_txt");
    ids->btnConfig = CLAY_ID("btn_config");
    ids->btnEstadisticas = CLAY_ID("btn_estadisticas");
}

static Clay_ElementId LangButtonId(int index)
{
    return Clay__HashNumber((uint32_t)(9000 + index), 0);
}

static void RenderSidebar(const UITheme *t, const UIIds *ids, int fontId,
                          char *amountDisplayText, char *descDisplayText)
{
    CLAY(CLAY_ID("Barra_de_menu"), {.backgroundColor = t->panel,
                                    .cornerRadius = 8,
                                    .layout = {
                                        .sizing = {.width = CLAY_SIZING_FIXED(320), .height = CLAY_SIZING_GROW()},
                                        .padding = {15, 15, 15, 15},
                                        .childGap = 15,
                                        .layoutDirection = CLAY_TOP_TO_BOTTOM}})
    {
        CLAY(CLAY_ID("sidebar_title"), {.layout = {.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)}}})
        {
            CLAY_TEXT(Clay_MakeString(get_text("TITULO_VENTANA")), CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 24, .textColor = t->texto}));
        }

        CLAY(CLAY_ID("amount_label"), {.layout = {.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(25)}}})
        {
            CLAY_TEXT(Clay_MakeString(get_text("CANTIDAD")), CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 16, .textColor = t->texto}));
        }

        CLAY(ids->amountInput, {.backgroundColor = uiState.amountInputActive ? (Clay_Color){75, 75, 85, 255} : t->input,
                                .cornerRadius = 4,
                                .layout = {
                                    .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)},
                                    .padding = {10, 10, 10, 10}}})
        {
            snprintf(amountDisplayText, 64, "%s%s",
                     uiState.amountInput,
                     uiState.amountInputActive ? "_" : "");
            CLAY_TEXT(Clay_MakeString(amountDisplayText), CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 18, .textColor = t->texto}));
        }

        CLAY(CLAY_ID("desc_label"), {.layout = {.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(25)}}})
        {
            CLAY_TEXT(Clay_MakeString(get_text("DESCRIPCION")), CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 16, .textColor = t->texto}));
        }

        CLAY(ids->descInput, {.backgroundColor = uiState.descriptionInputActive ? (Clay_Color){75, 75, 85, 255} : t->input,
                              .cornerRadius = 4,
                              .layout = {
                                  .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)},
                                  .padding = {10, 10, 10, 10}}})
        {
            snprintf(descDisplayText, 140, "%s%s",
                     uiState.descriptionInput,
                     uiState.descriptionInputActive ? "_" : "");
            CLAY_TEXT(Clay_MakeString(descDisplayText), CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 16, .textColor = t->texto}));
        }

        CLAY(ids->btnIncome, {.backgroundColor = Clay_Hovered() ? t->botonPrimaryHover : t->botonPrimary,
                              .cornerRadius = 6,
                              .layout = {
                                  .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)},
                                  .padding = {10, 10, 10, 10},
                                  .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}}})
        {
            CLAY_TEXT(Clay_MakeString(get_text("AÑADIR_INGRESO")), CLAY_TEXT_CONFIG({.fontId = 0, .fontSize = 18, .textColor = {255, 255, 255, 255}}));
        }

        CLAY(ids->btnExpense, {.backgroundColor = Clay_Hovered() ? t->botonDangerHover : t->botonDanger,
                               .cornerRadius = 6,
                               .layout = {
                                   .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)},
                                   .padding = {10, 10, 10, 10},
                                   .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}}})
        {
            CLAY_TEXT(Clay_MakeString(get_text("AÑADIR_GASTO")), CLAY_TEXT_CONFIG({.fontId = 0, .fontSize = 18, .textColor = {255, 255, 255, 255}}));
        }

        CLAY(CLAY_ID("spacer1"), {.layout = {.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()}}}) {}

        CLAY(CLAY_ID("file_ops_title"), {.layout = {.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(30)}}})
        {
            CLAY_TEXT(Clay_MakeString(get_text("OPERACIONES_DE_ARCHIVO")), CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 20, .textColor = t->texto}));
        }

        CLAY(ids->btnSaveCsv, {.backgroundColor = Clay_Hovered() ? t->botonSecondaryHover : t->botonSecondary,
                               .cornerRadius = 6,
                               .layout = {
                                   .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)},
                                   .padding = {10, 10, 10, 10},
                                   .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}}})
        {
            CLAY_TEXT(Clay_MakeString(get_text("GUARDAR_CSV")), CLAY_TEXT_CONFIG({.fontId = 0, .fontSize = 16, .textColor = {255, 255, 255, 255}}));
        }

        CLAY(ids->btnLoadCsv, {.backgroundColor = Clay_Hovered() ? t->botonSecondaryHover : t->botonSecondary,
                               .cornerRadius = 6,
                               .layout = {
                                   .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)},
                                   .padding = {10, 10, 10, 10},
                                   .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}}})
        {
            CLAY_TEXT(Clay_MakeString(get_text("CARGAR_CSV")), CLAY_TEXT_CONFIG({.fontId = 0, .fontSize = 16, .textColor = {255, 255, 255, 255}}));
        }

        CLAY(ids->btnExportTxt, {.backgroundColor = Clay_Hovered() ? t->botonSecondaryHover : t->botonSecondary,
                                 .cornerRadius = 6,
                                 .layout = {
                                     .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)},
                                     .padding = {10, 10, 10, 10},
                                     .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}}})
        {
            CLAY_TEXT(Clay_MakeString(get_text("EXPORTAR_REPORTE")), CLAY_TEXT_CONFIG({.fontId = 0, .fontSize = 16, .textColor = {255, 255, 255, 255}}));
        }

        CLAY(ids->btnEstadisticas, {.backgroundColor = Clay_Hovered() ? t->botonSecondaryHover : t->botonSecondary,
                                    .cornerRadius = 6,
                                    .layout = {
                                        .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)},
                                        .padding = {10, 10, 10, 10},
                                        .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}}})
        {
            CLAY_TEXT(Clay_MakeString(get_text("ESTADISTICAS")),
                      CLAY_TEXT_CONFIG({.fontId = 0, .fontSize = 16, .textColor = {255, 255, 255, 255}}));
        }

        CLAY(ids->btnConfig, {.backgroundColor = Clay_Hovered() ? t->botonSecondaryHover : t->botonSecondary,
                              .cornerRadius = 6,
                              .layout = {
                                  .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)},
                                  .padding = {10, 10, 10, 10},
                                  .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}}})
        {
            CLAY_TEXT(Clay_MakeString(get_text("CONFIGURACIONES")), CLAY_TEXT_CONFIG({.fontId = 0, .fontSize = 16, .textColor = {255, 255, 255, 255}}));
        }
    }
}

static void RenderSummaryCards(int fontId, const UITheme *t)
{
    (void)t; // theme not currently used for summary colors
    CLAY(CLAY_ID("summary_row"), {.layout = {
                                      .layoutDirection = CLAY_LEFT_TO_RIGHT,
                                      .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(120)},
                                      .childGap = 15}})
    {
        CLAY(CLAY_ID("ingresos"), {.backgroundColor = {50, 155, 50, 255},
                                   .cornerRadius = 8,
                                   .layout = {
                                       .sizing = {.width = CLAY_SIZING_PERCENT(0.33), .height = CLAY_SIZING_GROW()},
                                       .padding = {15, 15, 15, 15},
                                       .childGap = 8,
                                       .layoutDirection = CLAY_TOP_TO_BOTTOM}})
        {
            CLAY_TEXT(Clay_MakeString(get_text("TOTAL_INGRESOS")), CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 16, .textColor = {220, 255, 220, 255}}));

            snprintf(uiState.incomeText, sizeof(uiState.incomeText), "$%.2f", budget.totalIncome);
            CLAY_TEXT(Clay_MakeString(uiState.incomeText), CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 32, .textColor = {255, 255, 255, 255}}));
        }

        CLAY(CLAY_ID("gastos"), {.backgroundColor = {200, 50, 50, 255},
                                 .cornerRadius = 8,
                                 .layout = {
                                     .sizing = {.width = CLAY_SIZING_PERCENT(0.33), .height = CLAY_SIZING_GROW()},
                                     .padding = {15, 15, 15, 15},
                                     .childGap = 8,
                                     .layoutDirection = CLAY_TOP_TO_BOTTOM}})
        {
            CLAY_TEXT(Clay_MakeString(get_text("TOTAL_GASTOS")), CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 16, .textColor = {255, 220, 220, 255}}));

            snprintf(uiState.expenseText, sizeof(uiState.expenseText), "$%.2f", budget.totalExpense);
            CLAY_TEXT(Clay_MakeString(uiState.expenseText), CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 32, .textColor = {255, 255, 255, 255}}));
        }

        CLAY(CLAY_ID("balance"), {.backgroundColor = budget.balance >= 0 ? (Clay_Color){65, 105, 225, 255} : (Clay_Color){180, 50, 180, 255},
                                  .cornerRadius = 8,
                                  .layout = {
                                      .sizing = {.width = CLAY_SIZING_PERCENT(0.34), .height = CLAY_SIZING_GROW()},
                                      .padding = {15, 15, 15, 15},
                                      .childGap = 8,
                                      .layoutDirection = CLAY_TOP_TO_BOTTOM}})
        {
            CLAY_TEXT(Clay_MakeString(get_text("BALANCE_NETO")), CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 16, .textColor = {220, 220, 255, 255}}));

            snprintf(uiState.balanceText, sizeof(uiState.balanceText), "$%.2f", budget.balance);
            CLAY_TEXT(Clay_MakeString(uiState.balanceText), CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 32, .textColor = {255, 255, 255, 255}}));
        }
    }
}

static void RenderTransactions(int fontId, const UITheme *t, const UIIds *ids)
{
    (void)ids; // parámetro reservado para uso futuro
    CLAY(CLAY_ID("transactions_title"), {.layout = {.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(35)},
                                                    .layoutDirection = CLAY_LEFT_TO_RIGHT,
                                                    .childAlignment = {.x = CLAY_ALIGN_X_RIGHT, .y = CLAY_ALIGN_Y_CENTER}}})
    {
        char titleText[128];
        snprintf(titleText, sizeof(titleText), "Transactions (%d)", budget.count);
        CLAY_TEXT(Clay_MakeString(titleText), CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 22, .textColor = t->texto}));
    }

    CLAY(CLAY_ID("transactions_container"), {.backgroundColor = t->fondo,
                                             .cornerRadius = 8,
                                             .layout = {
                                                 .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
                                                 .padding = {10, 10, 10, 10},
                                                 .childGap = 8,
                                                 .layoutDirection = CLAY_TOP_TO_BOTTOM,
                                                 .childAlignment = {.x = CLAY_ALIGN_X_RIGHT, .y = CLAY_ALIGN_Y_TOP}}})
    {
        if (budget.count == 0)
        {
            CLAY(CLAY_ID("no_transactions"), {.layout = {
                                                  .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(50)},
                                                  .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}}})
            {
                CLAY_TEXT(Clay_MakeString(get_text("NOTIFICACION")),
                          CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 16, .textColor = {150, 150, 150, 255}}));
            }
        }
        else
        {
            // Buffers estáticos: Clay guarda punteros, no copias.
            // Variables locales del stack se corrompen al renderizar.
            static char amountBuffers[20][32];
            int bufIdx = 0;
            for (int i = budget.count - 1; i >= 0 && i >= budget.count - 20; i--, bufIdx++)
            {
                Transaction *tnx = &budget.transactions[i];
                uint32_t itemHashId = (uint32_t)(123456 + i);

                CLAY(Clay__HashNumber(itemHashId, 0), {.backgroundColor = tnx->type == TRANSACTION_INCOME ? (Clay_Color){40, 80, 40, 255} : (Clay_Color){80, 40, 40, 255},
                                                       .cornerRadius = 6,
                                                       .layout = {
                                                           .layoutDirection = CLAY_LEFT_TO_RIGHT,
                                                           .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(60)},
                                                           .padding = {12, 12, 12, 12},
                                                           .childGap = 10,
                                                           .childAlignment = {.y = CLAY_ALIGN_Y_CENTER}}})
                {
                    CLAY(Clay__HashNumber(itemHashId + 1000, 0), {.layout = {
                                                                      .layoutDirection = CLAY_TOP_TO_BOTTOM,
                                                                      .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
                                                                      .childGap = 4}})
                    {
                        CLAY_TEXT(Clay_MakeString(tnx->description), CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 16, .textColor = t->texto}));

                        CLAY_TEXT(Clay_MakeString(tnx->date), CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 12, .textColor = {180, 180, 180, 255}}));
                    }

                    snprintf(amountBuffers[bufIdx], sizeof(amountBuffers[bufIdx]), "$%.2f", tnx->amount);
                    CLAY(Clay__HashNumber(itemHashId + 2000, 0), {.layout = {
                                                                      .sizing = {.width = CLAY_SIZING_FIXED(100), .height = CLAY_SIZING_GROW()},
                                                                      .childAlignment = {.x = CLAY_ALIGN_X_RIGHT, .y = CLAY_ALIGN_Y_CENTER}}})
                    {
                        CLAY_TEXT(Clay_MakeString(amountBuffers[bufIdx]), CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 20, .textColor = t->texto}));
                    }

                    Clay_ElementId btnDeleteId = Clay__HashNumber(itemHashId + 3000, 0);
                    CLAY(btnDeleteId, {.backgroundColor = Clay_Hovered() ? t->botonDangerHover : t->botonDanger,
                                       .cornerRadius = 6,
                                       .layout = {
                                           .sizing = {.width = CLAY_SIZING_FIXED(40), .height = CLAY_SIZING_FIXED(40)},
                                           .padding = {10, 10, 10, 10},
                                           .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}}})
                    {
                        CLAY_TEXT(CLAY_STRING("X"), CLAY_TEXT_CONFIG({.fontId = 0,
                                                                      .fontSize = 16,
                                                                      .textColor = {255, 255, 255, 255}}));
                    }
                }
            }
        }
    }
}

static void RenderWorkspace(const UITheme *t, int fontId, const UIIds *ids)
{
    CLAY(CLAY_ID("area_de_trabajo"), {.backgroundColor = t->panel,
                                      .cornerRadius = 8,
                                      .layout = {
                                          .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
                                          .padding = {15, 15, 15, 15},
                                          .childGap = 15,
                                          .layoutDirection = CLAY_TOP_TO_BOTTOM}})
    {
        if (uiState.statusMessageTime > 0)
        {
            CLAY(CLAY_ID("status_message"), {.backgroundColor = {100, 150, 100, 255},
                                             .cornerRadius = 6,
                                             .layout = {
                                                 .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)},
                                                 .padding = {10, 10, 10, 10},
                                                 .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}}})
            {
                CLAY_TEXT(Clay_MakeString(uiState.statusMessage), CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 16, .textColor = {255, 255, 255, 255}}));
            }
        }
        if (uiState.showConfig)
        {
            CLAY(CLAY_ID("config_panel"), {.backgroundColor = t->fondo,
                                           .cornerRadius = 8,
                                           .layout = {
                                               .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
                                               .padding = {15, 15, 15, 15},
                                               .childGap = 15,
                                               .layoutDirection = CLAY_TOP_TO_BOTTOM}})
            {
                CLAY(CLAY_ID("config_title"), {.layout = {.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(30)}}})
                {
                    CLAY_TEXT(Clay_MakeString(get_text("SELECCIONAR_IDIOMA")), CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 20, .textColor = t->texto}));
                }

                const char *langKeys[] = {
                    "IDIOMA_ES",
                    "IDIOMA_EN",
                    "IDIOMA_DE",
                    "IDIOMA_FR"};
                for (int i = 0; i < 4; i++)
                {
                    bool selected = (idioma_global == (i + 1));
                    Clay_Color baseColor = selected ? t->botonPrimary : t->botonSecondary;
                    Clay_Color hoverColor = selected ? t->botonPrimaryHover : t->botonSecondaryHover;

                    CLAY(LangButtonId(i), {.backgroundColor = Clay_Hovered() ? hoverColor : baseColor,
                                           .cornerRadius = 6,
                                           .layout = {
                                               .sizing = {.width = CLAY_SIZING_FIXED(240), .height = CLAY_SIZING_FIXED(40)},
                                               .padding = {10, 10, 10, 10},
                                               .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}}})
                    {
                        CLAY_TEXT(Clay_MakeString(get_text(langKeys[i])), CLAY_TEXT_CONFIG({.fontId = 0, .fontSize = 16, .textColor = {255, 255, 255, 255}}));
                    }
                }
            }
        }
        else
        {
            RenderSummaryCards(fontId, t);
            RenderTransactions(fontId, t, ids);
        }
    }
}

Clay_RenderCommandArray UI_BuildLayout(const UITheme *t, const UIIds *ids, int fontId)

{
    static char amountDisplayText[64];
    static char descDisplayText[140];

    Clay_BeginLayout();

    CLAY(CLAY_ID("Contenedor_total"), {.backgroundColor = t->fondo,
                                       .layout = {
                                           .layoutDirection = CLAY_LEFT_TO_RIGHT,
                                           .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
                                           .padding = {15, 15, 15, 15},
                                           .childGap = 15}})
    {
        // ¡IMPORTANTE! Llamar a RenderSidebar aquí
        RenderSidebar(t, ids, fontId, amountDisplayText, descDisplayText);

        if (uiState.showStats)
        {
            RenderStatsView(t, fontId, ids);
        }
        else
        {
            RenderWorkspace(t, fontId, ids);
        }
    }

    return Clay_EndLayout();
}
void UI_HandlePointerFocus(const UIIds *ids)
{
    if (Clay_PointerOver(ids->amountInput) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        uiState.amountInputActive = true;
        uiState.descriptionInputActive = false;
    }

    if (Clay_PointerOver(ids->descInput) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        uiState.descriptionInputActive = true;
        uiState.amountInputActive = false;
    }
}

void UI_HandleTextInput(void)
{
    int key = GetCharPressed();
    while (key > 0)
    {
        if (uiState.amountInputActive && key >= 32 && key <= 125)
        {
            size_t len = strlen(uiState.amountInput);
            if (len < sizeof(uiState.amountInput) - 1)
            {
                if ((key >= '0' && key <= '9') || key == '.')
                {
                    if (key == '.')
                    {
                        bool hasDecimal = false;
                        for (size_t i = 0; i < len; i++)
                        {
                            if (uiState.amountInput[i] == '.')
                            {
                                hasDecimal = true;
                                break;
                            }
                        }
                        if (!hasDecimal)
                        {
                            uiState.amountInput[len] = (char)key;
                            uiState.amountInput[len + 1] = '\0';
                        }
                    }
                    else
                    {
                        uiState.amountInput[len] = (char)key;
                        uiState.amountInput[len + 1] = '\0';
                    }
                }
            }
        }
        if (uiState.descriptionInputActive && key >= 32 && key <= 125)
        {
            size_t len = strlen(uiState.descriptionInput);
            if (len < sizeof(uiState.descriptionInput) - 1)
            {
                uiState.descriptionInput[len] = (char)key;
                uiState.descriptionInput[len + 1] = '\0';
            }
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        if (uiState.amountInputActive)
        {
            int len = (int)strlen(uiState.amountInput);
            if (len > 0)
                uiState.amountInput[len - 1] = '\0';
        }
        if (uiState.descriptionInputActive)
        {
            int len = (int)strlen(uiState.descriptionInput);
            if (len > 0)
                uiState.descriptionInput[len - 1] = '\0';
        }
    }
}

void UI_HandleButtons(const UIIds *ids)
{
    // Botón de configuración (siempre visible)
    if (Clay_PointerOver(ids->btnConfig) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        uiState.showConfig = !uiState.showConfig;
        if (uiState.showConfig)
        {
            uiState.showStats = false; // al abrir configuración, cerrar estadísticas
        }
    }

    // Botón de estadísticas (siempre visible)
    if (Clay_PointerOver(ids->btnEstadisticas) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        uiState.showStats = !uiState.showStats;
        if (uiState.showStats)
        {
            uiState.showConfig = false; // al abrir estadísticas, cerrar configuración
        }
    }

    // Si estamos en vista de estadísticas, no procesamos más botones
    if (uiState.showStats)
    {
        return;
    }

    // Si estamos en configuración, solo procesamos los botones de idioma
    if (uiState.showConfig)
    {
        for (int i = 0; i < 4; i++)
        {
            Clay_ElementId langBtn = LangButtonId(i);
            if (Clay_PointerOver(langBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                idioma_global = i + 1;
                break;
            }
        }
        return; // salir, no procesar otros botones
    }

    // --- Vista normal: procesar botones de transacciones y archivos ---
    if (Clay_PointerOver(ids->btnIncome) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        float amount = (float)atof(uiState.amountInput);
        if (amount > 0 && strlen(uiState.descriptionInput) > 0)
        {
            AddTransaction(TRANSACTION_INCOME, amount, uiState.descriptionInput);
            uiState.amountInput[0] = '\0';
            uiState.descriptionInput[0] = '\0';
        }
        else
        {
            snprintf(uiState.statusMessage, sizeof(uiState.statusMessage),
                     get_text("ENTRADA_INVALIDA"));
            uiState.statusMessageTime = 2.0f;
        }
    }

    if (Clay_PointerOver(ids->btnExpense) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        float amount = (float)atof(uiState.amountInput);
        if (amount > 0 && strlen(uiState.descriptionInput) > 0)
        {
            AddTransaction(TRANSACTION_EXPENSE, amount, uiState.descriptionInput);
            uiState.amountInput[0] = '\0';
            uiState.descriptionInput[0] = '\0';
        }
        else
        {
            snprintf(uiState.statusMessage, sizeof(uiState.statusMessage),
                     get_text("ENTRADA_INVALIDA"));
            uiState.statusMessageTime = 2.0f;
        }
    }

    if (Clay_PointerOver(ids->btnSaveCsv) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        SaveBudgetToCSV();
    }

    if (Clay_PointerOver(ids->btnLoadCsv) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        LoadBudgetFromCSV();
    }

    if (Clay_PointerOver(ids->btnExportTxt) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        SaveBudgetToTXT();
    }

    // Botones de eliminar transacciones (solo en vista normal)
    for (int i = budget.count - 1; i >= 0 && i >= budget.count - 20; i--)
    {
        uint32_t itemHashId = (uint32_t)(123456 + i);
        Clay_ElementId btnDeleteId = Clay__HashNumber(itemHashId + 3000, 0);
        if (Clay_PointerOver(btnDeleteId) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            DeleteTransaction(i);
            break;
        }
    }
}