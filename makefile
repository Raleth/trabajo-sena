# Configuración del compilador y flags
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -O0
LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm -lshell32

# Directorios
SRC_DIR = src
IDIOMAS_DIR = idiomas
BUILD_DIR = build
BIN_DIR = bin
EXECUTABLE = $(BIN_DIR)/BudgetManager.exe

# Archivos fuente
SRC_FILES = $(SRC_DIR)/main.c $(IDIOMAS_DIR)/idiomas.c \
			$(SRC_DIR)/transacciones.c
# Archivos objeto (cambia .c por .o y cambia directorio)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(filter $(SRC_DIR)/%, $(SRC_FILES))) \
            $(patsubst $(IDIOMAS_DIR)/%.c, $(BUILD_DIR)/%.o, $(filter $(IDIOMAS_DIR)/%, $(SRC_FILES)))

# DLLs necesarios (ajusta las rutas según tu instalación)
RAYLIB_DLL = C:/msys64/mingw64/bin/libraylib.dll
GCC_DLL = C:/msys64/mingw64/bin/libgcc_s_seh-1.dll
WINPTHREAD_DLL = C:/msys64/mingw64/bin/libwinpthread-1.dll
DLL_FILES = $(RAYLIB_DLL) $(GCC_DLL) $(WINPTHREAD_DLL)

# Meta por defecto (la primera regla es la que se ejecuta por defecto)
all: $(EXECUTABLE)

# Regla para crear el ejecutable (enlazado)
$(EXECUTABLE): $(OBJ_FILES) | $(BIN_DIR)
	@echo Linking...
	$(CC) $(OBJ_FILES) -o $@ $(LDFLAGS)
	@echo Copying DLLs...
	@for dll in $(DLL_FILES); do \
		if [ -f "$$dll" ]; then \
			cp "$$dll" $(BIN_DIR)/ 2>/dev/null; \
		fi \
	done
	@echo Build successful!

# Regla para compilar main.c -> build/main.o
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c | $(BUILD_DIR)
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para compilar idiomas.c -> build/idiomas.o
$(BUILD_DIR)/idiomas.o: $(IDIOMAS_DIR)/idiomas.c | $(BUILD_DIR)
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para compilar transacciones.c -> build/transacciones.o
$(BUILD_DIR)/transacciones.o: $(SRC_DIR)/transacciones.c | $(BUILD_DIR)
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c $< -o $@

# Reglas para crear directorios necesarios
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Regla para ejecutar la aplicación
run: $(EXECUTABLE)
	@echo Running application...
	cmd //c start $(EXECUTABLE)

# Regla para limpiar archivos generados
clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(BIN_DIR)

# Regla para limpiar y recompilar desde cero
rebuild: clean all

# Indicar que estas reglas no son archivos
.PHONY: all run clean rebuild