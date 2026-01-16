#ifndef datos
#define datos

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

// ===== ESTADO GLOBAL =====
extern Budget budget;
extern UIState uiState;
// ===== FUNCIONES DE TRANSACCIONES =====
const char* get_text(const char *clave);
void UpdateBudgetTotals(void);
void GetCurrentDate(char* buffer, size_t size);
void AddTransaction(TransactionType type, float amount, const char* description);
void DeleteTransaction(int index);
void SaveBudgetToCSV(void);
void LoadBudgetFromCSV(void);
void SaveBudgetToTXT(void);

//CONSTANTES
extern const int altura;
extern const int ancho;

//Definiciones de Idiomas.c
extern int idioma_global; // extern para acceder a la variable global del idioma

const char* get_text(const char *clave); // Prototipo de la función para obtener texto traducido

//texto adicionales
#define MSG_FALLO_AL_ABRIR "No se pudo abrir el archivo"
#define MSG_ERROR_SELECCION "Error al seleccionar archivo"
#define MSG_CANCELADO "Operación cancelada"
#define MSG_ARCHIVO_VACIO "El archivo está vacío"
#define MSG_FORMATO_INVALIDO "Formato de archivo inválido"
#define MSG_CARGADO_CON_ERRORES "Cargadas %d transacciones (%d errores)"
#define MSG_CARGADO_EXITOSO "%d transacciones cargadas exitosamente"
#define MSG_ERROR_LINEAS "%d líneas con formato incorrecto"
#define MSG_SIN_DATOS "No se encontraron datos válidos"

#endif
