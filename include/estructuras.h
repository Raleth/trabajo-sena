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

//CONSTANTES
const int altura = 720;  // altura
const int ancho = 1280;  // ancho



#endif