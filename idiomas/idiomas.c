#include <stdio.h>
#include <string.h>
// 1. Español
// 2. Inglés
// 3. Frances
// y puedes agregar más idiomas aquí
int idioma_global = 1; 

#define MAX_CLAVES 100

typedef struct {
    const char *clave;
    const char *texto;
} ParClaveValor;

const ParClaveValor mensajes_es[] = {
    {"TITULO_VENTANA", "Bienvenido a tu gestor de presupuestos."},
    {"CANTIDAD","Cantidad: "},
    {"MENSAJE_MAXIMA_CANTIDAD_TRANSACCIONES", "error: ¡Cantidad máxima de transacciones alcanzada!"},
    {"TRANSACCION_BORRADA", "Transacción eliminada"},
    {"GUARDADO_FALLIDO", "Error al guardar el archivo."},
    {"FALLO_AL_ABRIR", "Error: No se pudo abrir el archivo."},
    {"AÑADIR_TRANSACCION", "Añadir Transacción"},
    {"DESCRIPCION", "Descripcion: "},
    {"AÑADIR_INGRESO", "Añadir Ingreso"},
    {"AÑADIR_GASTO", "Añadir Gasto"},
    {"OPERACIONES_DE_ARCHIVO", "Operaciones de archivo"},
    {"GUARDAR_CSV"," Guardar como CSV"},
    {"CARGAR_CSV"," Cargar CSV"},
    {"EXPORTAR_REPORTE"," Exportar Reporte en TXT"},
    {"TOTAL_INGRESOS", "Total de Ingresos: "},
    {"TOTAL_GASTOS", "Total de Gastos: "},
    {"BALANCE_NETO", "Balance Neto: "},
    {"NOTIFICACION","Aún no hay transacciones. ¡Agrega la primera!"},
    {"ERROR_VALIDACION", "Por favor, introduce una cantidad y descripción válidas"},
    {"INGRESO", "Ingreso"},
    {"GASTO", "Gasto"},
    {"AÑADIDO_FORMATO", "%s añadido: $%.2f"},
    {"ENTRADA_INVALIDA", "Entrada inválida. Por favor, inténtalo de nuevo."},
    {"GUARDADO_EN", "Guardado como %s"},
    {"CARGADO_DESDE", "Cargado desde %s (%d transacciones)"},
    {"REPORTE_GUARDADO_EN", "Reporte guardado en %s"},
    {"TOTAL_INGRESOS", "Total de Ingresos: $"},
    {"TOTAL_GASTOS", "Total de Gastos: $"},
    {"BALANCE_NETO", "Balance Neto: $"},
    {"TITULO_REPORTE", "INFORME DE GESTOR DE PRESUPUESTO"},
    {"LISTA_TRANSACCIONES", "LISTA DE TRANSACCIONES"},
    {"INGRESO_MAYUS", "INGRESO"},
    {"GASTO_MAYUS", "GASTO"},
    {"ETIQUETA_MONTO", "Monto:"},
    {"ETIQUETA_DESCRIPCION", "Descripción:"},
    {"NOMBRE_BASE_ARCHIVO", "Presupuesto"},
    {"SELECCIONAR_ARCHIVO", "Seleccionar archivo CSV de presupuesto" },
    {"ERROR_SELECCION", "Error al seleccionar el archivo."},
    {"CANCELADO", "Operación cancelada por el usuario."},
    {"ARCHIVO_VACIO", "El archivo está vacío o no contiene datos válidos."},
    {"FORMATO_INVALIDO", "Formato de archivo inválido."},
    {"CARGADO_CON_ERRORES", "Cargado %d transacciones (%d errores)."},
    {"CARGADO_EXITOSO", "Cargado exitosamente %d transacciones."},
    {"ERROR_LINEAS", "Error en %d líneas del archivo."},
    {"SIN_DATOS", "No se encontraron datos válidos en el archivo."},
    {NULL, NULL}
};

