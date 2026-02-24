# Configuración del compilador y flags
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -O2
LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm -lshell32 -lcomdlg32 -luser32

# Directorios
SRC_DIR = src
IDIOMAS_DIR = idiomas
BUILD_DIR = build
BIN_DIR = bin
DLL_DIR = dlls

EXECUTABLE = $(BIN_DIR)/BudgetManager.exe

# Archivos fuente
SRC_FILES = $(SRC_DIR)/main.c $(IDIOMAS_DIR)/idiomas.c \
			$(SRC_DIR)/transacciones.c $(SRC_DIR)/pantallas.c \
			$(SRC_DIR)/graficas.c

# Archivos objeto
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(filter $(SRC_DIR)/%, $(SRC_FILES))) \
            $(patsubst $(IDIOMAS_DIR)/%.c, $(BUILD_DIR)/%.o, $(filter $(IDIOMAS_DIR)/%, $(SRC_FILES)))

# DLLs: se toman todas las que estén en la carpeta dlls/
DLL_FILES = $(wildcard $(DLL_DIR)/*.dll)

# Meta por defecto
all: $(EXECUTABLE)

# Regla para crear el ejecutable
$(EXECUTABLE): $(OBJ_FILES) | $(BIN_DIR)
	@echo Linking...
	$(CC) $(OBJ_FILES) -o $@ $(LDFLAGS)
	@echo Copying DLLs...
	@cp $(DLL_FILES) $(BIN_DIR)/
	@echo Copying assets...
	@cp -r fuente $(BIN_DIR)/ 2>/dev/null || true
	@echo Build successful!

# Reglas de compilación
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c | $(BUILD_DIR)
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/idiomas.o: $(IDIOMAS_DIR)/idiomas.c | $(BUILD_DIR)
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/transacciones.o: $(SRC_DIR)/transacciones.c | $(BUILD_DIR)
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/pantallas.o: $(SRC_DIR)/pantallas.c | $(BUILD_DIR)
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/graficas.o: $(SRC_DIR)/graficas.c | $(BUILD_DIR)
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c $< -o $@

# Crear directorios necesarios
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Ejecutar la aplicación
run: $(EXECUTABLE)
	@echo Running application...
	cmd //c start $(EXECUTABLE)

# Limpiar archivos generados
clean:
	@rm -rf $(BUILD_DIR)/* $(BUILD_DIR)/.[!.]* $(BUILD_DIR)/..?* 2>/dev/null || true
	@rm -rf $(BIN_DIR)/* $(BIN_DIR)/.[!.]* $(BIN_DIR)/..?* 2>/dev/null || true
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

# Limpiar y recompilar
rebuild: clean all

.PHONY: all run clean rebuild