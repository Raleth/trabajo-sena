#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "estructuras.h"

Budget budget = {0};
UIState uiState = {0};


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

// ===== UTILITY FUNCTIONS =====
void GetCurrentDate(char* buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(buffer, size, "%02d/%02d/%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
}


void AddTransaction(TransactionType type, float amount, const char* description) {
    if (budget.count >= 1000) {
        snprintf(uiState.statusMessage, sizeof(uiState.statusMessage),get_text("MENSAJE_MAXIMA_CANTIDAD_TRANSACCIONES"));
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
         get_text("AÑADIDO_FORMATO"), 
         type == TRANSACTION_INCOME ? get_text("INGRESO") : get_text("GASTO"), 
         amount);
    uiState.statusMessageTime = 2.0f;
}

void DeleteTransaction(int index) {
    if (index < 0 || index >= budget.count) return;
    
    for (int i = index; i < budget.count - 1; i++) {
        budget.transactions[i] = budget.transactions[i + 1];
    }
    budget.count--;
    UpdateBudgetTotals();
    
    snprintf(uiState.statusMessage, sizeof(uiState.statusMessage),get_text("TRANSACCION_BORRADA"));
    uiState.statusMessageTime = 2.0f;
}

void SaveBudgetToCSV(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), get_text("GUARDADO_FALLIDO"));
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
    snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), 
         get_text("GUARDADO_EN"), filename);  
    uiState.statusMessageTime = 2.0f;
}

void LoadBudgetFromCSV(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), get_text("FALLO_AL_ABRIR"));
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
    snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), 
         get_text("CARGADO_DESDE"), filename, budget.count);
    uiState.statusMessageTime = 2.0f;
}

void SaveBudgetToTXT(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), get_text("GUARDADO_FALLIDO"));
        uiState.statusMessageTime = 3.0f;
        return;
    }
    
    fprintf(file, "====================================\n");
    fprintf(file, "      %s\n", get_text("TITULO_REPORTE"));
    fprintf(file, "====================================\n\n");
    fprintf(file, "%s $%.2f\n", get_text("TOTAL_INGRESOS"), budget.totalIncome);
    fprintf(file, "%s $%.2f\n", get_text("TOTAL_GASTOS"), budget.totalExpense);
    fprintf(file, "%s $%.2f\n\n", get_text("BALANCE_NETO"), budget.balance);
    fprintf(file, "         %s\n", get_text("LISTA_TRANSACCIONES"));
    fprintf(file, "====================================\n\n");
    
    for (int i = 0; i < budget.count; i++) {
        Transaction* t = &budget.transactions[i];
        // Tipo de transacción internacionalizado
        const char* tipoTexto = t->type == TRANSACTION_INCOME ? 
                               get_text("INGRESO_MAYUS") : get_text("GASTO_MAYUS");
        
        fprintf(file, "[%s] %s\n", t->date, tipoTexto);
        fprintf(file, "    %s $%.2f\n", get_text("ETIQUETA_MONTO"), t->amount);
        fprintf(file, "    %s %s\n\n", get_text("ETIQUETA_DESCRIPCION"), t->description);
    }
    
    fclose(file);
    snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), 
         get_text("REPORTE_GUARDADO_EN"), filename); 
    uiState.statusMessageTime = 2.0f;
}
