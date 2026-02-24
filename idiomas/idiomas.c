#include <stdio.h>
#include <string.h>
// 1. Espanol
// 2. Ingles
// 3. Aleman
// 4. Frances
int idioma_global = 1;

#define MAX_CLAVES 100

typedef struct
{
    const char *clave;
    const char *texto;
} ParClaveValor;

const ParClaveValor mensajes_es[] = {
    {"TITULO_VENTANA", "Bienvenido a tu gestor de presupuestos."},
    {"CANTIDAD", "Cantidad: "},
    {"MENSAJE_MAXIMA_CANTIDAD_TRANSACCIONES", "Error: Cantidad maxima de transacciones alcanzada!"},
    {"TRANSACCION_BORRADA", "Transaccion eliminada"},
    {"GUARDADO_FALLIDO", "Error al guardar el archivo."},
    {"FALLO_AL_ABRIR", "Error: No se pudo abrir el archivo."},
    {"AÑADIR_TRANSACCION", "Anadir Transaccion"},
    {"DESCRIPCION", "Descripcion: "},
    {"AÑADIR_INGRESO", "Anadir Ingreso"},
    {"AÑADIR_GASTO", "Anadir Gasto"},
    {"OPERACIONES_DE_ARCHIVO", "Operaciones de archivo"},
    {"GUARDAR_CSV", " Guardar como CSV"},
    {"CARGAR_CSV", " Cargar CSV"},
    {"EXPORTAR_REPORTE", " Exportar Reporte en TXT"},
    {"CONFIGURACIONES", "Configuraciones"},
    {"SELECCIONAR_IDIOMA", "Seleccionar idioma"},
    {"IDIOMA_ES", "Espanol"},
    {"IDIOMA_EN", "Ingles"},
    {"IDIOMA_DE", "Aleman"},
    {"IDIOMA_FR", "Frances"},
    {"TOTAL_INGRESOS", "Total de Ingresos: "},
    {"TOTAL_GASTOS", "Total de Gastos: "},
    {"BALANCE_NETO", "Balance Neto: "},
    {"NOTIFICACION", "Aun no hay transacciones. Agrega la primera!"},
    {"ERROR_VALIDACION", "Por favor, introduce una cantidad y descripcion validas"},
    {"INGRESO", "Ingreso"},
    {"GASTO", "Gasto"},
    {"AÑADIDO_FORMATO", "%s anadido: $%.2f"},
    {"ENTRADA_INVALIDA", "Entrada invalida. Por favor, intentalo de nuevo."},
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
    {"ETIQUETA_DESCRIPCION", "Descripcion:"},
    {"NOMBRE_BASE_ARCHIVO", "Presupuesto"},
    {"SELECCIONAR_ARCHIVO", "Seleccionar archivo CSV de presupuesto"},
    {"ERROR_SELECCION", "Error al seleccionar el archivo."},
    {"CANCELADO", "Operacion cancelada por el usuario."},
    {"ARCHIVO_VACIO", "El archivo esta vacio o no contiene datos validos."},
    {"FORMATO_INVALIDO", "Formato de archivo invalido."},
    {"CARGADO_CON_ERRORES", "Cargado %d transacciones (%d errores)."},
    {"CARGADO_EXITOSO", "Cargado exitosamente %d transacciones."},
    {"ERROR_LINEAS", "Error en %d lineas del archivo."},
    {"SIN_DATOS", "No se encontraron datos validos en el archivo."},
    {"INGRESOS", "Ingresos"},
    {"GASTOS", "Gastos"},
    {"TITULO_ESTADISTICAS", "Estadisticas de Balance"},
    {"NO_HAY_DATOS", "No hay datos para mostrar"},
    {"ESTADISTICAS", "Estadisticas"},
    {NULL, NULL}};

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
    {"CONFIGURACIONES", "Settings"},
    {"SELECCIONAR_IDIOMA", "Select language"},
    {"IDIOMA_ES", "Spanish"},
    {"IDIOMA_EN", "English"},
    {"IDIOMA_DE", "German"},
    {"IDIOMA_FR", "French"},
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
    {"INGRESOS", "Income"},
    {"GASTOS", "Expenses"},
    {"TITULO_ESTADISTICAS", "Balance Statistics"},
    {"NO_HAY_DATOS", "No data to display"},
    {"ESTADISTICAS", "Statistics"},
    {NULL, NULL}};

