#ifndef GRAFICAS_H
#define GRAFICAS_H

#include "estructuras.h"
#include "pantallas.h"   // para UITheme, UIIds

// Alterna la vista de estadísticas
void Prinftestadisticas(void);

// Renderiza la vista de estadísticas usando Clay
void RenderStatsView(const UITheme *t, int fontId, const UIIds *ids);

#endif