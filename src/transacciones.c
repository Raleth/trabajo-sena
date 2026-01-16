#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <windows.h>
#include <commdlg.h>
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

// ===== funcion para detectar la fecha actual =====
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

void SaveBudgetToCSV(void) {
    // 1. Obtener la frase base para el nombre del archivo
    const char* basePhrase = get_text("NOMBRE_BASE_ARCHIVO"); 
    
    // 2. Obtener la fecha actual formateada
    char currentDate[11];
    GetCurrentDate(currentDate, sizeof(currentDate));
    
    // 3. Convertir la fecha a formato válido para nombres de archivo
    char formattedDate[11];
    strcpy(formattedDate, currentDate);
    for (int i = 0; formattedDate[i]; i++) {
        if (formattedDate[i] == '/') formattedDate[i] = '_';
    }
    
    // 4. Crear el nombre base del archivo
    char baseFilename[256];
    snprintf(baseFilename, sizeof(baseFilename), "%s_%s.csv", basePhrase, formattedDate);
    
    // 5. Verificar si el archivo existe y encontrar un nombre disponible
    char filename[256];
    strcpy(filename, baseFilename);
    
    int counter = 1;
    while (access(filename, F_OK) == 0) {
        // Si el archivo existe, agregar un número al final
        snprintf(filename, sizeof(filename), "%s_%s_%d.csv", 
                basePhrase, formattedDate, counter);
        counter++;
        
        // Limitar el número máximo de intentos (por seguridad)
        if (counter > 1000) {
            snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), 
                    "Error: Demasiados archivos existentes");
            uiState.statusMessageTime = 3.0f;
            return;
        }
    }
    
    // 6. Abrir el archivo para escritura
    FILE* file = fopen(filename, "w");
    if (!file) {
        snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), 
                get_text("GUARDADO_FALLIDO"));
        uiState.statusMessageTime = 3.0f;
        return;
    }
    
    // 7. Escribir el encabezado CSV
    fprintf(file, "Type,Amount,Description,Date\n");
    
    // 8. Escribir todas las transacciones
    for (int i = 0; i < budget.count; i++) {
        Transaction* t = &budget.transactions[i];
        fprintf(file, "%s,%.2f,%s,%s\n", 
                t->type == TRANSACTION_INCOME ? "Income" : "Expense",
                t->amount, t->description, t->date);
    }
    
    // 9. Cerrar el archivo
    fclose(file);
    
    // 10. Mostrar mensaje de éxito con el nombre del archivo creado
    snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), 
            get_text("GUARDADO_EN"), filename);  
    uiState.statusMessageTime = 2.0f;
}

void LoadBudgetFromCSV() {
    // 1. Configurar y mostrar diálogo de selección de archivos
    OPENFILENAMEA ofn;
    char filename[MAX_PATH] = "";
    
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "CSV Files (*.csv)\0*.csv\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "csv";
    ofn.lpstrTitle = get_text("SELECCIONAR_ARCHIVO");
    
    // Mostrar diálogo de selección
    if (!GetOpenFileNameA(&ofn)) {
        // Usuario canceló o hubo error
        if (CommDlgExtendedError()) {
            snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), 
                     get_text("ERROR_SELECCION"));
        } else {
            // Usuario simplemente canceló, no mostrar error
            snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), 
                     get_text("CANCELADO"));
        }
        uiState.statusMessageTime = 2.0f;
        return;
    }
    
    // 2. Abrir archivo seleccionado
    FILE* file = fopen(filename, "r");
    if (!file) {
        snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), 
                 get_text("FALLO_AL_ABRIR"));
        uiState.statusMessageTime = 3.0f;
        return;
    }
    
    // 3. Reiniciar presupuesto actual
    budget.count = 0;
    char line[1024];  // Aumentado de 512 a 1024 para líneas más largas
    
    // 4. Leer y validar encabezado
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), 
                 get_text("ARCHIVO_VACIO"));
        uiState.statusMessageTime = 3.0f;
        return;
    }
    
    // Verificar formato básico del encabezado
    char headerCheck[256];
    if (sscanf(line, "%255[^\n]", headerCheck) != 1) {
        fclose(file);
        snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), 
                 get_text("FORMATO_INVALIDO"));
        uiState.statusMessageTime = 3.0f;
        return;
    }
    
    // 5. Leer transacciones con mejor manejo de errores
    int lineNum = 1;  // Para mensajes de error detallados
    int loadedCount = 0;
    int errorCount = 0;
    
    while (fgets(line, sizeof(line), file) && budget.count < 1000) {
        lineNum++;
        
        // Saltar líneas vacías o solo espacios
        int isEmpty = 1;
        for (char* p = line; *p; p++) {
            if (!isspace(*p) && *p != '\0') {
                isEmpty = 0;
                break;
            }
        }
        if (isEmpty) continue;
        
        Transaction* t = &budget.transactions[budget.count];
        char typeStr[32];
        char amountStr[64];
        char description[256];
        char date[32];
        
        // Limpiar buffers
        memset(typeStr, 0, sizeof(typeStr));
        memset(amountStr, 0, sizeof(amountStr));
        memset(description, 0, sizeof(description));
        memset(date, 0, sizeof(date));
        
        // Parsear con mejor manejo de errores
        int parsed = sscanf(line, "%31[^,],%63[^,],%255[^,],%31[^\n\r]", 
                           typeStr, amountStr, description, date);
        
        if (parsed == 4) {
            // Convertir monto de string a float con validación
            char* endptr;
            t->amount = (float)strtod(amountStr, &endptr);
            if (endptr == amountStr || *endptr != '\0') {
                // Error en conversión numérica
                errorCount++;
                continue;
            }
            
            // Validar y limpiar descripción (remover comillas si existen)
            if (description[0] == '"' && description[strlen(description)-1] == '"') {
                // Remover comillas exteriores
                memmove(description, description+1, strlen(description)-2);
                description[strlen(description)-2] = '\0';
            }
            
            // Validar tipo de transacción
            if (strcmp(typeStr, "Income") == 0) {
                t->type = TRANSACTION_INCOME;
            } else if (strcmp(typeStr, "Expense") == 0) {
                t->type = TRANSACTION_EXPENSE;
            } else {
                errorCount++;
                continue;
            }
            
            // Validar fecha básica (debería tener 10 caracteres para YYYY-MM-DD)
            if (strlen(date) < 8) {
                errorCount++;
                continue;
            }
            
            // Copiar datos validados
            strncpy(t->description, description, sizeof(t->description)-1);
            t->description[sizeof(t->description)-1] = '\0';
            
            strncpy(t->date, date, sizeof(t->date)-1);
            t->date[sizeof(t->date)-1] = '\0';
            
            budget.count++;
            loadedCount++;
        } else {
            errorCount++;
            // Opcional: Log detallado del error
            #ifdef DEBUG_MODE
            printf("Error parsing line %d: %s\n", lineNum, line);
            #endif
        }
    }
    
    fclose(file);
    
    // 6. Actualizar y mostrar resultados
    if (budget.count > 0) {
        UpdateBudgetTotals();
        
        if (errorCount > 0) {
            snprintf(uiState.statusMessage, sizeof(uiState.statusMessage),
                     get_text("CARGADO_CON_ERRORES"), 
                     loadedCount, errorCount);
        } else {
            snprintf(uiState.statusMessage, sizeof(uiState.statusMessage),
                     get_text("CARGADO_EXITOSO"), 
                     loadedCount);
        }
    } else {
        if (errorCount > 0) {
            snprintf(uiState.statusMessage, sizeof(uiState.statusMessage),
                     get_text("ERROR_LINEAS"), errorCount);
        } else {
            snprintf(uiState.statusMessage, sizeof(uiState.statusMessage),
                     get_text("SIN_DATOS"));
        }
    }
    
    uiState.statusMessageTime = 3.0f;
    
    // 7. Opcional: Forzar redibujado de UI
    #ifdef UI_REFRESH_NEEDED
    RequestUIRefresh();
    #endif
}


