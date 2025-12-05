#define CLAY_IMPLEMENTATION
#include "clay.h"
#include <stdio.h>
#include "clay_renderer_raylib.c"
#include <stdlib.h>

const int altura = 1280;
const int ancho = 720;

void HandleClayErrors(Clay_ErrorData errorData)
{
    printf("%s", errorData.errorText.chars);
}

int main(void)
{

    Clay_Raylib_Initialize(altura, ancho, "Prototipo de interfaz grafica", FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    // aqui inicializo la libreria de raylib que es el renderizador de graficos que estoy usando

    uint64_t clayRequiredMemory = Clay_MinMemorySize();
    // llamo a la funcion que me dice cuanta memoria necesito para clay

    Clay_Arena claymemory = (Clay_Arena){
        .memory = malloc(clayRequiredMemory),
        .capacity = clayRequiredMemory};
    // usamos esta estructura para manejar cuanta memoria usara clay y donde la movera

    Clay_Initialize(claymemory, (Clay_Dimensions){.width = altura, .height = ancho},
                    (Clay_ErrorHandler){HandleClayErrors, .userData = NULL});

    // inicializamos clay con estos 3 parametros:
    // claymemory: la memoria que va a usar clay
    // clay_dimensions: las dimensiones iniciales del layout -- en este caso de las mimas dimensiones de la ventana
    // Clay_ErrorHandler: para reportar errores

    // ahora crearemos el ciclo de renderizado

    while (!WindowShouldClose())
    { // aqui estan el inicio y final del ciclo de renderizado de CLAY
        Clay_SetLayoutDimensions((Clay_Dimensions){.width = GetScreenWidth(),
                                                   .height = GetScreenHeight()}); // aqui declaramos que la UI de clay tendra el tamaño de la ventana, es decir que escalara

        Clay_BeginLayout();

        CLAY(CLAY_ID("Contenedor_total"), {.backgroundColor = {43, 41, 51, 255},
                                           .layout = {
                                               .layoutDirection = CLAY_LEFT_TO_RIGHT,
                                               .sizing = {
                                                   .width = CLAY_SIZING_GROW(),
                                                   .height = CLAY_SIZING_GROW()},
                                               .padding = {20, 20,20,20}}})
        {
            CLAY(CLAY_ID("Barra_de_menu"), {.backgroundColor = {90, 90, 90, 255},
                                            .cornerRadius = 8,
                                            .layout = {.sizing = {
                                                           .width = CLAY_SIZING_PERCENT(0.1),
                                                           .height = CLAY_SIZING_PERCENT(1)}}})
            {
            }
        }

        Clay_RenderCommandArray renderCommands = Clay_EndLayout();

        BeginDrawing();
        ClearBackground(BLACK);
        Clay_Raylib_Render(renderCommands, NULL);
        EndDrawing();
    }
    return 0;
    free(claymemory.memory);
}