const ParClaveValor mensajes_en[] = {
    {"TITULO_VENTANA", "Welcome to your budget manager."},
    {"CANTIDAD", "Amount: "},
    {"MENSAJE_MAXIMA_CANTIDAD_TRANSACCIONES", "Error: Maximum number of transactions reached!"},
    {"TRANSACCION_BORRADA", "Transaction deleted"},
    {"GUARDADO_FALLIDO", "Error saving file."},
    {"FALLO_AL_ABRIR", "Error: Could not open file."},
    {"AÑADIR_TRANSACCION", "Add Transaction"},
    {"DESCRIPCION", "Description: "},
    {"AÑADIR_INGRESO", "Add Income"},
    {"AÑADIR_GASTO", "Add Expense"},
    {"OPERACIONES_DE_ARCHIVO", "File Operations"},
    {"GUARDAR_CSV", " Save as CSV"},
    {"CARGAR_CSV", " Load CSV"},
    {"EXPORTAR_REPORTE", " Export Report as TXT"},
    {"TOTAL_INGRESOS", "Total Income: "},
    {"TOTAL_GASTOS", "Total Expenses: "},
    {"BALANCE_NETO", "Net Balance: "},
    {"NOTIFICACION", "No transactions yet. Add the first one!"},
    {"ERROR_VALIDACION", "Please enter a valid amount and description"},
    {"INGRESO", "Income"},
    {"GASTO", "Expense"},
    {"AÑADIDO_FORMATO", "%s added: $%.2f"},
    {"ENTRADA_INVALIDA", "Invalid input. Please try again."},
    {"GUARDADO_EN", "Saved as %s"},
    {"CARGADO_DESDE", "Loaded from %s (%d transactions)"},
    {"REPORTE_GUARDADO_EN", "Report saved in %s"},
    {"TOTAL_INGRESOS", "Total Income: $"},
    {"TOTAL_GASTOS", "Total Expenses: $"},
    {"BALANCE_NETO", "Net Balance: $"},
    {"TITULO_REPORTE", "BUDGET MANAGER REPORT"},
    {"LISTA_TRANSACCIONES", "TRANSACTION LIST"},
    {"INGRESO_MAYUS", "INCOME"},
    {"GASTO_MAYUS", "EXPENSE"},
    {"ETIQUETA_MONTO", "Amount:"},
    {"ETIQUETA_DESCRIPCION", "Description:"},
    {"NOMBRE_BASE_ARCHIVO", "Budget"},
    {"SELECCIONAR_ARCHIVO", "Select budget CSV file"},
    {"ERROR_SELECCION", "Error selecting file."},
    {"CANCELADO", "Operation cancelled by user."},
    {"ARCHIVO_VACIO", "The file is empty or contains no valid data."},
    {"FORMATO_INVALIDO", "Invalid file format."},
    {"CARGADO_CON_ERRORES", "Loaded %d transactions (%d errors)."},
    {"CARGADO_EXITOSO", "Successfully loaded %d transactions."},
    {"ERROR_LINEAS", "Error in %d lines of the file."},
    {"SIN_DATOS", "No valid data found in the file."},
    {NULL, NULL}
};

const ParClaveValor mensajes_de[] = {
    {"TITULO_VENTANA", "Willkommen zu Ihrem Budgetmanager."},
    {"CANTIDAD", "Betrag: "},
    {"MENSAJE_MAXIMA_CANTIDAD_TRANSACCIONES", "Fehler: Maximale Anzahl an Transaktionen erreicht!"},
    {"TRANSACCION_BORRADA", "Transaktion gelöscht"},
    {"GUARDADO_FALLIDO", "Fehler beim Speichern der Datei."},
    {"FALLO_AL_ABRIR", "Fehler: Datei konnte nicht geöffnet werden."},
    {"AÑADIR_TRANSACCION", "Transaktion hinzufügen"},
    {"DESCRIPCION", "Beschreibung: "},
    {"AÑADIR_INGRESO", "Einnahme hinzufügen"},
    {"AÑADIR_GASTO", "Ausgabe hinzufügen"},
    {"OPERACIONES_DE_ARCHIVO", "Dateioperationen"},
    {"GUARDAR_CSV", " Als CSV speichern"},
    {"CARGAR_CSV", " CSV laden"},
    {"EXPORTAR_REPORTE", " Bericht als TXT exportieren"},
    {"TOTAL_INGRESOS", "Gesamteinnahmen: "},
    {"TOTAL_GASTOS", "Gesamtausgaben: "},
    {"BALANCE_NETO", "Netto-Saldo: "},
    {"NOTIFICACION", "Noch keine Transaktionen. Fügen Sie die erste hinzu!"},
    {"ERROR_VALIDACION", "Bitte geben Sie einen gültigen Betrag und eine Beschreibung ein"},
    {"INGRESO", "Einnahme"},
    {"GASTO", "Ausgabe"},
    {"AÑADIDO_FORMATO", "%s hinzugefügt: %.2f €"},
    {"ENTRADA_INVALIDA", "Ungültige Eingabe. Bitte versuchen Sie es erneut."},
    {"GUARDADO_EN", "Gespeichert als %s"},
    {"CARGADO_DESDE", "Geladen von %s (%d Transaktionen)"},
    {"REPORTE_GUARDADO_EN", "Bericht gespeichert in %s"},
    {"TOTAL_INGRESOS", "Gesamteinnahmen: €"},
    {"TOTAL_GASTOS", "Gesamtausgaben: €"},
    {"BALANCE_NETO", "Netto-Saldo: €"},
    {"TITULO_REPORTE", "BUDGET-MANAGER-BERICHT"},
    {"LISTA_TRANSACCIONES", "TRANSAKTIONSLISTE"},
    {"INGRESO_MAYUS", "EINNAHME"},
    {"GASTO_MAYUS", "AUSGABE"},
    {"ETIQUETA_MONTO", "Betrag:"},
    {"ETIQUETA_DESCRIPCION", "Beschreibung:"},
    {"NOMBRE_BASE_ARCHIVO", "Budget"},
    {"SELECCIONAR_ARCHIVO", "Budget-CSV-Datei auswählen"},
    {"ERROR_SELECCION", "Fehler bei der Dateiauswahl."},
    {"CANCELADO", "Vorgang vom Benutzer abgebrochen."},
    {"ARCHIVO_VACIO", "Die Datei ist leer oder enthält keine gültigen Daten."},
    {"FORMATO_INVALIDO", "Ungültiges Dateiformat."},
    {"CARGADO_CON_ERRORES", "%d Transaktionen geladen (%d Fehler)."},
    {"CARGADO_EXITOSO", "Erfolgreich %d Transaktionen geladen."},
    {"ERROR_LINEAS", "Fehler in %d Zeilen der Datei."},
    {"SIN_DATOS", "Keine gültigen Daten in der Datei gefunden."},
    {NULL, NULL}
};

