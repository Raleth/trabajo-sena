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
    {"GUARDADO_EN", "Guardado en %s"},
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
    {NULL, NULL}
};

const ParClaveValor mensajes_en[] = {
    {"WELCOME", "Hello! Welcome to your budget manager."},
    {"SAVE_SUCCESS", "data saved successfully."},
    {"LOAD_ERROR", "Error loading file."},
    {"TOTAL_LABEL", "Monthly total:"},
    {NULL, NULL}
};

const ParClaveValor* obtener_catalogo() {

    switch (idioma_global) {
        case 1:
            return mensajes_es;
        case 2:
            return mensajes_en;
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

    