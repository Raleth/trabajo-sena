#define CLAY_IMPLEMENTATION
#include "clay.h"
#include <stdio.h>
#include "clay_renderer_raylib.c"
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int altura = 720;  // altura = height
const int ancho = 1280;  // ancho = width

// ===== DATA STRUCTURES =====
typedef enum {
    TRANSACTION_INCOME,
    TRANSACTION_EXPENSE
} TransactionType;

typedef struct {
    TransactionType type;
    float amount;
    char description[128];
    char date[32];
} Transaction;

typedef struct {
    Transaction transactions[1000];
    int count;
    float totalIncome;
    float totalExpense;
    float balance;
} Budget;

typedef struct {
    char amountInput[32];
    char descriptionInput[128];
    bool amountInputActive;
    bool descriptionInputActive;
    int selectedTransactionIndex;
    char statusMessage[256];
    float statusMessageTime;
    char incomeText[64];
    char expenseText[64];
    char balanceText[64];
} UIState;

// ===== GLOBAL STATE =====
Budget budget = {0};
UIState uiState = {0};

// ===== UTILITY FUNCTIONS =====
void GetCurrentDate(char* buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(buffer, size, "%02d/%02d/%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
}

void UpdateBudgetTotals() {
    budget.totalIncome = 0.0f;
    budget.totalExpense = 0.0f;
    
    for (int i = 0; i < budget.count; i++) {
        if (budget.transactions[i].type == TRANSACTION_INCOME) {
            budget.totalIncome += budget.transactions[i].amount;
        } else {
            budget.totalExpense += budget.transactions[i].amount;
        }
    }
    
    budget.balance = budget.totalIncome - budget.totalExpense;
}

void AddTransaction(TransactionType type, float amount, const char* description) {
    if (budget.count >= 1000) {
        snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), "Error: Maximum transactions reached!");
        uiState.statusMessageTime = 3.0f;
        return;
    }
    
    Transaction* t = &budget.transactions[budget.count];
    t->type = type;
    t->amount = amount;
    strncpy(t->description, description, sizeof(t->description) - 1);
    t->description[sizeof(t->description) - 1] = '\0';
    GetCurrentDate(t->date, sizeof(t->date));
    
    budget.count++;
    UpdateBudgetTotals();
    
    snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), 
             "%s added: $%.2f", type == TRANSACTION_INCOME ? "Income" : "Expense", amount);
    uiState.statusMessageTime = 2.0f;
}

void DeleteTransaction(int index) {
    if (index < 0 || index >= budget.count) return;
    
    for (int i = index; i < budget.count - 1; i++) {
        budget.transactions[i] = budget.transactions[i + 1];
    }
    budget.count--;
    UpdateBudgetTotals();
    
    snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), "Transaction deleted");
    uiState.statusMessageTime = 2.0f;
}

void SaveBudgetToCSV(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), "Error: Could not save file!");
        uiState.statusMessageTime = 3.0f;
        return;
    }
    
    fprintf(file, "Type,Amount,Description,Date\n");
    for (int i = 0; i < budget.count; i++) {
        Transaction* t = &budget.transactions[i];
        fprintf(file, "%s,%.2f,%s,%s\n", 
                t->type == TRANSACTION_INCOME ? "Income" : "Expense",
                t->amount, t->description, t->date);
    }
    
    fclose(file);
    snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), "Saved to %s", filename);
    uiState.statusMessageTime = 2.0f;
}

void LoadBudgetFromCSV(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), "Error: Could not open file!");
        uiState.statusMessageTime = 3.0f;
        return;
    }
    
    budget.count = 0;
    char line[512];
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return;  // Empty file or header missing
    }
    
    while (fgets(line, sizeof(line), file) && budget.count < 1000) {
        Transaction* t = &budget.transactions[budget.count];
        char typeStr[32];
        
        if (sscanf(line, "%[^,],%f,%[^,],%[^\n]", 
                   typeStr, &t->amount, t->description, t->date) == 4) {
            t->type = (strcmp(typeStr, "Income") == 0) ? TRANSACTION_INCOME : TRANSACTION_EXPENSE;
            budget.count++;
        }
    }
    
    fclose(file);
    UpdateBudgetTotals();
    snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), "Loaded from %s (%d transactions)", filename, budget.count);
    uiState.statusMessageTime = 2.0f;
}

