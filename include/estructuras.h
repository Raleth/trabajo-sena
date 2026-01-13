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
void SaveBudgetToCSV(const char* filename);
void LoadBudgetFromCSV(const char* filename);
void SaveBudgetToTXT(const char* filename);

//CONSTANTES
extern const int altura;
extern const int ancho;

//Definiciones de Idiomas.c
extern int idioma_global; // extern para acceder a la variable global del idioma

const char* get_text(const char *clave); // Prototipo de la función para obtener texto traducido

#endif
