# ======================================================
#  MAKEFILE — Proyecto en C con Raylib y Clay
# ======================================================

# ===== Rutas de salida =====
OBJDIR := build
BINDIR := bin

# ===== Herramientas y banderas de compilación =====
CC       := gcc
CPPFLAGS := -Iinclude
CFLAGS   := -Wall -Wextra -O2 -MMD -MP

# ===== Librerías a enlazar =====
LDFLAGS  := -Llib
LDLIBS   := -lraylib -lopengl32 -lgdi32 -lwinmm -lshell32

# ===== Utilidades portables (sh) =====
MKDIR_P := mkdir -p
RM      := rm -f
RMR     := rm -rf

# ===== Fuentes, objetos y ejecutable =====
# Si quieres detectar automáticamente todos los .c en src/, descomenta la línea siguiente
# SRC := $(wildcard src/*.c)
SRC := src/main.c

# Mapeo: src/xxx.c -> build/xxx.o y build/xxx.d
OBJ := $(patsubst src/%.c,$(OBJDIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

TARGET := prueba
# En Windows el ejecutable termina en .exe; en otros SO, sin extensión.
# Para simplificar en Windows, dejamos .exe fijo (no molesta en sh).
BINARY := $(BINDIR)/$(TARGET).exe

# ===== Reglas principales =====
.PHONY: all clean

all: $(BINARY)

# ----- Enlace: crea bin/ y enlaza -----
$(BINARY): $(OBJ)
	@$(MKDIR_P) $(BINDIR)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# ----- Compilación: build/%.o desde src/%.c -----
$(OBJDIR)/%.o: src/%.c
	@$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

# ----- Limpieza portable -----
clean:
	-$(RM) $(OBJ) $(DEP) $(BINARY)
	-$(RM) $(OBJDIR)/*.o $(OBJDIR)/*.d 2>/dev/null || true
	-$(RM) $(BINDIR)/*.exe 2>/dev/null || true

# ----- Dependencias automáticas -----
-include $(DEP)
