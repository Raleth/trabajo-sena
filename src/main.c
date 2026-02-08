#define CLAY_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <raylib.h>
#include "clay.h"
#include "clay_renderer_raylib.c"
#include "estructuras.h"
#include "pantallas.h"

const int altura = 720;
const int ancho = 1280;

void HandleClayErrors(Clay_ErrorData errorData)
{
    printf("%s", errorData.errorText.chars);
}


int main(void)
{
    Clay_Raylib_Initialize(ancho, altura,get_text("TITULO_VENTANA"), 
                          FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);

    SetTargetFPS(60);

    uint64_t clayRequiredMemory = Clay_MinMemorySize();
    Clay_Arena claymemory = (Clay_Arena){
        .memory = malloc(clayRequiredMemory),
        .capacity = clayRequiredMemory
    };
    
    if (!claymemory.memory) {
        printf("ERROR: Failed to allocate Clay memory!\n");
        return 1;
    }

    Clay_Initialize(claymemory, (Clay_Dimensions){.width = ancho, .height = altura},
                    (Clay_ErrorHandler){HandleClayErrors, .userData = NULL});

    Clay_SetDebugModeEnabled(true);
    
    const int TU_FONT_ID = 0;
    UITheme theme = UI_DefaultTheme();

    Font fonts[1];
    UI_InitFonts(fonts, TU_FONT_ID);
    Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);

    // Input handling variables
    float deltaTime = 0.0f;
    UIIds ids = {0};
    
    while (!WindowShouldClose())
    {
        deltaTime = GetFrameTime();

        if (uiState.statusMessageTime > 0) {
            uiState.statusMessageTime -= deltaTime;
        }
        
        Clay_SetLayoutDimensions((Clay_Dimensions){.width = GetScreenWidth(),
                                                   .height = GetScreenHeight()});

        // Set pointer state for mouse interaction
        Vector2 mousePos = GetMousePosition();
        Clay_SetPointerState((Clay_Vector2){mousePos.x, mousePos.y}, IsMouseButtonDown(MOUSE_LEFT_BUTTON));

        UI_AssignElementIds(&ids);

        Clay_RenderCommandArray renderCommands = UI_BuildLayout(&theme, &ids, TU_FONT_ID);
        
        UI_HandlePointerFocus(&ids);
        UI_HandleTextInput();
        UI_HandleButtons(&ids);

        BeginDrawing();
        ClearBackground(BLACK);
        Clay_Raylib_Render(renderCommands, fonts);
        EndDrawing();
    }

    free(claymemory.memory);
    Clay_Raylib_Close();
    return 0;

}