const ParClaveValor mensajes_de[] = {
    {"TITULO_VENTANA", "Willkommen zu Ihrem Budgetmanager."},
    {"CANTIDAD", "Betrag: "},
    {"MENSAJE_MAXIMA_CANTIDAD_TRANSACCIONES", "Fehler: Maximale Anzahl an Transaktionen erreicht!"},
    {"TRANSACCION_BORRADA", "Transaktion geloscht"},
    {"GUARDADO_FALLIDO", "Fehler beim Speichern der Datei."},
    {"FALLO_AL_ABRIR", "Fehler: Datei konnte nicht geoffnet werden."},
    {"AÑADIR_TRANSACCION", "Transaktion hinzufugen"},
    {"DESCRIPCION", "Beschreibung: "},
    {"AÑADIR_INGRESO", "Einnahme hinzufugen"},
    {"AÑADIR_GASTO", "Ausgabe hinzufugen"},
    {"OPERACIONES_DE_ARCHIVO", "Dateioperationen"},
    {"GUARDAR_CSV", " Als CSV speichern"},
    {"CARGAR_CSV", " CSV laden"},
    {"EXPORTAR_REPORTE", " Bericht als TXT exportieren"},
    {"CONFIGURACIONES", "Einstellungen"},
    {"SELECCIONAR_IDIOMA", "Sprache auswahlen"},
    {"IDIOMA_ES", "Spanisch"},
    {"IDIOMA_EN", "Englisch"},
    {"IDIOMA_DE", "Deutsch"},
    {"IDIOMA_FR", "Franzosisch"},
    {"TOTAL_INGRESOS", "Gesamteinnahmen: "},
    {"TOTAL_GASTOS", "Gesamtausgaben: "},
    {"BALANCE_NETO", "Netto-Saldo: "},
    {"NOTIFICACION", "Noch keine Transaktionen. Fugen Sie die erste hinzu!"},
    {"ERROR_VALIDACION", "Bitte geben Sie einen gultigen Betrag und eine Beschreibung ein"},
    {"INGRESO", "Einnahme"},
    {"GASTO", "Ausgabe"},
    {"AÑADIDO_FORMATO", "%s hinzugefugt: %.2f EUR"},
    {"ENTRADA_INVALIDA", "Ungultige Eingabe. Bitte versuchen Sie es erneut."},
    {"GUARDADO_EN", "Gespeichert als %s"},
    {"CARGADO_DESDE", "Geladen von %s (%d Transaktionen)"},
    {"REPORTE_GUARDADO_EN", "Bericht gespeichert in %s"},
    {"TOTAL_INGRESOS", "Gesamteinnahmen: EUR"},
    {"TOTAL_GASTOS", "Gesamtausgaben: EUR"},
    {"BALANCE_NETO", "Netto-Saldo: EUR"},
    {"TITULO_REPORTE", "BUDGET-MANAGER-BERICHT"},
    {"LISTA_TRANSACCIONES", "TRANSAKTIONSLISTE"},
    {"INGRESO_MAYUS", "EINNAHME"},
    {"GASTO_MAYUS", "AUSGABE"},
    {"ETIQUETA_MONTO", "Betrag:"},
    {"ETIQUETA_DESCRIPCION", "Beschreibung:"},
    {"NOMBRE_BASE_ARCHIVO", "Budget"},
    {"SELECCIONAR_ARCHIVO", "Budget-CSV-Datei auswahlen"},
    {"ERROR_SELECCION", "Fehler bei der Dateiauswahl."},
    {"CANCELADO", "Vorgang vom Benutzer abgebrochen."},
    {"ARCHIVO_VACIO", "Die Datei ist leer oder enthalt keine gultigen Daten."},
    {"FORMATO_INVALIDO", "Ungultiges Dateiformat."},
    {"CARGADO_CON_ERRORES", "%d Transaktionen geladen (%d Fehler)."},
    {"CARGADO_EXITOSO", "Erfolgreich %d Transaktionen geladen."},
    {"ERROR_LINEAS", "Fehler in %d Zeilen der Datei."},
    {"SIN_DATOS", "Keine gultigen Daten in der Datei gefunden."},
    {"INGRESOS", "Einnahmen"},
    {"GASTOS", "Ausgaben"},
    {"TITULO_ESTADISTICAS", "Bilanzstatistiken"},
    {"NO_HAY_DATOS", "Keine Daten zum Anzeigen"},
    {"ESTADISTICAS", "Statistiken"},
    {NULL, NULL}};

