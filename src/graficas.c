#include "graficas.h"
#include "clay.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

void Prinftestadisticas(void) {
    uiState.showStats = !uiState.showStats;
    if (uiState.showStats) {
        uiState.showConfig = false;
    }
}

void RenderStatsView(const UITheme *t, int fontId, const UIIds *ids) {
    (void)ids; // parámetro reservado para uso futuro
    CLAY(CLAY_ID("stats_view"), {
        .backgroundColor = t->panel,
        .cornerRadius = 8,
        .layout = {
            .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
            .padding = {20, 20, 20, 20},
            .childGap = 20,
            .layoutDirection = CLAY_TOP_TO_BOTTOM
        }
    }) {
        CLAY(CLAY_ID("stats_title"), {
            .layout = {
                .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)},
                .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}
            }
        }) {
            CLAY_TEXT(Clay_MakeString(get_text("TITULO_ESTADISTICAS")),
                      CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 28, .textColor = t->texto}));
        }

        if (budget.count == 0) {
            CLAY(CLAY_ID("stats_no_data"), {
                .layout = {
                    .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
                    .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}
                }
            }) {
                CLAY_TEXT(Clay_MakeString(get_text("NO_HAY_DATOS")),
                          CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 20, .textColor = {150,150,150,255}}));
            }
        } else {
            float maxVal = (budget.totalIncome > budget.totalExpense) ? budget.totalIncome : budget.totalExpense;
            if (maxVal < 1.0f) maxVal = 1.0f;

            /* FIX: buffers estaticos. Clay guarda punteros, no copias.
               Variables locales del stack se corrompen al momento de renderizar. */
            static char incomeStr[32];
            static char expenseStr[32];
            snprintf(incomeStr, sizeof(incomeStr), "$%.2f", budget.totalIncome);
            snprintf(expenseStr, sizeof(expenseStr), "$%.2f", budget.totalExpense);

            /* Altura maxima de barra: 200px (50px reservados para label+valor) */
            const float BAR_MAX_H = 200.0f;

            /* FIX alineacion: un espaciador GROW dentro de cada columna
               empuja la barra hacia abajo, creando una base comun. */
            CLAY(CLAY_ID("chart_container"), {
                .layout = {
                    .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(300)},
                    .childGap = 40,
                    .layoutDirection = CLAY_LEFT_TO_RIGHT,
                    .childAlignment = {.y = CLAY_ALIGN_Y_BOTTOM}
                }
            }) {
                float incomeHeight = (budget.totalIncome / maxVal) * BAR_MAX_H;
                if (incomeHeight < 10) incomeHeight = 10;

                CLAY(CLAY_ID("income_bar_container"), {
                    .layout = {
                        .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(250)},
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        .childAlignment = {.x = CLAY_ALIGN_X_CENTER}
                    }
                }) {
                    CLAY(CLAY_ID("income_spacer"), {
                        .layout = {.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()}}
                    }) {}
                    CLAY(CLAY_ID("income_bar"), {
                        .backgroundColor = {50, 200, 50, 255},
                        .cornerRadius = 4,
                        .layout = {.sizing = {.width = CLAY_SIZING_FIXED(80), .height = CLAY_SIZING_FIXED(incomeHeight)}}
                    }) {}
                    CLAY(CLAY_ID("income_label"), {
                        .layout = {
                            .sizing = {.width = CLAY_SIZING_FIXED(100), .height = CLAY_SIZING_FIXED(30)},
                            .childAlignment = {.x = CLAY_ALIGN_X_CENTER}
                        }
                    }) {
                        CLAY_TEXT(Clay_MakeString(get_text("INGRESOS")),
                                  CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 16, .textColor = t->texto}));
                    }
                    CLAY(CLAY_ID("income_value"), {
                        .layout = {
                            .sizing = {.width = CLAY_SIZING_FIXED(100), .height = CLAY_SIZING_FIXED(20)},
                            .childAlignment = {.x = CLAY_ALIGN_X_CENTER}
                        }
                    }) {
                        CLAY_TEXT(Clay_MakeString(incomeStr),
                                  CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 14, .textColor = t->texto}));
                    }
                }

                float expenseHeight = (budget.totalExpense / maxVal) * BAR_MAX_H;
                if (expenseHeight < 10) expenseHeight = 10;

                CLAY(CLAY_ID("expense_bar_container"), {
                    .layout = {
                        .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(250)},
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        .childAlignment = {.x = CLAY_ALIGN_X_CENTER}
                    }
                }) {
                    CLAY(CLAY_ID("expense_spacer"), {
                        .layout = {.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()}}
                    }) {}
                    CLAY(CLAY_ID("expense_bar"), {
                        .backgroundColor = {200, 50, 50, 255},
                        .cornerRadius = 4,
                        .layout = {.sizing = {.width = CLAY_SIZING_FIXED(80), .height = CLAY_SIZING_FIXED(expenseHeight)}}
                    }) {}
                    CLAY(CLAY_ID("expense_label"), {
                        .layout = {
                            .sizing = {.width = CLAY_SIZING_FIXED(100), .height = CLAY_SIZING_FIXED(30)},
                            .childAlignment = {.x = CLAY_ALIGN_X_CENTER}
                        }
                    }) {
                        CLAY_TEXT(Clay_MakeString(get_text("GASTOS")),
                                  CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 16, .textColor = t->texto}));
                    }
                    CLAY(CLAY_ID("expense_value"), {
                        .layout = {
                            .sizing = {.width = CLAY_SIZING_FIXED(100), .height = CLAY_SIZING_FIXED(20)},
                            .childAlignment = {.x = CLAY_ALIGN_X_CENTER}
                        }
                    }) {
                        CLAY_TEXT(Clay_MakeString(expenseStr),
                                  CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 14, .textColor = t->texto}));
                    }
                }
            }

            CLAY(CLAY_ID("net_balance"), {
                .layout = {
                    .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(50)},
                    .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}
                }
            }) {
                static char balanceStr[64];
                snprintf(balanceStr, sizeof(balanceStr), "%s $%.2f", get_text("BALANCE_NETO"), budget.balance);
                Clay_Color balanceColor = budget.balance >= 0 ? (Clay_Color){100,255,100,255} : (Clay_Color){255,100,100,255};
                CLAY_TEXT(Clay_MakeString(balanceStr),
                          CLAY_TEXT_CONFIG({.fontId = fontId, .fontSize = 24, .textColor = balanceColor}));
            }
        }
    }
}