void SaveBudgetToTXT(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), "Error: Could not save file!");
        uiState.statusMessageTime = 3.0f;
        return;
    }
    
    fprintf(file, "====================================\n");
    fprintf(file, "      BUDGET MANAGER REPORT\n");
    fprintf(file, "====================================\n\n");
    fprintf(file, "Total Income:   $%.2f\n", budget.totalIncome);
    fprintf(file, "Total Expenses: $%.2f\n", budget.totalExpense);
    fprintf(file, "Net Balance:    $%.2f\n\n", budget.balance);
    fprintf(file, "====================================\n");
    fprintf(file, "         TRANSACTION LIST\n");
    fprintf(file, "====================================\n\n");
    
    for (int i = 0; i < budget.count; i++) {
        Transaction* t = &budget.transactions[i];
        fprintf(file, "[%s] %s\n", t->date, t->type == TRANSACTION_INCOME ? "INCOME" : "EXPENSE");
        fprintf(file, "    Amount: $%.2f\n", t->amount);
        fprintf(file, "    Description: %s\n\n", t->description);
    }
    
    fclose(file);
    snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), "Report saved to %s", filename);
    uiState.statusMessageTime = 2.0f;
}

void HandleClayErrors(Clay_ErrorData errorData)
{
    printf("%s", errorData.errorText.chars);
}

// Helper function to create a Clay string from a C string
Clay_String Clay_MakeString(const char* str) {
    return (Clay_String) { 
        .length = (int)strlen(str), 
        .chars = (char*)str
    };
}

