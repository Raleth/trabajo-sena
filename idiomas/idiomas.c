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
    {"MENSAJE_MAXIMA_CANTIDAD_TRANSACCIONES", "error: ¡Cantidad máxima de transacciones alcanzada!"},
    {"TRANSACCION_BORRADA", "Transacción eliminada"},
    {"GUARDADO_FALLIDO", "Error al guardar el archivo."},
    {"FALLO_AL_ABRIR", "Error: No se pudo abrir el archivo."},
    {"AÑADIR_TRANSACCION", "Añadir Transacción"},
    {"TOTAL_LABEL", "Total del mes:"},
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

    