const ParClaveValor mensajes_fr[] = {
    {"TITULO_VENTANA", "Bienvenue dans votre gestionnaire de budget."},
    {"CANTIDAD", "Montant : "},
    {"MENSAJE_MAXIMA_CANTIDAD_TRANSACCIONES", "Erreur : Nombre maximum de transactions atteint !"},
    {"TRANSACCION_BORRADA", "Transaction supprimée"},
    {"GUARDADO_FALLIDO", "Erreur lors de l'enregistrement du fichier."},
    {"FALLO_AL_ABRIR", "Erreur : Impossible d'ouvrir le fichier."},
    {"AÑADIR_TRANSACCION", "Ajouter une transaction"},
    {"DESCRIPCION", "Description : "},
    {"AÑADIR_INGRESO", "Ajouter un revenu"},
    {"AÑADIR_GASTO", "Ajouter une dépense"},
    {"OPERACIONES_DE_ARCHIVO", "Opérations de fichiers"},
    {"GUARDAR_CSV", " Enregistrer en CSV"},
    {"CARGAR_CSV", " Charger CSV"},
    {"EXPORTAR_REPORTE", " Exporter le rapport en TXT"},
    {"TOTAL_INGRESOS", "Total des revenus : "},
    {"TOTAL_GASTOS", "Total des dépenses : "},
    {"BALANCE_NETO", "Solde net : "},
    {"NOTIFICACION", "Aucune transaction pour le moment. Ajoutez la première !"},
    {"ERROR_VALIDACION", "Veuillez saisir un montant et une description valides"},
    {"INGRESO", "Revenu"},
    {"GASTO", "Dépense"},
    {"AÑADIDO_FORMATO", "%s ajouté : %.2f €"},
    {"ENTRADA_INVALIDA", "Saisie invalide. Veuillez réessayer."},
    {"GUARDADO_EN", "Enregistré sous %s"},
    {"CARGADO_DESDE", "Chargé depuis %s (%d transactions)"},
    {"REPORTE_GUARDADO_EN", "Rapport enregistré dans %s"},
    {"TOTAL_INGRESOS", "Total des revenus : €"},
    {"TOTAL_GASTOS", "Total des dépenses : €"},
    {"BALANCE_NETO", "Solde net : €"},
    {"TITULO_REPORTE", "RAPPORT DU GESTIONNAIRE DE BUDGET"},
    {"LISTA_TRANSACCIONES", "LISTE DES TRANSACTIONS"},
    {"INGRESO_MAYUS", "REVENU"},
    {"GASTO_MAYUS", "DÉPENSE"},
    {"ETIQUETA_MONTO", "Montant :"},
    {"ETIQUETA_DESCRIPCION", "Description :"},
    {"NOMBRE_BASE_ARCHIVO", "Budget"},
    {"SELECCIONAR_ARCHIVO", "Sélectionner un fichier CSV de budget"},
    {"ERROR_SELECCION", "Erreur lors de la sélection du fichier."},
    {"CANCELADO", "Opération annulée par l'utilisateur."},
    {"ARCHIVO_VACIO", "Le fichier est vide ou ne contient pas de données valides."},
    {"FORMATO_INVALIDO", "Format de fichier invalide."},
    {"CARGADO_CON_ERRORES", "%d transactions chargées (%d erreurs)."},
    {"CARGADO_EXITOSO", "Succès du chargement de %d transactions."},
    {"ERROR_LINEAS", "Erreur dans %d lignes du fichier."},
    {"SIN_DATOS", "Aucune donnée valide trouvée dans le fichier."},
    {NULL, NULL}
};

const ParClaveValor* obtener_catalogo() {

    switch (idioma_global) {
        case 1:
            return mensajes_es;
        case 2:
            return mensajes_en;
        case 3:
            return mensajes_de;
        case 4:
            return mensajes_fr;
        default:
            return mensajes_es; 
    }

}

const char* get_text(const char *clave) {
    if (clave == NULL) {
        return "[ERROR: CLAVE NULA]"; // Caso extremo
    }

    // Obtener el catálogo según el idioma actual
    const ParClaveValor *catalogo = obtener_catalogo();

    // Recorrer el catálogo hasta encontrar la clave
    for (int i = 0; catalogo[i].clave != NULL; i++) {
        if (strcmp(catalogo[i].clave, clave) == 0) {
            return catalogo[i].texto; // ¡Clave encontrada!
        }
    }

    // Si no se encuentra la clave
    return "[TEXTO NO ENCONTRADO]";
}

    