void SaveBudgetToTXT() {
    // 1. Obtener la frase base para el nombre del archivo
    const char* basePhrase = get_text("NOMBRE_BASE_ARCHIVO"); 
    
    // 2. Obtener la fecha actual formateada
    char currentDate[11];
    GetCurrentDate(currentDate, sizeof(currentDate));
    
    // 3. Convertir la fecha a formato válido para nombres de archivo
    char formattedDate[11];
    strcpy(formattedDate, currentDate);
    for (int i = 0; formattedDate[i]; i++) {
        if (formattedDate[i] == '/') formattedDate[i] = '_';
    }
    
    // 4. Crear el nombre base del archivo (TXT, no CSV como en tu código)
    char baseFilename[256];
    snprintf(baseFilename, sizeof(baseFilename), "%s_%s.txt", basePhrase, formattedDate);
    
    // 5. Verificar si el archivo existe y encontrar un nombre disponible
    char finalFilename[256];  // Cambiado de 'filename' a 'finalFilename'
    strcpy(finalFilename, baseFilename);
    
    int counter = 1;
    while (access(finalFilename, F_OK) == 0) {
        // Si el archivo existe, agregar un número al final
        snprintf(finalFilename, sizeof(finalFilename), "%s_%s_%d.txt", 
                basePhrase, formattedDate, counter);
        counter++;
        
        // Limitar el número máximo de intentos (por seguridad)
        if (counter > 1000) {
            snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), 
                    "Error: Demasiados archivos existentes");
            uiState.statusMessageTime = 3.0f;
            return;
        }
    }
    
    // 6. Abrir el archivo para escritura (usando finalFilename)
    FILE* file = fopen(finalFilename, "w");
    if (!file) {
        snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), 
                get_text("GUARDADO_FALLIDO"));
        uiState.statusMessageTime = 3.0f;
        return;
    }
    
    // 7. Escribir el contenido en el archivo (usando 'file', no 'baseFilename')
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
        const char* tipoTexto = t->type == TRANSACTION_INCOME ? 
                               get_text("INGRESO_MAYUS") : get_text("GASTO_MAYUS");
        
        fprintf(file, "[%s] %s\n", t->date, tipoTexto);
        fprintf(file, "    %s $%.2f\n", get_text("ETIQUETA_MONTO"), t->amount);
        fprintf(file, "    %s %s\n\n", get_text("ETIQUETA_DESCRIPCION"), t->description);
    }
    
    fclose(file);
    snprintf(uiState.statusMessage, sizeof(uiState.statusMessage), 
         get_text("REPORTE_GUARDADO_EN"), finalFilename); 
    uiState.statusMessageTime = 2.0f;
}