int main(void)
{
    Clay_Raylib_Initialize(ancho, altura, "Budget Manager - University Project", 
                          FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);

    SetTargetFPS(60);

    uint64_t clayRequiredMemory = Clay_MinMemorySize();
    Clay_Arena claymemory = (Clay_Arena){
        .memory = malloc(clayRequiredMemory),
        .capacity = clayRequiredMemory
    };
    
    if (!claymemory.memory) {
        printf("ERROR: Failed to allocate Clay memory!\n");
        return 1;
    }

    Clay_Initialize(claymemory, (Clay_Dimensions){.width = ancho, .height = altura},
                    (Clay_ErrorHandler){HandleClayErrors, .userData = NULL});

    const int TU_FONT_ID = 0;
    const Clay_Color colorfondo = {43, 41, 51, 255};
    const Clay_Color colorobjeto = {70, 70, 70, 255};
    const Clay_Color colorbotonPrimary = {50, 150, 250, 255};  // Bright blue
    const Clay_Color colorbotonPrimaryHover = {70, 170, 255, 255};
    const Clay_Color colorbotonSecondary = {65, 105, 225, 255};  // Royal blue
    const Clay_Color colorbotonSecondaryHover = {80, 120, 240, 255};
    const Clay_Color colorbotonDanger = {220, 50, 50, 255};  // Red
    const Clay_Color colorbotonDangerHover = {240, 65, 65, 255};
    const Clay_Color colorInput = {55, 55, 65, 255};
    const Clay_Color colorText = {240, 240, 240, 255};

    Font fonts[1];
    fonts[TU_FONT_ID] = LoadFontEx("fuente/Roboto-Regular.ttf", 48, 0, 250);
    if (fonts[TU_FONT_ID].texture.id == 0) {
        fonts[TU_FONT_ID] = GetFontDefault();
    }
    SetTextureFilter(fonts[TU_FONT_ID].texture, TEXTURE_FILTER_BILINEAR);
    Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);

    // Input handling variables
    float deltaTime = 0.0f;
    
    // Element IDs for click detection after layout
    Clay_ElementId amount_input_id;
    Clay_ElementId desc_input_id;
    Clay_ElementId btn_add_income_id;
    Clay_ElementId btn_add_expense_id;
    Clay_ElementId btn_save_csv_id;
    Clay_ElementId btn_load_csv_id;
    Clay_ElementId btn_export_txt_id;
    
    while (!WindowShouldClose())
    {
        // Static buffers for display text that must persist until rendering
        static char amountDisplayText[64];
        static char descDisplayText[140];
        
        deltaTime = GetFrameTime();
        
        // Update status message timer
        if (uiState.statusMessageTime > 0) {
            uiState.statusMessageTime -= deltaTime;
        }
        
        Clay_SetLayoutDimensions((Clay_Dimensions){.width = GetScreenWidth(),
                                                   .height = GetScreenHeight()});

        // Set pointer state for mouse interaction
        Vector2 mousePos = GetMousePosition();
        Clay_SetPointerState((Clay_Vector2){mousePos.x, mousePos.y}, IsMouseButtonDown(MOUSE_LEFT_BUTTON));

        // Assign element IDs BEFORE layout begins
        amount_input_id = CLAY_ID("amount_input");
        desc_input_id = CLAY_ID("desc_input");
        btn_add_income_id = CLAY_ID("btn_add_income");
        btn_add_expense_id = CLAY_ID("btn_add_expense");
        btn_save_csv_id = CLAY_ID("btn_save_csv");
        btn_load_csv_id = CLAY_ID("btn_load_csv");
        btn_export_txt_id = CLAY_ID("btn_export_txt");

        Clay_BeginLayout();

        CLAY(CLAY_ID("Contenedor_total"), {
            .backgroundColor = colorfondo,
            .layout = {
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
                .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
                .padding = {15, 15, 15, 15},
                .childGap = 15
            }
        })
        {
            // LEFT SIDEBAR - Input Form
            CLAY(CLAY_ID("Barra_de_menu"), {
                .backgroundColor = colorobjeto,
                .cornerRadius = 8,
                .layout = {
                    .sizing = {.width = CLAY_SIZING_FIXED(320), .height = CLAY_SIZING_GROW()},
                    .padding = {15, 15, 15, 15},
                    .childGap = 15,
                    .layoutDirection = CLAY_TOP_TO_BOTTOM
                }
            })
            {
                // Title
                CLAY(CLAY_ID("sidebar_title"), {
                    .layout = {.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)}}
                }) {
                    CLAY_TEXT(CLAY_STRING("Add Transaction"), CLAY_TEXT_CONFIG({
                        .fontId = TU_FONT_ID, .fontSize = 24, .textColor = colorText
                    }));
                }
                
                // Amount Input
                CLAY(CLAY_ID("amount_label"), {
                    .layout = {.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(25)}}
                }) {
                    CLAY_TEXT(CLAY_STRING("Amount ($):"), CLAY_TEXT_CONFIG({
                        .fontId = TU_FONT_ID, .fontSize = 16, .textColor = colorText
                    }));
                }
                
                CLAY(amount_input_id, {
                    .backgroundColor = uiState.amountInputActive ? 
                        (Clay_Color){75, 75, 85, 255} : colorInput,
                    .cornerRadius = 4,
                    .layout = {
                        .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)},
                        .padding = {10, 10, 10, 10}
                    }
                }) {
                    snprintf(amountDisplayText, sizeof(amountDisplayText), "%s%s", 
                            uiState.amountInput, 
                            uiState.amountInputActive ? "_" : "");
                    CLAY_TEXT(Clay_MakeString(amountDisplayText), CLAY_TEXT_CONFIG({
                        .fontId = TU_FONT_ID, .fontSize = 18, .textColor = colorText
                    }));
                }
                
                // Description Input
                CLAY(CLAY_ID("desc_label"), {
                    .layout = {.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(25)}}
                }) {
                    CLAY_TEXT(CLAY_STRING("Description:"), CLAY_TEXT_CONFIG({
                        .fontId = TU_FONT_ID, .fontSize = 16, .textColor = colorText
                    }));
                }
                
                CLAY(desc_input_id, {
                    .backgroundColor = uiState.descriptionInputActive ? 
                        (Clay_Color){75, 75, 85, 255} : colorInput,
                    .cornerRadius = 4,
                    .layout = {
                        .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)},
                        .padding = {10, 10, 10, 10}
                    }
                }) {
                    snprintf(descDisplayText, sizeof(descDisplayText), "%s%s", 
                            uiState.descriptionInput, 
                            uiState.descriptionInputActive ? "_" : "");
                    CLAY_TEXT(Clay_MakeString(descDisplayText), CLAY_TEXT_CONFIG({
                        .fontId = TU_FONT_ID, .fontSize = 16, .textColor = colorText
                    }));
                }
                
                // Add Income Button
                CLAY(btn_add_income_id, {
                    .backgroundColor = Clay_Hovered() ? colorbotonPrimaryHover : colorbotonPrimary,
                    .cornerRadius = 6,
                    .layout = {
                        .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)},
                        .padding = {10, 10, 10, 10},
                        .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}
                    }
                }) {
                    CLAY_TEXT(CLAY_STRING("+ Add Income"), CLAY_TEXT_CONFIG({
                        .fontId = 0,
                        .fontSize = 18,
                        .textColor = {255, 255, 255, 255}
                    }));
                }
                
                // Add Expense Button
                CLAY(btn_add_expense_id, {
                    .backgroundColor = Clay_Hovered() ? colorbotonDangerHover : colorbotonDanger,
                    .cornerRadius = 6,
                    .layout = {
                        .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)},
                        .padding = {10, 10, 10, 10},
                        .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}
                    }
                }) {
                    CLAY_TEXT(CLAY_STRING("- Add Expense"), CLAY_TEXT_CONFIG({
                        .fontId = 0,
                        .fontSize = 18,
                        .textColor = {255, 255, 255, 255}
                    }));
                }
                
                // Spacer
                CLAY(CLAY_ID("spacer1"), {
                    .layout = {.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()}}
                }) {}
                
                // File Operations
                CLAY(CLAY_ID("file_ops_title"), {
                    .layout = {.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(30)}}
                }) {
                    CLAY_TEXT(CLAY_STRING("File Operations"), CLAY_TEXT_CONFIG({
                        .fontId = TU_FONT_ID, .fontSize = 20, .textColor = colorText
                    }));
                }
                
                CLAY(btn_save_csv_id, {
                    .backgroundColor = Clay_Hovered() ? colorbotonSecondaryHover : colorbotonSecondary,
                    .cornerRadius = 6,
                    .layout = {
                        .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)},
                        .padding = {10, 10, 10, 10},
                        .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}
                    }
                }) {
                    CLAY_TEXT(CLAY_STRING("Save to CSV"), CLAY_TEXT_CONFIG({
                        .fontId = 0,
                        .fontSize = 16,
                        .textColor = {255, 255, 255, 255}
                    }));
                }
                
                CLAY(btn_load_csv_id, {
                    .backgroundColor = Clay_Hovered() ? colorbotonSecondaryHover : colorbotonSecondary,
                    .cornerRadius = 6,
                    .layout = {
                        .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)},
                        .padding = {10, 10, 10, 10},
                        .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}
                    }
                }) {
                    CLAY_TEXT(CLAY_STRING("Load from CSV"), CLAY_TEXT_CONFIG({
                        .fontId = 0,
                        .fontSize = 16,
                        .textColor = {255, 255, 255, 255}
                    }));
                }
                
                CLAY(btn_export_txt_id, {
                    .backgroundColor = Clay_Hovered() ? colorbotonSecondaryHover : colorbotonSecondary,
                    .cornerRadius = 6,
                    .layout = {
                        .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)},
                        .padding = {10, 10, 10, 10},
                        .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}
                    }
                }) {
                    CLAY_TEXT(CLAY_STRING("Export Report (TXT)"), CLAY_TEXT_CONFIG({
                        .fontId = 0,
                        .fontSize = 16,
                        .textColor = {255, 255, 255, 255}
                    }));
                }
            }

            // RIGHT AREA - Summary and Transactions
            CLAY(CLAY_ID("area_de_trabajo"), {
                .backgroundColor = colorobjeto,
                .cornerRadius = 8,
                .layout = {
                    .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
                    .padding = {15, 15, 15, 15},
                    .childGap = 15,
                    .layoutDirection = CLAY_TOP_TO_BOTTOM
                }
            })
            {
                // Status Message
                if (uiState.statusMessageTime > 0) {
                    CLAY(CLAY_ID("status_message"), {
                        .backgroundColor = {100, 150, 100, 255},
                        .cornerRadius = 6,
                        .layout = {
                            .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(40)},
                            .padding = {10, 10, 10, 10},
                            .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}
                        }
                    }) {
                        CLAY_TEXT(Clay_MakeString(uiState.statusMessage), CLAY_TEXT_CONFIG({
                            .fontId = TU_FONT_ID, .fontSize = 16, .textColor = {255, 255, 255, 255}
                        }));
                    }
                }
                
                // Summary Cards Row
                CLAY(CLAY_ID("summary_row"), {
                    .layout = {
                        .layoutDirection = CLAY_LEFT_TO_RIGHT,
                        .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(120)},
                        .childGap = 15
                    }
                })
                {
                    // Income Card
                    CLAY(CLAY_ID("ingresos"), {
                        .backgroundColor = {50, 155, 50, 255},
                        .cornerRadius = 8,
                        .layout = {
                            .sizing = {.width = CLAY_SIZING_PERCENT(0.33), .height = CLAY_SIZING_GROW()},
                            .padding = {15, 15, 15, 15},
                            .childGap = 8,
                            .layoutDirection = CLAY_TOP_TO_BOTTOM
                        }
                    })
                    {
                        CLAY_TEXT(CLAY_STRING("TOTAL INCOME"), CLAY_TEXT_CONFIG({
                            .fontId = TU_FONT_ID, .fontSize = 16, .textColor = {220, 255, 220, 255}
                        }));
                        
                        snprintf(uiState.incomeText, sizeof(uiState.incomeText), "$%.2f", budget.totalIncome);
                        CLAY_TEXT(Clay_MakeString(uiState.incomeText), CLAY_TEXT_CONFIG({
                            .fontId = TU_FONT_ID, .fontSize = 32, .textColor = {255, 255, 255, 255}
                        }));
                    }

                    // Expense Card
                    CLAY(CLAY_ID("gastos"), {
                        .backgroundColor = {200, 50, 50, 255},
                        .cornerRadius = 8,
                        .layout = {
                            .sizing = {.width = CLAY_SIZING_PERCENT(0.33), .height = CLAY_SIZING_GROW()},
                            .padding = {15, 15, 15, 15},
                            .childGap = 8,
                            .layoutDirection = CLAY_TOP_TO_BOTTOM
                        }
                    })
                    {
                        CLAY_TEXT(CLAY_STRING("TOTAL EXPENSES"), CLAY_TEXT_CONFIG({
                            .fontId = TU_FONT_ID, .fontSize = 16, .textColor = {255, 220, 220, 255}
                        }));
                        
                        snprintf(uiState.expenseText, sizeof(uiState.expenseText), "$%.2f", budget.totalExpense);
                        CLAY_TEXT(Clay_MakeString(uiState.expenseText), CLAY_TEXT_CONFIG({
                            .fontId = TU_FONT_ID, .fontSize = 32, .textColor = {255, 255, 255, 255}
                        }));
                    }
                    
                    // Balance Card
                    CLAY(CLAY_ID("balance"), {
                        .backgroundColor = budget.balance >= 0 ? 
                            (Clay_Color){65, 105, 225, 255} : (Clay_Color){180, 50, 180, 255},
                        .cornerRadius = 8,
                        .layout = {
                            .sizing = {.width = CLAY_SIZING_PERCENT(0.34), .height = CLAY_SIZING_GROW()},
                            .padding = {15, 15, 15, 15},
                            .childGap = 8,
                            .layoutDirection = CLAY_TOP_TO_BOTTOM
                        }
                    })
                    {
                        CLAY_TEXT(CLAY_STRING("NET BALANCE"), CLAY_TEXT_CONFIG({
                            .fontId = TU_FONT_ID, .fontSize = 16, .textColor = {220, 220, 255, 255}
                        }));
                        
                        snprintf(uiState.balanceText, sizeof(uiState.balanceText), "$%.2f", budget.balance);
                        CLAY_TEXT(Clay_MakeString(uiState.balanceText), CLAY_TEXT_CONFIG({
                            .fontId = TU_FONT_ID, .fontSize = 32, .textColor = {255, 255, 255, 255}
                        }));
                    }
                }
                
                // Transactions List Title
                CLAY(CLAY_ID("transactions_title"), {
                    .layout = {.sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(35)}}
                }) {
                    char titleText[128];
                    snprintf(titleText, sizeof(titleText), "Transactions (%d)", budget.count);
                    CLAY_TEXT(Clay_MakeString(titleText), CLAY_TEXT_CONFIG({
                        .fontId = TU_FONT_ID, .fontSize = 22, .textColor = colorText
                    }));
                }
                
                // Transactions List (scrollable)
                CLAY(CLAY_ID("transactions_container"), {
                    .backgroundColor = colorfondo,
                    .cornerRadius = 8,
                    .layout = {
                        .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
                        .padding = {10, 10, 10, 10},
                        .childGap = 8,
                        .layoutDirection = CLAY_TOP_TO_BOTTOM
                    }
                })
                {
                    if (budget.count == 0) {
                        CLAY(CLAY_ID("no_transactions"), {
                            .layout = {
                                .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(50)},
                                .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}
                            }
                        }) {
                            CLAY_TEXT(CLAY_STRING("No transactions yet. Add your first one!"), 
                                    CLAY_TEXT_CONFIG({
                                .fontId = TU_FONT_ID, .fontSize = 16, 
                                .textColor = {150, 150, 150, 255}
                            }));
                        }
                    } else {
                        for (int i = budget.count - 1; i >= 0 && i >= budget.count - 20; i--) {
                            Transaction* t = &budget.transactions[i];
                            
                            uint32_t itemHashId = (uint32_t)(123456 + i);
                            
                            CLAY(Clay__HashNumber(itemHashId, 0), {
                                .backgroundColor = t->type == TRANSACTION_INCOME ? 
                                    (Clay_Color){40, 80, 40, 255} : (Clay_Color){80, 40, 40, 255},
                                .cornerRadius = 6,
                                .layout = {
                                    .layoutDirection = CLAY_LEFT_TO_RIGHT,
                                    .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_FIXED(60)},
                                    .padding = {12, 12, 12, 12},
                                    .childGap = 10,
                                    .childAlignment = {.y = CLAY_ALIGN_Y_CENTER}
                                }
                            })
                            {
                                // Transaction info
                                CLAY(Clay__HashNumber(itemHashId + 1000, 0), {
                                    .layout = {
                                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                                        .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
                                        .childGap = 4
                                    }
                                }) {
                                    CLAY_TEXT(Clay_MakeString(t->description), CLAY_TEXT_CONFIG({
                                        .fontId = TU_FONT_ID, .fontSize = 16, .textColor = colorText
                                    }));
                                    
                                    CLAY_TEXT(Clay_MakeString(t->date), CLAY_TEXT_CONFIG({
                                        .fontId = TU_FONT_ID, .fontSize = 12, 
                                        .textColor = {180, 180, 180, 255}
                                    }));
                                }
                                
                                // Amount
                                char amountStr[32];
                                snprintf(amountStr, sizeof(amountStr), "$%.2f", t->amount);
                                CLAY(Clay__HashNumber(itemHashId + 2000, 0), {
                                    .layout = {
                                        .sizing = {.width = CLAY_SIZING_FIXED(100), .height = CLAY_SIZING_GROW()},
                                        .childAlignment = {.x = CLAY_ALIGN_X_RIGHT, .y = CLAY_ALIGN_Y_CENTER}
                                    }
                                }) {
                                    CLAY_TEXT(Clay_MakeString(amountStr), CLAY_TEXT_CONFIG({
                                        .fontId = TU_FONT_ID, .fontSize = 20, .textColor = colorText
                                    }));
                                }
                                
                                // Delete button (using hash ID from loop index)
                                Clay_ElementId btnDeleteId = Clay__HashNumber(itemHashId + 3000, 0);
                                CLAY(btnDeleteId, {
                                    .backgroundColor = Clay_Hovered() ? colorbotonDangerHover : colorbotonDanger,
                                    .cornerRadius = 6,
                                    .layout = {
                                        .sizing = {.width = CLAY_SIZING_FIXED(40), .height = CLAY_SIZING_FIXED(40)},
                                        .padding = {10, 10, 10, 10},
                                        .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER}
                                    }
                                }) {
                                    CLAY_TEXT(CLAY_STRING("X"), CLAY_TEXT_CONFIG({
                                        .fontId = 0, 
                                        .fontSize = 16, 
                                        .textColor = {255, 255, 255, 255}
                                    }));
                                }
                            }
                        }
                    }
                }
            }
        }  // Close Contenedor_total

        Clay_RenderCommandArray renderCommands = Clay_EndLayout();
        
        // Handle input field clicks AFTER layout is complete
        if (Clay_PointerOver(amount_input_id) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            uiState.amountInputActive = true;
            uiState.descriptionInputActive = false;
            printf("[CLICK] Amount field activated! amountActive=%d, descActive=%d\n", 
                   uiState.amountInputActive, uiState.descriptionInputActive);
        }
        
        if (Clay_PointerOver(desc_input_id) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            uiState.descriptionInputActive = true;
            uiState.amountInputActive = false;
            printf("[CLICK] Description field activated! amountActive=%d, descActive=%d\n", 
                   uiState.amountInputActive, uiState.descriptionInputActive);
        }
        
        // Handle keyboard input AFTER click detection
        int key = GetCharPressed();
        if (key > 0) {
            printf("[KEYBOARD] Key detected: '%c' (code=%d) | amountActive=%d, descActive=%d\n", 
                   (char)key, key, uiState.amountInputActive, uiState.descriptionInputActive);
        }
        while (key > 0) {
            if (uiState.amountInputActive && key >= 32 && key <= 125) {
                size_t len = strlen(uiState.amountInput);
                if (len < sizeof(uiState.amountInput) - 1) {
                    // Only allow numbers and decimal point
                    if ((key >= '0' && key <= '9') || key == '.') {
                        // Check if decimal point already exists
                        if (key == '.') {
                            bool hasDecimal = false;
                            for (size_t i = 0; i < len; i++) {
                                if (uiState.amountInput[i] == '.') {
                                    hasDecimal = true;
                                    break;
                                }
                            }
                            if (!hasDecimal) {
                                uiState.amountInput[len] = (char)key;
                                uiState.amountInput[len + 1] = '\0';
                            }
                        } else {
                            uiState.amountInput[len] = (char)key;
                            uiState.amountInput[len + 1] = '\0';
                            printf("[INPUT] Added '%c' to amount field. New value: '%s'\n", (char)key, uiState.amountInput);
                        }
                    }
                }
            }
            if (uiState.descriptionInputActive && key >= 32 && key <= 125) {
                size_t len = strlen(uiState.descriptionInput);
                if (len < sizeof(uiState.descriptionInput) - 1) {
                    uiState.descriptionInput[len] = (char)key;
                    uiState.descriptionInput[len + 1] = '\0';
                    printf("[INPUT] Added '%c' to description field. New value: '%s'\n", (char)key, uiState.descriptionInput);
                }
            }
            key = GetCharPressed();
        }
        
        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (uiState.amountInputActive) {
                int len = strlen(uiState.amountInput);
                if (len > 0) uiState.amountInput[len - 1] = '\0';
            }
            if (uiState.descriptionInputActive) {
                int len = strlen(uiState.descriptionInput);
                if (len > 0) uiState.descriptionInput[len - 1] = '\0';
            }
        }
        
        // Handle button clicks AFTER layout is complete
        // Add Income Button
        if (Clay_PointerOver(btn_add_income_id) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            float amount = atof(uiState.amountInput);
            if (amount > 0 && strlen(uiState.descriptionInput) > 0) {
                AddTransaction(TRANSACTION_INCOME, amount, uiState.descriptionInput);
                uiState.amountInput[0] = '\0';
                uiState.descriptionInput[0] = '\0';
            } else {
                snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), 
                        "Please enter valid amount and description");
                uiState.statusMessageTime = 2.0f;
            }
        }
        
        // Add Expense Button
        if (Clay_PointerOver(btn_add_expense_id) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            float amount = atof(uiState.amountInput);
            if (amount > 0 && strlen(uiState.descriptionInput) > 0) {
                AddTransaction(TRANSACTION_EXPENSE, amount, uiState.descriptionInput);
                uiState.amountInput[0] = '\0';
                uiState.descriptionInput[0] = '\0';
            } else {
                snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), 
                        "Please enter valid amount and description");
                uiState.statusMessageTime = 2.0f;
            }
        }
        
        // Save CSV Button
        if (Clay_PointerOver(btn_save_csv_id) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            SaveBudgetToCSV("budget_data.csv");
        }
        
        // Load CSV Button
        if (Clay_PointerOver(btn_load_csv_id) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            LoadBudgetFromCSV("budget_data.csv");
        }
        
        // Export TXT Button
        if (Clay_PointerOver(btn_export_txt_id) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            SaveBudgetToTXT("budget_report.txt");
        }
        
        // Handle delete button clicks for each transaction
        for (int i = budget.count - 1; i >= 0 && i >= budget.count - 20; i--) {
            uint32_t itemHashId = (uint32_t)(123456 + i);
            Clay_ElementId btnDeleteId = Clay__HashNumber(itemHashId + 3000, 0);
            if (Clay_PointerOver(btnDeleteId) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                DeleteTransaction(i);
                break; // Only delete one per frame
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);
        Clay_Raylib_Render(renderCommands, fonts);
        EndDrawing();
    }

    free(claymemory.memory);
    Clay_Raylib_Close();
    return 0;
}