const ParClaveValor mensajes_fr[] = {
    {"TITULO_VENTANA", "Bienvenue dans votre gestionnaire de budget."},
    {"CANTIDAD", "Montant : "},
    {"MENSAJE_MAXIMA_CANTIDAD_TRANSACCIONES", "Erreur : Nombre maximum de transactions atteint !"},
    {"TRANSACCION_BORRADA", "Transaction supprimee"},
    {"GUARDADO_FALLIDO", "Erreur lors de l'enregistrement du fichier."},
    {"FALLO_AL_ABRIR", "Erreur : Impossible d'ouvrir le fichier."},
    {"AÑADIR_TRANSACCION", "Ajouter une transaction"},
    {"DESCRIPCION", "Description : "},
    {"AÑADIR_INGRESO", "Ajouter un revenu"},
    {"AÑADIR_GASTO", "Ajouter une depense"},
    {"OPERACIONES_DE_ARCHIVO", "Operations de fichiers"},
    {"GUARDAR_CSV", " Enregistrer en CSV"},
    {"CARGAR_CSV", " Charger CSV"},
    {"EXPORTAR_REPORTE", " Exporter le rapport en TXT"},
    {"CONFIGURACIONES", "Parametres"},
    {"SELECCIONAR_IDIOMA", "Selectionner la langue"},
    {"IDIOMA_ES", "Espagnol"},
    {"IDIOMA_EN", "Anglais"},
    {"IDIOMA_DE", "Allemand"},
    {"IDIOMA_FR", "Francais"},
    {"TOTAL_INGRESOS", "Total des revenus : "},
    {"TOTAL_GASTOS", "Total des depenses : "},
    {"BALANCE_NETO", "Solde net : "},
    {"NOTIFICACION", "Aucune transaction pour le moment. Ajoutez la premiere !"},
    {"ERROR_VALIDACION", "Veuillez saisir un montant et une description valides"},
    {"INGRESO", "Revenu"},
    {"GASTO", "Depense"},
    {"AÑADIDO_FORMATO", "%s ajoute : %.2f EUR"},
    {"ENTRADA_INVALIDA", "Saisie invalide. Veuillez reessayer."},
    {"GUARDADO_EN", "Enregistre sous %s"},
    {"CARGADO_DESDE", "Charge depuis %s (%d transactions)"},
    {"REPORTE_GUARDADO_EN", "Rapport enregistre dans %s"},
    {"TOTAL_INGRESOS", "Total des revenus : EUR"},
    {"TOTAL_GASTOS", "Total des depenses : EUR"},
    {"BALANCE_NETO", "Solde net : EUR"},
    {"TITULO_REPORTE", "RAPPORT DU GESTIONNAIRE DE BUDGET"},
    {"LISTA_TRANSACCIONES", "LISTE DES TRANSACTIONS"},
    {"INGRESO_MAYUS", "REVENU"},
    {"GASTO_MAYUS", "DEPENSE"},
    {"ETIQUETA_MONTO", "Montant :"},
    {"ETIQUETA_DESCRIPCION", "Description :"},
    {"NOMBRE_BASE_ARCHIVO", "Budget"},
    {"SELECCIONAR_ARCHIVO", "Selectionner un fichier CSV de budget"},
    {"ERROR_SELECCION", "Erreur lors de la selection du fichier."},
    {"CANCELADO", "Operation annulee par l'utilisateur."},
    {"ARCHIVO_VACIO", "Le fichier est vide ou ne contient pas de donnees valides."},
    {"FORMATO_INVALIDO", "Format de fichier invalide."},
    {"CARGADO_CON_ERRORES", "%d transactions chargees (%d erreurs)."},
    {"CARGADO_EXITOSO", "Succes du chargement de %d transactions."},
    {"ERROR_LINEAS", "Erreur dans %d lignes du fichier."},
    {"SIN_DATOS", "Aucune donnee valide trouvee dans le fichier."},
    {"INGRESOS", "Revenus"},
    {"GASTOS", "Depenses"},
    {"TITULO_ESTADISTICAS", "Statistiques de solde"},
    {"NO_HAY_DATOS", "Aucune donnee a afficher"},
    {"ESTADISTICAS", "Statistiques"},
    {NULL, NULL}};

const ParClaveValor *obtener_catalogo()
{
    switch (idioma_global)
    {
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

const char *get_text(const char *clave)
{
    if (clave == NULL)
    {
        return "[ERROR: CLAVE NULA]";
    }

    const ParClaveValor *catalogo = obtener_catalogo();

    for (int i = 0; catalogo[i].clave != NULL; i++)
    {
        if (strcmp(catalogo[i].clave, clave) == 0)
        {
            return catalogo[i].texto;
        }
    }

    return "[TEXTO NO ENCONTRADO]";
}