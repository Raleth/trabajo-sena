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

const int FONT_ID_BODY_16 = 0;

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

    const Clay_Color colorfondo = {43, 41, 51, 255};

    const Clay_Color colorobjeto = {90, 90, 90, 255};



    while (!WindowShouldClose())
    { // aqui estan el inicio y final del ciclo de renderizado de CLAY
        Clay_SetLayoutDimensions((Clay_Dimensions){.width = GetScreenWidth(),
                                                   .height = GetScreenHeight()}); // aqui declaramos que la UI de clay tendra el tamaño de la ventana, es decir que escalara

        Clay_BeginLayout();

        CLAY(CLAY_ID("Contenedor_total"), {.backgroundColor = colorfondo,
                                           .layout = {
                                               .layoutDirection = CLAY_LEFT_TO_RIGHT,
                                               .sizing = {
                                                   .width = CLAY_SIZING_GROW(),
                                                   .height = CLAY_SIZING_GROW()},
                                               .padding = {20, 20, 20, 20},
                                               .childGap = 20}})
        {
            CLAY(CLAY_ID("Barra_de_menu"), {
                                               .backgroundColor = colorobjeto,
                                               .cornerRadius = 8,
                                               .layout = {
                                                   .sizing = {
                                                       .width = CLAY_SIZING_PERCENT(0.2),
                                                       .height = CLAY_SIZING_PERCENT(1)},
                                                   .padding = {20, 20, 20, 20},
                                               },
                                           })
            {
            }

            CLAY(CLAY_ID("area de trabajo"), {.backgroundColor = colorobjeto,
                                              .cornerRadius = 8,
                                              .layout = {.sizing = {
                                                             .width = CLAY_SIZING_GROW(),
                                                             .height = CLAY_SIZING_GROW(),
                                                         },
                                                         .padding = {20, 20, 20, 20},
                                                         .childGap = 30}})
            {
                CLAY(CLAY_ID("ingresos"), {

                                              .backgroundColor = colorfondo,
                                              .cornerRadius = 8,
                                              .layout = {.sizing = {
                                                             .width = CLAY_SIZING_PERCENT(0.33),
                                                             .height = CLAY_SIZING_PERCENT(0.2),
                                                         },
                                                         .padding = {10, 10, 10, 10}}})
                {
                    
                }

                CLAY(CLAY_ID("gastos"), {

                                            .backgroundColor = colorfondo,
                                            .cornerRadius = 8,
                                            .layout = {.sizing = {
                                                           .width = CLAY_SIZING_PERCENT(0.33),
                                                           .height = CLAY_SIZING_PERCENT(0.2),
                                                       },
                                                       .padding = {10, 10, 10, 10}}})
                {
                }
                CLAY(CLAY_ID("balance"), {

                                             .backgroundColor = colorfondo,
                                             .cornerRadius = 8,
                                             .layout = {.sizing = {
                                                            .width = CLAY_SIZING_PERCENT(0.33),
                                                            .height = CLAY_SIZING_PERCENT(0.2),
                                                        },
                                                        .padding = {10, 10, 10, 10}}})
                {
                }
            }
        }

        Clay_RenderCommandArray renderCommands = Clay_EndLayout();

        BeginDrawing();
        ClearBackground(BLACK);
        Clay_Raylib_Render(renderCommands, NULL);
        EndDrawing();
    }

    free(claymemory.memory);
    return 0;
}
